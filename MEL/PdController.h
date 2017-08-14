#pragma once


namespace mel {


    class PdController {

    public:
 
        /// Default constructor
        PdController() : kp_(0.0), kd_(0.0) {}
        /// Prefered constructor
        PdController(double kp, double kd) : kp_(kp), kd_(kd) {};

        /// Calculates the control effort given the current state and desired reference 
        double calculate(double x_ref, double x, double xdot_ref, double xdot) {
            e_ = x_ref - x;
            edot_ = xdot_ref - xdot;
            effort_ = kp_ * e_  + kd_ * edot_;
        }

        double kp_; ///< the proportional control gain
        double kd_; ///< the derivative control gain

        double e_; ///< the error between the process varable x and the setpoint x_ref
        double edot_; ///< the error between xdot and xdot_ref
        double effort_; ///< the calculated control effort from the PD controller

    };

}