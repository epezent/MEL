#include "Joint.h"
#include <iostream>

namespace mel {

    Joint::Joint() :
        Device("joint"),
        position_sensor_(nullptr),
        actuator_(nullptr),
        position_sensor_transmission_(0.0),
        actuator_transmission_(0.0),
        position_(0.0),
        velocity_(0.0),
        torque_(0.0)
    { }

    Joint::Joint(std::string name, PositionSensor* position_sensor, double position_sensor_transmission, Actuator* actuator, double actuator_transmission) :
        Device(name),
        position_sensor_(position_sensor),
        actuator_(actuator),
        position_sensor_transmission_(position_sensor_transmission),
        actuator_transmission_(actuator_transmission),
        position_(0.0),
        has_position_limits_(false),
        velocity_(0.0),
        has_velocity_limit_(false),
        torque_(0.0),
        has_torque_limit_(false)
    { }

    Joint::Joint(std::string name, PositionSensor* position_sensor, double position_sensor_transmission,
        Actuator* actuator, double actuator_transmission,
        std::array<double, 2> position_limits, double velocity_limit, double torque_limit) :
        Device(name),
        position_sensor_(position_sensor),
        actuator_(actuator),
        position_sensor_transmission_(position_sensor_transmission),
        actuator_transmission_(actuator_transmission),
        position_(0.0),
        position_limits_(position_limits),
        has_position_limits_(true),
        velocity_(0.0),
        velocity_limit_(velocity_limit),
        has_velocity_limit_(true),
        torque_(0.0),
        torque_limit_(torque_limit),
        has_torque_limit_(true)
    { }

    double Joint::get_position() {
        position_ = position_sensor_transmission_ * position_sensor_->get_position();
        return position_;
    }

    double Joint::get_velocity() {
        velocity_ = position_sensor_transmission_ * position_sensor_->get_velocity();
        return velocity_;
    }

    double Joint::get_torque() {
        return torque_;
    }

    void Joint::set_torque(double new_torque) {
        torque_ = new_torque;
        if (has_torque_limit_ && abs(torque_) > torque_limit_) {
            print("WARNING: Joint " + namify(name_) + " was commanded a torque greater than torque limit with a value of " + std::to_string(torque_) + ". Saturating command.");
            torque_ = saturate(torque_, torque_limit_);
        }
        actuator_->set_torque(actuator_transmission_ * torque_);
    }

    bool Joint::check_limits() {
        get_velocity();
        get_position();
        bool exceeded = false;
        if (has_velocity_limit_ && abs(velocity_) > velocity_limit_) {
            mel::print("WARNING: Joint " + namify(name_) + " velocity exceeded velocity limit with a value of " + std::to_string(velocity_) + ".");
            exceeded = true;
        }
        if (has_position_limits_ && position_ < position_limits_[0]) {
            mel::print("WARNING: Joint " + namify(name_) + " position exceeded min position limit with a value of " + std::to_string(position_) + ".");
            exceeded = true;
        }
        if (has_position_limits_ && position_ > position_limits_[1]) {
            mel::print("WARNING: Joint " + namify(name_) + " position exceeded max position limit with a value of " + std::to_string(position_) + ".");
            exceeded = true;
        }
        return exceeded;
    }

    void Joint::enable() {
        enabled_ = true;
        position_sensor_->enable();
        actuator_->enable();
    }

    void Joint::disable() {
        enabled_ = false;
        position_sensor_->disable();
        actuator_->disable();
    }
}