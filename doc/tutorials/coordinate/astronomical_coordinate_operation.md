# Astronomical coordinate Operations

There are only a few operations available at this time on coordinate which are demonstrated below.

```c++
#include <iostream>
#include <boost/astronomy/coordinate/coordinate.hpp>
#include <boost/astronomy/coordinate/sky_point.hpp>
#include <boost/astronomy/coordinate/frame.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/io.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;
namespace bud = boost::units::degree;

typedef spherical_representation<double, quantity<degree::plane_angle>, quantity<degree::plane_angle>,
        quantity<si::length>> spherical_rep;
typedef spherical_coslat_differential<double, quantity<degree::plane_angle>,
        quantity<degree::plane_angle>, quantity<si::velocity>> spherical_coslat_diff;

int main()
{
    //first we create a point we want to represent in the sky
    auto star1 = make_spherical_representation(45.0 * bud::degrees, 45.0 * bud::degrees, 3.0 * meters);
    auto star2 = make_spherical_representation(15.0 * bud::degrees, 5.0 * bud::degrees, 5.0 * meters);

    //we define motion it has (if it has)
    auto motion1 = make_spherical_coslat_differential(10.0 * bud::degrees, 15.0 * bud::degrees, 1.0 * meters/seconds);
    auto motion2 = make_spherical_coslat_differential(70.0 * bud::degrees, 1.5 * bud::degrees, 10.0 * meters/seconds);

    //creating  sky point object
    sky_point<icrs<spherical_rep, spherical_coslat_diff>> object1(star1, motion1);

    sky_point<galactic<spherical_rep, spherical_coslat_diff>> object2(star1, motion1);
    //Note that here object1 and object2 are not the same points in the sky as they are represented in different frames

    //Different frames have different origins so even after having the same values they refer to the different point in the sky

    //creating sky point object of icrs frame
    sky_point<icrs<spherical_rep, spherical_coslat_diff>> object3(star2, motion2);

    //to retrive frame from object
    icrs<spherical_rep, spherical_coslat_diff> frame_object = object1.get_point();

    if(object1.is_equivalent_system(object2))
    {
        std::cout << "They are in same frame" << std::endl;
    }//will print nothing

    //to find separation between two points
    //both objects must be in same frame
    std::cout << object1.separation(object3) << std::endl;

    //to find positional angle between two point
    //https://en.wikipedia.org/wiki/Position_angle
    //both objects must be in same frame
    std::cout << object1.positional_angle(object3) << std::endl;
    return 0;
}
```

[Previous](astronomical_coordinate.md) | [Next]()
