#ifndef EB_DEVICE_H
#define EB_DEVICE_H

#include <string.h>
#include <cmath>
#include <fstream>
#include <streambuf>
#include <string>
#include <sstream>


using namespace std;

/**
 * 
 * A struct representing the device  
**/
struct EbDevice {
    double length_shaft;
    double width_towtruck;
    double width_platform;
    double rotation;
    double sliding;
};

/**
    A function initing a device 
**/
EbDevice* eb_init(double length_shaft, double width_towtruck, double width_platform, double rotation, double sliding);

/**
    A function checking mechanical costraints  
**/
bool eb_checkConstraints(double length_shaft, double width_towtruck, double width_platform, double rotation, double sliding);


/**
    A function checking costraints in relation to the svg draw
**/
bool eb_drawConstraints(EbDevice* eb_device);


/**
    A function which produce a string with svg code
    if with_measures is true the drawing will include measurements   
**/
string eb_to_svg(EbDevice* eb_device, bool with_measures);

/**
    A function which calculate coordinate X of CiR
**/
double eb_Xcir(EbDevice* eb_device);


/**
    A function which calculate coordinate X of the platform
**/
double eb_Xplatform(EbDevice* eb_device);


/**
    A function which calculate coordinate Y of the platform
**/
double eb_Yplatform(EbDevice* eb_device);


/*
    Sets a new length_shaft in the structure       
    if the new length is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_lengthShaft(EbDevice* device, double new_length_shaft);

/*
    Sets a new width_towtruck in the structure       
    if the new width is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_widthTowtruck(EbDevice* device, double new_width_towtruck);

/*
    Sets a new width_platform in the structure       
    if the new width is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_widthPlatform(EbDevice* device, double new_width_platform);

/*
    Sets  rotation in the structure       
    if the new rotation is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_rotation(EbDevice* device, double new_rotation);

/*
    Sets  sliding in the structure       
    if the new sliding is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_sliding(EbDevice* device, double new_sliding);


/*
    A function which write on a file svg
*/
void eb_save_to_file(string stringToWrite, string filename);

/*
    function, which creates a struct from a SVG textual representation
*/
EbDevice* eb_parse(string svg);

/*
    A function which read from a file a string
*/
string eb_read_from_file(string filename);

#endif //DEVICE_H
