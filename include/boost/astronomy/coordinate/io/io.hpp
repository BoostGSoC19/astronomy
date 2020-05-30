/*=============================================================================
Copyright 2020 Nitin Konidala  <konidalanitin@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_COORDINATE_IO_HPP
#define BOOST_ASTRONOMY_COORDINATE_IO_HPP

//! Header to facilitate priting of coordinates of a point with units.

#include <iostream>

#include <boost/units/quantity.hpp>
#include <boost/units/io.hpp>

#include <boost/astronomy/coordinate/rep/cartesian_representation.hpp>
#include <boost/astronomy/coordinate/rep/spherical_representation.hpp>
#include <boost/astronomy/coordinate/rep/spherical_equatorial_representation.hpp>

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

}}} //namespace boost::astronomy::coordinate
#endif // !BOOST_ASTRONOMY_COORDINATE_IO_HPP
