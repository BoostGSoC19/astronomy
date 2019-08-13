# Astronomy library

## **Introduction**

Development of Astronomy library started as a part Google Summer of Code 2018 under mentor organization **Boost C++ Library** and mentor/guide **Vinícius dos Santos Oliveira**. 

This library provides basic functionalities which are required daily in the field of astronomy.

**Scope of this library:**
 1. Coordinate system
 2. FITS file handling 

 We are planning to expand this in the future to support more features.

## Prerequisites
* Minimum C++11 is required
* Familiarity with the boost.geometry (recommended but not necessary)
* Knowledge of the coordinate system
* Knowledge of File handling with C++
* Knowledge of C++ templates

## Limitations
* Limited functionality for FITS file

## **Index**
1. Coordinate System
    * [1.1 Euclidean Coordinate Systems](coordinate/euclidean_coordinate.md)
        * [1.1.1 Creating a coordinate point (Cartesian, Spherical, Equatorial)](coordinate/coordinate_point.md)
        * [1.1.2 Conversions of coordinate systems](coordinate/coordinate_conversion.md)
        * [1.1.3 Operations on vector](coordinate/vector_operation.md)
    * [1.2 Motion in Euclidean Space](coordinate/motion.md)
    * [1.3 Astronomical Coordinate System](coordinate/astronomical_coordinate.md)
        * [1.3.1 Creating celestial points](coordinate/astronomical_coordinate.md)
        * [1.3.2 Operations on Celestial points](coordinate/astronomical_coordinate_operation.md)
2. FITS File Handling
    * 2.1 Reading FITS
        * 2.1.1 Accessing headers
        * 2.1.1 Accessing data

[Next](coordinate/euclidean_coordinate.md)
