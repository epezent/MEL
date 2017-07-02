#pragma once
#include "RobotJoint.h"

namespace mel {

    class PrismaticRobotJoint : public RobotJoint {

    public:

        // CONSTRUCTOR(S) / DESTRUCTOR(S)

        PrismaticRobotJoint();
        PrismaticRobotJoint(std::string name, PositionSensor* position_sensor, double position_sensor_transmission, Actuator* actuator, double actuator_transmission);
        
    private:

        
    };

}