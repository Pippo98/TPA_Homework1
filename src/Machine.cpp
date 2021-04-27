#include "Machine.h"

PhilMachine* phil_init_default_machine(){

  EbDevice* gru = eb_init(300, 400, 50, 20, 170);

  //Gear* gear = g_init(false, 50, 70, 50, 20);
  Gear* gear = g_init(true, 70, 50, 50, 20);

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
  ret->gru = gru;
  ret->gear = gear;

  return ret;
}

string phil_machine_to_svg(PhilMachine* machine){

  string ret = "";

  double height = 300;

  // RPM
  double gear_speed = 5;
  double elevation_speed = 1/(((g_get_external_radius(machine->gear) * 2 * G_PI) / gear_speed) / height);
  //double elevation_speed = (gear_speed*240) / (g_get_external_radius(machine->gear) * 2 * G_PI);

  //------------------------- GRU -------------------------//
  // Getting gru svg string
  string gru_svg = eb_to_svg(machine->gru, false);
  
  // Positions of platform to then lay gears onto it
  double x_platform = eb_Xplatform(machine->gru);
  double y_platform = eb_Yplatform(machine->gru);
  
  // Getting center of platform and adding gear size
  x_platform += machine->gru->width_platform / 2;
  y_platform += 20/2;
  
  //------------------------- GEAR -------------------------//
  // Translating gear to center of platform
  string gear_svg = "<g transform='translate(" + _str(x_platform) + " " + _str(y_platform) + ")'>\n";
  gear_svg += g_to_svg(machine->gear, false, false, -gear_speed) + "\n";
  gear_svg += "</g>\n";
  
  // Adding gear string before </svg> tag
  gru_svg.insert(gru_svg.find("</svg>"), gear_svg + "\n");

  //------------------------- CAR -------------------------//
  string car = coca_strg_device(machine->car);

  // REMOVING THE HEADER!!!
  car.erase(0, car.find("\">")+2);
  car.erase(car.find("</svg>"));

  double car_x = -SFONDOX/2 + x_platform + g_get_external_radius(machine->gear);
  
  // Animating CAR
  string animation = "<g>";
  animation += 
        "<animateTransform attributeName='transform' attributeType='XML' "
        "type='translate' from='"+_str(car_x)+" " + _str(height + y_platform) +
        "' to='"+_str(car_x)+" " + _str(y_platform) + "' " +
        "dur='" + _str(elevation_speed) +
        "s' "
        "repeatCount='indefinite' />";

  car = animation + car;
  car += "</g>";

  //------------------------- LINE -------------------------//
  // Creating line between gear and car
  string line = "";
  double y_car = (machine->car->cap.y3 ) + y_platform;
  double x = x_platform + g_get_external_radius(machine->gear);
  double scale = ((y_car - y_platform)+height)/(y_car - y_platform);
  // Animating the line to stay attacched to the top of the car
  line += "<g transform='translate(" + _str(x) + " " + _str(y_platform) + ")'>";
  line += "<animateTransform attributeName='transform' additive='sum' "
          "type='scale' from='1 " + _str(scale) + "' to='1 1' " +
          "dur='" + _str(elevation_speed) +
          "s' "
          "repeatCount='indefinite' />";
  line += _g_get_line(0, 0, 0, y_car-y_platform, "stroke='brown' stroke-width='2' id='def-rope'");
  line += "</g>";

  car += line;

  // Iserting gear device
  gru_svg.insert(gru_svg.find("</svg>"), car);

  return gru_svg;
}