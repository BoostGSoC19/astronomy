/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_EQUATORIAL_HA_COORD_HPP
#define BOOST_ASTRONOMY_EQUATORIAL_HA_COORD_HPP

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
 * Hour Angle
 * If we use an observer’s meridian instead of the celestial prime meridian as a reference point,
 * we have another way to measure “celestial longitude" called “hour angle” (H). While right ascension
 * is an angular measurement (although expressed in HMS format) of an object’s distance from the First
 * Point of Aries, hour angle is very much a time measurement. The hour angle for an object is a measure
 * of how long it has been since the object crossed an observer’s meridian.
 * Because of the way that an hour angle is defined (i.e., relative to an observer’s local celestial meridian),
 * it varies both with time of day and an observer’s location.
 *
**/

namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

template
<
    typename CoordinateType = double,
    typename HourAngleQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename Declination = bu::quantity<bu::si::plane_angle, CoordinateType>
>
struct equatorial_ha_coord : public coord_sys
       <2, bg::cs::spherical<bg::radian>, CoordinateType>
{
  ///@cond INTERNAL
  BOOST_STATIC_ASSERT_MSG(
      ((std::is_same<typename bu::get_dimension<HourAngleQuantity>::type,
          bu::plane_angle_dimension>::value) &&
       (std::is_same<typename bu::get_dimension<Declination>::type,
           bu::plane_angle_dimension>::value)),
      "Hour Angle and Declination must be of plane angle type");
  BOOST_STATIC_ASSERT_MSG((std::is_floating_point<CoordinateType>::value),
                          "CoordinateType must be a floating-point type");
  ///@endcond
public:
    typedef HourAngleQuantity quantity1;
    typedef Declination quantity2;

    //Default constructor
    equatorial_ha_coord() {}

    equatorial_ha_coord
        (
            HourAngleQuantity const &Ha,
            Declination const &Dec
        )
    {
      this->set_ha_dec(Ha, Dec);
    }

    //Create tuple of Hour Angle and Declination
    std::tuple<HourAngleQuantity, Declination> get_ha_dec() const
    {
      return std::make_tuple(this->get_ha(), this->get_dec());
    }

    //Get Hour Angle
    HourAngleQuantity get_ha() const
    {
      return static_cast<HourAngleQuantity>
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

    //Set value of Hour Angle and Declination
    void set_ha_dec
        (
            HourAngleQuantity const &Ha,
            Declination const &Dec
        )
    {
      this->set_ha(Ha);
      this->set_dec(Dec);
    }

    //Set Hour Angle
    void set_ha(HourAngleQuantity const &Ha)
    {
      bg::set<0>
          (
              this->point,
              static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(Ha).value()
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

}; //equatorial_ha_coord

//Make Equatorial Coordinate
template
<
    typename CoordinateType,
    template<typename Unit2, typename CoordinateType_> class HourAngleQuantity,
    template<typename Unit1, typename CoordinateType_> class Declination,
    typename Unit1,
    typename Unit2
>
equatorial_ha_coord
<
    CoordinateType,
    Declination<Unit1, CoordinateType>,
    HourAngleQuantity<Unit2, CoordinateType>
> make_equatorial_ha_coord
(
    HourAngleQuantity<Unit2, CoordinateType> const &Ha,
    Declination<Unit1, CoordinateType> const &Dec
)
{
  return equatorial_ha_coord
      <
          CoordinateType,
          HourAngleQuantity<Unit2, CoordinateType>,
          Declination<Unit1, CoordinateType>
      > (Ha, Dec);
}

//Print Equatorial Hour Angle Coordinates
template
<
    typename CoordinateType,
    class HourAngleQuantity,
    class Declination
>
std::ostream &operator << (std::ostream &out, equatorial_ha_coord
                          <CoordinateType, HourAngleQuantity, Declination> const &point) {
  out << "Equatorial Coordinate (Hour Angle: "
      << point.get_ha() << ", Declination: "
      << point.get_dec() << ")";

  return out;
}

}}}

#endif  // BOOST_ASTRONOMY_EQUATORIAL_HA_COORD_HPP
