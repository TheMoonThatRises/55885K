#include "KRONOS/QueueApi/queue.hpp"

QueueManager::~QueueManager() {
    cleanQueue();
}

void QueueManager::runQueue() {
    for (int priority = 0; priority < queue.size(); ++priority)
        queue.at(priority).runFunction();
}

bool QueueManager::addQueue(const int& priority, QueueItem queueItem) {
    try {
        bool containsId = false;

        for (auto [priority, item] : queue)
            if (item.getId() == queueItem.getId())
                removeQueue(item.getId());

        if (priority > queue.size())
            queue.insert({queue.size(), queueItem});
        else {
            for (int pos = priority; pos < queue.size(); ++pos) {
                auto item = queue.extract(pos);
                item.key() = pos + 1;
                queue.insert(std::move(item));
            }
            queue.insert({priority, queueItem});
        }
        return true;
    } catch (std::exception exception) {
        return false;
    }
}

bool QueueManager::removeQueue(const std::string& id) {
    try {
        bool foundId = false;

        for (int priority = 0; priority < queue.size(); ++priority)
            if (foundId) {
                auto item = queue.extract(priority);
                item.key() = priority - 1;
                queue.insert(std::move(item));
            } else if (queue.at(priority).getId() == id) {
                queue.erase(priority);
                foundId = true;
            }

        return true;
    } catch (std::exception exception) {
        return false;
    }
}

bool QueueManager::cleanQueue() {
    try {
        queue.clear();
        return queue.empty();
    } catch (std::exception exception) {
        return false;
    }
}