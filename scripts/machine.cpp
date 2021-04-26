#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "Gear.h"
#include "EB_Device.h"

int main(){
  // Creating two basics devices
  EbDevice* gru = eb_init(600, 300, 350, -30, 0);

  if(gru == NULL){
    cout << "Gru init Failed!" << endl;
    return -1;
  }

  //Gear* gear = g_init(true, 120, 50, 50, 20);
  Gear* gear = g_init(false, 80, 120, 50, 20);
  
  if(gear == NULL){
    cout << "Gear init Failed!" << endl;
    return -1;
  }

  // Getting gru svg string

  string gru_svg = eb_to_svg(gru, false);
  string gru_gear = gru_svg;
  string gru_chain = gru_svg;
  
  // Positions of platform to then lay gears onto it
  double x_platform = eb_Xplatform(gru);
  double y_platform = eb_Yplatform(gru);
  
  // Getting center of platform and adding gear size
  x_platform += gru->width_platform / 2;
  y_platform -= g_get_external_radius(gear);
  
  // Translating gear to center of platform
  string gear_svg = "<g transform='translate(" + _str(x_platform) + " " + _str(y_platform) + ")'>\n";
  gear_svg += g_to_svg(gear, false, false, 5) + "\n";
  gear_svg += "</g>\n";
  
  // Adding gear string before </svg> tag
  gru_svg.insert(gru_svg.find("</svg>"), gear_svg + "\n");
  g_export_svg(gru_svg, "output/machine_gear");
  
  //------ CHAIN ------//

  
  // Creating two gears to initialize a chain
  Gear* gear1 = g_init(true, 50, 10, 20, 20);
  Gear* gear2 = g_init_for_connection(gear1, true, 20, 10);
  Connection* conn1 = g_init_connection(gear1, gear2, 40);

  // Getting gru platform coordinates
  x_platform = eb_Xplatform(gru);
  y_platform = eb_Yplatform(gru);

  double w, h;
  g_get_connection_sizes(conn1, &w, &h);

  x_platform += gru->width_platform / 2;
  y_platform -= h;
  
  // Traslating chain to platform position
  string chain = "<g transform='translate(" + _str(x_platform) + " " + _str(y_platform) + ")'>\n";
  chain += g_connection_to_svg(conn1, false) + "\n";
  chain += "</g>";


  // Adding chain string before svg close tag
  gru_chain.insert(gru_chain.find("</svg>"), chain);
  g_export_svg(gru_chain, "output/machine_chain");

  cout << w << " " << h << endl;
} 
