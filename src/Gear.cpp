#include "Gear.h"

Gear* g_init(bool external_gear, double reference_radius, double axle_radius, int teeth){
    Gear* newGear = new Gear();

    newGear->external_gear = external_gear;

    newGear->reference_radius = reference_radius;
    newGear->axle_radius = axle_radius;

    newGear->teeth = teeth;

    if(g_check_integrity(newGear) != 0)
        return NULL;

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

int g_check_integrity(Gear* gear){

    if(gear->reference_radius <= 0)
        return -1;

    if(gear->axle_radius < 0)
        return -1;

    if(gear->teeth <= 0)
        return -1;

    
    double modulo = g_get_modulo(gear);
    double addendum = g_get_addendum(gear);
    double dedendum = g_get_dedendum(gear);

    if(gear->external_gear){
        if(gear->axle_radius >= gear->reference_radius - dedendum){
            return -1;
        }
    }
    else{
        if(gear->axle_radius <= gear->reference_radius - dedendum){
            return -1;
        }
    }

    return 0;
}

int g_set_external_gear(Gear* gear, bool external, double axle_radius){
    Gear old_gear = *gear;

    gear->external_gear = external;
    gear->axle_radius = axle_radius;

    int errcode = g_check_integrity(gear);
    if(errcode != 0)
        *gear = old_gear;

    return errcode;
}

int g_set_reference_radius(Gear* gear, double radius){
    Gear old_gear = *gear;

    gear->reference_radius = radius;

    int errcode = g_check_integrity(gear);
    if(errcode != 0)
        *gear = old_gear;

    return 0;
}

int g_set_axle_radius(Gear* gear, double radius){
    Gear old_gear = *gear;

    gear->axle_radius = radius;

    int errcode = g_check_integrity(gear);
    if(errcode != 0)
        *gear = old_gear;

    return 0;
}

int g_set_teeth(Gear* gear, int teeth){
    Gear old_gear = *gear;
    
    gear->teeth = teeth;

    int errcode = g_check_integrity(gear);
    if(errcode != 0)
        *gear = old_gear;

    return 0;
}

int g_set_modulo(Gear* gear, double modulo){

    Gear old_gear = *gear;

    double teeth = (gear->reference_radius*2) / modulo;
    gear->teeth = int(teeth);

    int errcode = g_check_integrity(gear);
    if(errcode != 0)
        *gear = old_gear;

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

double g_get_addendum(Gear* gear){
    return 1 * g_get_modulo(gear);
}

double g_get_dedendum(Gear* gear){
    return 1.25 * g_get_modulo(gear);
}

double g_get_tooth_height(Gear* gear){
    return 2.25 * g_get_modulo(gear);
}

double g_get_tooth_thickness(Gear* gear){
    return (G_PI * g_get_modulo(gear)) / 2;
}
