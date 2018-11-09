// MIT License
//
// MEL - Mechatronics Engine & Library
// Copyright (c) 2018 Mechatronics and Haptic Interfaces Lab - Rice University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#ifndef MEL_PDCONTROLLER_HPP
#define MEL_PDCONTROLLER_HPP

#include <MEL/Config.hpp>

namespace mel {

//==============================================================================
// CLASS DECLARATION
//==============================================================================

class MEL_API PdController {
public:
    /// Constructor
    PdController(double kp = 0.0, double kd = 0.0);

    /// Sets proportional and derivative control gains
    void set_gains(double kp, double kd);

    /// Sets the proportional control gain
    void set_kp(double kp);

    /// Sets the derivative control gain
    void set_kd(double kd);

    /// Gets the proportional control gian
    double get_kp() const;

    /// Gets the derivative control gain
    double get_kd() const;

    /// Calculates the control effort given the current state and desired reference
    double operator()(double x_ref, double x, double xdot_ref, double xdot);

    /// Calculates the control effort given the current state and desired reference
    double calculate(double x_ref, double x, double xdot_ref, double xdot);

    /// Computes the control effort to move to a desired location #x_ref with a
    /// constant velocity #xdot_ref, then hold that position.
    /// #delta_time should be the elapsed time since the last call to this
    /// function (e.g. Clock.delta_time_). #hold_tol is the tolerance
    /// within which the controller switches from the moving state to the
    /// holding state. #break_tol is the tolerance within which the
    /// controller switches from a holding state to a moving state; it should
    /// be larger than #hold_tol
    double move_to_hold(double x_ref,
                        double x,
                        double xdot_ref,
                        double xdot,
                        double delta_time,
                        double hold_tol,
                        double break_tol);

    /// Resets move_to_hold function. Call this before calling move_to_hold
    /// again if there was a period of inactivity since the last call
    /// to move_to_hold. If this isn't called and move_to_hold is within the
    /// break_tol, the controller will snap to x_ref instead of
    /// moving smoothly to it.
    void reset_move_to_hold();

private:
    double kp_;          ///< the proportional control gain
    double kd_;          ///< the derivative control gain
    double last_x_;      ///< the last x used for move to hold
    bool holding_;       ///< true if holding
    bool move_started_;  ///< true if moving
};

}  // namespace mel

#endif  // MEL_PDCONTROLLER_HPP
