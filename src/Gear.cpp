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

string g_to_svg(Gear* gear){
    int width = 640;
    int height = 480;
    
    float oversize = 1.1;

    string svg = "<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n"
    "<svg version='1.1' viewBox='0 0 640 480' xmlns='http://www.w3.org/2000/svg'>\n";

    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius, gear->reference_radius, "fill:none;stroke:black") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->axle_radius, gear->axle_radius, "fill:none;stroke:black") + "\n";

    svg += _g_get_line((width/2) - gear->reference_radius * oversize,
                       (height/2),
                       (width/2) + gear->reference_radius * oversize,
                       (height/2),
                       "stroke='black' stroke-dasharray='5, 4' stroke-opacity='0.3'"
                       ) + "\n";
    svg += _g_get_line((width/2),
                       (height/2) - gear->reference_radius * oversize,
                       (width/2),
                       (height/2) + gear->reference_radius * oversize,
                       "stroke='black' stroke-dasharray='5, 4' stroke-opacity='0.3'"
                       ) + "\n";

    for (int i = 0; i < gear->teeth; i++){
        double r = gear->reference_radius - g_get_dedendum(gear);
        double alpha = (2 * G_PI / gear->teeth) * i;
        double x = 0.0;
        double y = 0.0;

        _g_polar_to_cartesian(r, alpha, &x, &y);
        x += width/2;
        y += height/2;
        alpha = alpha * 180/( G_PI) + 90 + 2;

        double scale = (g_get_tooth_height(gear) / 100) * 1.5;

        svg += _g_get_tooth(x, y, alpha, scale, "stroke:black;fill:none;stroke-width:10") + "\n";
    }               

    
    svg += "\n</svg>";
    return svg;
}

int g_export_svg(Gear* gear, string filename){
    ofstream file(filename + ".svg");
    file << g_to_svg(gear);
    file.close();
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

double g_get_pitch_angle(Gear* gear){
    return (2 * G_PI * gear->reference_radius) / gear->teeth;
}

string _g_get_ellipse(double cx, double cy, double rx, double ry, string style){
    string ellipse = "<ellipse ";

    ellipse += "cx='" + to_string(cx) + "' ";
    ellipse += "cy='" + to_string(cy) + "' ";
    ellipse += "rx='" + to_string(rx) + "' ";
    ellipse += "ry='" + to_string(ry) + "' ";

    ellipse += "style='" + style + "' ";

    ellipse += "/>";
    return ellipse;
}

string _g_get_line(double p1x, double p1y, double p2x, double p2y, string style){
    string line = "<line ";

    line += "x1='" + to_string(p1x) + "' ";
    line += "y1='" + to_string(p1y) + "' ";
    line += "x2='" + to_string(p2x) + "' ";
    line += "y2='" + to_string(p2y) + "' ";
    
    line += " " + style + " ";

    line += "/>";
    return line;
}


string _g_get_tooth_path(){
    return "M 0.28239468,-0.12839759 15.870449,-3.2483876 c 3.62898,-1.03 6.051609,-3.46 7.055629,-7.5500004 0.5086,-4.41 1.52616,-7.13 0.8205,-15.587 -1.15718,-8.04 -3.516779,-8.18 -4.266229,-14.275 0.979299,-13.108 5.146839,-29.746 17.064823,-46.92899 l 25.597261,-0.2 c 3.91099,6.46 16.539136,20.07699 18.213257,49.38899 -0.9539,5.17 -2.22611,4.36 -3.77391,12.142 -0.4077,4.56 -0.792799,10.603 0.8205,16.4090004 1.74844,4.81 2.20722,4.97 5.57887,6.23 l 16.736476,3.77999001";
}


string _g_get_tooth(double traslationX, double traslationY, double rotation, double scale, string style){
    string tooth = "";
    
    tooth += "<g ";
    tooth += "transform='";
    tooth += "translate(" + to_string(traslationX) + " " + to_string(traslationY) + ") ";
    tooth += "rotate(" + to_string(rotation) + " 0 0) ";
    tooth += "scale(" + to_string(scale) + " " + to_string(scale) +") ";
    tooth += "' >\n";

    tooth += "\t<path ";
    tooth += "style='" + style + "' ";
    tooth += "d='" + _g_get_tooth_path() + "' ";
    tooth += "/>\n";

    tooth += "</g>";

    return tooth;
}

void _g_polar_to_cartesian(double r, double alpha, double* x, double* y){
    *x = r * cos(alpha);
    *y = r * sin(alpha);
}
