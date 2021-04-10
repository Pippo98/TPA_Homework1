#include "Gear.h"

Gear* g_init(double radius, uint8_t teeth){
    Gear* newGear = new Gear();

    newGear->radius = radius;
    newGear->teeth  = teeth;

    return newGear;
}

string g_to_string(Gear* gear){
    string ret = "";

    ret += "Gear:\n";
    ret += "\tRadius: ";
    ret += to_string(gear->radius);
    ret += "\tTeeth: ";
    ret += to_string(gear->teeth);

    return ret;
}

int g_set_radius(Gear* gear, double radius){
    gear->radius = radius;

    return 0;
}

int g_set_teeth(Gear* gear, uint8_t teeth){
    gear->teeth = teeth;

    return 0;
}
