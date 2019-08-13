# Astronomical Coordinate Systems

The way we use the geographical coordinate system consisting of longitude and latitude to pinpoint any location on earth, there are many different systems available to pinpoint any location in the cosmos.

Any celestial body in the cosmos would have some motion and its position would change accordingly. So to represent a celestial body in cosmos this library has provided a single class called `sky_point` which supports multiple astronomical coordinate systems and its differentials.

Supported coordinate systems:
* Alt-Az
* ICRS
* CIRS
* Galactic
* SuperGalactic
* HelioCentric
* GeoCentric

>**NOTE:** All the astronomical coordinate systems are referred to as frames in this library.

## Example:

```c++
#include <iostream>
#include <boost/astronomy/coordinate/coordinate.hpp>
#include <boost/astronomy/coordinate/sky_point.hpp>
#include <boost/astronomy/coordinate/frame.hpp>
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
    auto star = make_spherical_representation(45.0 * bud::degrees, 45.0 * bud::degrees, 3.0 * meters);

    //we define motion it has (if it has)
    auto motion = make_spherical_coslat_differential(10.0 * bud::degrees, 15.0 * bud::degrees, 1.0 * meters/seconds);

    //we create a frame
    icrs<spherical_rep, spherical_coslat_diff> frame(star, motion);

    //creating a sky point object
    sky_point<icrs<spherical_rep, spherical_coslat_diff>> object(frame);
    
    return 0;
}
```

[Previous](motion.md) | [Next](astronomical_coordinate_operation.md)
