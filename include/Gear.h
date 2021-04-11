#ifndef GEAR_H
#define GEAR_H

#include <iostream>
#include <string.h>

using namespace std;

struct Gear{
    
    double radius = 0.0;

    int teeth = 0;
};

/**
* Initialize gear struct
*
* @param radius radius of gear
* @param teeth numbers of teeths
* return initialized Gear
*/

Gear* g_init(double radius, int teeth);


/**
* Converts Gear string to readable string
* 
* @param gear to be converted
* return string representation of gear
*/
string g_to_string(Gear* gear);


/**
* Sets radius of given gear
*
* @param gear to be modified
* @param radius to be setted
* return errcode
*/
int g_set_radius(Gear* gear, double radius);


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
* Returns radius value
* 
* return radius
*/
double g_get_radius(Gear* gear);

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

#endif
