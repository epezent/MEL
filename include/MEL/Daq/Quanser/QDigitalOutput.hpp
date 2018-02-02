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
// Author(s): Evan Pezent (epezent@rice.edu)

#ifndef MEL_QDIGITALOUTPUT_HPP
#define MEL_QDIGITALOUTPUT_HPP

#include <MEL/Daq/Output.hpp>
#include <MEL/Utility/NonCopyable.hpp>

namespace mel {

    //==============================================================================
    // FORWARD DECLARATIONS
    //==============================================================================

    class QDaq;

    //==============================================================================
    // CLASS DECLARATION
    //==============================================================================

    class QDigitalOutput : public Output<logic>, NonCopyable {

    public:

        QDigitalOutput(QDaq& daq, const std::vector<uint32>& channel_numbers);

        ~QDigitalOutput();

        bool enable() override;

        bool disable() override;

        bool update() override;

        bool update_channel(uint32 channel_number) override;

        bool set_expire_values(const std::vector<logic>& expire_values) override;

        bool set_expire_value(uint32 channel_number, logic expire_value) override;

    private:

        QDaq& daq_;  ///< Reference to parent QDaq

    };

} // namespace mel

#endif // MEL_QDIGITALOUTPUT_HPP

  //==============================================================================
  // CLASS DOCUMENTATION
  //==============================================================================
