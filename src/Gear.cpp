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

    

    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius, gear->reference_radius, "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->axle_radius, gear->axle_radius, "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, g_get_base_radius(gear),  g_get_base_radius(gear), "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius - g_get_dedendum(gear),  gear->reference_radius - g_get_dedendum(gear), "fill:none;stroke:black;stroke-width:0.3") + "\n";
    svg += _g_get_ellipse(width/2, height/2, gear->reference_radius + g_get_addendum(gear),  gear->reference_radius + g_get_addendum(gear), "fill:none;stroke:black;stroke-width:0.3") + "\n";
    /*

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
        alpha = alpha * 180/( G_PI) + 90;

        double scale = (g_get_tooth_height(gear) / 83);
        double stroke_width = 1/scale;

        svg += _g_get_tooth(x, y, alpha, scale, "stroke:black;fill:none;stroke-width:" + to_string(stroke_width)) + "\n";
    }

    */
    
    string invL = g_generate_tooth_involute(gear, 0.08, true );
    string invR = g_generate_tooth_involute(gear, 0.08, false);

    svg += "<g transform='translate(" + to_string(width/2) + " " + to_string(height/2) + ")' >\n";
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
    double alpha = (sqrt(dp*dp-db*db)/db) * 180/G_PI - a;
    return 2.0 * (360.0/(4.0*N) - alpha);
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
    return "m -50,0 c 7.98916639,-1.0591 13.06514719,-8.7503 14.15291219,-16.2238 2.94506,-12.0289 5.721739,-24.1012 9.100499,-36.0213 2.95132,-10.6221 6.373089,-21.1807 11.366588,-31.0392 10.254553,-0.036 20.509142,-0.066 30.763632,-0.1151 7.252781,14.3371 11.14309,30.0662 15.258794,45.5122 2.534188,9.1705 3.90878,18.6713 7.106733,27.6448 1.935329,5.3018 6.631847,9.2907 12.23689,10.1263";
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



string g_generate_tooth_involute(Gear* gear, double increment, bool left_face){

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


    string path = "<path fill='transparent' stroke-width='0.1' stroke='black' d='M " + to_string(db/2) + " 0" ;
    while(true){
        x = r * (cos(t) + t*sin(t));
        y = r * (sin(t) - t*cos(t));

        if(left_face)
            t -= increment;
        else
            t += increment;

        if(sqrt(x*x+y*y) > dp / 2 + addendum)
            break;
//        if(sqrt(x*x+y*y) < dp / 2 - dedendum)
//            continue;

        // path += " L " + to_string(x) + " " + to_string(y);
        path += " C " + to_string(x*1.001) + " " + to_string(y*0.999) + ", " + to_string(x*0.999) + " " + to_string(y*0.99) + ", " + to_string(x) + " " + to_string(y);

    }

    path += "'/>'";
    double rp = dp / 2;
    double a = addendum;
    t = -sqrt((a - r + rp)*(a + r + rp))/r;
    if(!left_face)
        t *= -1;
    x = r * (cos(t) + t*sin(t));
    y = r * (sin(t) - t*cos(t));
    
    path += _g_get_ellipse(x,y,1,1,"");

    double b = dedendum;
    t = sqrt(-(-b + r + rp)*(b + r - rp))/r;
    x = r * (cos(t) + t*sin(t));
    y = r * (sin(t) - t*cos(t));

    cout << x << " " << y << endl;
    
    path += _g_get_ellipse(x,y,1,1,"");

    return path;

}
