#include "KRONOS/helpers.h"
#include "queueItem.hpp"

#ifndef _KRONOS_QUEUEAPI_QUEUE_HPP_
#define _KRONOS_QUEUEAPI_QUEUE_HPP_

class QueueManager {
	protected:
        std::map<int, QueueItem> queue;
    public:
        ~QueueManager();
        void runQueue();
        bool addQueue(const int& priority, QueueItem queueItem);
        bool removeQueue(const std::string& id);
        bool cleanQueue();
};

#endif