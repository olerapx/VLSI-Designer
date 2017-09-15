#include "compositionalgorithm.h"

CompositionAlgorithm::CompositionAlgorithm(QList<Grid*> grids, Scheme* scheme)
{
    setParameters(grids, scheme);
}

void CompositionAlgorithm::setParameters(QList<Grid*> grids, Scheme* scheme)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the algorithm is working."));

    this->grids = grids;
    this->scheme = scheme;
}

void CompositionAlgorithm::onStart()
{
    try
    {
        Grid* grid = execute();
        sendResult(grid);
        sendFinish();
    }
    catch(ThreadStoppedException&)
    {
        sendFinish();
    }
    catch(Exception& e)
    {
        sendError(e.what());
        sendFinish();
    }
}

void CompositionAlgorithm::fillComposedWireData()
{
    if(stopped) return;

    sendLog(tr("Composing wires."));

    composedExternalWireData.clear();
    composedInternalWireData.clear();

    QList<ExternalWireData> externalData(fillExternalWiresData());

    while(!externalData.empty())
    {
        if(stopped) return;

        bool pairFound = false;

        if(externalData.size() == 1)
        {
            composedExternalWireData.append(externalData[0]);
            externalData.removeAt(0);
            return;
        }

        ExternalWireData& data = externalData[0];

        for(int i=1; i<externalData.size(); i++)
        {
            if(stopped) return;

            if(externalData[i].index == data.index)
            {
                Wire wire = SchemeUtils::findWireByIndex(scheme, data.index);

                QPoint dataCoord = data.srcCoord;
                qint64 index = grids[data.gridIndex]->getCells()[dataCoord.y()][dataCoord.x()].getIndex();
                QString pinId = grids[data.gridIndex]->getCells()[dataCoord.y()][dataCoord.x()].getPinId();

                if(wire.getSrcIndex() == index && wire.getSrcPinId() == pinId)
                    composedInternalWireData.append({ data.index, data.srcCoord, data.gridIndex,
                                           externalData[i].srcCoord, externalData[i].gridIndex, wire.getType() });
                else if(wire.getDestIndex() == index && wire.getDestPinId() == pinId)
                    composedInternalWireData.append({ data.index, externalData[i].srcCoord, externalData[i].gridIndex,
                                           data.srcCoord, data.gridIndex, wire.getType() });
                else
                    throw Exception(tr("Cannot locate a cell pin with index %1 and pin id %1 at grid %1.")
                                    .arg(QString::number(index), pinId, QString::number(data.index)));

                externalData.removeAt(i);
                externalData.removeAt(0);

                pairFound = true;

                break;
            }
        }

        if(!pairFound)
        {
            composedExternalWireData.append(externalData[0]);
            externalData.removeAt(0);
        }
    }
}

QList<CompositionAlgorithm::ExternalWireData> CompositionAlgorithm::fillExternalWiresData()
{
    QList<ExternalWireData> res;

    for(int i=0; i<grids.size(); i++)
    {
        for(WireData& data: grids[i]->getWiresData())
        {
            if(data.getWirePosition() == WirePosition::External)
                res.append({ data.getIndex(), data.getSrcCoord(), i});
        }
    }

    return res;
}
