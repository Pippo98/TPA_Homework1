#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "Gear.h"

using namespace std;
int main() {
  srand(time(NULL));

  string svg = "";

  bool external = false;
  double r1 = 0.0;
  double r2 = 0.0;
  int N = 0;
  double alpha = 0;

  int success = 0;
  int failed = 0;
  int target = 10;
  while (success < target) {
    external = int(rand() % 2);
    r1 = rand() % 200;
    r2 = rand() % 200;
    N = int(rand() % 50);
    alpha = rand() % 35;

    cout << external << " " << r1 << " ";
    cout << r2 << " " << N << " " << alpha << "\t";
    cout << flush;

    Gear* gear = g_init(external, r1, r2, N, alpha);

    if (gear == NULL) {
      cout << "FAIL" << endl;
      failed++;
      continue;
    } else {
      cout << "OK\t";
      success++;
    }
    svg = g_to_svg(gear, false);
    g_export_svg(svg, "output/gear");
    cout << "saved gear.svg ";
    cout << "next iteration in one second ..." << endl;
    usleep(1 * 1000000);
  }

  cout << "Success: " << success << endl;
  cout << "Failed:  " << failed << endl;

  cout << "Percentage: " << (float(success) / (failed + success) * 100) << endl;

  cout << "\n\n-------------\n\n"
       << endl;
  cout << "Saving then Loading:" << endl;

  Gear* g1 = g_init(true, 100.0 / 3, 22, 50, 20);
  cout << "Saving this gear: " << endl;
  cout << g_to_string(g1) << endl;
  g_export_svg(g_to_svg(g1), "output/g1");
  cout << "Saved!" << endl;

  cout << "Loading Gear" << endl;
  Gear* g2 = g_from_svg("output/g1");
  if (g2 == NULL) {
    cout << "FAILED LOADING" << endl;
  } else {
    cout << "Loaded this gear: " << endl;
    cout << g_to_string(g1) << endl;
    if (g_are_same(g1, g2))
      cout << "Are the same" << endl;
    else
      cout << "Some parameters are different" << endl;
  }
}
