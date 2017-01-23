/*
 * Copyright (c) 2017, <copyright holder> <email>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <ft_estimate.h>

ft_perception::FTEstimation::FTEstimation(std::string robot, std::string hand,std::string wbd)
{
    robot_name_ = robot;
    hand_name_ = hand;
    whole_body_dynamics_module_name_ = wbd;
    std::cout << "Selected hand is " << hand_name_ << std::endl;
    
    if(!yarp::os::Network::initialized())
    {
        yarp::os::Network::init();
    }
    std::string dummy = "/ft_perception/" + hand + "_arm" + "/" + "cartesianEndEffectorWrench:i" ;
    end_effector_wrench_input_port_ = new yarp::os::BufferedPort<yarp::sig::Vector>;
    if(end_effector_wrench_input_port_->open(dummy))
    {
        std::cout << "Opened the port" << end_effector_wrench_input_port_->getName() << std::endl;
        
    }
    
    connectToWDB();
}


bool ft_perception::FTEstimation::connectToWDB()
{
    std::cout << "connecting to whole body dynamics module ports" << std::endl;
    if(!yarp::os::Network::checkNetwork())
    {
        std::cout << "Yarp network is not detected!" << std::endl;
    }
    else
    {
        
        std::string wrench_estimate_port_name = "/" + whole_body_dynamics_module_name_ + "/" + hand_name_+ "_arm" + "/cartesianEndEffectorWrench:o";
        bool port_connection = yarp::os::Network::connect(wrench_estimate_port_name,end_effector_wrench_input_port_->getName());
        if(!port_connection)
        {
            std::cout << "Error in connecting port " << wrench_estimate_port_name << \
            " to " << end_effector_wrench_input_port_->getName() << std::endl;
            return true;
        }
        else
        {
            std::cout << "Successfully connected port " << wrench_estimate_port_name << \
            " to " << end_effector_wrench_input_port_->getName() <<std::endl;
            return false;
        }
    } 

}


bool ft_perception::FTEstimation::getWrench()
{
    wrench_estimate_ = end_effector_wrench_input_port_->read();
    if(wrench_estimate_->length()!=0)
    {
        displayWrench();
        return true;
    }
    else
    {
        std::cout << "Error while reading wrench estimates from " << hand_name_ << " hand!" << std::endl;
        return false;
    }
    
}

void ft_perception::FTEstimation::displayWrench()
{
    std::cout << "Received wrench from " << hand_name_ << " hand : " << wrench_estimate_->toString() << std::endl;
}


