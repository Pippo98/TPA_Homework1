#ifndef GEAR_H
#define GEAR_H

#include <iostream>
#include <string.h>

#define G_PI 3.14159265359

using namespace std;

struct Gear{

    bool external_gear = true;
    
    double reference_radius = 0.0;
    double axle_radius = 0.0;

    int teeth = 0;
};

/**
* Initialize gear struct
*
* @param external_gear defines if teeth are external or internal
* @param reference_radius reference radius of gear
* @param axle_radius radius of axle
* @param teeth numbers of teeths
* return initialized Gear
*/

Gear* g_init(bool external_gear, double reference_radius, double axle_radius, int teeth);


/**
* Converts Gear string to readable string
* 
* @param gear to be converted
* return string representation of gear
*/
string g_to_string(Gear* gear);

/**
* Checks if the current is valid, so all contrant etc..
*
* @param gear to be checked
* return errcode
*/
int g_check_integrity(Gear* gear);

/**
* Sets external gear flag
* when changing external_gear flag also the axle radius
* must be changed for validation of the gear
* 
* @param gear to be modified
* @param external flag to be setted
* @param axle_radius radius to be modified
* return errcode
*/
int g_set_external_gear(Gear* gear, bool external, double axle_radius);


/**
* Sets reference radius of given gear
*
* @param gear to be modified
* @param radius to be setted
* return errcode
*/
int g_set_reference_radius(Gear* gear, double radius);


/**
* Sets axle radius of given gear
*
* @param gear to be modified
* @param radius to be setted
* return errcode
*/
int g_set_axle_radius(Gear* gear, double radius);


/**
* Sets teeth of given gear
*
* @param gear to be modified
* @param teeth to be setted
* return errcode
*/
int g_set_teeth(Gear* gear, int teeth);


/**
* Sets teeth number with modulo value defined by: m = d/z
* where d is diameter and z is number of teeths
* 
* @param gear gear to be modified
* @param modulo value to be setted
* return errcode
*/
int g_set_modulo(Gear* gear, double modulo);


/**
* Returns the external gear flag
* 
* return external_gear flag
*/
bool g_get_external_gear(Gear* gear);

/**
* Returns reference radius value
* 
* return radius
*/
double g_get_reference_radius(Gear* gear);

/**
* Returns axle radius value
* 
* return radius
*/
double g_get_axle_radius(Gear* gear);


/*
* Returns number of teeths
*
* return teeth
*/
int g_get_teeth(Gear* gear);


/*
* Return modulo of the gear m=d/z
* where d is diameter and z is teeth number
*
* return modulo
*/
double g_get_modulo(Gear* gear);

/**
* Returns addendum, is the distance from reference radius
* to the tip of the tooth.
*
* return addendum 
*/
double g_get_addendum(Gear* gear);

/**
* Returns dedendum, is the distance from reference radius
* to the bottom of the tooth.
*
* return dedendum
*/
double g_get_dedendum(Gear* gear);

/**
* Returns the height of the tooth
* 
* return height
*/
double g_get_tooth_height(Gear* gear);

/**
* Returns the thickness of one tooth of the gear
*
* return thickness
*/
double g_get_tooth_thickness(Gear* gear);

#endif
