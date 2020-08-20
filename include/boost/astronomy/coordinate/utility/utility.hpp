/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_UTILITY_HPP
#define BOOST_ASTRONOMY_UTILITY_HPP

#include <iostream>
#include <utility>
#include <cmath>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

//Angle
#include <boost/units/quantity.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/dimensionless.hpp>
#include <boost/units/physical_dimensions/plane_angle.hpp>

//Time
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/astronomy/coordinate/coord_sys/coord_sys.hpp>

#define PI        3.14159265358979323846264338327950288

using namespace std;
namespace bu = boost::units;
namespace bud = boost::units::degree;
using namespace boost::numeric::ublas;
namespace bnu = boost::numeric::ublas;

using namespace boost::gregorian;

namespace boost { namespace astronomy { namespace coordinate {

// Phi φ is the geographical latitude
// ST is the Local Sidereal Time
// ε is the obliquity of the ecliptic.

template
    <
        typename CoordinateType = double,
        typename Angle = bu::quantity<bu::si::plane_angle, CoordinateType>,
        typename ElementType = double
    >
struct col_vec
{
 public:
  matrix<ElementType> vec = matrix<ElementType>(3, 1);

  col_vec() {}

  col_vec(Angle u, Angle v){
    double _u = static_cast<bu::quantity<bu::si::plane_angle>>(u).value();
    double _v = static_cast<bu::quantity<bu::si::plane_angle>>(v).value();

    vec(0,0) = std::cos(_u) * std::cos(_v);
    vec(1,0) = std::sin(_u) * std::cos(_v);
    vec(2,0) = std::sin(_v);
  }

  matrix<ElementType> get(){
    return vec;
  }

  std::string to_string(){
    return "Column Vector";
  }
};

template
    <typename ElementType = double>
struct ha_dec_hor
{
 public:
  matrix<ElementType> conv = matrix<ElementType>(3, 3);

  ha_dec_hor() {}

  ha_dec_hor(ElementType phi){
    conv(0,0) = -std::sin(phi);
    conv(0,1) = 0;
    conv(0,2) = std::cos(phi);
    conv(1,0) = 0;
    conv(1,1) = -1;
    conv(1,2) = 0;
    conv(2,0) = std::cos(phi);
    conv(2,1) = 0;
    conv(2,2) = std::sin(phi);
  }

  matrix<ElementType> get(){
    return conv;
  }

  std::string to_string(){
    return "Equatorial Coordinate Hour Angle to and from Horizon";
  }
};

template
    <typename ElementType = double>
struct ha_dec_ra_dec
{
 public:
  matrix<ElementType> conv = matrix<ElementType>(3, 3);

  ha_dec_ra_dec() {}

  ha_dec_ra_dec(ElementType ST){
    conv(0,0) = std::cos(ST);
    conv(0,1) = std::sin(ST);
    conv(0,2) = 0;
    conv(1,0) = std::sin(ST);
    conv(1,1) = -std::cos(ST);
    conv(1,2) = 0;
    conv(2,0) = 0;
    conv(2,1) = 0;
    conv(2,2) = 1;
  }

  matrix<ElementType> get(){
    return conv;
  }

  std::string to_string(){
    return "Equatorial Coordinate Hour Angle to and from Equatorial Coordinate Right Ascension";
  }
};

template
    <typename ElementType = double>
struct ecliptic_to_ra_dec
{
 public:
  matrix<ElementType> conv = matrix<ElementType>(3, 3);

  ecliptic_to_ra_dec() {}

  ecliptic_to_ra_dec(ElementType obliquity){
    conv(0,0) = 1;
    conv(0,1) = 0;
    conv(0,2) = 0;
    conv(1,0) = 0;
    conv(1,1) = std::cos(obliquity);
    conv(1,2) = -std::sin(obliquity);
    conv(2,0) = 0;
    conv(2,1) = std::sin(obliquity);
    conv(2,2) = std::cos(obliquity);
  }

  matrix<ElementType> get(){
    return conv;
  }

  std::string to_string(){
    return "Ecliptic to Equatorial Coordinate Right Ascension";
  }
};

template
    <typename ElementType = double>
struct ra_dec_to_ecliptic
{
 public:
  matrix<ElementType> conv = matrix<ElementType>(3, 3);

  ra_dec_to_ecliptic() {}

  ra_dec_to_ecliptic(ElementType obliquity){
    conv(0,0) = 1;
    conv(0,1) = 0;
    conv(0,2) = 0;
    conv(1,0) = 0;
    conv(1,1) = std::cos(obliquity);
    conv(1,2) = -std::sin(obliquity);
    conv(2,0) = 0;
    conv(2,1) = std::sin(obliquity);
    conv(2,2) = std::cos(obliquity);
  }

  matrix<ElementType> get(){
    return conv;
  }

  std::string to_string(){
    return "Equatorial Coordinate Right Ascension to Ecliptic";
  }
};

template
    <typename ElementType = double>
struct galactic_to_ra_dec
{
 public:
  matrix<ElementType> conv = matrix<ElementType>(3, 3);

  galactic_to_ra_dec(){
    conv(0,0) = -0.0669887;
    conv(0,1) = -0.8727558;
    conv(0,2) = -0.4835389;
    conv(1,0) = 0.4927285;
    conv(1,1) = -0.4503470;
    conv(1,2) = 0.7445846;
    conv(2,0) = -0.8676008;
    conv(2,1) = -0.1883746;
    conv(2,2) = 0.4601998;
  }

  matrix<ElementType> get(){
    return conv;
  }

  std::string to_string(){
    return "Galactic to Equatorial Coordinate Right Ascension";
  }
};

template
    <typename ElementType = double>
struct ra_dec_to_galactic
{
 public:
  matrix<ElementType> conv = matrix<ElementType>(3, 3);

  ra_dec_to_galactic()
  {
    conv(0,0) = -0.0669887;
    conv(0,1) = 0.8727558;
    conv(0,2) = -0.4835389;
    conv(1,0) = -0.4927285;
    conv(1,1) = -0.4503470;
    conv(1,2) = -0.7445846;
    conv(2,0) = -0.8676008;
    conv(2,1) = 0.1883746;
    conv(2,2) = 0.4601998;
  }

  matrix<ElementType> get()
  {
    return conv;
  }

  std::string to_string()
  {
    return "Equatorial Coordinate Right Ascension to Galactic";
  }

};

typedef bu::quantity<bu::si::plane_angle, double> angle_radian;

struct extract_coordinates{
 private:
  double theta = 0;
  double phi = 0;

 public:
  extract_coordinates(){}

  explicit extract_coordinates(matrix<double> col_vec)
  {
    double m = col_vec(0,0);
    double n = col_vec(1,0);
    double p = col_vec(2,0);

    theta = atan2(n,m);
    phi = asin(p);
  }

  pair<angle_radian,angle_radian> get_coordinates() const{
    return make_pair(theta * bu::si::radian,phi * bu::si::radian);
  }
};

struct obliquity_of_ecliptic{
 private:
  angle_radian e = 0.0 * bu::si::radian;

 public:
  obliquity_of_ecliptic(date d)
  {
    double JD = d.julian_day();

    double S = JD - 2451545.0;

    double T = S / 36525.0;

    double DE = 46.815 * T + (0.0006 - 0.00181 * T) * T * T;

    DE = DE / 3600.0;

    double e_degrees =  23.439292 - DE;

    e = (e_degrees * PI / 180.0) * bu::si::radian;
  }

  angle_radian get(){
    return e;
  }
};

}}}

#endif  // BOOST_ASTRONOMY_UTILITY_HPP
