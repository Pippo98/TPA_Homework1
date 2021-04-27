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

/**
* Create a machine with these simple parameters
* 
* @param gru_height specify the height in px of the gru device
* @param raising_speed defines the gear type and dimensions
* @param car_width width of the car
* @param angle angle of the gru arm
* return machine or NULL if it failed
*/
PhilMachine* phil_init_machine(double gru_height, double raising_speed, double car_width, double angle);

/**
* Create a default machine
* return machine instance
*/
PhilMachine* phil_init_default_machine();

/**
* Generates a svg string representation of the machine.
* 
* @param machine to be converted
*/
string phil_machine_to_svg(PhilMachine* machine);
/**
* Converts an array of machines in one single svg string 
* 
* @param cplx complex machine array
* @param size size of the array
*/
string phil_complex_machine_to_svg(PhilMachine** cplx, size_t size);

/**
 * Computes the width of the machine
 * @param machine
 * return double width
*/
double phil_get_machine_width(PhilMachine* machine);

#endif