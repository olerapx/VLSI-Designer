#ifndef DECOMPOSITIONALGORITHM_H
#define DECOMPOSITIONALGORITHM_H

#include <QObject>
#include <QList>

#include "datamodels/scheme/scheme.h"

/**
 * @brief The DecompositionAlgorithm class
 * Base class for decomposition algorithms.
 */
class DecompositionAlgorithm: public QObject
{
    Q_OBJECT

public:
    DecompositionAlgorithm(Scheme* scheme, int number);

    /**
     * @brief execute
     * Performs the decomposition of the given scheme on the given number of parts.
     * @return the list of schemes.
     */
    virtual QList<Scheme*> execute() = 0;

signals:
    /**
     * @brief sendSchemes
     * Emits when the process is finished.
     * @param list - the list of schemes.
     */
    void sendSchemes(QList<Scheme*> list);

    /**
     * @brief sendFinish
     * Emits when the process is finished or interrupted.
     */
    void sendFinish();

    /**
     * @brief sendError
     * Emits when an exception is occurred.
     * @param error - the exception text.
     */
    void sendError(QString error);

    /**
     * @brief sendLog
     * @param log
     */
    void sendLog(QString log);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

    /**
     * @brief onStop
     * Requests the algorithm to stop.
     * When the algorithm will be stopped, a sendFinish will be emitted.
     */
    void onStop();

protected:
    Scheme* scheme;
    int number;

    bool stopped;
    bool actuallyStopped;
};

#endif // DECOMPOSITIONALGORITHM_H
