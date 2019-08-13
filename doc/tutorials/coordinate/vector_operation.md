# Operations on vector

Representation classes have provided a few of the most used vector operations which are as follows:
* Cross product of two vectors
* Dot product of two vectors
* Unit Vector
* Magnitude of a vector
* Addition of two vectors
* Mean of two vectors

>**NOTE:** All these functions first convert vectors into cartesian_representation, performs the operation on it and then returns the result in requested representation

```c++
#include <iostream>
#include <boost/astronomy/coordinate/representation.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/io.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;
namespace bud = boost::units::degree;

int main()
{
    //Cross Product 
    auto point1 = make_cartesian_representation(15.0 * meters, 25.0 * meters, 30.0 * meters);
    auto point2 = make_spherical_representation(45.0 * bud::degrees, 45.0 * bud::degrees, 3.0 * meters);

    cartesian_representation<double, quantity<si::length>, quantity<si::length>, quantity<si::length>>
        cross_result = cross(point1, point2);

    //dot product
    std::cout << dot(point1, point2) << std::endl;

    //unit vector
    auto unit_vector_point = unit_vector(point1);

    //magnitude
    std::cout << magnitude(point1) << std::endl;

    //sum of two vectors
    //Here we show the flexibility provided by the library to use any representation for the operations and implicit casting
    //right-hand side of the equal returns object of cartesian_representation
    //But it gets converted implicitly into spherical_representation
    spherical_representation<double, quantity<si::plane_angle>, quantity<si::plane_angle>,
        quantity<si::length>> sum_result = sum(point1, point2);

    //mean of two vectors
    spherical_representation<double, quantity<degree::plane_angle>, quantity<degree::plane_angle>,
        quantity<si::length>> mean_result = mean(point1, point2);

    return 0;
}
```

[Previous](coordinate_conversion.md) | [Next](motion.md)
