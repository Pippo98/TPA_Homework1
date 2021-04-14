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
    "<svg version='1.1' viewBox='0 0 640 480' xmlns='http://www.w3.org/2000/svg' style='background: white' >\n";

    
    /*
    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius, gear->reference_radius, "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->axle_radius, gear->axle_radius, "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius - g_get_dedendum(gear),  gear->reference_radius - g_get_dedendum(gear), "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius + g_get_addendum(gear),  gear->reference_radius + g_get_addendum(gear), "fill:none;stroke:black;stroke-width:0.3") + "\n";
    */
    svg += _g_get_ellipse(width/2, height/2, g_get_base_radius(gear),  g_get_base_radius(gear), "fill:none;stroke:black;stroke-width:0.3") + "\n";
    
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

    string invL = g_generate_tooth_involute(gear, 10, true );
    string invR = g_generate_tooth_involute(gear, 10, false);

    svg += "<g transform='translate(" + to_string(width/2) + " " + to_string(height/2) + ") ";
    svg += "rotate(" + to_string(-g_get_beta(gear)/2) + " 0 0) ";
    svg += "' >\n";
    for(int i = 0; i < gear->teeth; i++){
        svg += "<g transform='rotate(" + to_string((360.0/gear->teeth)*i) + " 0 0)' >\n";
        svg += invR + "\n";
        svg += "<g transform='rotate(" + to_string(-g_get_beta(gear)) + " 0 0)' >\n";
        svg += invL + "\n";
        svg += "</g>\n";
        svg += "</g>\n";
    }
    svg += "</g>";
    
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

double g_get_base_radius(Gear* gear){
    return  gear->reference_radius * cos(gear->pressure_angle * G_PI / 180.0);
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
    return 1.157 * g_get_modulo(gear);
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

double g_get_beta(Gear* gear){
    int N = gear->teeth;
    double a =  gear->pressure_angle;
    double dp = gear->reference_radius * 2;
    double db = g_get_base_radius(gear) * 2;
    double alpha = (sqrt(dp*dp-db*db)/db) * 180.0/G_PI - a;
    return (180.0/N - alpha);
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

void _g_polar_to_cartesian(double r, double alpha, double* x, double* y){

    *x = r * cos(alpha);
    *y = r * sin(alpha);
}

void _g_cartesian_to_polar(double x, double y, double* r, double* alpha){
    *r = sqrt(x*x + y*y);
    *alpha = atan2(y, x);
}

void _g_rotate_point(double* x, double *y, double alpha){
    *x = *x * cos(alpha) - *y * sin(alpha);
    *y = *x * sin(alpha) + *y * cos(alpha);
}

string g_generate_tooth_involute(Gear* gear, double chunks, bool left_face){

    string path = "";

    int N = gear->teeth;
    double addendum = g_get_addendum(gear);
    double dedendum = g_get_dedendum(gear);
    double dp = gear->reference_radius * 2;
    double db = g_get_base_radius(gear) * 2;

    double beta = g_get_beta(gear);
    
    double x = 0;
    double y = 0;
    double r = db/2;
    double t = 0;
    double t0 = 0.0;
    double t1 = 0.0;

    double rp = dp / 2;
    double rb = db / 2;
    double a = addendum;
    double b = dedendum;
    
    // This is the solution of intersection between involute curve
    // and top circle (gear->reference_radius + addendum).
    t1 = sqrt((a - r + rp)*(a + r + rp))/r;

    // This is the solution of intersection between involute curve
    // and bottom circle (gear->reference_radius - dedendum).
    // This equation can have no solution, so check for nan value (not a number)
    t0 = sqrt(-(-b + r + rp)*(b + r - rp))/r;
    if(isnan(t0))
        t0 = 0;
    
    // Path command
    path += "<path fill='transparent' stroke-width='0.5' stroke='black' d='";
    // Moves the cursor to (radius, 0)
    path += "M " + to_string(db/2) + " 0" ;

    
    // t is an angle, draw the involute curve by changing it.
    // t must go from t0 to t1 (t0 is on base_circle, t1 is on top_circle)
    // last_cycle helper bool to force, on the last loop, setting t to t1
    t = t0;
    bool last_cycle = false;
    double increment = (t1 - t0)/chunks;
    while(true){

        // If left face use negative values of t
        if(left_face){
            x = r * (cos(-t) - t*sin(-t));
            y = r * (sin(-t) + t*cos(-t));
        }
        else{
            x = r * (cos(t) + t*sin(t));
            y = r * (sin(t) - t*cos(t));
        }
        
        // Line command: draws line from previous point to x,y
        path += " L " + to_string(x) + " " + to_string(y);
        
        t += increment;
        if(last_cycle)
            break;
        // If t is greater than t1 force one last loop with t = t1
        if(t > t1 && !last_cycle){
            last_cycle=true;
            t = t1;
        }
    }

    path += "'/>'";

    double alpha;
    double radius;
    double x1, x2, y1, y2;

    if(left_face){
       t0 = 0;
       x1 = r * (cos(t0) + t0*sin(t0));
       y1 = r * (sin(t0) - t0*cos(t0));
   
       
       path += "<path fill='transparent' stroke='black' stroke-width='0.5' d='";
       path += "M " + to_string(x1) + " " + to_string(y1) + " " ;

       path += "A " + to_string(g_get_beta(gear)) + " " + to_string(g_get_beta(gear)) + " ";
       path += "0 0 0 ";
       _g_rotate_point(&x1, &y1, g_get_beta(gear) * G_PI/180.0);
       path += to_string(x1) + " " + to_string(y1);

       path += "' />";
    }
    else{
       // Computing connection arc between two teeth
       x2 = r * (cos(-t1) - t1*sin(-t1));
       y2 = r * (sin(-t1) + t1*cos(-t1));
       x1 = r * (cos(t1) + t1*sin(t1));
       y1 = r * (sin(t1) - t1*cos(t1));
       
       alpha = 2 * G_PI/N - g_get_beta(gear) * G_PI/180.0;
       
       _g_rotate_point(&x2, &y2, alpha);
       
       path += "<path fill='transparent' stroke='black' stroke-width='0.5' d='";
       path += "M " + to_string(x1) + " " + to_string(y1) + " " ;
       path += "A " + to_string(rp + a) + " " + to_string(rp + a) + " ";
       path += "0 0 1 ";
       path += to_string(x2) + " " + to_string(y2);
       path += "' />";
    }

    return path;

}
