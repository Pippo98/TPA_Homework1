#include <iostream>

#include "Gear.h"

using namespace std;

int main(){
    Gear* gear = g_init(20, 5);
    
    cout << g_to_string(gear) << endl;

    g_set_radius(gear, 21.3901431413);

    cout << g_to_string(gear) << endl;
 
    g_set_teeth(gear, -51);

    cout << g_to_string(gear) << endl;

    g_set_modulo(gear, 0.6);

    cout << g_to_string(gear) << endl;

    cout << g_get_radius(gear) << endl;
    cout << g_get_teeth(gear) << endl;
    cout << g_get_modulo(gear) << endl;

    return 0;
}
