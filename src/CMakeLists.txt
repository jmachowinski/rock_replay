find_package(Threads)

rock_library(rock_replay
    SOURCES ReplayHandler.cpp LogTask.cpp
    HEADERS ReplayHandler.hpp LogTask.hpp
    DEPS_PKGCONFIG 
        orocos_cpp
        pocolog_cpp
        rtt_typelib-${OROCOS_TARGET}
        orocos-rtt-${OROCOS_TARGET}
        orocos-rtt-mqueue-${OROCOS_TARGET}
        orocos-rtt-corba-${OROCOS_TARGET}        
)

target_link_libraries(rock_replay pthread)

rock_executable(rock_replay2 Main.cpp
    DEPS rock_replay
    DEPS_PKGCONFIG 
)

