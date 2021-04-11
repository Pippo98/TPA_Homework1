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

int g_set_modulo(Gear* gear, double modulo){
    double teeth = (gear->radius*2) / modulo;
    gear->teeth = int(teeth);

    return 0;
}

double g_get_radius(Gear* gear){
    return gear->radius;
}

uint8_t g_get_teeth(Gear* gear){
    return gear->teeth;
}

double g_get_modulo(Gear* gear){
    return (gear->radius*2) / gear->teeth;
}
