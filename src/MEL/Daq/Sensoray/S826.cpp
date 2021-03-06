#include <MEL/Daq/Sensoray/S826.hpp>
#include <MEL/Logging/Log.hpp>
#include <windows.h>
#include <826api.h> 
#include <bitset>

namespace mel
{
    
S826::S826(int board) :
    DaqBase("s826_" + std::to_string(board)),
    board_(board),
    AI(*this),
    AO(*this),
    DIO(*this),
    encoder(*this),
    watchdog(*this, milliseconds(10))
{

}

S826::~S826() {
    if (is_enabled())
        disable();
    if (is_open())
        close();
}

bool S826::on_open() {
    // open comms with all boards
    bool success = true;
    int detected_boards = S826_SystemOpen();
    if (detected_boards == S826_ERR_DUPADDR) {
        LOG(Error) << "More than one S826 board with same board number detected.";
        success = false;
    }
    if (detected_boards == 0) {
        LOG(Error) << "No S826 boards detected.";
        success = false;
    }
    std::bitset<32> board_bits(detected_boards);
    if (board_bits[board_]) {
        // call on_open for modules
        if (!AI.on_open())
            success = false;
        if (!AO.on_open())
            success = false;
        if (!encoder.on_open())
            success = false;
        return true;
    }
    else {
        LOG(Error) << "The requested S826 board " << board_ << " was not detected.";
        success = false;
    }
    return success;
}

bool S826::on_close() {
    // close comms with all boards
    S826_SystemClose();
    return true;
}

bool S826::on_enable() {
    return true;
}

bool S826::on_disable() {
    return true;
}

Time S826::get_timestamp() const {
    uint32 timestamp;
    int result = S826_TimestampRead(board_, &timestamp);
    if (result != S826_ERR_OK) {
        LOG(Error) << "Failed to read " << get_name() << " timestamp (" << get_error_message(result) << ").";
        return Time::Zero;
    }
    return microseconds(timestamp);
}


std::string S826::get_error_message(int error) {
    switch(error) {
        case S826_ERR_OK           : return "No error";
        case S826_ERR_BOARD        : return "Illegal board number";
        case S826_ERR_VALUE        : return "Illegal argument value";
        case S826_ERR_NOTREADY     : return "Device not ready or timeout waiting for device";
        case S826_ERR_CANCELLED    : return "Wait cancelled";
        case S826_ERR_DRIVER       : return "Driver call failed";
        case S826_ERR_MISSEDTRIG   : return "Missed adc trigger";
        case S826_ERR_DUPADDR      : return "Two boards set to same board number";
        case S826_ERR_BOARDCLOSED  : return "Board is not open";
        case S826_ERR_CREATEMUTEX  : return "Can't create mutex";
        case S826_ERR_MEMORYMAP    : return "Can't map board to memory address";
        case S826_ERR_MALLOC       : return "Can't allocate memory";
        case S826_ERR_FIFOOVERFLOW : return "Counter's snapshot fifo overflowed";
        case S826_ERR_LOCALBUS     : return "Can't read local bus (register contains illegal value)";
        case S826_ERR_OSSPECIFIC   : return "Port-specific error (base error number)";
        default                    : return "Unknown error code";
    }
}

} // namespace mel
