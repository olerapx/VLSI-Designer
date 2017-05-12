#include "generator.h"

Generator::Generator(GeneratorParameters param):
    param(param),
    mt{static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())},
    nodeCapacityDistribution(param.getNodeCapacityMean(), param.getNodeCapacitySigma()),
    branchingDistribution(param.getBranchingMean(), param.getBranchingSigma()),
    libraryRandom(0, param.getLibraries().size() - 1)
{
    stopped = true;
    mt.discard(1000);
}

Generator::~Generator()
{
    for(Library* l: param.getLibraries())
        delete l;
}

Scheme Generator::generate()
{
    stopped = false;

    QList<NodeElement> elements = generateElements();
    QList<Wire> wires = generateWires(elements);

    Scheme scheme;

    for(NodeElement el: elements)
        scheme.getElements().append(el.getElement());

    scheme.getWires().append(wires);

    stopped = true;

    return scheme;
}

QList<NodeElement> Generator::generateElements()
{
    QList<NodeElement> elements;

    currentIndex = 0;

    int capacity = 0;
    int elapsedElements = param.getElementsNumber();
    int currentNodeNumber = 1;

    while((capacity = pow(2, getTruncatedDistributedValue(nodeCapacityDistribution, param.getNodeCapacityLeftLimit(), param.getNodeCapacityRightLimit())))
          <= elapsedElements)
    {
        SchemeElement element = getRandomElement();

        for(int i=0; i<capacity; i++)
        {
            SchemeElement el (element);

            if (i > 0)
            {
                el.setIndex(currentIndex);
                currentIndex ++;
            }

            elements.append(NodeElement(el, currentNodeNumber));
        }

        currentNodeNumber ++;

        elapsedElements -= capacity;
    }

    for(int i=0; i<elapsedElements; i++)
        elements.append(NodeElement(getRandomElement(), 0));

    return elements;
}

int Generator::getTruncatedDistributedValue(std::normal_distribution<>& dist, int leftRange, int rightRange)
{
    while(true)
    {
        int value = round(dist(mt));
        if (value >= leftRange && value <= rightRange)
            return value;
    }
}

SchemeElement Generator::getRandomElement()
{
    Library* library = param.getLibraries()[libraryRandom(mt)];

    std::uniform_int_distribution<int> libraryElementRandom(0, library->getElements().size() - 1);
    LibraryElement element = library->getElements()[libraryElementRandom(mt)];

    SchemeElement res(library->getId(), element.getId(), currentIndex);
    currentIndex ++;

    return res;
}

QList<Wire> Generator::generateWires(QList<NodeElement> elements)
{
    QList<Wire> res;

    for(int i=0; i<elements.size(); i++)
    {
        LibraryElement el = getCorrespondingElement(elements[i].getElement());

        for(Pin p: el.getPins())
        {
            if(p.getType() == PinType::Output)
            {
                generateWiresForOutput(elements[i], el, p, res);
            }
        }
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

    throw Exception("Corresponding library element cannot be found.");
}

void Generator::generateWiresForOutput(NodeElement element, LibraryElement el, Pin p, QList<Wire>& wires)
{

}
