#include "Machine.h"

PhilMachine* phil_init_machine(double gru_height, double raising_speed, double car_width){
  //EbDevice* gru = eb_init(300, 400, 50, 20, 0);
  EbDevice* gru = eb_init(gru_height*5/7, gru_height, gru_height/10, -20, 10);
  if(gru == NULL){
    cout << "Gru failed" << endl;
    return NULL;
  } 

  Gear* gear;
  if(gru_height/10 > (raising_speed + 5))
    gear = g_init(true, (raising_speed + 5), gru_height/20, gru_height/20, 20);
  else
    gear = g_init(false, gru_height/20, (raising_speed + 5), gru_height/20, 20);

  if(gear == NULL){
    cout << "Gear failed" << endl;
    return NULL;
  }

  coca_device* car = new coca_device();

  car->car.width = car_width;
  car->car.height = car_width/5;
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

  EbDevice* gru = eb_init(300, 400, 50, 20, 0);

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