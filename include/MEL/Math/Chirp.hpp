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

#ifndef MEL_CHIRP_HPP
#define MEL_CHIRP_HPP

#include <MEL/Config.hpp>
#include <MEL/Math/TimeFunction.hpp>
#include <MEL/Core/Frequency.hpp>

namespace mel {

    //==============================================================================
    // CLASS DECLARATION
    //==============================================================================

    /// A function whose frequency increases or decreases linearly as a function of time
    class MEL_API Chirp : public TimeFunction {
    public:



    public:
        /// Default constructor
        Chirp(Frequency start,
              Frequency final,
              Time T,
              double amplitude = 1.0,
              double offset    = 0.0);

        /// Evaluates the Chirp at Time t up until time T and then returns offset (default 0.0)
        double evaluate(Time t) override;

    public:
        Frequency start;   ///< The starting frequency (t = 0)
        Frequency final;   ///< The final frequency (t = T)
        Time T;            ///< The time required to sweep from intial to final
        double amplitude;  ///< The waveform peak amplitude
        double offset;     ///< The waveform offset from zero
    };

}  // namespace mel

#endif  // MEL_CHIRP_HPP
