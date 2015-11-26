rock_library(rock_replay
    SOURCES LogTask.cpp
    HEADERS LogTask.hpp
    DEPS_PKGCONFIG 
        orocos_cpp
        pocolog_cpp
        rtt_typelib-${OROCOS_TARGET}
        orocos-rtt-${OROCOS_TARGET}
        orocos-rtt-mqueue-${OROCOS_TARGET}
        orocos-rtt-corba-${OROCOS_TARGET}
)

rock_executable(rock_replay2 Main.cpp
    DEPS rock_replay
    DEPS_PKGCONFIG 
)

