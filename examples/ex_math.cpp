// MIT License
//
// MEL - Mechatronics Engine & Library
// Copyright (c) 2019 Mechatronics and Haptic Interfaces Lab - Rice University
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

#include <MEL/Math/Constants.hpp>
#include <MEL/Math/Differentiator.hpp>
#include <MEL/Math/Functions.hpp>
#include <MEL/Math/Integrator.hpp>
#include <MEL/Core/Console.hpp>
#include <MEL/Core/Timer.hpp>
#include <MEL/Logging/DataLogger.hpp>

using namespace mel;

int main() {

    // creat loop timer
    Timer timer(hertz(100), Timer::Hybrid);

    // create Differentiator and Integrators
    Differentiator differentiator;
    Integrator integrator = Integrator(5, Integrator::Technique::Simpsons);

    // create data logger
    DataLogger data_logger(WriterType::Buffered, false);
    std::vector<std::string> header = { "t", "x(t)", "dx/dt", "dx/dt", "integral(x)", "integral(x)" };
    data_logger.set_header(header);
    std::vector<double> data(6);

    // loop
    while (timer.get_elapsed_time_actual() < seconds(5)) {
        // get current time in seconds
        double t = timer.get_elapsed_time().as_seconds();
        data[0] = t;
        // x = 1/2 * [ cos(t) + 7 * cos(7t) ]
        data[1] = 0.5 * (mel::cos(t) + 7.0 * mel::cos(7.0 * t));
        // dx/dt = 1/2 * [-sin(t) - 49 * sin(7t)]
        data[2] = 0.5 * (-mel::sin(t) - 49.0 * mel::sin(7.0 * t));
        // dx/dt ~ differentiate(x);
        data[3] = differentiator.update(data[1], timer.get_elapsed_time());
        // integral(x) = sin(4t) * cos(3t) + 5
        data[4] = mel::sin(4 * t) * mel::cos(3 * t) + 5;
        // integral(x) ~ integrate(dx/dt)
        data[5] = integrator.update(data[1], timer.get_elapsed_time());
        // write to data log buffer
        data_logger.buffer(data);
        // wait timer
        timer.wait();
    }

    // save data
    data_logger.save_data("example_math_data");

    return 0;
}
