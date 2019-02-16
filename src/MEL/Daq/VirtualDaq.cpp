#include <MEL/Daq/VirtualDaq.hpp>
#include <MEL/Logging/Log.hpp>
#include <MEL/Math/Functions.hpp>
#include <MEL/Math/Constants.hpp>

namespace mel {

//==============================================================================
// VIRTUAL AI
//==============================================================================

static Voltage DEFAULT_AI_SOURCE(Time t) {
    return sin(2.0 * PI * t.as_seconds());
}

VirtualAI::VirtualAI(VirtualDaq& daq, const std::vector<uint32>& channel_numbers) :
    AnalogInput(channel_numbers),
    sources(this, DEFAULT_AI_SOURCE),
    daq_(daq)
{
    set_name(daq.get_name() + "_AI");
}

bool VirtualAI::update_channel(uint32 channel_number) {
    values_[channel_number] = sources[channel_number](daq_.clock_.get_elapsed_time());
    return true;
}

//==============================================================================
// VIRUTAL AO
//==============================================================================

VirtualAO::VirtualAO(VirtualDaq& daq, const std::vector<uint32>& channel_numbers) : 
    AnalogOutput(channel_numbers),
    daq_(daq) 
{
    set_name(daq.get_name() + "_AO");
}

bool VirtualAO::update_channel(uint32 channel_number) {
    return true;
}

//==============================================================================
// VIRTUAL DI
//==============================================================================

static Logic DEFAULT_DI_SOURCE(Time t) {
    if (sin(2.0 * PI * t.as_seconds()) > 0.0)
        return High;
    else
        return Low;
}

VirtualDI::VirtualDI(VirtualDaq& daq, const std::vector<uint32>& channel_numbers) :
    DigitalInput(channel_numbers),
    sources(this, DEFAULT_DI_SOURCE),
    daq_(daq)
{
    set_name(daq.get_name() + "_DI");
}

bool VirtualDI::update_channel(uint32 channel_number) {
    values_[channel_number] = sources[channel_number](daq_.clock_.get_elapsed_time());
    return true;
}

//==============================================================================
// VIRTUAL DO
//==============================================================================

VirtualDO::VirtualDO(VirtualDaq& daq, const std::vector<uint32>& channel_numbers) : 
    DigitalOutput(channel_numbers),
    daq_(daq)
{
    set_name(daq.get_name() + "_DO");
}

bool VirtualDO::update_channel(uint32 channel_number) {
    return true;
}

//==============================================================================
// VIRTUAL ENCODER
//==============================================================================

int32 DEFAULT_ENCODER_SOURCE(Time t) {
    return static_cast<int32>(1024.0 * (sin(2.0 * PI * t.as_seconds())));
}

VirtualEncoder::VirtualEncoder(VirtualDaq& daq, const std::vector<uint32>& channel_numbers) :
    Encoder(channel_numbers),
    sources(this, DEFAULT_ENCODER_SOURCE),
    daq_(daq)
{
    set_name(daq.get_name() + "_encoder");
}

bool VirtualEncoder::update_channel(uint32 channel_number) {
    values_[channel_number] = sources[channel_number](daq_.clock_.get_elapsed_time());
    return true;
}

//==============================================================================
// VIRTUAL DAQ
//==============================================================================

VirtualDaq::VirtualDaq(const std::string& name)
    : DaqBase(name),
      AI(*this, {0,1,2,3,4,5,6,7,8}),
      AO(*this, {0,1,2,3,4,5,6,7,8}),
      DI(*this, {0,1,2,3,4,5,6,7,8}),
      DO(*this, {0,1,2,3,4,5,6,7,8}),
      encoder(*this, {0,1,2,3,4,5,6,7,8})
{

}

VirtualDaq::~VirtualDaq() {
    if (is_enabled())
        this->disable();
    if (is_open())
        this->close();
}

bool VirtualDaq::on_enable() {
    AI.enable();
    AO.enable();
    DI.enable();
    DO.enable();
    encoder.enable();
    return true;
}

bool VirtualDaq::on_disable() {
    AI.disable();
    AO.disable();
    DI.disable();
    DO.disable();
    encoder.disable();
    return true;
}

bool VirtualDaq::update_input() {
    AI.update();
    DI.update();
    encoder.update();
    return true;
}

bool VirtualDaq::update_output() {
    AO.update();
    DO.update();
    return true;
}

bool VirtualDaq::on_open() {
    clock_.restart();
    return true;
}

bool VirtualDaq::on_close() {
    return true;
}

}  // namespace mel
