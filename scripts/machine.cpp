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
  g_export_svg(machine_svg, "output/machine");


  size_t size = 4;
  PhilMachine** complex_m = new PhilMachine* [size];
  complex_m[0] = phil_init_machine(400, 50, 350, -10);
  complex_m[1] = phil_init_machine(400, 50, 350, -20);
  complex_m[2] = phil_init_machine(400, 60, 350, -30);
  complex_m[3] = phil_init_machine(400, 60, 350, -40);

  string complex_machine = phil_complex_machine_to_svg(complex_m, size);

  g_export_svg(complex_machine, "output/complex_machine");
}
