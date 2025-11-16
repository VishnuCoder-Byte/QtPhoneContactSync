#include "QueueWorker.h"
#include <QMetaObject>
#include <iostream>
#include <unistd.h>

const char* MQ_CONTACT = "/contact_mq";
const char* MQ_REQUEST = "/request_mq";

QueueWorker::QueueWorker(ContactModel* model, QObject* parent)
    : QThread(parent), m_model(model), running(true) {}

QueueWorker::~QueueWorker() {
    running = false;
    wait();
    if (mq_request != (mqd_t)-1) mq_close(mq_request);
}

void QueueWorker::run() {
    mqd_t mq_contact = mq_open(MQ_CONTACT, O_RDONLY | O_NONBLOCK);
    if (mq_contact == (mqd_t)-1) { perror("mq_open contact"); return; }

    mq_request = mq_open(MQ_REQUEST, O_WRONLY);
    if (mq_request == (mqd_t)-1) { perror("mq_open request"); return; }

    // Request first batch at startup
    int req = 0;
    mq_send(mq_request, (char*)&req, sizeof(int), 0);

    while (running) {
        Contact c;
        ssize_t bytes = mq_receive(mq_contact, (char*)&c, sizeof(Contact), nullptr);

        if (bytes > 0) {
            QMetaObject::invokeMethod(
                m_model,
                "addContact",
                Qt::QueuedConnection,
                Q_ARG(Contact, c)
            );
        }
        usleep(100000); // avoid busy loop
    }

    mq_close(mq_contact);
}

void QueueWorker::sendSelectedIndex(int index) {
    if (mq_request != (mqd_t)-1) {
        mq_send(mq_request, (char*)&index, sizeof(int), 0);
        std::cout << "[QueueWorker] Selected index sent: " << index << std::endl;
    }
}

void QueueWorker::requestNextBatch(int index) {
    if (mq_request != (mqd_t)-1) {
        int requestFlag = -1; // special value for next batch
        mq_send(mq_request, (char*)&index, sizeof(int), 0);
        std::cout << "[QueueWorker] Requested next batch from server" << std::endl;
    }
}
