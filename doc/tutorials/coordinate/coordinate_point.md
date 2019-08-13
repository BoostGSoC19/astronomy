# Creation of a point in Euclidean space

Representation classes are structured such that they can be used as a vector or point at the same time as per the requirement.

All representations are accommodated in namespace `boost::astronomy::coordinate`.

## Methods to access components
All the representation classes have similar accessors. However, the name of their corresponding component changes. The corresponding component of each representation can be found from the table below.

| *Representation*                      | *Component 1* | *Component 2* | *Component 3*
|---------------------------------------|---------------|---------------|---
| `cartesian_representation`            | X             | Y             | Z
| `spherical_representation`            | lat           | lon           | dist
| `spherical_equatorial_representation` | lat           | lon           | dist

To access any of the component in any representation methods as below can be used:<br>
`get_<component>()`<br>
`set_<component>()`<br>
`get_<component1>_<component2>_<component3>()`<br>
`set_<component1>_<component2>_<component3>()`<br>
Here `<component>` is supposed to be replaced by the corrosponding component of the representation.<br>
>E.g. :- cartesian_representation: `get_x()`, `set_x_y_z()`<br>
spherical_representation: `get_lat()`, `get_lat_lon_dist()`

## Cartesian Point
This representation uses three components(x, y, z) to represent any point in the 3D space. This is most widely used for regular geometry in maths.

Required header:
```c++
#include <boost/astronomy/coordinate/cartesian_representation.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/representation.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/coordinate.hpp>
```
## Example of how to create a cartesian point and access its component:
```c++
#include <iostream>
#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/io.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;

typedef boost::geometry::model::point<double, 3, boost::geometry::cs::spherical<boost::geometry::degree>> geometry_point;

int main()
{
    //creating point without any value
    cartesian_representation<double, quantity<si::length>, quantity<si::length>, quantity<si::length>> point1;

    //creating a point by directly providing values of x, y and z
    auto point2 = make_cartesian_representation(10.0*meter, 20.0*meter, 30.0*meter);

    //assigning value of individual component
    point1.set_x(50.0*meter);
    point1.set_y(40.0*meter);
    point1.set_z(880.0*meter);

    //or to set all the values in single statement tuple could be used as follow
    point2.set_x_y_z(38.5*meter, 50.0*meter, 64.23*meter);

    //creating a point from another point
    auto point3 = make_cartesian_representation(point2);

    //creating a point from boost::geometry::model::point
    //any type of point can be used here 
    //here we will demonstrate with cartesian point
    //https://www.boost.org/doc/libs/1_71_0/libs/geometry/doc/html/geometry/reference/models/model_point.html
    //https://www.boost.org/doc/libs/1_71_0/libs/geometry/doc/html/geometry/reference/cs.html
    geometry_point gp(45.0, 60.0, 50.0);
    auto point4 = make_cartesian_representation
        <double,quantity<si::length>,quantity<si::length>,quantity<si::length>>(gp);

    //accessing each component of representation
    std::cout << point4.get_x() << std::endl; //methods get_y and get_z are available

    //get boost::geometry::model::point of current object
    auto stored_point = point3.get_point();

    //get the tuple of the component in the coordinate
    std::tuple<quantity<si::length>, quantity<si::length>, quantity<si::length>>
            components = point3.get_x_y_z();

    std::cin.get();
    return 0;
}
```

## Spherical Point
This representation uses (latitude, longitude, distance) components to represent any point in the 3D space. This is the most widely used in astronomy and positional geometry.

Required header:
```c++
#include <boost/astronomy/coordinate/spherical_representation.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/representation.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/coordinate.hpp>
```
## Example of how to create a spherical point and access its component:
```c++
#include <iostream>
#include <boost/astronomy/coordinate/spherical_representation.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/io.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;

typedef boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> geometry_point;

int main()
{
    //To create spherical point we need to specify is angles are in radian or degree
    //creating point without any value
    spherical_representation<double, quantity<si::plane_angle>, quantity<si::plane_angle>, quantity<si::length>> point1;

    //creating a point by directly providing values of latitude, longitude and distance
    auto point2 = make_spherical_representation(10.0*si::radians, 20.0*si::radians, 30.0*meter);

    //assigning value of individual component
    point1.set_lat(50.0*si::radians);
    point1.set_lon(40.0*si::radians);
    point1.set_dist(880.0*meter);

    //or to set all the values in single statement tuple could be used as follow
    point2.set_lat_lon_dist(38.5*si::radians, 50.0*si::radians, 64.23*meter);

    //creating a point from another point
    auto point3 = make_spherical_representation(point2);

    //creating a point from boost::geometry::model::point
    //any type of point can be used here 
    //here we will demonstrate with cartesian point
    //https://www.boost.org/doc/libs/1_71_0/libs/geometry/doc/html/geometry/reference/models/model_point.html
    //https://www.boost.org/doc/libs/1_71_0/libs/geometry/doc/html/geometry/reference/cs.html
    geometry_point gp(45.0, 60.0, 50.0);
    auto point4 = make_spherical_representation
        <double,quantity<si::plane_angle>,quantity<si::plane_angle>,quantity<si::length>>(gp);

    //accessing each component of representation
    std::cout << point4.get_lon() << std::endl; //methods get_lat and get_dist are available

    //get boost::geometry::model::point of current object
    auto stored_point = point3.get_point();

    //get the tuple of the component in the coordinate
    std::tuple<quantity<si::plane_angle>, quantity<si::plane_angle>, quantity<si::length>>
        components = point3.get_lat_lon_dist();

    std::cin.get();
    return 0;
}
```

## Spherical equatorial Point
This representation uses three components(latitude, longitude, distance) to represent any point in the 3D space. This one resembles the geographic coordinate system, and has latitude up from zero at the equator, to 90 at the pole (opposite to the spherical(polar) coordinate system).

Required header:
```c++
#include <boost/astronomy/coordinate/spherical_equatorial_representation.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/representation.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/coordinate.hpp>
```

All the APIs are the same as `spherical_representation`.

[Previous](euclidean_coordinate.md) | [Next](coordinate_conversion.md)
