#pragma once
#include "MahiExoII.h"
#include "mahiexoii_util.h"
#include "AtiMini45.h"

class MahiExoIIFrc : public virtual mel::MahiExoII {

public:

    struct Config : public virtual mel::MahiExoII::Config {
        std::array<mel::Daq::Ai,6> wrist_force_sensor_; // analog input channels that measure force/torque
    };

    struct Params : public virtual mel::MahiExoII::Params {
        Params() {
            calib_mat_[0] = {   0.45676,   0.37263,   2.84454, -95.32922,  -1.60986,  93.56974 };
            calib_mat_[1] = { -10.00557, 107.83272,   2.80461, -54.50607,   2.67834, -55.04209 };
            calib_mat_[2] = { 133.67479,   5.88886, 131.55424,   5.44438, 134.55104,   5.69414 };
            calib_mat_[3] = {  -0.02942,   0.74195,  -2.11485,  -0.48201,   2.19007,  -0.27703 };
            calib_mat_[4] = {   2.49045,   0.12279,  -1.26019,   0.59413,  -1.30218,  -0.70275 };
            calib_mat_[5] = {   0.07348,  -1.36804,   0.08441,  -1.41171,   0.05780,  -1.37930 };
        }
        mel::array_2D<double, 6, 6> calib_mat_;
    };

    // CONSTRUCTOR
    MahiExoIIFrc(Config configuration, Params parameters = Params());


    // PUBLIC VARIABLES
    
    mel::ForceSensor* wrist_force_sensor_;

};