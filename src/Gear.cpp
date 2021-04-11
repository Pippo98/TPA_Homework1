#include "Gear.h"

Gear* g_init(bool external_gear, double reference_radius, double axle_radius, int teeth){
    Gear* newGear = new Gear();

    newGear->external_gear = external_gear;

    newGear->reference_radius = reference_radius;
    newGear->axle_radius = axle_radius;

    newGear->teeth  = teeth;

    return newGear;
}

string g_to_string(Gear* gear){
    string ret = "";

    ret += "Gear:";
    ret += "\n\tExternal gear: ";
    ret += gear->external_gear ? "True" : "False";
    ret += "\n\tReference radius: ";
    ret += to_string(gear->reference_radius);
    ret += "\n\tAxle radius: ";
    ret += to_string(gear->axle_radius);
    ret += "\n\tTeeth: ";
    ret += to_string(gear->teeth);

    return ret;
}

int g_set_external_gear(Gear* gear, bool external, double axle_radius){
    gear->external_gear = external;
    gear->axle_radius = axle_radius;

    return 0;
}

int g_set_reference_radius(Gear* gear, double radius){
    gear->reference_radius = radius;

    return 0;
}

int g_set_axle_radius(Gear* gear, double radius){
    gear->axle_radius = radius;

    return 0;
}

int g_set_teeth(Gear* gear, int teeth){
    gear->teeth = teeth;

    return 0;
}

int g_set_modulo(Gear* gear, double modulo){
    double teeth = (gear->reference_radius*2) / modulo;
    gear->teeth = int(teeth);

    return 0;
}

bool g_get_external_gear(Gear* gear){
    return gear->external_gear;
}

double g_get_reference_radius(Gear* gear){
    return gear->reference_radius;
}

double g_get_axle_radius(Gear* gear){
    return gear->axle_radius;
}

int g_get_teeth(Gear* gear){
    return gear->teeth;
}

double g_get_modulo(Gear* gear){
    return (gear->reference_radius*2) / gear->teeth;
}
