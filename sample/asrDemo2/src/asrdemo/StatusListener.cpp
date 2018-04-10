//
// Created by fu on 3/13/18.
//

#include "StatusListener.hpp"
namespace asrdemo {
void StatusListener::on_last_status(int status){
    finished = true;
}
bool StatusListener::is_finished() const{

    return finished;
}
}