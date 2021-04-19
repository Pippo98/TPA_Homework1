#include <algorithm>
#include <iostream>

#include "Gear.h"

using namespace std;

char *get_cmd_option(char **argv, int argc, const std::string &option) {
  for (int i = 0; i < argc; i++) {
    size_t pos = string(argv[i]).find(option);
    if (pos != string::npos && i + 1 < argc) {
      if (string(argv[i + 1]).find("-") != string::npos) {
        char *end;
        strtod(argv[i + 1], &end);
        if (argv[i + 1] != end)
          return argv[i + 1];
      } else {
        return argv[i + 1];
      }
    }
  }
  return 0;
}

bool cmd_option_exists(char **argv, int argc, const std::string &option) {
  for (int i = 0; i < argc; i++) {
    size_t pos = string(argv[i]).find(option);
    if (pos != string::npos) {
      return true;
    }
  }
  return false;
}

void displayHelp(bool exception = false) {
  cout << "\n"
       << endl;
  if (exception) {
    cout << "Some parameters were wrong.\n"
         << endl;
  }
  cout << "Help: \n";
  cout << "\nGear parameters:\n";
  cout << "\t <external gear> <reference radius> <axle radius>  <teeth number> <pressure angle>\n";
  cout << "\t (0 or 1)        (double value)     (double value) (int value)    (double value °)\n";

  cout << "\nFile parameters:\n";
  cout << "\t-o <path>\tpath for svg output file\n";
  cout << "\t-l <path>\tpath for svg load file\n";

  cout << "\n\n";
  cout << "When using -l option the parameters passed as arguments are not used, will be used\n";
  cout << "the parameters contained in the file loaded!\n";

  cout << endl;
}

int main(int argc, char *argv[]) {
  if (cmd_option_exists(argv, argc, "-h")) {
    displayHelp();
    return 0;
  }

  // needed parameters to be parsed
  int parameters = 5 + 1;
  string infile = "gear";
  string outfile = "gear";

  // Default values for gear
  bool   external_gear = true;
  double r1 = 0.0;
  double r2 = 0.0;
  int    N  = 0;
  double pa = 20;

  if (cmd_option_exists(argv, argc, "-o")) {
    char *save = get_cmd_option(argv, argc, "-o");
    if (save) {
      outfile = string(save);
      parameters += 2;
    } else {
      displayHelp(true);
      return -1;
    }
  }

  if (cmd_option_exists(argv, argc, "-l")) {
    char *load = get_cmd_option(argv, argc, "-l");
    if (load) {
      infile = string(load);
    } else {
      displayHelp(true);
      return -1;
    }
  } else {
    if (argc > 1 && argc == parameters) {
      // Parsing gear parameters (5)
      // Excluding command parameters (-o)
      bool lone_param = false;
      int idx1 = 0;
      int lone_param_count = 0;
      for (int i = 1; i < argc; i++){
        // Checking if the current argv is a parameter
        char *end;
        strtod(argv[i], &end);
        if (argv[i] == end){ // Conversion to double failed so is a parameter
          if(lone_param == true)
            break;           // If this param is interrupting gear param list break!
          else
            continue;        // So jump to next iteration
        }

        // Save the first param index
        if(lone_param == false){
          idx1 = i;
          lone_param = true;
        }

        lone_param_count ++;
        // Finished
        if(lone_param_count + 1 == 5)
          break;
      }
      if(lone_param_count + 1 != 5){
        cout << "\n -> Gear parameters were wrong, IGNORING them!!" << endl;
        cout << " -> Use -h to display help.\n" << endl;
      }
      external_gear = stoi(argv[idx1]) == 1 ? true : false;
      r1 = stod(argv[idx1 + 1]);
      r2 = stod(argv[idx1 + 2]);
      N  = stoi(argv[idx1 + 3]);
      pa = stod(argv[idx1 + 4]);
    }
    else{
      cout << "\n -> Gear parameters were wrong, not considering them!!" << endl;
      cout << " -> Use -h to display help.\n" << endl;
    }
  }

  Gear *gear = g_init(external_gear, r1, r2, N, pa);

  if(gear == NULL){
    cout << "Init failed" << endl;
    return -1;
  }

  cout << g_to_string(gear) << endl;
  g_export_svg(gear, outfile);

  return 0;
  


  gear = g_init(true, 280, 2, 52, 15);
  g_set_modulo(gear, 8);
  g_export_svg(gear, outfile);
  gear = g_from_svg(infile);
  cout << g_to_string(gear) << endl;

  gear = g_init(false, 180, 200, 52, 11);
  if (gear == NULL)
    return -1;
  g_export_svg(gear, outfile);
  gear = g_from_svg(infile);

  if (gear == NULL)
    return -1;
  cout << g_to_string(gear) << endl;
  return 0;
}
