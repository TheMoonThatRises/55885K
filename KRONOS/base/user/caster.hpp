/*
  @file base/user/caster.hpp

  Group of dynamic_cast functions for easier use
*/

#ifndef _CASTER_HPP_
#define _CASTER_HPP_

#include "base/devices.hpp"

namespace KRONOS {
  /*
    Casts AbstractDevice pointer to Button pointer

    @param device AbstractDevice pointer

    @return Button pointer
  */
  Button* to_button(AbstractDevice* device) {
    return dynamic_cast<Button*>(device);
  }

  /*
    Casts AbstractDevice pointer to Color pointer

    @param device AbstractDevice pointer

    @return Color pointer
  */
  Color* to_color(AbstractDevice* device) {
    return dynamic_cast<Color*>(device);
  }

  /*
    Casts AbstractDevice pointer to Controller pointer

    @param device AbstractDevice pointer

    @return Controller pointer
  */
  Controller* to_controller(AbstractDevice* device) {
    return dynamic_cast<Controller*>(device);
  }

  /*
    Casts AbstractDevice pointer to Motor pointer

    @param device AbstractDevice pointer

    @return Motor pointer
  */
  Motor* to_motor(AbstractDevice* device) {
    return dynamic_cast<Motor*>(device);
  }

  /*
    Casts AbstractDevice pointer to Piston pointer

    @param device AbstractDevice pointer

    @return Piston pointer
  */
  Piston* to_piston(AbstractDevice* device) {
    return dynamic_cast<Piston*>(device);
  }

  /*
    Casts AbstractDevice pointer to Proximity pointer

    @param device AbstractDevice pointer

    @return Proximity pointer
  */
  Proximity* to_proximity(AbstractDevice* device) {
    return dynamic_cast<Proximity*>(device);
  }

  /*
    Casts AbstractDevice pointer to Vision pointer

    @param device AbstractDevice pointer

    @return Vision pointer
  */
  Vision* to_vision(AbstractDevice* device) {
    return dynamic_cast<Vision*>(device);
  }
}

#endif
