#include <MEL/Communications/SerialPort.hpp>
#include "Detail/rs232.h"

namespace mel {

SerialPort::SerialPort(std::size_t port, std::size_t baudrate, std::string mode) :
    is_open_(false),
    port_(port),
    baudrate_(baudrate),
    mode_(mode)
{

}

SerialPort::~SerialPort() {
    if (is_open())
        close();
}

bool SerialPort::open() {
    if (is_open())
        close();
    if (RS232_OpenComport(port_, baudrate_, mode_.c_str()) == 1)
        return false;
    else {
        is_open_ = true;
        return true;
    }
}

bool SerialPort::send_data(unsigned char* data, std::size_t size) {
    if (RS232_SendBuf(port_, data, (int)size) == 1)
        return false;
    else
        return true;
}

int SerialPort::receive_data(unsigned char* data, std::size_t size) {
    return RS232_PollComport(port_, data, size);
}

bool SerialPort::close() {
    RS232_CloseComport(port_);
    return true;
}

bool SerialPort::is_open() const {
    return is_open_;
}



} // namespace mel