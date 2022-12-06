/*
  @file base/user/caster.hpp

  Group of dynamic_cast functions for easier use
*/

#ifndef _CASTER_HPP_
#define _CASTER_HPP_

#include "base/devices.hpp"

namespace KRONOS {
  void assert_same_device(AbstractDevice* device, const device_types &type) {
    if (device->type() != type) {
      throw new UnexpectedDeviceTypeError(device->type(), type);
    }
  }

  /*
    Casts AbstractDevice pointer to Button pointer

    @param device AbstractDevice pointer

    @return Button pointer
  */
  Button* to_button(AbstractDevice* device) {
    assert_same_device(device, K_BUTTON);

    return dynamic_cast<Button*>(device);
  }

  /*
    Casts AbstractDevice pointer to Color pointer

    @param device AbstractDevice pointer

    @return Color pointer
  */
  Color* to_color(AbstractDevice* device) {
    assert_same_device(device, K_COLOR);

    return dynamic_cast<Color*>(device);
  }

  /*
    Casts AbstractDevice pointer to Controller pointer

    @param device AbstractDevice pointer

    @return Controller pointer
  */
  Controller* to_controller(AbstractDevice* device) {
    assert_same_device(device, K_CONTROLLER);

    return dynamic_cast<Controller*>(device);
  }

  /*
    Casts AbstractDevice pointer to Motor pointer

    @param device AbstractDevice pointer

    @return Motor pointer
  */
  Motor* to_motor(AbstractDevice* device) {
    assert_same_device(device, K_MOTOR);

    return dynamic_cast<Motor*>(device);
  }

  /*
    Casts AbstractDevice pointer to Piston pointer

    @param device AbstractDevice pointer

    @return Piston pointer
  */
  Piston* to_piston(AbstractDevice* device) {
    assert_same_device(device, K_PISTON);

    return dynamic_cast<Piston*>(device);
  }

  /*
    Casts AbstractDevice pointer to Proximity pointer

    @param device AbstractDevice pointer

    @return Proximity pointer
  */
  Proximity* to_proximity(AbstractDevice* device) {
    assert_same_device(device, K_PROXIMITY);

    return dynamic_cast<Proximity*>(device);
  }

  /*
    Casts AbstractDevice pointer to Vision pointer

    @param device AbstractDevice pointer

    @return Vision pointer
  */
  Vision* to_vision(AbstractDevice* device) {
    assert_same_device(device, K_VISION);

    return dynamic_cast<Vision*>(device);
  }
}

#endif
