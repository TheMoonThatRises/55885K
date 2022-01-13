#include "helpers.h"
#include "motor.hpp"
#include "piston.hpp"
#include "device.hpp"


#ifndef _KRONOS_ROBOT_HPP_
#define _KRONOS_ROBOT_HPP_

namespace KRONOS {
    class Robot {
        private:
            /*
                Get device by name.

                @param devices
                @param name
                @return Returns device.
            */
            template<class T>
            T getType(const std::vector<Device<T>>& devices, const std::string& name);

            /*
                Adds device to robot.

                @param devices
                @param device
                @return Returns robot to allow linking.
            */
            template<class T>
            Robot& addType(std::vector<Device<T>>& devices, const Device<T>& device);
        protected:
            std::vector<Device<Motor>> motors;
            std::vector<Device<Piston>> pistons;
            std::map<std::string, pros::ADIDigitalIn> buttons;
            std::map<std::string, std::vector<std::string>> devicePairs;
            std::map<std::string, device_types> deviceType;
        public:
            /*
                @param wheelAddVelocity
                @param chassisSensitivity
            */
            Robot(int wheelAddVelocity, int chassisSensitivity);

            int wheelAddVelocity, chassisSensitivity;

            /*
                Adds motor to robot.

                @param motor
                @return Allows you to train functions together.
            */
            Robot& addMotor(const Device<Motor>& motor);

            /*
                Adds piston to robot.

                @param piston
                @return Allows you to train functions together.
            */
            Robot& addPiston(const Device<Piston>& piston);

            /*
                Adds button to robot.

                @param name
                @param button
                @return Allows you to train functions together.
            */
            Robot& addButton(const std::string& name, const pros::ADIDigitalIn& button);

            /*
                Assigns one callsign to a device pair.

                @param callNames
                @param pairName
                @return Allows you to train functions together.
            */
            Robot& pairDevices(const std::vector<std::string>& callNames, const std::string& pairName);

            /*
                Returns motor by callsign or name.

                @param name
                @return Motor that you called for.
            */
            Motor getMotor(const std::string& name);

            /*
                Returns piston by callsign or name.

                @param name
                @return Piston that you called for.
            */
            Piston getPiston(const std::string& name);

            /*
                Returns motor pair by callsign.

                @param callsign
                @return Motor pair that you called for.
            */
            std::vector<std::string> getMotorPair(const std::string& callsign);

            /*
                Gets device type of device assigned to callsign.

                @param callsign
                @return Device type of device attached to callsign.
            */
            device_types getDeviceType(const std::string& callsign);

            /*
                @param type
                @param name
                @return Device pair that you called for.
            */
            template<class T>
            std::vector<Device<T>> getPairs(const device_types& type, const std::string& name);

            /*
                @param name
                @return Button that you called for.
            */
            pros::ADIDigitalIn getButton(const std::string& name);

            /*
                Moves motor pair.

                @param pairName
                @param velocity
            */
            void movePairMotors(const std::string& pairName, const int32_t& velocity);

            /*
                Moves motor pair to distance.

                @param pairName
                @param velocity
                @param distance
            */
            void movePairMotors(const std::string& pairName, const int32_t& velocity, const double& distance);

            /*
                Activates piston pair.

                @param pairName
                @param activated
            */
            void activatePairPiston(const std::string& pairName, bool activated);
            void controllerListener();
    };
}

#endif