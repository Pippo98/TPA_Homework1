
#include "catch2/catch2.hpp"
#include "EB_Device.h"
#include "Gear.h"

using namespace std;

TEST_CASE("init should create a new struct 1","[GEAR]"){
    Gear* gear = g_init(true, 150, 50, 50, 20);

    REQUIRE(gear != NULL);
}
TEST_CASE("init should create a new struct 2","[GEAR]"){
    Gear* gear = g_init(false, 100, 150, 50, 20);

    REQUIRE(gear != NULL);
}
TEST_CASE("init should create a new struct 3","[GEAR]"){
    Gear* gear1 = g_init(false, 100, 150, 50, 20);
    Gear* gear2 = g_init_for_connection(gear1, true, 100, 10);

    REQUIRE(gear2 != NULL);
}
TEST_CASE("init should create a new connection","[GEAR]"){
    Gear* gear1 = g_init(false, 100, 150, 50, 20);
    Gear* gear2 = g_init_for_connection(gear1, true, 100, 10);
    Connection* conn = g_init_connection(gear1, gear2, 0);

    REQUIRE(conn != NULL);
}

TEST_CASE("Are Same","[GEAR]"){
    Gear* gear1 = g_init(false, 100, 150, 50, 20);
    Gear* gear2 = g_init(false, 100, 150, 50, 20);
    bool are_same = g_are_same(gear1, gear2);
    REQUIRE(are_same == true);
}

TEST_CASE("To From Svg 1","[GEAR]"){
    string fname = "output/g1";
    Gear* gear1 = g_init(false, 100, 150, 50, 20);
    g_export_svg(g_to_svg(gear1), fname);
    Gear* gear2 = g_from_svg(fname);
    bool are_same = g_are_same(gear1, gear2);

    REQUIRE(are_same == true);
}

TEST_CASE("To From Svg 2","[GEAR]"){
    string fname = "output/g1";
    Gear* gear1 = g_init(false, 100, 150, 50, 20);
    g_export_svg(g_to_svg(gear1), fname);
    Gear* gear2 = g_from_svg(fname);

    g_set_axle_radius(gear2, 170);

    bool are_same = g_are_same(gear1, gear2);
    
    REQUIRE(are_same == false);
}