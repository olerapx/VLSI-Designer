#include "generator.h"

Generator::Generator(GeneratorParameters param):
    param(param),
    mt{static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())},
    nodeCapacityDistribution(param.getNodeCapacityMean(), param.getNodeCapacitySigma()),
    branchingDistribution(param.getBranchingMean(), param.getBranchingSigma()),
    libraryRandom(0, param.getLibraries().size() - 1)
{
    mt.discard(numbersToDiscard);
    clear();
}

void Generator::setParameters(GeneratorParameters param)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the generator is working."));

    this->param = param;
}

void Generator::onStart()
{
    try
    {
        Scheme* s = execute();
        sendResult(s);
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

Scheme* Generator::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The generator is already working."));

        stopped = false;
        actuallyStopped = false;

        generateElements();
        generateWires();

        if(stopped)
            throw ThreadStoppedException(tr("Generator is stopped."));

        Scheme* scheme = new Scheme();

        for(Library* l: param.getLibraries())
            scheme->getUsedLibraries().append(QPair<QString, Version>(l->getId(), l->getVersion()));

        for(NodeElement* el: elements)
            scheme->getElements().append(el->getElement());

        scheme->getWires().append(wires);

        generateAliases(scheme);
        clear();

        stopped = true;
        actuallyStopped = true;

        return scheme;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void Generator::clear()
{
    currentElementIndex = 0;
    currentWireIndex = 0;

    for(NodeElement* element: elements)
        delete element;

    elements.clear();
    groupedElements.clear();
    wires.clear();
}

void Generator::generateElements()
{
    groupedElements.append(QList<NodeElement*>());

    sendLog(tr("Functional nodes generation."));

    int capacity = 0;
    int elapsedElements = param.getElementsNumber();
    int currentNodeNumber = 1;

    while((capacity = pow(2, getTruncatedDistributedValue(nodeCapacityDistribution, param.getNodeCapacityLowerLimit(), param.getNodeCapacityUpperLimit())))
          <= elapsedElements)
    {
        elapsedElements -= capacity;
        SchemeElement element = getRandomElement();

        if(capacity == 1)
        {
            addFreeElement(element);
            continue;
        }

        groupedElements.append(QList<NodeElement*>());

        for(int i=0; i<capacity; i++)
        {
            if(stopped) return;

            SchemeElement el = element;

            if (i > 0)
            {
                el.setIndex(currentElementIndex);
                currentElementIndex ++;
            }

            NodeElement* nodeElement = new NodeElement(el, currentNodeNumber, param.getLibraries());

            elements.append(nodeElement);
            groupedElements[groupedElements.size() - 1].append(nodeElement);
        }

        currentNodeNumber ++;
    }

    sendLog(tr("Free elements generation."));

    for(int i=0; i<elapsedElements; i++)
    {
        addFreeElement(getRandomElement());
    }
}

void Generator::addFreeElement(SchemeElement element)
{
    NodeElement* nodeElement = new NodeElement(element, freeNodeElementIndex, param.getLibraries());

    elements.append(nodeElement);
    groupedElements[0].append(nodeElement);
}

int Generator::getTruncatedDistributedValue(std::normal_distribution<>& dist, int lowerLimit, int upperLimit)
{
    while(true)
    {
        int value = round(dist(mt));
        if (value >= lowerLimit && value <= upperLimit)
            return value;
    }
}

SchemeElement Generator::getRandomElement()
{
    Library* library = param.getLibraries()[libraryRandom(mt)];

    std::uniform_int_distribution<int> libraryElementRandom(0, library->getElements().size() - 1);
    LibraryElement element = library->getElements()[libraryElementRandom(mt)];

    SchemeElement res(library->getId(), element.getId(), currentElementIndex);
    currentElementIndex ++;

    return res;
}

void Generator::generateWires()
{
    checkBranching();

    sendLog(tr("Wires generation:"));

    int size = elements.size();

    for(int i=0; i<size; i++)
    {
        sendLog(tr("Element %1 of %2.").arg(QString::number(i), QString::number(size)));

        LibraryElement el = LibraryUtils::getCorrespondingElement(elements[i]->getElement(), param.getLibraries());

        for(Pin p: el.getPins())
        {
            if(p.getType() == PinType::Output)
            {
                if(stopped) return;
                generateWiresForOutput(elements[i], p);
            }
        }
    }
}

void Generator::checkBranching()
{
    if(stopped) return;

    qint64 inputPins = countAllPins(PinType::Input);
    qint64 outputPins = countAllPins(PinType::Output);

    if(inputPins < (param.getBranchingMean() * outputPins))
    {
        int mean = inputPins / outputPins;
        int upperLimit = param.getBranchingUpperLimit();
        int lowerLimit = param.getBranchingLowerLimit();

        if(mean < lowerLimit || mean > upperLimit)
        {
            int diff = param.getBranchingMean() - mean;
            lowerLimit -= diff;
            upperLimit -= diff;

            if(lowerLimit < 0)
                lowerLimit = 0;
        }

        param.setBranching(mean, param.getBranchingSigma(), lowerLimit, upperLimit);

        branchingDistribution = std::normal_distribution<>(param.getBranchingMean(), param.getBranchingSigma());

        sendLog(tr("Branching parameters changing:"));
        sendLog(tr("Mean = %1, lower limit = %2, upper limit = %3.").arg(QString::number(mean), QString::number(lowerLimit), QString::number(upperLimit)));
    }
}

qint64 Generator::countAllPins(PinType type)
{
    qint64 res = 0;
    for(NodeElement* element: elements)
    {
        LibraryElement libElement = LibraryUtils::getCorrespondingElement(element->getElement(), param.getLibraries());
        for(Pin& p: libElement.getPins())
        {
            if(p.getType() == type)
                res ++;
        }
    }

    return res;
}

void Generator::generateWiresForOutput(NodeElement* element, Pin p)
{
    std::uniform_real_distribution<double> wireRandom(0, 1);
    int branching = getTruncatedDistributedValue(branchingDistribution, param.getBranchingLowerLimit(), param.getBranchingUpperLimit());

    for(int i=0; i<branching; i++)
    {
        if(stopped) return;

        double chance = wireRandom(mt);
        if(element->getNodeNumber() == freeNodeElementIndex || chance > param.getInNodeWireChance())
        {
            tryGenerateInterNodeWire(element, p, branching);
            continue;
        }

        if (!tryGenerateInNodeWire(element, p, branching))
            tryGenerateInterNodeWire(element, p, branching);
    }
}

void Generator::tryGenerateInterNodeWire(NodeElement* element, Pin p, int attempts)
{
    for(int i=0; i<attempts; i++)
    {
        try
        {
            std::pair<NodeElement*, Pin> pair = getRandomPin();
            Wire w = buildWire(element, p, pair.first, pair.second, WireType::InterNode);

            wires.append(w);
            currentWireIndex ++;

            return;
        }
        catch(Exception& e)
        {
            if(i == attempts - 1)
                sendLog(e.what());
        }
    }
}

bool Generator::tryGenerateInNodeWire(NodeElement* element, Pin p, int attempts)
{
    for(int i=0; i<attempts; i++)
    {
        try
        {
            std::pair<NodeElement*, Pin> pair = getRandomPin(element->getNodeNumber());
            Wire w = buildWire(element, p, pair.first, pair.second, WireType::InNode);

            wires.append(w);
            currentWireIndex ++;

            return true;
        }
        catch(Exception&) {}
    }

    return false;
}

std::pair<NodeElement*, Pin> Generator::getRandomPin(int node)
{
    QList<NodeElement*> chosenElements;

    if(node == freeNodeElementIndex)
        chosenElements = elements;
    else
        chosenElements = groupedElements.at(node);

    for(int i=0; i<chosenElements.size(); i++)
    {
        if(chosenElements[i]->getAvailableInputPins().empty())
        {
            chosenElements.removeAt(i);
            i --;
        }
    }

    if(chosenElements.empty())
    {
        throw Exception(tr("Cannot get available input pin without any wires connected."));
    }

    std::uniform_int_distribution<int> elementRandom(0, chosenElements.size() - 1);

    NodeElement* element = chosenElements[elementRandom(mt)];

    std::uniform_int_distribution<int> pinRandom(0, element->getAvailableInputPins().size() - 1);

    int i = pinRandom(mt);
    Pin pin = element->getAvailableInputPins()[i];

    element->getAvailableInputPins().removeAt(i);

    return std::make_pair(element, pin);
}

Wire Generator::buildWire(NodeElement* sourceElement, Pin sourcePin, NodeElement* destElement, Pin destPin, WireType type)
{
    Wire wire(sourceElement->getElement().getIndex(), sourcePin.getId(), destElement->getElement().getIndex(), destPin.getId(), type, currentWireIndex);

    return wire;
}

void Generator::generateAliases(Scheme *scheme)
{
    sendLog(tr("Aliases generation."));

    Aliaser aliaser(3);
    aliaser.execute(scheme);
}
