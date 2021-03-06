#include "Machine.h"

PhilMachine* phil_init_machine(double gru_height, double raising_speed, double car_width, double angle){
  if(angle > 80)
    angle = 80;
  if(angle < -80)
    angle = -80;
  EbDevice* gru = eb_init(gru_height*5/7, gru_height, gru_height/10, angle, 20);
  if(gru == NULL){
    cout << "Gru failed" << endl;
    return NULL;
  } 

  Gear* gear;
  if(gru_height > (raising_speed))
    gear = g_init(true, (raising_speed + 5), gru_height/10, gru_height/10, 20);
  else
    gear = g_init(false, gru_height/10, (raising_speed + 5), gru_height/10, 20);

  if(gear == NULL){
    cout << "Gear failed" << endl;
    return NULL;
  }

  parametri car_param = {float(car_width/5), float(car_width), 0, 0, 16, 2};
  coca_device* car = coca_init_device(car_param);

  if(car == NULL){
    cout << "Car failed" << endl;
    return NULL;
  }

  PhilMachine* ret = new PhilMachine();
  ret->car = car;
  ret->gru = gru;
  ret->gear = gear;

  return ret;
}


PhilMachine* phil_init_default_machine(){

  EbDevice* gru = eb_init(300, 400, 50, 20, 50);

  //Gear* gear = g_init(false, 50, 70, 50, 20);
  Gear* gear = g_init(true, 70, 50, 50, 20);

  parametri car_param = {80, 350, 0, 0, 16, 2};
  coca_device* car = coca_init_device(car_param);

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

  gear_svg += _g_get_ellipse(x_platform, y_platform,
        g_get_external_radius(machine->gear), g_get_external_radius(machine->gear),
        "stroke:brown;stroke-width:2;fill:transparent");
  
  // Adding gear string before </svg> tag
  gru_svg.insert(gru_svg.find("</svg>"), gear_svg + "\n");

  //------------------------- CAR -------------------------//
  string car = coca_strg_device(machine->car, 0);

  double car_x = -machine->car->car.width/2 + x_platform + g_get_external_radius(machine->gear);
  double y1 = height + y_platform - machine->car->cap.y3;
  double y2 = y_platform - machine->car->cap.y3;
  
  // Animating CAR
  string animation = "<g>";
  animation += 
        "<animateTransform attributeName='transform' attributeType='XML' "
        "type='translate' from='"+_str(car_x)+" " + _str(y1) +
        "' to='"+_str(car_x)+" " + _str(y2) + "' " +
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
          "type='scale' from='1 1' to='1 0' "
          "dur='" + _str(elevation_speed) +
          "s' "
          "repeatCount='indefinite' />";
  line += _g_get_line(0, 0, 0, height, "stroke='brown' stroke-width='2' id='def-rope'");
  line += "</g>";

  car += line;

  // Iserting gear device
  gru_svg.insert(gru_svg.find("</svg>"), car);

  return gru_svg;
}


string phil_complex_machine_to_svg(PhilMachine** cplx, size_t size){
  string svg = "";

  int x = 0;
  for (int i = 0; i < size; i++){
    if(cplx[i] == NULL)
      continue;

    string curr_svg = phil_machine_to_svg(cplx[i]); 

    // REMOVING HEADER.
    curr_svg.erase(0, curr_svg.find("\">")+2);
    curr_svg.erase(curr_svg.find("</svg>"));

    // X Traslation
    curr_svg  = "<g transform='translate("+_str(x)+" 0)'>" + curr_svg;
    curr_svg += "</g>";

    svg += curr_svg;

    // Adding width of current machine
    x += phil_get_machine_width(cplx[i]);
  }

  svg = "<svg version='1.1' viewBox='0 0 "+_str(x)+ " 900' xmlns='http://www.w3.org/2000/svg' style='background: white' >\n" + svg;
  svg = "<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n" + svg;

  svg += "</svg>";
  return svg;
}


double phil_get_machine_width(PhilMachine* machine){

  double w1 = 0;
  double w2 = 0;
  double w3 = 0;

  w1  = machine->gru->sliding + machine->gru->width_towtruck;
  w2  = machine->gru->sliding + machine->gru->width_towtruck/2;
  w2 += machine->gru->length_shaft*sin(-(G_PI/180.0) * machine->gru->rotation);
  w2 += g_get_external_radius(machine->gear);

  w3 = w2 + machine->car->car.width/2;

  if(w1 > w2 && w1 > w3)
    return w1;
  if(w2 > w1 && w2 > w3)
    return w2;
  if(w3 > w1 && w3 > w2)
    return w3;
  return w1;
}