#include <iostream>

#include "Gear.h"

using namespace std;

int main(){
    Gear* gear = g_init(true, 20, 10, 5);

    if(gear == NULL){
        cout << "Gear not valid" << endl;
        return -1;
    }
    
    cout << g_to_string(gear) << endl;

    g_set_reference_radius(gear, 210.3901431413);

    g_set_teeth(gear, -51);

    cout << g_to_string(gear) << endl;
 
    g_set_modulo(gear, 6);

    cout << g_to_string(gear) << endl;
    
    // When setting eternal gear to false (so tooths are internal)
    // axle radius must be greater than reference radius!
    g_set_external_gear(gear, false, 5);

    cout << g_to_string(gear) << endl;

    cout << g_get_external_gear(gear)       << endl;
    cout << g_get_reference_radius(gear)    << endl;
    cout << g_get_axle_radius(gear)         << endl;
    cout << g_get_teeth(gear)               << endl;
    cout << g_get_modulo(gear)              << endl;

    cout << "Addeddum: "        << g_get_addendum(gear)         << endl;
    cout << "Dedendum: "        << g_get_dedendum(gear)         << endl;
    cout << "Tooth height: "    << g_get_tooth_height(gear)     << endl;
    cout << "Tooth thickness: " << g_get_tooth_thickness(gear)  << endl;

    g_set_teeth(gear, 10);
    g_set_axle_radius(gear, 20);
    g_set_reference_radius(gear, 180);
    g_export_svg(gear, "gear");


    return 0;
}
