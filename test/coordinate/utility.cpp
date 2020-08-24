/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE utility

#include <iostream>
#include <boost/units/io.hpp>
#include <boost/units/systems/angle/degrees.hpp>

#include <boost/astronomy/coordinate/coord_sys/equatorial_ha_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/ecliptic_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/equatorial_ra_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/galactic_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/horizon_coord.hpp>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/astronomy/coordinate/utility/utility.hpp>

#include <boost/astronomy/time/parser.hpp>


#include <boost/test/unit_test.hpp>

using namespace boost::units;
using namespace boost::units::si;
using namespace boost::astronomy::coordinate;

namespace bud = boost::units::degree;
namespace bac = boost::astronomy::coordinate;

using namespace boost::numeric::ublas;
namespace bnu = boost::numeric::ublas;

#define deb(x)          std::cout << #x << "=" << x << std::endl


BOOST_AUTO_TEST_SUITE(utility)

BOOST_AUTO_TEST_CASE(column_vector) {

    bu::quantity<bud::plane_angle, double> u = 97.638119 * bud::degree;
    bu::quantity<bud::plane_angle, double> v = -17.857969 * bud::degree;

    bac::column_vector<double, quantity<bud::plane_angle>, double> vec(u,v);

    BOOST_CHECK_CLOSE(vec.get()(0,0), -0.126512, 0.001);
    BOOST_CHECK_CLOSE(vec.get()(1,0), 0.943374, 0.001);
    BOOST_CHECK_CLOSE(vec.get()(2,0), -0.306658, 0.001);
}

BOOST_AUTO_TEST_CASE(hour_angle_declination_horizon) {

  /**
   * What are the Horizon Coordinated (Altitude and Azimuth)
   * of a star whose Hour Angle is 5h 51m 44s and
   * Declination is +23◦ 13′ 10′′?
   * The observer’s latitude is 52◦ N.
   */

  double hour_angle = decimal_hour(5,51,44).get() * 15.0;
  double declination = 23.21944444;
  bu::quantity<bud::plane_angle, double> phi1 = 52.0 * bud::degree;

  equatorial_ha_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      eha(hour_angle * bud::degrees, declination * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec1(eha.get_ha(),eha.get_dec());

  matrix<double> resultant_vector1 = prod(bac::hour_angle_declination_horizon<double, quantity<bud::plane_angle>, double>(phi1).get(),vec1.get());

  auto coordinates1 = bac::extract_coordinates(resultant_vector1).get_coordinates();
  auto theta1 = coordinates1.first;
  auto gama1 = coordinates1.second;

  BOOST_CHECK_CLOSE(theta1.value() * 180.0 / PI, -76.728973, 0.001);
  BOOST_CHECK_CLOSE(gama1.value() * 180.0 / PI, 19.33434444, 0.001);

  /**
   * What are the Equatorial Coordinates (Hour Angle and Declination)
   * of a star that is observed by an observer at latitude 52◦ N to
   * have an Altitude of 19◦ 20′ 03.64′′ and an Azimuth of 283◦ 16′ 15.7′
   */

  double altitude_a = 19.334344;
  double azimuth_A = 283.271028;
  bu::quantity<bud::plane_angle, double> phi2 = 52.0 * bud::degree;

  horizon_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      hc(altitude_a * bud::degrees, azimuth_A * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec2(hc.get_azimuth(),hc.get_altitude());

  matrix<double> resultant_vector2 = prod(bac::hour_angle_declination_horizon<double, quantity<bud::plane_angle>, double>(phi2).get(),vec2.get());

  auto coordinates2 = bac::extract_coordinates(resultant_vector2).get_coordinates();
  auto theta2 = coordinates2.first;
  auto gama2 = coordinates2.second;

  BOOST_CHECK_CLOSE(theta2.value() * 180.0 / PI, 87.933334, 0.001);
  BOOST_CHECK_CLOSE(gama2.value() * 180.0 / PI, 23.219444, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
