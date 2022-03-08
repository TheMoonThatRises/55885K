#include "KRONOS/QueueApi/queueItem.hpp"

QueueItem::QueueItem(std::string  id, queueFunction  run, std::vector<std::string> env):
    env(std::move(env)),
    id(std::move(id)),
    run(std::move(run))
{

}

std::string QueueItem::getId() {
    return id;
}

bool QueueItem::runFunction() {
    try {
        run(env);

        return true;
    } catch (std::exception& exception) {
        return false;
    }
}