#include "KRONOS/QueueApi/queueItem.hpp"

QueueItem::QueueItem(const std::string& id, const queueFunction& run, const std::vector<std::string>& env):
    env(env),
    id(id),
    run(run)
{

}

std::string QueueItem::getId() {
    return id;
}

bool QueueItem::runFunction() {
    try {
        run(env);

        return true;
    } catch (std::exception exception) {
        return false;
    }
}