// MIT License
//
// MEL - MAHI Exoskeleton Library
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
// Author(s): Craig McDonald (craig.g.mcdonald@gmail.com)

#ifndef MEL_FORCESENSOR_HPP
#define MEL_FORCESENSOR_HPP

#include <MEL/Core/Device.hpp>
#include <MEL/Daq/Input.hpp>

namespace mel {

//==============================================================================
// CLASS DECLARATION
//==============================================================================

class ForceSensor : public Device {

public:

    ForceSensor();
    ForceSensor(std::string name, std::vector<Input<voltage>::Channel> ai_channels);

    virtual std::vector<double> get_forces() = 0;

protected:

    std::vector<double> get_voltages();

protected:

    std::vector<Input<voltage>::Channel> ai_channels_; // the DAQ analog input channels bound to this sensor
    int num_channels_;
    std::vector<double> forces_;
    std::vector<double> voltages_;

};

} // namespace mel

#endif // MEL_FORCESENSOR_HPP

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
