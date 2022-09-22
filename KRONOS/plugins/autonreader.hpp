/*
  @file plugin/autonreader.hpp

  Reads autonomous byte code for KRONOS
*/

#ifndef _AUTONREADER_HPP_
#define _AUTONREADER_HPP_

class AutonReader {
  private:
    enum mnemonics {
      setv, setp, slep = 0x02
    };

    std::vector<char> ports {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 'A', 'B', 'C', 'D', 'E', 'F'};

    short littleEndian(const std::vector<unsigned char> &bytes, const int &start) {
      return (bytes[start + 1] << 8) | bytes[start];
    }

    short hexToInt(const unsigned char &byte) {
      return byte & 0xff;
    }
  protected:
    KRONOS::Robot *_robot;
    std::map<std::string, std::vector<unsigned char>> _autons;
    std::string selected;
  public:
    inline explicit AutonReader(KRONOS::Robot *robot, const std::map<std::string, std::vector<unsigned char>> &autonlist) : _robot(robot) {
      _autons = std::move(autonlist);
      selected = _autons.begin()->first;
    }

    inline void selectAuton() {

    }

    inline void runAuton() {
      const std::vector<unsigned char> &bytes = _autons.at(selected);

      for (auto i = 0; i < bytes.size(); ++i) {
        switch (bytes[i]) {
          case setv:
            std::cout << "Setting motor port p" << hexToInt(bytes[i + 1]) << " to velocity " << littleEndian(bytes, i + 2) << std::endl;
            dynamic_cast<KRONOS::Motor*>(_robot->getDevice(ports.at(hexToInt(bytes[i + 1]))))->move_velocity(littleEndian(bytes, i + 2));
            i += 3;
            break;
          case setp:
            std::cout << "Moving robot to location " << littleEndian(bytes, i + 1) << ", " << littleEndian(bytes, i + 3) << " at velocity " << littleEndian(bytes, i + 5) << std::endl;
            i += 6;
            break;
          case slep:
            pros::delay(littleEndian(bytes, i + 1));
            i += 2;
            break;
        }
      }
    }
};

#endif
