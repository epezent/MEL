#include "PdController.h"
#include "mel_math.h"

namespace mel {

    namespace core {

        double PdController::calculate(double x_ref, double x, double xdot_ref, double xdot) {
            e_ = x_ref - x;
            edot_ = xdot_ref - xdot;
            effort_ = kp_ * e_ + kd_ * edot_;
            return effort_;
        }

        double PdController::move_to_hold(double x_ref, double x, double xdot_ref, double xdot, double delta_time, double window, bool start_here) {
            if (start_here) {
                last_x_ = x;
                holding_ = false;
            }
            double next_x = last_x_ - mel::math::sign(x - x_ref) * xdot_ref * delta_time;
            last_x_ = next_x;
            if (abs(x_ref - x) < window)
                holding_ = true;
            if (holding_)
                return calculate(x_ref, x, 0, xdot);  // holding condition
            else
                return calculate(next_x, x, 0, xdot); // moving condition
        }

        double PdController::move_to_hold2(double x_ref, double x, double xdot_ref, double xdot, double delta_time, double tolerance) {
            if (std::abs(x_ref - x) < tolerance) {
                move_started_ = false;
                return calculate(x_ref, x, 0, xdot);                
            }
            else {
                if (!move_started_ || std::abs(last_x_ - x) > tolerance) {
                    last_x_ = x;
                    move_started_ = true;
                }
                double next_x = last_x_ - mel::math::sign(x - x_ref) * xdot_ref * delta_time;
                last_x_ = next_x;
                return calculate(next_x, x, 0, xdot); 
            }
        }


    }

}