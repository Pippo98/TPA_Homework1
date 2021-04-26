#ifndef PHIL_MACHINE_H_
#define PHIL_MACHINE_H_

#include <stdio.h>
#include <iostream>

#include "Gear.h"

// External
#include "Car.h"
#include "EB_Device.h"


struct PhilMachine{
  Gear* gear;
  coca_device* car;
};

PhilMachine* phil_init_defailt_machine();

string phil_machine_to_svg(PhilMachine* );

#endif