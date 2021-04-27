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
  EbDevice* gru;
  coca_device* car;
};

PhilMachine* phil_init_machine(double gru_height, double raising_speed, double car_width);

PhilMachine* phil_init_default_machine();

string phil_machine_to_svg(PhilMachine* );
string phil_complex_machine_to_svg(PhilMachine** cplx, size_t size);

double phil_get_machine_width(PhilMachine* machine);

#endif