#include <MEL/Daq/Quanser/QDaq.hpp>
#include <MEL/Daq/Quanser/QDigitalInputOutput.hpp>
#include <hil.h>

namespace mel {

    //==============================================================================
    // CLASS DEFINITIONS
    //==============================================================================

    QDigitalInputOutput::QDigitalInputOutput(QDaq& daq, const std::vector<uint32>& channel_numbers, const std::vector<Direction>& directions) :
        InputOutput(daq.name_ + "_digital_input", channel_numbers, directions),
        daq_(daq)
    {
    }

    QDigitalInputOutput::~QDigitalInputOutput() {

    }

    bool QDigitalInputOutput::enable() {
        if (enabled_)
            return true;
        print("Enabling " + namify(name_) + " ... ", false);
        set_values(enable_values_);
        if (update()) {
            print("Done");
            return Device::enable();
        }
        print("Failed");
        return false;
    }

    bool QDigitalInputOutput::disable() {
        if (!enabled_)
            return true;
        print("Disabling " + namify(name_) + " ... ", false);
        set_values(disable_values_);
        if (update()) {
            print("Done");
            return Device::disable();
        }
        print("Failed");
        return false;
    }

    bool QDigitalInputOutput::update() {
        if (daq_.open_) {
            // create intermediate buffers
            std::vector<logic> read_buffer(input_channel_numbers_.size());
            std::vector<logic> write_buffer(output_channel_numbers_.size());
            for (std::size_t i = 0; i < output_channel_numbers_.size(); ++i)
                write_buffer[i] = values_[channel_map_.at(output_channel_numbers_[i])];
            t_error result;
            result = hil_read_digital_write_digital(daq_.handle_,
                &input_channel_numbers_[0], static_cast<uint32>(input_channel_numbers_.size()),
                &output_channel_numbers_[0], static_cast<uint32>(output_channel_numbers_.size()),
                &read_buffer[0], &write_buffer[0]);
            for (std::size_t i = 0; i < input_channel_numbers_.size(); ++i)
                values_[channel_map_.at(input_channel_numbers_[i])] = read_buffer[i];
            if (result == 0)
                return true;
            else {
                print(QDaq::get_quanser_error_message(result));
                return false;
            }
        }
        else {
            print(namify(daq_.get_name()) + " has not been opened; unable to call " + __FUNCTION__);
            return false;
        }
    }

    bool QDigitalInputOutput::update_channel(uint32 channel_number) {
        if (daq_.open_) {
            t_error result;
            if (directions_[channel_map_.at(channel_number)] == Direction::Input)
                result = hil_read_digital(daq_.handle_, &channel_number, static_cast<uint32>(1), &values_[channel_map_.at(channel_number)]);
            else if (directions_[channel_map_.at(channel_number)] == Direction::Output)
                result = hil_write_digital(daq_.handle_, &channel_number, static_cast<uint32>(1), &values_[channel_map_.at(channel_number)]);
            if (result == 0)
                return true;
            else {
                print(QDaq::get_quanser_error_message(result));
                return false;
            }
        }
        else {
            print(namify(daq_.get_name()) + " has not been opened; unable to call " + __FUNCTION__);
            return false;
        }
    }

    bool QDigitalInputOutput::set_directions(const std::vector<Direction>& directions) {
        if (!InputOutput::set_directions(directions))
            return false;
        if (daq_.open_) {
            t_error result;
            result = hil_set_digital_directions(daq_.handle_,
                &input_channel_numbers_[0], static_cast<uint32>(input_channel_numbers_.size()),
                &output_channel_numbers_[0], static_cast<uint32>(output_channel_numbers_.size()));
            if (result == 0)
                return true;
        }
        else {
            print(namify(daq_.get_name()) + " has not been opened; unable to call " + __FUNCTION__);
            return false;
        }
    }

    bool QDigitalInputOutput::set_direction(uint32 channel_number, Direction direction) {
        if (!InputOutput::set_direction(channel_number, direction))
            return false;
        return set_directions(directions_);
    }

    bool QDigitalInputOutput::set_expire_values(const std::vector<logic>& expire_values) {
        if (!InputOutput::set_expire_values(expire_values))
            return false;
        if (daq_.open_) {
            // convert MEL logic to Quanser t_encoder_quadratue_mode
            std::vector<t_digital_state> converted_expire_values;
            for (auto it = expire_values.begin(); it != expire_values.end(); ++it) {
                if (*it == HIGH)
                    converted_expire_values.push_back(DIGITAL_STATE_HIGH);
                else
                    converted_expire_values.push_back(DIGITAL_STATE_LOW);
            }
            t_error result;
            result = hil_watchdog_set_digital_expiration_state(daq_.handle_, &channel_numbers_[0], static_cast<uint32>(channel_count_), &converted_expire_values[0]);
            if (result == 0)
                return true;
            else {
                print(QDaq::get_quanser_error_message(result));
                return false;
            }
        }
        else {
            print(namify(daq_.get_name()) + " has not been opened; unable to call " + __FUNCTION__);
            return false;
        }
    }

    bool QDigitalInputOutput::set_expire_value(uint32 channel_number, logic expire_value) {
        if (!InputOutput::set_expire_value(channel_number, expire_value))
            return false;
        if (daq_.open_) {
            // convert MEL logic to Quanser t_encoder_quadratue_mode
            t_digital_state converted_expire_value;
            if (expire_value == HIGH)
                converted_expire_value = DIGITAL_STATE_HIGH;
            else
                converted_expire_value = DIGITAL_STATE_LOW;
            t_error result;
            result = hil_watchdog_set_digital_expiration_state(daq_.handle_, &channel_number, static_cast<uint32>(1), &converted_expire_value);
            if (result == 0)
                return true;
            else {
                print(QDaq::get_quanser_error_message(result));
                return false;
            }
        }
        else {
            print(namify(daq_.get_name()) + " has not been opened; unable to call " + __FUNCTION__);
            return false;
        }
    }

} // namespace mel