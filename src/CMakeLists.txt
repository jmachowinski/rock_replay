rock_library(rock_replay
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(rock_replay_bin Main.cpp
    DEPS rock_replay)

