#ifndef BOOST_ASTRONOMY_COORDINATE_IO_HPP
#define BOOST_ASTRONOMY_COORDINATE_IO_HPP

//! Header to facilitate priting of coordinates of a point with units.

#include <iostream>

#include <boost/units/quantity.hpp>
#include <boost/units/io.hpp>

#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/astronomy/coordinate/spherical_representation.hpp>
#include <boost/astronomy/coordinate/spherical_equatorial_representation.hpp>
#include <boost/astronomy/coordinate/differential.hpp>

namespace boost { namespace astronomy { namespace coordinate {

//!"<<" operator overload to print details of a Cartesian point
template
<
    typename CoordinateType,
    class XQuantity,
    class YQuantity,
    class ZQuantity
>
std::ostream& operator<< (std::ostream &out, cartesian_representation
	<CoordinateType, XQuantity, YQuantity, ZQuantity> const& point)
{
    out << "Cartesian Representation ( " 
    	<< point.get_x() << " , " 
        << point.get_y() << " , " 
        << point.get_z() << " )";

    return out;
}

//!"<<" operator overload to print details of a Spherical Equatorial Point
template
<
    typename CoordinateType,
    class LatQuantity,
    class LonQuantity,
    class DistQuantity
>
std::ostream& operator<< (std::ostream &out, spherical_equatorial_representation
	<CoordinateType, LatQuantity, LonQuantity, DistQuantity> const& point)
{
    out << "Spherical Equatorial Representation ( " 
    	<< point.get_lat() << " , " 
        << point.get_lon() << " , " 
        << point.get_dist() << " )";

    return out;
}

//!"<<" operator overload to print details of a Spherical point
template
<
    typename CoordinateType,
    class LatQuantity,
    class LonQuantity,
    class DistQuantity
>
std::ostream& operator<< (std::ostream &out, spherical_representation
	<CoordinateType, LatQuantity, LonQuantity, DistQuantity> const& point)
{
    out << "Spherical Representation ( " 
    	<< point.get_lat() << " , " 
        << point.get_lon() << " , " 
        << point.get_dist() << " )";

    return out;
}
//!"<<" operator overload to print details of a Cartesian differential point
template
<
    typename CoordinateType,
    class XQuantity,
    class YQuantity,
    class ZQuantity
>
std::ostream& operator<< (std::ostream &out, cartesian_differential
	<CoordinateType, XQuantity, YQuantity, ZQuantity> const& point)
{
    out << "Cartesian differential ( " 
    	<< point.get_dx() << " , " 
        << point.get_dy() << " , " 
        << point.get_dz() << " )";

    return out;
}

//!"<<" operator overload to print details of a Spherical Equatorial differential Point
template
<
    typename CoordinateType,
    class LatQuantity,
    class LonQuantity,
    class DistQuantity
>
std::ostream& operator<< (std::ostream &out, spherical_equatorial_differential
	<CoordinateType, LatQuantity, LonQuantity, DistQuantity> const& point)
{
    out << "Spherical Equatorial differential ( " 
    	<< point.get_dlat() << " , " 
        << point.get_dlon() << " , " 
        << point.get_ddist() << " )";

    return out;
}

//!"<<" operator overload to print details of a Spherical differential point
template
<
    typename CoordinateType,
    class LatQuantity,
    class LonQuantity,
    class DistQuantity
>
std::ostream& operator<< (std::ostream &out, spherical_differential
	<CoordinateType, LatQuantity, LonQuantity, DistQuantity> const& point)
{
    out << "Spherical differential ( " 
    	<< point.get_dlat() << " , " 
        << point.get_dlon() << " , " 
        << point.get_ddist() << " )";

    return out;
}

//!"<<" operator overload to print details of a spherical_coslat_differential
template
<
    typename CoordinateType,
    class LatQuantity,
    class LonQuantity,
    class DistQuantity
>
std::ostream& operator<< (std::ostream &out, spherical_coslat_differential
	<CoordinateType, LatQuantity, LonQuantity, DistQuantity> const& point)
{
    out << "Spherical Coslat Differential ( " 
    	<< point.get_dlat() << " , " 
        << point.get_dlon_coslat() << " , " 
        << point.get_ddist() << " )";

    return out;
}

}}} //namespace boost::astronomy::coordinate
#endif // !BOOST_ASTRONOMY_COORDINATE_IO_HPP