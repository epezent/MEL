#include <MEL/Mechatronics/PdController.hpp>
#include <MEL/Math/Functions.hpp>

namespace mel {

PdController::PdController(double kp, double kd) :
    kp_(kp),
    kd_(kd),
    last_x_(0.0),
    holding_(false),
    move_started_(false)
{ }

void PdController::set_gains(double kp, double kd) {
    kp_ = kp;
    kd_ = kd;
}

void PdController::set_kp(double kp) {
    kp_ = kp;
}

void PdController::set_kd(double kd) {
    kd_ = kd;
}

double PdController::get_kp() const {
    return kp_;
}

double PdController::get_kd() const {
    return kd_;
}

double PdController::operator()(double x_ref, double x, double xdot_ref, double xdot) {
    return calculate(x_ref, x, xdot_ref, xdot);
}

double PdController::calculate(double x_ref, double x, double xdot_ref, double xdot) {
    return kp_ * (x_ref - x) + kd_ * (xdot_ref - xdot);
}

double PdController::move_to_hold(double x_ref, double x, double xdot_ref, double xdot, double delta_time, double hold_tol, double break_tol) {
    if (abs(x_ref - x) < break_tol && holding_) {
        move_started_ = false;
        return calculate(x_ref, x, 0, xdot);
    }
    else {
        holding_ = false;
        if (!move_started_) {
            last_x_ = x;
            move_started_ = true;
        }
        double next_x = last_x_ - sign(x - x_ref) * xdot_ref * delta_time;
        last_x_ = next_x;
        if (abs(x_ref - x) < hold_tol)
            holding_ = true;
        return calculate(next_x, x, 0, xdot);
    }
}

void PdController::reset_move_to_hold() {
    move_started_ = false;
    holding_ = false;
}

} // namespace mel
