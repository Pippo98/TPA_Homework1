#include "Machine.h"

PhilMachine* phil_init_defailt_machine(){

  Gear* gear = g_init(false, 50, 70, 50, 20);
  //Gear* gear = g_init(true, 70, 50, 50, 20);

  coca_device* car = new coca_device();

  car->car.width = 350;
  car->car.height = 80;
  car->car.cx = (SFONDOX/2) - (car->car.width/2);
  car->car.cy = (SFONDOY/2) - (car->car.height/2);

  car->sx.ruota = (car->car.height - 1) / 2;
  car->sx.cerchione = car->sx.ruota / 1.6;
  car->dx.ruota = (car->car.height - 1) / 2;
  car->dx.cerchione = car->dx.ruota / 1.6;

  car->sx.centrox = car->car.cx + (car->car.width / 5) + 5;
  car->dx.centrox = car->car.cx + car->car.width - (car->car.width / 5) - 5;

  coca_try_finestrini(car);
  coca_try_spoiler(car);
  coca_try_tetto(car);


  PhilMachine* ret = new PhilMachine();
  ret->car = car;
  ret->gear = gear;

  return ret;
}

string phil_machine_to_svg(PhilMachine* machine){

  string ret = "";

  ret += coca_strg_device(machine->car);
  
  // RPM
  double gear_speed = 5;
  double elevation_speed = g_get_external_radius(machine->gear) * 3 * G_PI / (gear_speed*60);

  double height = 300;  
  double duration = 5;
  // Animating CAR
  string animation = "<g>";
  animation += 
        "<animateTransform attributeName='transform' attributeType='XML' "
        "type='translate' from='0 " + _str(height) + "' to='0 0' " +
        "dur='" + _str(elevation_speed) +
        "s' "
        "repeatCount='indefinite' />";

  //Inserting animation
  ret.insert(ret.find("\">")+2, animation);

  double y = g_get_external_radius(machine->gear);
  double x = SFONDOX/2 - y;

  // Translating gear to top of car
  string gear = "";
  gear += "<g transform='translate(" + _str(x) + " " + _str(y) + ")'>";
  gear += g_to_svg(machine->gear, false, false, -gear_speed);
  gear += "</g>";

  // Creating line between gear and car
  string line = "";
  double y_car = machine->car->car.cy;
  x += g_get_external_radius(machine->gear);
  double scale = ((y_car - y)+height)/(y_car - y);
  // Animating the line to stay attacched to the top of the car
  line += "<g transform='translate(" + _str(x) + " " + _str(y) + ")'>";
  line += "<animateTransform attributeName='transform' additive='sum' "
          "type='scale' from='1 " + _str(scale) + "' to='1 1' " +
          "dur='" + _str(elevation_speed) +
          "s' "
          "repeatCount='indefinite' />";
  line += _g_get_line(0, 0, 0, y_car-y, "stroke='black' stroke-width='2' id='def-rope'");
  line += "</g>";

  gear += line;

  // Close tag for animation
  ret.insert(ret.find("</svg>"), "</g>");

  // Iserting gear device
  ret.insert(ret.find("</svg>"), gear);

  return ret;
}