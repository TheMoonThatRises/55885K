#include "KRONOS/helpers.h"

#ifndef _KRONOS_QUEUEAPI_QUEUEITEM_HPP_
#define _KRONOS_QUEUEAPI_QUEUEITEM_HPP_

class QueueItem {
    protected:
        std::vector<std::string> env;
        std::string id;
        queueFunction run;
    public:
        /*
            @param id
            @param run
            @param env
        */
        QueueItem(const std::string& id, const queueFunction run, const std::vector<std::string>& env);
        std::string getId();
        bool runFunction();
};

#endif