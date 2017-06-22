#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "util.h"
#include <boost/filesystem.hpp>

class Daq {

public:

    /* constructor */
    Daq(std::string name, std::string id,
        uint_vec ai_channels = uint_vec(),
        uint_vec ao_channels = uint_vec(),
        uint_vec di_channels = uint_vec(),
        uint_vec do_channels = uint_vec(),
        uint_vec enc_channels = uint_vec(),
        uint_vec vel_channels = uint_vec());

    std::string type_;   /* string representing the board type e.g. "q8_usb" */
    std::string id_;     /* string representing the board ID numner e.g. "0" */

	/* pure virtual functions that must be implemented by all derived DAQs */
	virtual int init() = 0;
	virtual int terminate() = 0;

    /* virtual functions that may or may not be implemented by DAQs */
    virtual void read_analog() {}
    virtual void read_digital() {}
    virtual void read_encoder() {}
    virtual void read_encoder_velocity() {}
    virtual void read_all() {}

	virtual void write_analog() {}
	virtual void write_digital() {}
    virtual void write_all() {}

	virtual void zero_encoder_counts() {}

    virtual void log_data(double timestamp) {}

	virtual void reload_watchdog() {}
	virtual void start_watchdog(double watchdog_timeout) {}
	virtual void stop_watchdog() {}

    /* getters and setters of state variables 
     * these functions currently pass by value (copy) and may eventually need to pass by reference for better performance */
    double_vec get_analog_voltages();
    double get_analog_voltage(int channel_number);
    void set_analog_voltages(double_vec new_voltages);
    void set_analog_voltage(int channel_number, double new_voltage);

    char_vec get_digital_states();
    char get_digital_state(int channel_number);
    void set_digital_states(char_vec new_states);
    void set_digital_state(int channel_numner, char new_state);

    int_vec get_encoders_counts();
    int get_encoder_counts(int channel_number);
    double_vec get_encoders_counts_per_sec();
    double get_encoder_counts_per_sec();

protected:     

	bool active_ = false;
    std::string log_filename_;
    std::ofstream data_log_;          

    /* state variables */
    double_vec ai_voltages_;          /* vector for which analog inputs should be read in to*/
    double_vec ao_voltages_;          /* vector for which analog outputs should be read out from*/
    char_vec   di_states_;            /* vector for which digital inputs should be read in to*/
    char_vec   do_states_;            /* vector for which digital outputs should be read out from*/
    int_vec    enc_counts_;           /* vector for which encoder counts should be read in to*/
    double_vec enc_counts_per_sec_;   /* vector for which encoder counts per second should be read in to*/

                                      /* I/O channel information */
    uint_vec   ai_channels_;          /* vector of analog input channels being used */
    double_vec ai_min_voltages_;      /* vector of minimum input voltages allowed by the board */
    double_vec ai_max_voltages_;      /* vector of maximum input voltages allowed by the board */

    uint_vec   ao_channels_;          /* vector of analog output channels being used */
    double_vec ao_min_voltages_;      /* vector of minimum output voltages allowed by the board */
    double_vec ao_max_voltages_;      /* vector of maximum output voltages allowed by the board */
    double_vec ao_initial_voltages_;  /* vector of voltages analog outputs will go to when the program starts */
    double_vec ao_final_voltages_;    /* vector of voltages analog outputs will go to when the program finishes */
    double_vec ao_exp_voltages_;      /* vector of voltages analog outputs will go to if the watchdog expires */

    uint_vec   di_channels_;          /* vector of digital input channels being used */

    uint_vec   do_channels_;          /* vector of digital output channels being used */
    char_vec   do_initial_states_;    /* vector of states digital outputs will go to when the program starts */
    char_vec   do_final_states_;      /* vector of states digital outputs will go to when the program finishes */

    uint_vec   enc_channels_;         /* vector of encoder channels being used */
    uint_vec   vel_channels_;         /* vector of encoder velocity channels being used */
    int_vec    enc_initial_counts_;   /* vector of initial encoder counts to be used when zero_encoder_counts() is called */

};