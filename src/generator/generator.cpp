#include "generator.h"

Generator::Generator(GeneratorParameters param):
    param(param),
    mt{static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())},
    nodeCapacityDistribution(param.getNodeCapacityMean(), param.getNodeCapacitySigma()),
    branchingDistribution(param.getBranchingMean(), param.getBranchingSigma()),
    libraryRandom(0, param.getLibraries().size() - 1)
{
    stopped = true;
    actuallyStopped = true;
    mt.discard(numbersToDiscard);
}

Generator::~Generator()
{
    for(Library* l: param.getLibraries())
        delete l;
}

void Generator::onStart()
{
    generate();
}

void Generator::onStop()
{
    stopped = true;
}

Scheme* Generator::generate()
{
    if(!actuallyStopped) return new Scheme();

    stopped = false;
    actuallyStopped = false;

    try
    {
        generateElements();
        generateWires();

        if(stopped)
        {
            actuallyStopped = true;
            sendFinish();
            return new Scheme();
        }

        Scheme* scheme = new Scheme();

        for(NodeElement el: elements)
            scheme->getElements().append(el.getElement());

        scheme->getWires().append(wires);

        generateAliases(scheme);

        stopped = true;
        actuallyStopped = true;

        sendScheme(scheme);
        sendFinish();

        return scheme;
    }
    catch(Exception e)
    {
        stopped = true;
        actuallyStopped = true;

        sendError(e.what());
        sendFinish();
        return new Scheme();
    }
}

void Generator::generateElements()
{
    elements.clear();
    groupedElements.clear();
    groupedElements.append(QList<NodeElement>());

    sendLog(tr("Functional nodes generation."));

    currentElementIndex = 0;

    int capacity = 0;
    int elapsedElements = param.getElementsNumber();
    int currentNodeNumber = 1;

    while((capacity = pow(2, getTruncatedDistributedValue(nodeCapacityDistribution, param.getNodeCapacityLowerLimit(), param.getNodeCapacityUpperLimit())))
          <= elapsedElements)
    {
        SchemeElement element = getRandomElement();

        groupedElements.append(QList<NodeElement>());

        for(int i=0; i<capacity; i++)
        {
            if(stopped) return;

            SchemeElement el = element;

            if (i > 0)
            {
                el.setIndex(currentElementIndex);
                currentElementIndex ++;
            }

            NodeElement nodeElement(el, currentNodeNumber);

            elements.append(nodeElement);
            groupedElements[groupedElements.size() - 1].append(nodeElement);
        }

        currentNodeNumber ++;

        elapsedElements -= capacity;
    }

    sendLog(tr("Free elements generation."));

    for(int i=0; i<elapsedElements; i++)
    {
        NodeElement nodeElement(getRandomElement(), freeNodeElementIndex);

        elements.append(nodeElement);
        groupedElements[0].append(nodeElement);
    }
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
    wires.clear();
    currentWireIndex = 0;

    checkBranching();

    sendLog(tr("Wires generation:"));

    int size = elements.size();

    for(int i=0; i<size; i++)
    {
        sendLog(tr("Element #%1 of %2").arg(QString::number(i), QString::number(size)));

        LibraryElement el = getCorrespondingElement(elements[i].getElement());

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

    qint64 inputPins = countAllInputPins();

    if(inputPins < param.getBranchingUpperLimit())
    {
        int upperLimit = inputPins;
        int lowerLimit = param.getBranchingLowerLimit();
        int mean = param.getBranchingMean();

        if(lowerLimit > upperLimit)
            lowerLimit = upperLimit;

        if(mean < lowerLimit || mean > upperLimit)
            mean = (lowerLimit + upperLimit) / 2;

        param.setBranching(mean, param.getBranchingSigma(), lowerLimit, upperLimit);

        sendLog(tr("Branching parameters changing:"));
        sendLog(tr("Mean = %1, lower limit = %2, upper limit = %3.").arg(mean, lowerLimit, upperLimit));
    }
}

qint64 Generator::countAllInputPins()
{
    qint64 res = 0;
    for(NodeElement element: elements)
    {
        LibraryElement el = getCorrespondingElement(element.getElement());
        for(Pin p: el.getPins())
            if(p.getType() == PinType::Input)
                res ++;
    }

    return res;
}

LibraryElement Generator::getCorrespondingElement(SchemeElement element)
{
    for(Library* l: param.getLibraries())
    {
        if(l->getId() == element.getLibraryId())
        {
            for(LibraryElement el: l->getElements())
            {
                if (el.getId() == element.getElementId())
                    return el;
            }
        }
    }

    throw Exception(tr("Corresponding library element cannot be found, passed element with id: %1.")
                    .arg(element.getElementId()));
}

void Generator::generateWiresForOutput(NodeElement& element, Pin p)
{
    std::uniform_real_distribution<double> wireRandom(0, 1);
    int branching = getTruncatedDistributedValue(branchingDistribution, param.getBranchingLowerLimit(), param.getBranchingUpperLimit());

    for(int i=0; i<branching; i++)
    {
        if(stopped) return;

        double chance = wireRandom(mt);
        if(element.getNodeNumber() == freeNodeElementIndex || chance > param.getInnerWireChance())
        {
            generateOuterWire(element, p);
            continue;
        }

        if (!tryGenerateInnerWire(element, p, branching))
            generateOuterWire(element, p);
    }
}

void Generator::generateOuterWire(NodeElement& element, Pin p)
{
    std::pair<NodeElement, Pin> pair = getRandomPin();
    Wire w = buildWire(element, p, pair.first, pair.second, WireType::Outer);

    while(isWireExist(element, w))
    {
        pair = getRandomPin();
        w = buildWire(element, p, pair.first, pair.second, WireType::Outer);
    }

    wires.append(w);
    element.getGeneratedWires().append(w);
    currentWireIndex ++;
}

bool Generator::tryGenerateInnerWire(NodeElement& element, Pin p, int attempts)
{
    std::pair<NodeElement, Pin> pair(getRandomPin(element.getNodeNumber()));
    Wire w = buildWire(element, p, pair.first, pair.second, WireType::Inner);

    for(int i=0; i<attempts; i++)
    {
        if(isWireExist(element, w))
        {
            pair = getRandomPin(element.getNodeNumber());
            w = buildWire(element, p, pair.first, pair.second, WireType::Inner);
        }
        else break;
    }

    if(isWireExist(element, w))
        return false;

    wires.append(w);
    element.getGeneratedWires().append(w);
    currentWireIndex ++;

    return true;
}

std::pair<NodeElement, Pin> Generator::getRandomPin(int node)
{
    QList<NodeElement> chosenElements;

    if(node == freeNodeElementIndex)
        chosenElements = elements;
    else
        chosenElements = groupedElements.at(node);

    std::uniform_int_distribution<int> elementRandom(0, chosenElements.size() - 1);

    while(true)
    {
        NodeElement element(chosenElements[elementRandom(mt)]);

        LibraryElement libElement(getCorrespondingElement(element.getElement()));

        std::uniform_int_distribution<int> pinRandom(0, libElement.getPins().size() - 1);
        Pin pin(libElement.getPins()[pinRandom(mt)]);

        if(pin.getType() == PinType::Input)
            return std::make_pair(element, pin);
    }
}

bool Generator::isWireExist(NodeElement sourceElement, Wire other)
{
    for(Wire w: sourceElement.getGeneratedWires())
    {
        if(w.getSrcIndex() == other.getSrcIndex() && w.getSrcPinId() == other.getSrcPinId() &&
                w.getDestIndex() == other.getDestIndex() && w.getDestPinId() == other.getDestPinId())
            return true;
    }

    return false;
}

Wire Generator::buildWire(NodeElement sourceElement, Pin sourcePin, NodeElement destElement, Pin destPin, WireType type)
{
    Wire wire(sourceElement.getElement().getIndex(), sourcePin.getId(), destElement.getElement().getIndex(), destPin.getId(), type, currentWireIndex);

    return wire;
}

void Generator::generateAliases(Scheme *scheme)
{
    sendLog(tr("Aliases generation."));

    Aliaser aliaser(3);
    aliaser.generate(scheme);
}
