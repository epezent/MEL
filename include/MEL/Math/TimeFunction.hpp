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

#ifndef MEL_TIME_FUNCTION_HPP
#define MEL_TIME_FUNCTION_HPP

#include <MEL/Config.hpp>
#include <MEL/Core/Time.hpp>

namespace mel {

/// Base class for functions which are evaluated in the time domain
class MEL_API TimeFunction {
public:

    /// Pure virtual function which must be implemented
    virtual double evaluate(Time t) = 0;

    /// Evaluates the function at Time t
    double operator()(Time t);
};

} // namespace mel

#endif // MEL_TIME_FUNCTION_HPP

