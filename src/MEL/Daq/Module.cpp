#include <MEL/Daq/Module.hpp>
#include <MEL/Logging/Log.hpp>
#include <algorithm>

namespace mel {

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

ChanNums sort_and_reduce_channels(const ChanNums& channels) {
    ChanNums sorted_channels = channels;
    std::sort(sorted_channels.begin(), sorted_channels.end());
    sorted_channels.erase(std::unique(sorted_channels.begin(), sorted_channels.end()), sorted_channels.end());
    if (sorted_channels != channels) {
        LOG(Warning) << "Specifed channels " << channels << " sorted and reduced to " << sorted_channels;
    }
    return sorted_channels;
}

ChanMap make_channel_map(const ChanNums& channel_numbers) {
    ChanMap channel_map;
    for (std::size_t i = 0; i < channel_numbers.size(); ++i)
        channel_map[channel_numbers[i]] = i;
    return channel_map;
}

//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

ModuleBase::ModuleBase() { }

ModuleBase::ModuleBase(const ChanNums& channel_numbers) {
    set_channel_numbers(channel_numbers);
}

bool ModuleBase::on_enable() {
    // return true by default
    return true;
}

bool ModuleBase::on_disable() {
    // return true by default
    return true;
}

bool ModuleBase::on_set_channel_numbers(const ChanNums& cur, const ChanNums& req) {
    // return true by default
    return true;
}

bool ModuleBase::update() {
    bool success = true;
    for (std::size_t i = 0; i < channel_numbers_.size(); ++i) {
        if (!update_channel(channel_numbers_[i]))
            success = false;
    }
    return success;
}

void ModuleBase::set_channel_numbers(const ChanNums& channel_numbers) {
    ChanMap old_map = channel_map_;
    auto new_channel_numbers = sort_and_reduce_channels(channel_numbers);
    if (on_set_channel_numbers(channel_numbers_, new_channel_numbers)) {
        channel_numbers_ = new_channel_numbers;
        channel_map_ = make_channel_map(channel_numbers_);
        for (std::size_t i = 0; i < registries_.size(); i++)
            registries_[i]->change_channel_numbers(old_map, channel_map_);
    }
}

const ChanNums& ModuleBase::get_channel_numbers() const {
    return channel_numbers_;
}

std::size_t ModuleBase::get_channel_count() const {
    return channel_numbers_.size();
}

bool ModuleBase::validate_channel_number(uint32 channel_number, bool quiet) const {
    if (channel_map_.count(channel_number) > 0)
        return true;
    if (!quiet) {
        LOG(Error) << "Invalid channel number " << channel_number
                   << " not declared in channel numbers [" << channel_numbers_ << "] on Module " << get_name();
    }
    return false;
}

bool ModuleBase::validate_channel_count(std::size_t size, bool quiet) const {
    if (channel_numbers_.size() == size)
        return true;
    if (!quiet) {
        LOG(Error) << "Invalid number of elements (" << size
                   << ") not equal to channel count of " << get_channel_count() << " on Module " << get_name();
    }
    return false;
}

void ModuleBase::add_registry(RegistryBase* registry) {
    registries_.push_back(registry);
}

std::size_t ModuleBase::index(uint32 channel_number) const {
    return channel_map_.at(channel_number);
}

} // namespace mel
