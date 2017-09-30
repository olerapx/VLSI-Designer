#pragma once

#include <QObject>

#include "exception/threadstoppedexception.h"

/**
 * @brief The Threadable class
 * Base class for all objects which can work in a separate thread.
 * Provides basic data exchanging and stopping capability.
 */
class Threadable : public QObject
{
    Q_OBJECT

public:
    Threadable();
    virtual ~Threadable() {}

    /**
     * @brief isStopped
     * Use this to safely dispose the working thread.
     * @return whether the process is stopped or not.
     */
    bool isStopped() { return actuallyStopped; }

signals:
    /**
     * @brief sendFinish
     * Emits when the process is finished or interrupted because of any reason.
     * From that moment on, the process will be stopped.
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
     * Starts the process.
     */
    virtual void onStart() = 0;

    /**
     * @brief onStop
     * Requests the process to stop.
     * When the process will be stopped, a sendFinish will be emitted.
     */
    void onStop();

protected:
    bool stopped;
    bool actuallyStopped;
};
