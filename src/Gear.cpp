#include "Gear.h"

Gear* g_init(bool external_gear, double reference_radius, double axle_radius, int teeth, double pressure_angle) {
  Gear* newGear = new Gear();

  newGear->external_gear = external_gear;

  newGear->reference_radius = reference_radius;
  newGear->axle_radius = axle_radius;

  newGear->teeth = teeth;

  newGear->pressure_angle = pressure_angle;

  if (g_check_integrity(newGear) != 0)
    return NULL;

  return newGear;
}

string g_to_string(Gear* gear, bool extended) {
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
  ret += "\n\tAlpha: ";
  ret += to_string(gear->pressure_angle);

  if(extended){
    ret += "\n";
    ret += "\n\tBase Radius: ";
    ret += to_string(g_get_base_radius(gear));
    ret += "\n\tModulo: ";
    ret += to_string(g_get_modulo(gear));
    ret += "\n\tAddendum: ";
    ret += to_string(g_get_addendum(gear));
    ret += "\n\tDedendum: ";
    ret += to_string(g_get_dedendum(gear));
    ret += "\n\tTooth height: ";
    ret += to_string(g_get_tooth_height(gear));
    ret += "\n\tBeta: ";
    ret += to_string(g_get_beta(gear));
    ret += "\n\tAlpha: ";
    ret += to_string(g_get_alpha(gear));
  }

  return ret;
}

int g_check_integrity(Gear* gear) {
  if (gear->reference_radius <= 0)
    return -1;

  if (gear->axle_radius < 0)
    return -1;

  if (gear->teeth <= 0)
    return -1;

  if (gear->pressure_angle < 0 || gear->pressure_angle > 45)
    return -1;

  double modulo = g_get_modulo(gear);
  double addendum = g_get_addendum(gear);
  double dedendum = g_get_dedendum(gear);
  if (gear->external_gear) {
    if (gear->axle_radius >= gear->reference_radius - dedendum) {
      return -1;
    }
  } else {
    if (gear->axle_radius <= gear->reference_radius - dedendum) {
      return -1;
    }
  }

  return 0;
}

string g_to_svg(Gear* gear) {
  int width = 640;
  int height = 480;
  int a_duration = 20;

  float stroke = 1;
  float oversize = 1.1;

  string svg = "";
  string quote_style = "fill:none;stroke:blue;stroke-width:" + to_string(stroke / 3);
  string gear_style = "fill:none;stroke:black;stroke-width:" + to_string(stroke);

  // SVG header
  svg += "<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n";
  svg += "<svg version='1.1' viewBox='0 0 640 480' xmlns='http://www.w3.org/2000/svg' style='background: white' >\n";

  // Header to be able to load gear parameters
  svg += "\n\n<!--\n";
  svg += "Gear n° 1\n";
  svg += "external_gear:" + to_string(gear->external_gear) + "\n";
  svg += "reference_radius:" + to_string(gear->reference_radius) + "\n";
  svg += "axle_radius:" + to_string(gear->axle_radius) + "\n";
  svg += "teeth:" + to_string(gear->teeth) + "\n";
  svg += "pressure_angle:" + to_string(gear->pressure_angle) + "\n";
  svg += "-->\n\n\n";

  // Container for animation
  // Add rotation animation
  svg += "<g>";
  svg +=
      "<animateTransform attributeName='transform' attributeType='XML' "
      "type='rotate' "
      "from='0 " +
      to_string(width / 2) + " " + to_string(height / 2) +
      "' "
      "to='" +
      to_string(360.0) + " " + to_string(width / 2) + " " + to_string(height / 2) +
      "' "
      "dur='" +
      to_string(a_duration) +
      "s' "
      "repeatCount='indefinite' "
      "/>";


  if(false){
      // Draw reference circle
      svg += _g_get_ellipse(width / 2,
                            height / 2,
                            gear->reference_radius,
                            gear->reference_radius,
                            quote_style,
                            "reference") + "\n";

      // Dimension lines and circles
      svg += _g_get_ellipse(width / 2,
                            height / 2,
                            gear->reference_radius + g_get_addendum(gear),
                            gear->reference_radius + g_get_addendum(gear),
                            quote_style) + "\n";
      svg += _g_get_ellipse(width / 2,
                            height / 2,
                            g_get_base_radius(gear),
                            g_get_base_radius(gear),
                            quote_style) + "\n";

      svg += _g_get_line((width / 2) - gear->reference_radius * oversize,
                         (height / 2),
                         (width / 2) + gear->reference_radius * oversize,
                         (height / 2),
                         "stroke='black' stroke-dasharray='5, 4' stroke-opacity='0.3'") + "\n";
      svg += _g_get_line((width / 2),
                         (height / 2) - gear->reference_radius * oversize,
                         (width / 2),
                         (height / 2) + gear->reference_radius * oversize,
                         "stroke='black' stroke-dasharray='5, 4' stroke-opacity='0.3'") + "\n";
  }

  // Draw axle radius
  svg += _g_get_ellipse(width / 2, height / 2, gear->axle_radius, gear->axle_radius, gear_style, "axle") + "\n";

  // Generate the tooth shape
  string invL = "<path style='" + gear_style + "' d='";
  invL += g_generate_tooth_involute(gear, 30, true) + "' />\n";
  string invR = "<path style='" + gear_style + "' d='";
  invR += g_generate_tooth_involute(gear, 30, false) + "' />\n";
  

  svg += "<g transform='translate(" + to_string(width / 2) + " " + to_string(height / 2) + ") ";
  svg += "' >\n";

  // Draw right face, then rotate of beta angle and draw left face
  // Then draw another tooth
  for (int i = 0; i < gear->teeth; i++) {
    svg += "<g id='toothn" + to_string(i + 1) + "' " + "transform='rotate(" + to_string((360.0 / gear->teeth) * i) + " 0 0)' >\n";
    svg += invR;
    svg += "<g transform='rotate(" + to_string(-g_get_beta(gear)) + " 0 0)' >\n";
    svg += invL;
    svg += "</g>\n";
    svg += "</g>\n";
  }
  
  svg += "</g>";
  svg += "</g>";

  svg += "\n</svg>";
  return svg;
}

int g_export_svg(Gear* gear, string filename) {
  ofstream file(filename + ".svg");
  file << g_to_svg(gear);
  file.close();
  return 0;
}

string _g_get_svg_arg(string line, string arg) {
  size_t index1 = 0;
  size_t index2 = 0;

  if (line.find(arg) == string::npos)
    return "";

  if (arg.find("=") == string::npos) {
    arg += "='";
  } else if (arg.find("'") == string::npos) {
    arg += "'";
  }

  index1 = line.find(arg);
  index1 += arg.length();
  index2 = line.find("'", index1);
  return line.substr(index1, index2 - index1);
}

Gear* g_from_svg(string filename) {
  ifstream svg(filename + ".svg");
  string line = "";

  bool external = false;
  double r1 = 0.0;
  double r2 = 0.0;
  int N = 0;
  double alpha = 0.0;

  bool gear_param_comment = false;

  if (svg.is_open()) {
    while (getline(svg, line)) {
      size_t index = 0;

      if (gear_param_comment) {
        if (line.find("pressure_angle:") != string::npos) {
          index = line.find_last_of(":");
          alpha = stod(line.substr(index + 1));
        }
        if (line.find("external_gear:") != string::npos) {
          index = line.find_last_of(":");
          external = stoi(line.substr(index + 1)) == 1 ? true : false;
        }

        if (line.find("-->") != string::npos)
          gear_param_comment = false;
      } else {
        if (line.find("<!--") != string::npos)
          gear_param_comment = true;
      }

      if (line.find("id='tooth") != string::npos)
        N++;
      if (line.find("<ellipse") != string::npos) {
        if (line.find("id='reference") != string::npos) {
          r1 = stof(_g_get_svg_arg(line, "rx"));
        } else if (line.find("id='axle") != string::npos) {
          r2 = stof(_g_get_svg_arg(line, "rx"));
        }
      }
    }
    svg.close();
  }

  Gear* gear = g_init(external, r1, r2, N, alpha);

  return gear;
}

int g_set_external_gear(Gear* gear, bool external, double axle_radius) {
  Gear old_gear = *gear;

  gear->external_gear = external;
  gear->axle_radius = axle_radius;

  int errcode = g_check_integrity(gear);
  if (errcode != 0)
    *gear = old_gear;

  return errcode;
}

int g_set_reference_radius(Gear* gear, double radius) {
  Gear old_gear = *gear;

  gear->reference_radius = radius;

  int errcode = g_check_integrity(gear);
  if (errcode != 0)
    *gear = old_gear;

  return 0;
}

int g_set_axle_radius(Gear* gear, double radius) {
  Gear old_gear = *gear;

  gear->axle_radius = radius;

  int errcode = g_check_integrity(gear);
  if (errcode != 0)
    *gear = old_gear;

  return 0;
}

int g_set_teeth(Gear* gear, int teeth) {
  Gear old_gear = *gear;

  gear->teeth = teeth;

  int errcode = g_check_integrity(gear);
  if (errcode != 0)
    *gear = old_gear;

  return 0;
}

int g_set_modulo(Gear* gear, double modulo) {
  Gear old_gear = *gear;

  double teeth = (gear->reference_radius * 2) / modulo;
  gear->teeth = int(teeth);

  int errcode = g_check_integrity(gear);
  if (errcode != 0)
    *gear = old_gear;

  return 0;
}

int g_set_pressure_angle(Gear* gear, double angle) {
  Gear old_gear = *gear;

  gear->pressure_angle = angle;

  int errcode = g_check_integrity(gear);
  if (errcode != 0)
    *gear = old_gear;

  return 0;
}

bool g_get_external_gear(Gear* gear) {
  return gear->external_gear;
}

double g_get_reference_radius(Gear* gear) {
  return gear->reference_radius;
}

double g_get_axle_radius(Gear* gear) {
  return gear->axle_radius;
}

double g_get_base_radius(Gear* gear) {
  return gear->reference_radius * cos(gear->pressure_angle * G_PI / 180.0);
}

int g_get_teeth(Gear* gear) {
  return gear->teeth;
}

double g_get_modulo(Gear* gear) {
  return (gear->reference_radius * 2) / gear->teeth;
}

double g_get_addendum(Gear* gear) {
  return 1 * g_get_modulo(gear);
}

double g_get_dedendum(Gear* gear) {
  return 1.25 * g_get_modulo(gear);
}

double g_get_tooth_height(Gear* gear) {
  return 2.25 * g_get_modulo(gear);
}

double g_get_tooth_thickness(Gear* gear) {
  return (G_PI * g_get_modulo(gear)) / 2;
}

double g_get_pitch_angle(Gear* gear) {
  return (2 * G_PI * gear->reference_radius) / gear->teeth;
}

double g_get_alpha(Gear* gear) {
  int N = gear->teeth;
  double a = gear->pressure_angle;
  double dp = gear->reference_radius * 2;
  double db = g_get_base_radius(gear) * 2;
  return (sqrt(dp * dp - db * db) / db) * 180.0 / G_PI - a;
}

double g_get_beta(Gear* gear) {
  return (180.0 / gear->teeth - g_get_alpha(gear));
}

string _g_get_ellipse(double cx, double cy, double rx, double ry, string style, string id, string extra_arg) {
  string ellipse = "<ellipse ";

  if (id != "")
    ellipse += "id='" + id + "' ";

  ellipse += "cx='" + to_string(cx) + "' ";
  ellipse += "cy='" + to_string(cy) + "' ";
  ellipse += "rx='" + to_string(rx) + "' ";
  ellipse += "ry='" + to_string(ry) + "' ";

  ellipse += "style='" + style + "' ";

  ellipse += "/>";
  return ellipse;
}

string _g_get_line(double p1x, double p1y, double p2x, double p2y, string extra_arg) {
  string line = "<line ";

  line += "x1='" + to_string(p1x) + "' ";
  line += "y1='" + to_string(p1y) + "' ";
  line += "x2='" + to_string(p2x) + "' ";
  line += "y2='" + to_string(p2y) + "' ";

  line += " " + extra_arg + " ";

  line += "/>";
  return line;
}

void _g_polar_to_cartesian(double r, double alpha, double* x, double* y) {
  *x = r * cos(alpha);
  *y = r * sin(alpha);
}

void _g_cartesian_to_polar(double x, double y, double* r, double* alpha) {
  *r = sqrt(x * x + y * y);
  *alpha = atan2(y, x);
}

void _g_rotate_point(double* x, double* y, double alpha) {
  double _x = *x;
  double _y = *y;
  *x = _x * cos(alpha) - _y * sin(alpha);
  *y = _x * sin(alpha) + _y * cos(alpha);
}

double _g_get_t_intersection(Gear* gear){

  double threshold = 0.1;

  double increment = 0.01;
  double t = 0.0;
  double r = gear->reference_radius;

  double x1, y1, x2, y2, dx, dy;
  x1 = 0;
  y1 = 0;
  x2 = 0;
  y2 = 0;
  dx = 1000000;
  dy = 1000000;
  
  double a1 = G_PI/180.0 * (90.0/gear->teeth + g_get_alpha(gear)/2);

  double a2 = -a1 + G_PI/180.0 * g_get_beta(gear);
  while(true){
    x1 = r * (cos(t) + t * sin(t));
    y1 = r * (sin(t) - t * cos(t));
    x2 = r * (cos(-t) - t * sin(-t));
    y2 = r * (sin(-t) + t * cos(-t));
    
    
    _g_rotate_point(&x1, &y1, -a1);
    _g_rotate_point(&x2, &y2,  a1);
    
    //_g_rotate_point(&x2, &y2, G_PI/180.0 * g_get_beta(gear));

    if(y2 - y1 < threshold && y1 - y2 < threshold){
      break;
    }

    if(y2 - y1 > 0){
      t += increment;
    }
    else{
      t -= increment;
      increment /= 2;
    }

    dy = y2 - y1;
  }
  return t;
}

string g_generate_tooth_involute(Gear* gear, double chunks, bool left_face) {


  string path = "";

  int N = gear->teeth;
  double addendum = g_get_addendum(gear);
  double dedendum = g_get_dedendum(gear);
  double dp = gear->reference_radius * 2;
  double db = g_get_base_radius(gear) * 2;

  double beta = g_get_beta(gear);

  double x = 0;
  double y = 0;
  double r = db / 2;
  double t = 0;
  double t0 = 0.0;
  double t1 = 0.0;

  double rp = dp / 2;
  double rb = db / 2;
  double a = addendum;
  double b = dedendum;

  // This is the solution of intersection between involute curve
  // and top circle (gear->reference_radius + addendum).
  t1 = sqrt((a - r + rp) * (a + r + rp)) / r;
  double t1_ = _g_get_t_intersection(gear);
  if(t1_ < t1)
    t1 = t1_;

  // This is the solution of intersection between involute curve
  // and bottom circle (gear->reference_radius - dedendum).
  // This equation can have no solution, so check for nan value (not a number)
  t0 = sqrt(-(-b + r + rp) * (b + r - rp)) / r;
  if (isnan(t0))
    t0 = 0;
  t0 = 0;


  // Moves the cursor to (radius, 0)
  path += "M " + to_string(db / 2) + " 0";

  // t is an angle, draw the involute curve by changing it.
  // t must go from t0 to t1 (t0 is on base_circle, t1 is on top_circle)
  // last_cycle helper bool to force, on the last loop, setting t to t1
  t = t0;
  bool last_cycle = false;
  double increment = (t1 - t0) / chunks;
  while (true) {
    // If left face use negative values of t
    if (left_face) {
      x = r * (cos(-t) - t * sin(-t));
      y = r * (sin(-t) + t * cos(-t));
    } else {
      x = r * (cos(t) + t * sin(t));
      y = r * (sin(t) - t * cos(t));
    }

    // Line command: draws line from previous point to x,y
    path += " L " + to_string(x) + " " + to_string(y);

    t += increment;
    if (last_cycle)
      break;
    // If t is greater than t1 force one last loop with t = t1
    if (t > t1 && !last_cycle) {
      last_cycle = true;
      t = t1;
    }
  }

  double alpha;
  double radius;
  double x1, x2, y1, y2;
  
  if(gear->external_gear){
      if (left_face) {
        // Computing connection arc between two teeth
        t0 = 0;
        x1 = r * (cos(t0) - t0 * sin(t0));
        y1 = r * (sin(t0) + t0 * cos(t0));

        path += "M " + to_string(x1) + " " + to_string(y1) + " ";

        path += "A " + to_string(g_get_beta(gear)) + " " + to_string(g_get_beta(gear)) + " ";
        path += "0 0 0 ";
        _g_rotate_point(&x1, &y1, G_PI/180 * g_get_beta(gear));
        path += to_string(x1) + " " + to_string(y1);

      } else {
        // Computing connection in top of tooth
        x2 = r * (cos(-t1) - t1 * sin(-t1));
        y2 = r * (sin(-t1) + t1 * cos(-t1));
        x1 = r * (cos(t1) + t1 * sin(t1));
        y1 = r * (sin(t1) - t1 * cos(t1));

        alpha = 2 * G_PI / N - g_get_beta(gear) * G_PI / 180.0;

        _g_rotate_point(&x2, &y2, alpha);

        path += "M " + to_string(x1) + " " + to_string(y1) + " ";
        path += "A " + to_string(rp + a) + " " + to_string(rp + a) + " ";
        path += "0 0 1 ";
        path += to_string(x2) + " " + to_string(y2);
      }
  }
  else{
      if (left_face) {
        // Computing connection arc between two teeth
        t0 = 0;
        x1 = r * (cos(t0) - t0 * sin(t0));
        y1 = r * (sin(t0) + t0 * cos(t0));

        path += "M " + to_string(x1) + " " + to_string(y1) + " ";

        path += "A " + to_string(rp) + " " + to_string(rp) + " ";
        path += "0 0 1 ";
        _g_rotate_point(&x1, &y1, G_PI/180 * g_get_beta(gear));
        path += to_string(x1) + " " + to_string(y1);

      } else {
        // Computing connection in top of tooth
        x2 = r * (cos(-t1) - t1 * sin(-t1));
        y2 = r * (sin(-t1) + t1 * cos(-t1));
        x1 = r * (cos(t1) + t1 * sin(t1));
        y1 = r * (sin(t1) - t1 * cos(t1));

        alpha = 2 * G_PI / N - g_get_beta(gear) * G_PI / 180.0;

        _g_rotate_point(&x2, &y2, alpha);

        path += "M " + to_string(x1) + " " + to_string(y1) + " ";
        path += "A " + to_string(g_get_beta(gear)) + " " + to_string(g_get_beta(gear)) + " ";
        path += "0 0 1 ";
        path += to_string(x2) + " " + to_string(y2);
      }
  }

  return path;
}
