#ifndef QUEUEWORKER_H
#define QUEUEWORKER_H

#include <QThread>
#include "ContactModel.h"
#include <mqueue.h>

class QueueWorker : public QThread
{
    Q_OBJECT
public:
    explicit QueueWorker(ContactModel* model, QObject* parent = nullptr);
    ~QueueWorker();

    void run() override;

    Q_INVOKABLE void sendSelectedIndex(int index);
    Q_INVOKABLE void requestNextBatch(int index);  // explicit next batch request

private:
    ContactModel* m_model;
    mqd_t mq_request;
    bool running;
};

#endif // QUEUEWORKER_H
