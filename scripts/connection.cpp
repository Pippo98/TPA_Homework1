#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "Gear.h"

using namespace std;
int main() {
  Gear* g1 = g_init(true, 100, 70, 30, 20);
  Gear* g2 = g_init_for_connection(g1, true, 70, 50);

  if (g1 == NULL || g2 == NULL) {
    cout << "Gear is null" << endl;
    return 0;
  }

  Connection* conn = g_init_connection(g1, g2, 30);

  Gear* g3 = g_init(true, 100, 50, 30, 20);
  Gear* g4 = g_init_for_connection(g1, true, 30, 20);

  Connection* conn2 = g_init_connection(g3, g4, 0);
  Connection* conn3 = g_init_connection(g3, g4, 0);
  g_set_next_connection(&conn, conn2);
  g_set_next_connection(&conn, conn3);

  g_export_connection(conn, "output/connection");

  g_connection_delete(&conn);
}
