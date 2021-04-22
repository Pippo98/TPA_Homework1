#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "Gear.h"

using namespace std;
int main() {
  Gear* g1 = g_init(true, 100, 70, 32, 15);
  Gear* g2 = g_init_for_connection(g1, true, 50, 10);

  if (g1 == NULL || g2 == NULL) {
    cout << "Gear is null" << endl;
    return 0;
  }

  Connection* conn = g_init_connection(g1, g2, 33);

  Gear* g3 = g_init(true, 80, 40, 30, 10);
  Gear* g4 = g_init_for_connection(g3, false, 200, 220);

  if(g3 == NULL)
    cout << "G3 Invalid " << endl;
  if(g4 == NULL)
    cout << "G4 Invalid " << endl;

  Connection* conn2 = g_init_connection(g3, g4, 50);
  Connection* conn3 = g_init_connection(g3, g4, -70);

  if(conn2 == NULL)
    cout << "Connection2 Invalid" << endl;
  if(conn3 == NULL)
    cout << "Connection3 Invalid" << endl;

  g_set_next_connection(&conn, conn2);
  //g_set_next_connection(&conn, conn3);

  g_export_connection(conn, "output/connection");

  g_connection_delete(&conn);
}
