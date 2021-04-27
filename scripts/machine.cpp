#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "Gear.h"
#include "Machine.h"

// External
#include "Car.h"        // @eliabonte
#include "EB_Device.h"  // @GiacomoCorradini

int main(){
  //---- CAR ----//
  PhilMachine* machine = phil_init_default_machine();
  string machine_svg = phil_machine_to_svg(machine);
  g_export_svg(machine_svg, "output/machine_car");


  size_t size = 5;
  PhilMachine** complex_m = new PhilMachine* [size];
  complex_m[0] = machine;
  complex_m[1] = phil_init_default_machine();
  complex_m[2] = phil_init_default_machine();
  complex_m[3] = phil_init_default_machine();
  complex_m[4] = phil_init_default_machine();

  string complex_machine = phil_complex_machine_to_svg(complex_m, size);

  g_export_svg(complex_machine, "output/complex_machine");
}
