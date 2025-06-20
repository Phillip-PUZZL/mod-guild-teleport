cmake_minimum_required(VERSION 3.5)
project(mod-guild-teleport)

include(ACCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_ac_module(mod-guild-teleport SHARED
    src/guild_teleport.cpp
)
