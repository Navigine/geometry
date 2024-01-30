cmake_minimum_required(VERSION 3.15)

add_library(geometry
    geometry.cpp
    random.cpp
    utils.cpp
    vector2d.cpp
    vector3d.cpp
    vectorXd.cpp
)

add_library(geometry::geometry ALIAS geometry)
