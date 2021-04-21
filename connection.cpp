#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "Gear.h"

using namespace std;
int main(){
  
  Gear* g1 = g_init(true, 100, 70, 30, 20);
  Gear* g2 = g_init_(g1, true, 70, 50);
  
  if(g1 == NULL || g2 == NULL) {
    cout << "Gear is null" << endl;
    return 0;
  }

  Connection* conn = g_init_connection(g1, g2, 30);
  
  Gear* g3 = g_init(true, 100, 50, 30, 20);
  Gear* g4 = g_init_(g1, true, 30, 20);

  Connection* conn2 = g_init_connection(g3, g4, 0);
  conn->next = conn2;

  g_export_connection(conn, "conn");


}
