//
// Created by fu on 3/13/18.
//

#ifndef ASRDEMO_ASRDEMO_STATUSLISTENER_HPP
#define ASRDEMO_ASRDEMO_STATUSLISTENER_HPP

#include <atomic>
#include "ResultListener.hpp"

namespace asrdemo {
/**
 * @brief 在ResultListener基础上增加判断识别是否结束
 */
class StatusListener : public ResultListener {
public:
    virtual void on_last_status(int status);
    virtual bool is_finished() const;
private:
    std::atomic_bool finished = ATOMIC_VAR_INIT(false);
};

}
#endif
