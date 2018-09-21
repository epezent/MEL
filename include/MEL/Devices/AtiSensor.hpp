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
// Author(s): Evan Pezent (craig.g.mcdonald@gmail.com)

#ifndef MEL_ATI_SENSOR_HPP
#define MEL_ATI_SENSOR_HPP

#include <MEL/Mechatronics/ForceSensor.hpp>

namespace mel {

/// Implements an ATI force/torque transducer
class MEL_API AtiSensor {

public:
    AtiSensor(/* args */);

    void load_calibration(const std::string& filename);

private:
    /* data */
};

} // namespace mel

#endif // MEL_ATI_SENSOR_HPP
