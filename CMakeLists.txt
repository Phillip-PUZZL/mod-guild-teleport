cmake_minimum_required(VERSION 3.5)
project(guild_teleport_module)

include(ACCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_ac_module(guild_teleport_module SHARED
    src/guild_teleport_npc.cpp
)
