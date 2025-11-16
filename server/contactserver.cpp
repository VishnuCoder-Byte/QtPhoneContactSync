#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>

struct Contact {
    int index;
    char name[64];
    char number[32];
};

const char* MQ_CONTACT = "/contact_mq";
const char* MQ_REQUEST = "/request_mq";

int main() {
    mq_unlink(MQ_CONTACT);
    mq_unlink(MQ_REQUEST);

    // Attributes for contact message queue
    struct mq_attr attr_messages;
    attr_messages.mq_flags = 0;
    attr_messages.mq_maxmsg = 10;
    attr_messages.mq_msgsize = sizeof(Contact);
    attr_messages.mq_curmsgs = 0;

    // Attributes for request queue
    struct mq_attr attr_request;
    attr_request.mq_flags = 0;
    attr_request.mq_maxmsg = 10;
    attr_request.mq_msgsize = sizeof(int);
    attr_request.mq_curmsgs = 0;

    // Open message queues
    mqd_t mq_contact = mq_open(MQ_CONTACT, O_CREAT | O_WRONLY, 0644, &attr_messages);
    if (mq_contact == (mqd_t)-1) {
        perror("mq_open contact_mq");
        return 1;
    }

    mqd_t mq_request = mq_open(MQ_REQUEST, O_CREAT | O_RDONLY, 0644, &attr_request);
    if (mq_request == (mqd_t)-1) {
        perror("mq_open request_mq");
        return 1;
    }

    const int TOTAL = 100;
    int sentCount = 0;
    const int batchSize = 20;

    std::cout << "[Server] Sending initial batch of " << batchSize << " contacts...\n";

    // Send initial batch
    for (int i = 0; i < batchSize && sentCount < TOTAL; ++i) {
        Contact c;
        c.index = sentCount;
        snprintf(c.name, sizeof(c.name), "Contact %d", c.index);
        snprintf(c.number, sizeof(c.number), "555-010-%03d", c.index);
        if (mq_send(mq_contact, (char*)&c, sizeof(Contact), 0) == -1) {
            perror("mq_send");
        }
        sentCount++;
    }

    std::cout << "[Server] Initial batch sent. Waiting for requests...\n";

    while (sentCount < TOTAL) {
        int selectedIndex = -1;
        ssize_t bytes = mq_receive(mq_request, (char*)&selectedIndex, sizeof(int), nullptr);

        if (bytes > 0) {
            std::cout << "[Server] Selected index received from Qt: " << selectedIndex << "\n";

            // Send next batch if Qt reaches 18th element of the current batch
            int currentBatchStart = sentCount - batchSize;
            int prefetchIndex = currentBatchStart + 18; // 18th element of current batch
            std::cout << "[Server] prefetchIndex received from Qt: " << prefetchIndex << "\n";
            if (selectedIndex == prefetchIndex) {
                int batch = std::min(batchSize, TOTAL - sentCount);

                for (int i = 0; i < batch; i++) {
                    Contact c;
                    c.index = sentCount;
                    snprintf(c.name, sizeof(c.name), "Contact %d", c.index);
                    snprintf(c.number, sizeof(c.number), "555-010-%03d", c.index);

                    if (mq_send(mq_contact, (char*)&c, sizeof(Contact), 0) == -1) {
                        perror("mq_send");
                    }
                    sentCount++;
                }
                std::cout << "[Server] Sent next batch of " << batch << " contacts.\n";
            }
        }
    }

    mq_close(mq_contact);
    mq_close(mq_request);

    std::cout << "[Server] All contacts sent. Exiting.\n";
    return 0;
}

