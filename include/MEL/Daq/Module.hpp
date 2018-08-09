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

#ifndef MEL_MODULE_HPP
#define MEL_MODULE_HPP

#include <MEL/Core/Device.hpp>
#include <MEL/Daq/Buffer.hpp>
#include <map>

#ifdef _MSC_VER
#pragma warning( disable : 4589 )
#endif

namespace mel {

//==============================================================================
// BASE DECLARATION
//==============================================================================

/// Defines non-templated Module functions/members
class MEL_API ModuleBase : public Device {

public:

    /// Default Constructor
    ModuleBase();

    /// Calls the Modules's API to update a single channel with the real-world.
    virtual bool update_channel(uint32 channel_number) = 0;

    /// Calls the Modules's API to update all channels with the real-world.
    /// By default, iteratively calls update_channel() on all channel numbers.
    virtual bool update();

    /// Sets the vector of channel numbers this Module maintains
    void set_channel_numbers(const std::vector<uint32>& channel_numbers);

    /// Gets the vector of channel numbers this Module maintains
    const std::vector<uint32>& get_channel_numbers() const;

    /// Returns the number of channels on this Module
    std::size_t get_channel_count() const;

    /// Checks if a channel number is a number defined on this Module
    bool validate_channel_number(uint32 channel_number) const;

    /// Checks if the size of a vector equals the number of channels
    bool validate_channel_count(std::size_t size) const;

private:

    friend class BufferBase;

    /// Adds a value container to this Module
    void add_container(BufferBase* container);

    /// Returns vector index associated with channel number
    std::size_t index(uint32 channel_number) const;

private:

    std::vector<uint32> channel_numbers_;         ///< The channel numbers used by this ModuleBase
    std::map<uint32, std::size_t> channel_map_;   ///< Maps a channel number with a vector index position
    std::vector<BufferBase*> containers_; ///< Containers managed by this Module

};

//==============================================================================
// CLASS DECLARATION
//==============================================================================

/// Defines templated Module functions/members
template <typename T>
class Module : public ModuleBase {

public:

    Module() :
        values_(this),
        min_values_(this),
        max_values_(this)
    {}

    /// Destructor
    virtual ~Module() { }

    /// Sets the min and max possible values of each channel
    virtual bool set_ranges(const std::vector<T>& min_values, const std::vector<T>& max_values) {
        min_values_.set(min_values);
        max_values_.set(max_values);
        return true;
    }

    /// Sets the min and max possible value of a single channel
    virtual bool set_range(uint32 channel_number, T min_value, T max_value) {
        if (validate_channel_number(channel_number)) {
            min_values_[channel_number] = min_value;
            max_values_[channel_number] = max_value;
            return true;
        }
        return false;
    }

public:

    /// Gets non-const reference to the current channel values of this Module
    std::vector<T>& get_values() {
        return values_.get();
    }

    /// Gets the current value of a a single channel. If an invalid channel number
    /// is passed, the default value of the underlying channel type is returned.
    T get_value(uint32 channel_number) const {
        return values_[channel_number];
    }

    /// Sets the current channel values of this Module. If the incorrect number
    /// of values is pass, no values are set.
    void set_values(const std::vector<T>& values) {
        values_.set(values);
    }

    /// Sets the current value of a single channel. If an invalid channel number
    /// is passed, non value is set
    void set_value(uint32 channel_number, T value) {
        values_[channel_number] = value;
    }

protected:

    Buffer<T> values_;      ///< The real-world values of the channels in this Module
    Buffer<T> min_values_;  ///< The minimum possible values of each channel
    Buffer<T> max_values_;  ///< The maximum possible values of each channel

};

} // namespace mel

#endif // MEL_MODULE_HPP

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
