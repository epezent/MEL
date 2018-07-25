#include <MEL/Daq/Quanser/QuanserDaq.hpp>
#include <MEL/Daq/Quanser/QuanserAI.hpp>
#include <MEL/Logging/Log.hpp>
#include <hil.h>

namespace mel {

//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

QuanserAI::QuanserAI(QuanserDaq& daq, const std::vector<uint32>& channel_numbers) :
    Module(daq.get_name() + "_analog_input", IoType::InputOnly, channel_numbers),
    daq_(daq)
{
}

QuanserAI::~QuanserAI() {

}

bool QuanserAI::enable() {
    return Device::enable();
}

bool QuanserAI::disable() {
    return Device::disable();
}

bool QuanserAI::update() {
    if (!daq_.open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
                   << daq_.get_name() << " is not open";
        return false;
    }
    t_error result;
    result = hil_read_analog(daq_.handle_, &channel_numbers_[0], static_cast<uint32>(channel_count_), &values_[0]);
    if (result == 0)
        return true;
    else {
        LOG(Error) << "Failed to update " << get_name() << " "
            << QuanserDaq::get_quanser_error_message(result);
        return false;
    }
}

bool QuanserAI::update_channel(uint32 channel_number) {
    if (!daq_.open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
                   << daq_.get_name() << " is not open";
        return false;
    }
    t_error result;
    result = hil_read_analog(daq_.handle_, &channel_number, static_cast<uint32>(1), &values_[channel_map_.at(channel_number)]);
    if (result == 0)
        return true;
    else {
        LOG(Error) << "Failed to update " << get_name() << " channel number " << channel_number << " "
            << QuanserDaq::get_quanser_error_message(result);
        return false;
    }
}

bool QuanserAI::set_ranges(const std::vector<Voltage>& min_values, const std::vector<Voltage>& max_values) {
    if (!daq_.open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << daq_.get_name() << " is not open";
        return false;
    }
    if (!Module::set_ranges(min_values, max_values))
        return false;
    t_error result;
    result = hil_set_analog_input_ranges(daq_.handle_, &channel_numbers_[0], static_cast<uint32>(channel_count_), &min_values_[0], &max_values_[0]);
    if (result == 0) {
        LOG(Verbose) << "Set " << get_name() << "ranges to min=" << min_values << ", max=" << max_values;
        return true;
    }
    else {
        LOG(Error) << "Failed to set " << get_name() << " ranges "
            << QuanserDaq::get_quanser_error_message(result);
        return false;
    }
}

bool QuanserAI::set_range(uint32 channel_number, Voltage min_value, Voltage max_value) {
    if (!daq_.open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << daq_.get_name() << " is not open";
        return false;
    }
    if (!Module::set_range(channel_number, min_value, max_value))
        return false;
    t_error result;
    result = hil_set_analog_input_ranges(daq_.handle_, &channel_number, static_cast<uint32>(1), &min_values_[channel_map_.at(channel_number)], &max_values_[channel_map_.at(channel_number)]);
    if (result == 0) {
        LOG(Verbose) << "Set " << get_name() << " channel number " << channel_number << " range to min=" << min_value << ", max=" << max_value;
        return true;
    }
    else {
        LOG(Error) << "Failed to set " << get_name() << " channel number " << channel_number << " range "
            << QuanserDaq::get_quanser_error_message(result);
        return false;
    }
}

} // namespace mel