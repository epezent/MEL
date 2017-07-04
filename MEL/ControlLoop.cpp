#include "ControlLoop.h"

namespace mel {

    bool ControlLoop::stop_ = false;
    bool ControlLoop::pause_ = false;

    ControlLoop::ControlLoop(Clock& clock) :
        clock_(clock),
        stop_time_(-1)
    {
        // register signal SIGINT and SIGBREAK with ctrl_c_handler */
        signal(SIGINT, ctrl_c_handler);
        signal(SIGBREAK, ctrl_c_handler);
    }

    void ControlLoop::queue_controller(Controller* controller) {
        // connect the controller Clock to the control loop Clock
        controller->clock_ = &clock_;
        // add the controller to the queue
        controllers_.push_back(controller);
    }

    void ControlLoop::execute(uint32 stop_time_seconds) {

        stop_time_ = stop_time_seconds;

        // start the Controller(s)
        for (auto it = controllers_.begin(); it != controllers_.end(); ++it)
            (*it)->start();

        // start the Clock
        clock_.start();

        // start the control loop
        while (!stop_ && clock_.get_time() <= stop_time_) {

            // step the Controller(s)
            for (auto it = controllers_.begin(); it != controllers_.end(); ++it)
                (*it)->step();

            // wait for the next clock tick
            clock_.wait();
        }

        // stop the Clock
        clock_.stop();

        // stop the Controller(s) in REVERSE order
        for (auto it = controllers_.rbegin(); it != controllers_.rend(); ++it)
            (*it)->stop();

        // reset stop_
        stop_ = false;
    }

    void ControlLoop::ctrl_c_handler(int signum) {
        stop_ = true;
    }

    void ControlLoop::ctrl_break_handler(int signum) {
        pause_ = !pause_;
    }

}