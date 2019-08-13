# Coordinate Conversions

The following image can explain the relation between cartesian and spherical representation.
![coordinate relation](coordinate_relation.svg)

Converting one coordinate to another coordinate is really easy. The system is developed in such a way that all the conversions are done implicitly.

## Example:
```c++
#include <iostream>
#include <boost/astronomy/coordinate/spherical_representation.hpp>
#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;

int main()
{
    auto point1 = make_cartesian_representation(10.0*meter, 20.0*meter, 30.0*meter);

    auto point2 = make_spherical_representation(point1);

    spherical_representation<double, quantity<si::plane_angle>, quantity<si::plane_angle>,
        quantity<si::length>> point3(point1);

    auto point4 = make_cartesian_representation(point3);
    std::cin.get();
    return 0;
}
```
[Previous](coordinate_point.md) | [Next](vector_operation.md)
