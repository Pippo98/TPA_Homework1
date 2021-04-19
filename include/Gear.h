#ifndef GEAR_H
#define GEAR_H

#include <math.h>
#include <fstream>
#include <iostream>
#include <string.h>

#define G_PI 3.14159265359

using namespace std;

struct Gear{

    bool external_gear = true;
    
    double reference_radius = 0.0;
    double axle_radius = 0.0;

    int teeth = 0;

    double pressure_angle = 0.0;
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

Gear* g_init(bool external_gear, double reference_radius, double axle_radius, int teeth, double pressure_angle=20);


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
* Returns a string in svg format
* 
* return svg string
*/
string g_to_svg(Gear* gear);

/**
* Saves gear as a svg file
* 
* @param gear gear to be saved
* @param filename file name that will be used,
*                 without extension
* return errcode
*/
int g_export_svg(Gear* gear, string filename);


/**
* Gets a svg file, parse it, and returns an instance of the gear or NULL
*
* @param filename, path without extension
* return NULL or instance of Gear
*/
Gear* g_from_svg(string filename);

string _g_get_svg_arg(string line, string arg);


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
* Sets Pressure angle
* 
* @param gear gear to be modified
* @param angle pressure angle
* return errcode
*/
int g_set_pressure_angle(Gear* gear, double angle);


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

/**
* Returns base radius (base of the tooth)
* 
* return radius
*/
double g_get_base_radius(Gear* gear);

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

/**
* Get the angle between two near teeth
* 
* return angle
*/
double get_pitch_angle(Gear* gear);

/**
* Get the angle between two near teeth
* 
* return angle
*/
double g_get_beta(Gear* gear);

/**
* Returns string of svg ellipse component
*
* @param cx center x of the ellipse
* @param cy center y of the ellipse
* @param rx radius x of the ellipse
* @param ry radius y of the ellipse
* @param style of the ellipse component
* return string ellipse
*/
string _g_get_ellipse(double cx, double cy, double rx, double ry, string style, string id="");


/**
* Returns string of svg line
* 
* @param p1x
* @param p1y
* @param p2x
* @param p2y
* @param style of the ellipse component 
*/
string _g_get_line(double p1x, double p1y, double p2x, double p2y, string style);


/**
* Returns the path of a single tooth centered in origin
* 
* return string of path
*/
string _g_get_tooth_path();


/**
* Returns a string representing the svg component of one tooth
* It can be rotated scaled and translated
* 
* @param traslationX traslate on x axis
* @param traslationY traslate on y axis
* @param rotation rotates tooth in degrees
* @param scale the tooth by the percentage (100% = 1.0). Note that
*        the tooth component by default is 100 pixels wide.
* @param style to spply ad the path, default null
*/
string _g_get_tooth(double traslationX, double traslationY, double rotation, double scale, string style="");

/**
* Converts polar coordinates to cartesian coordinates
* 
* @param r radius
* @param alpha angle in radians
* @param x pointer to new x
* @param y pointer to new y
*/
void _g_polar_to_cartesian(double r, double alpha, double* x, double* y);

/**
* Converts cartesian coordinates to polar
* 
* @param x
* @param y
* @param r radius
* @param alpha angle
*/
void _g_cartesian_to_polar(double x, double y, double* r, double* alpha);

/**
* 
* 
*/
void _g_rotate_point(double* x, double* y, double alpha);
/**
* The shape of the tooth depends on the parameters of the gear.
* Calculate a path for the given gear, then can be repeated for all the teeth.
* 
* @param gear
* @param cunks resolution of the tooth shape ( greater num greater resolution 10 is ok)
* @param left_face computes different paths depending on the face requested
* return string representing svg path of one tooth face
*/
string g_generate_tooth_involute(Gear* gear, double chunks, bool left_face);


/**
* Generates the path to draw the bottom path to connect two teeth
* 
* 
* 
*/
string g_generate_tooth_connection_path(Gear* gear);

#endif
