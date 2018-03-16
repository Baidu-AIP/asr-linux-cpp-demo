//
// Created by fu on 3/13/18.
//

#include "StatusListener.hpp"
namespace asrdemo {
void StatusListener::output_callback(bds::BDSSDKMessage &message, int status){
    switch (status) {
        case bds::EVoiceRecognitionClientWorkStatusLongSpeechEnd:
        case bds::EVoiceRecognitionClientWorkStatusError:
        case bds::EVoiceRecognitionClientWorkStatusCancel: {
            // std::cout << status << " status finish" << std::endl;
            finished = true;
            break;
        }
    }
    ResultListener::output_callback(message,status);
}

bool StatusListener::is_finished() const{

    return finished;
}
}