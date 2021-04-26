#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "Gear.h"
#include "EB_Device.h"

int main(){

  EbDevice* gru = eb_init(600, 300, 350, -30, 0);

  if(gru == NULL){
    cout << "Gru init Failed!" << endl;
    return -1;
  }

  Gear* gear = g_init(true, 120, 50, 50, 20);
  
  if(gear == NULL){
    cout << "Gear init Failed!" << endl;
    return -1;
  }

  string gru_svg = eb_to_svg(gru, false);
  eb_save_to_file(gru_svg, "gru");

  double x_platform = eb_Xplatform(gru);
  double y_platform = eb_Yplatform(gru);

  x_platform += gru->width_platform / 2;
  y_platform -= g_get_external_radius(gear);


  string gear_svg = "<g transform='translate(" + _str(x_platform) + " " + _str(y_platform) + ")'>\n";
  gear_svg += g_to_svg(gear, false, false, 5) + "\n";
  gear_svg += "</g>\n";

  gru_svg.insert(gru_svg.find("</svg>"), gear_svg + "\n");
  eb_save_to_file(gru_svg, "machine");
  
} 
