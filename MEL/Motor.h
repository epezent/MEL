#pragma once
#include "Actuator.h"

namespace mel {

    class Motor : public Actuator {

    public:

        //---------------------------------------------------------------------
        // CONSTRUCTOR(S) / DESTRUCTOR(S)
        //---------------------------------------------------------------------

        /// Default constructor
        Motor();
        /// Constructor for motor without DO enable/disable channel
        Motor(std::string name, double kt, double amp_gain, Daq::Ao ao_channel);
        /// Constructor for current/torque limitied motor without DO enable/disable channel
        Motor(std::string name, double kt, double amp_gain, double current_limit, double torque_limit, Daq::Ao ao_channel);

        /// Constructor for motor with DO enable/disable channel
        Motor(std::string name, double kt, double amp_gain, 
            Daq::Ao ao_channel, Daq::Do do_channel, EnableMode enable_mode);
        /// Constructor for current/torque limitied motor with DO enable/disable channel
        Motor(std::string name, double kt, double amp_gain, double current_limit, double torque_limit, 
            Daq::Ao ao_channel, Daq::Do do_channel, EnableMode enable_mode);

        /// Constructor for motor with DO enable/disable channel and current sensing
        Motor(std::string name, double kt, double amp_gain, 
            Daq::Ao ao_channel, Daq::Do do_channel, EnableMode enable_mode, Daq::Ai ai_channel);
        /// Constructor for urrent/torque limitied motor with DO enable/disable channel and current sensing
        Motor(std::string name, double kt, double amp_gain, double current_limit, double torque_limit, 
            Daq::Ao ao_channel, Daq::Do do_channel, EnableMode enable_mode, Daq::Ai ai_channel);

        //---------------------------------------------------------------------
        // PUBLIC FUNCTIONS
        //---------------------------------------------------------------------

        /// Sets the desired torque to be generated at the Motor, converts from torque to current, and calls set_current()
        void set_torque(double new_torque) override; 
        /// Set the desired current to be generated at the Motor, calls limit_current(), converts from current to amplifier voltage command, and sets voltage on associated analog output channel
        void set_current(double new_current);
        /// Gets the current measured by the current sensor of the current amplifier
        double get_current_sense(); 
        /// Enables the Motor using the associated digital output channel
        void enable() override; 
        /// Disables the Motor using using the associated digital output channel and writes zero to the associated analog output channel
        void disable() override; 
        
        //---------------------------------------------------------------------
        // PUBLIC VARIABLES
        //---------------------------------------------------------------------

        const double kt_; ///< torque constant of the Motor
        const double amp_gain_; ///< the conversion rate between voltage sent to the analog output channel and the current generated by the amplifier [A/V]

    private:      

        //---------------------------------------------------------------------
        // PRIVATE VARIABLES
        //---------------------------------------------------------------------      
        
        double current_; ///< stores the desired Motor current since the last call to set_current()
        double current_limit_; ///< the absolute limit of current that should be allowed to the Motor
        bool has_current_limit_; ///< whether or not the Motor should enforce a current limit
       
        double current_sense_; ///< store the measured current since the last call to get_current_sense()       
        bool has_current_sense_; ///< whether or not the Motor comes with current sensing, i.e., whether or not the ai_channel is provided 

        Daq::Ao ao_channel_; ///< the DAQ analog output channel bound to this Motor (for commanding torque/current)
        Daq::Do do_channel_; ///< the DAQ digital output channel bound to this Motor (for enable/disable)
        Daq::Ai ai_channel_; ///< the DAQ analog input channel bound to this Motor (for current sensing)

    };
}