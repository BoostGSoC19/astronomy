# The Motion of a point

Astronomy library provides a way to integrate motions with the coordinate. For every representation class, there is one corresponding differential class for it which will denote the motion of the point. 

Creation of differential is exactly as representation classes.

There are 4 available differential classes:
* cartesian_differential
* spherical_differential
* spherical_equatorial_differential
* spherical_coslat_differential

>**NOTE:** spherical_coslat_differential is the differential longitude with cos(lat) included, latitude and distance. This differential is used in all the astronomical coordinate system.

These classes also provide multiplication and addition operator overload.
* To add a differential to differential: `differential + differential`
* To multiply a differential with time N instance: `differential * N)`

Required header files to include all the differential classes:

```c++
#include <boost/astronomy/coordinate/coordinate.hpp>
```
or
```c++
#include <boost/astronomy/coordinate/differential.hpp>
```

## Example:
```c++
#include <iostream>
#include <boost/astronomy/coordinate/coordinate.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/io.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;

int main()
{
    auto motion1 = make_cartesian_differential(1.0 * meters/seconds, 2.0 * meters/seconds, 0.0 * meters/seconds);
    auto motion2 = make_cartesian_differential(6.0 * meters/seconds, -3.0 * meters/seconds, 1.0 * meters/seconds);

    //adding two motions
    auto sum = make_cartesian_differential(motion1 + motion2); // sum = (7.0, -1.0, 1.0)

    //multiply 5 instance of time with single motion
    auto product1 = make_cartesian_differential(motion1 * (5 * seconds)); // product1 = (5.0, 10.0 0.0);

    //multiply 5 instance of time with multiple motion
    auto product2 = make_cartesian_differential((motion1 + motion2) * (5 * seconds)); // x = (35.0, -5.0, 5.0)

    return 0;
}
```

[Previous](vector_operation.md) | [Next](astronomical_coordinate.md)
