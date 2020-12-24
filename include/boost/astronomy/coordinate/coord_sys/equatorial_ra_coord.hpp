/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_EQUATORIAL_RA_COORD_HPP
#define BOOST_ASTRONOMY_EQUATORIAL_RA_COORD_HPP

#include <iostream>
#include <boost/static_assert.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/dimensionless.hpp>
#include <boost/units/physical_dimensions/plane_angle.hpp>
#include <boost/astronomy/coordinate/coord_sys/coord_sys.hpp>

/**
 * The Equatorial Coordinates, are referred to the plane of the Earth’s equator
 *
 * Declination
 * Declination is analogous to latitude and indicates how far away an object is
 * from the celestial equator. Declination is in the range ±90◦ with positive
 * angles indicating locations north of the celestial equator and negative angles
 * indicating locations south of the celestial equator. Because declination is
 * measured with respect to the celestial equator, and the celestial equator’s
 * location does not vary with time of day or an observer’s location, declination
 * for an object is fixed and does not vary with the time of day or an
 * observer’s location.
 *
 * Right Ascension
 * Right ascension is analogous to longitude and indicates how far an object is away
 * from the First Point of Aries, which is the point used to define a celestial prime meridian.
 * It measures the distance from an object to the celestial prime meridian. As with declination,
 * right ascension does not vary with time of day or an observer’s location because it is measured
 * with respect to a fixed location (the First Point of Aries).
 *
**/

namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

template
<
    typename CoordinateType = double,
    typename RightAscensionQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename Declination = bu::quantity<bu::si::plane_angle, CoordinateType>
>
struct equatorial_ra_coord : public coord_sys
       <2, bg::cs::spherical<bg::radian>, CoordinateType>
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG(
        ((std::is_same<typename bu::get_dimension<RightAscensionQuantity>::type,
          bu::plane_angle_dimension>::value) &&
         (std::is_same<typename bu::get_dimension<Declination>::type,
          bu::plane_angle_dimension>::value)),
        "Right Ascension and Declination must be of plane angle type");
    BOOST_STATIC_ASSERT_MSG((std::is_floating_point<CoordinateType>::value),
                            "CoordinateType must be a floating-point type");
    ///@endcond
public:
    typedef RightAscensionQuantity quantity1;
    typedef Declination quantity2;

    //Default constructor
    equatorial_ra_coord() {}

    equatorial_ra_coord
    (
        RightAscensionQuantity const &Ra,
        Declination const &Dec
    )
    {
            this->set_ra_dec(Ra, Dec);
    }

    //Create tuple of Right Ascension and Declination
    std::tuple<RightAscensionQuantity, Declination> get_ra_dec() const
    {
            return std::make_tuple(this->get_ra(), this->get_dec());
    }

    //Get Right Ascension
    RightAscensionQuantity get_ra() const
    {
            return static_cast<RightAscensionQuantity>
                   (
                       bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                       (bg::get<0>(this->point))
                   );
    }

    //Get Declination
    Declination get_dec() const
    {
            return static_cast<Declination>
                   (
                       bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                       (bg::get<1>(this->point))
                   );
    }

    //Set value of Right Ascension and Declination
    void set_ra_dec
    (
        RightAscensionQuantity const &Ra,
        Declination const &Dec
    )
    {
            this->set_ra(Ra);
            this->set_dec(Dec);
    }

    //Set Right Ascension
    void set_ra(RightAscensionQuantity const &Ra)
    {
            bg::set<0>
            (
                this->point,
                static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(Ra).value()
            );
    }

    //Set Declination
    void set_dec(Declination const &Dec)
    {
            bg::set<1>
            (
                this->point,
                static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(Dec).value()
            );
    }

}; //equatorial_ra_coord

//Make Equatorial Coordinate
template
<
    typename CoordinateType,
    template<typename Unit2, typename CoordinateType_> class RightAscensionQuantity,
    template<typename Unit1, typename CoordinateType_> class Declination,
    typename Unit1,
    typename Unit2
>
equatorial_ra_coord
<
  CoordinateType,
  Declination<Unit1, CoordinateType>,
  RightAscensionQuantity<Unit2, CoordinateType>
> make_equatorial_ra_coord
(
    RightAscensionQuantity<Unit2, CoordinateType> const &Ra,
    Declination<Unit1, CoordinateType> const &Dec
)
{
    return equatorial_ra_coord
           <
           CoordinateType,
           RightAscensionQuantity<Unit2, CoordinateType>,
           Declination<Unit1, CoordinateType>
           > (Ra, Dec);
}

//Print Equatorial Right Ascension Coordinates
template
<
    typename CoordinateType,
    class RightAscensionQuantity,
    class Declination
>
std::ostream &operator << (std::ostream &out, equatorial_ra_coord
                           <CoordinateType, RightAscensionQuantity, Declination> const &point) {
  out << "Equatorial Coordinate (Right Ascension: "
      << point.get_ra() << ", Declination: "
      << point.get_dec() << ")";

  return out;
}

}}}

#endif  // BOOST_ASTRONOMY_EQUATORIAL_RA_COORD_HPP
