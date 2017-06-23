#include "MahiExoII.h"



MahiExoII::MahiExoII(Daq* e0_daq, uint e0_chan, Daq* e1_daq, uint e1_chan, Daq* ao0_daq, uint ao0_chan, Daq* ao1_daq, uint ao1_chan, Daq* do0_daq, uint do0_chan, Daq* do1_daq, uint do1_chan) : 
    e0_daq_(e0_daq), e0_chan_(e0_chan),
    e1_daq_(e1_daq), e1_chan_(e1_chan),
    ao0_daq_(ao0_daq), ao0_chan_(ao0_chan),
    ao1_daq_(ao1_daq), ao1_chan_(ao1_chan),
    do0_daq_(do0_daq), do0_chan_(do0_chan),
    do1_daq_(do1_daq), do1_chan_(do1_chan),
    Robot(2)
{
    
    encoder0_ = new Encoder(1.0, 2048, 4, e0_daq_, e0_chan_);
    encoder1_ = new Encoder(1.0, 2048, 4, e1_daq_, e1_chan_);
    
    actuator0_ = new Actuator(1.0, 0.1, 1.8, 3.0, ao0_daq_, ao0_chan_, do0_chan_);
    actuator1_ = new Actuator(1.0, 0.1, 1.8, 3.0, ao1_daq_, ao1_chan_, do1_chan_);
    
    //joint0_ = Joint(0.0);
    //joint1_ = Joint(1.0);
    //joints_[0] = RevoluteJoint(encoder0_, actuator0_);
    //joints_[1] = RevoluteJoint(encoder1_, actuator1_);
}

/*
double_vec MahiExoII::get_joint_positions() {

}
*/