add_executable(GearTest catch2Main.cpp) # Just this one contains BOOST_TEST_MAIN
target_sources(GearTest PRIVATE
        GearTests.cpp
	)

# Libraries used in the tests
target_link_libraries(GearTest)

# Including headers
target_include_directories(GearTest PRIVATE
        ../external_libs
        ../include
        )

# Including sources
target_sources(GearTest PRIVATE
        ../external_libs/EB_Device.cpp
        ../src/Gear.cpp
        )
