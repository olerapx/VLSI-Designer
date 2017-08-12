#ifndef DECOMPOSITIONALGORITHM_H
#define DECOMPOSITIONALGORITHM_H

#include <QObject>
#include <QList>

#include "threadable.h"
#include "datamodels/scheme/scheme.h"

/**
 * @brief The DecompositionAlgorithm class
 * Base class for decomposition algorithms.
 */
class DecompositionAlgorithm : public Threadable
{
    Q_OBJECT

public:
    DecompositionAlgorithm(Scheme* scheme, int number);

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param scheme
     * @param number
     * @throw Exception
     */
    void setParameters(Scheme* scheme, int number);

    /**
     * @brief execute
     * Performs the decomposition of the given scheme on the given number of parts.
     * @return the list of schemes.
     */
    virtual QList<Scheme*> execute() = 0;

signals:
    /**
     * @brief sendResult
     * Emits when the process is finished.
     * @param list - the list of schemes.
     */
    void sendResult(QList<Scheme*> list);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

protected:
    Scheme* scheme;
    int number;
};

#endif // DECOMPOSITIONALGORITHM_H
