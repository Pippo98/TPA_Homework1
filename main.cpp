#include <iostream>

#include "Gear.h"

using namespace std;

int main(){
    Gear* gear = g_init(20, 5);
    
    cout << g_to_string(gear) << endl;

    g_set_radius(gear, 21.3901431413);

    cout << g_to_string(gear) << endl;
 
    g_set_teeth(gear, 51);

    cout << g_to_string(gear) << endl;

    return 0;
}
