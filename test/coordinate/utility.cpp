/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE utility

#include <iostream>
#include <boost/units/io.hpp>
#include <boost/units/systems/angle/degrees.hpp>

#include <boost/astronomy/coordinate/coord_sys/horizon_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/ecliptic_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/galactic_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/equatorial_ra_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/equatorial_ha_coord.hpp>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/astronomy/coordinate/utility/utility.hpp>

#include <boost/astronomy/time/parser.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/astronomy/time/time_conversions.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/test/unit_test.hpp>

using namespace boost::units;
using namespace boost::units::si;
using namespace boost::astronomy::coordinate;

namespace bud = boost::units::degree;
namespace bac = boost::astronomy::coordinate;

using namespace boost::numeric::ublas;
namespace bnu = boost::numeric::ublas;

using namespace boost::gregorian;
using namespace boost::posix_time;

BOOST_AUTO_TEST_SUITE(utility)

BOOST_AUTO_TEST_CASE(column_vector) {

  quantity<bud::plane_angle, double> u = 97.638119 * bud::degree;
  quantity<bud::plane_angle, double> v = -17.857969 * bud::degree;

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec(u,v);

  BOOST_CHECK_CLOSE(vec.get()(0,0), -0.126512, 0.001);
  BOOST_CHECK_CLOSE(vec.get()(1,0), 0.943374, 0.001);
  BOOST_CHECK_CLOSE(vec.get()(2,0), -0.306658, 0.001);
}

BOOST_AUTO_TEST_CASE(ha_declination_horizon) {

  /**
   * What are the Horizon Coordinated (Altitude and Azimuth)
   * of a star whose Hour Angle is 5h 51m 44s and
   * Declination is +23◦ 13′ 10′′?
   * The observer’s latitude is 52◦ N.
   */

  double ha = decimal_hour(5,51,44).get() * 15.0;
  double declination = 23.21944444;
  quantity<bud::plane_angle, double> phi1 = 52.0 * bud::degree;

  equatorial_ha_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      eha(ha * bud::degrees, declination * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec1(eha.get_ha(),eha.get_dec());

  matrix<double> resultant_vector1 = prod(bac::ha_declination_horizon<double, quantity<bud::plane_angle>, double>(phi1).get(),vec1.get());

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
  quantity<bud::plane_angle, double> phi2 = 52.0 * bud::degree;

  horizon_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      hc(altitude_a * bud::degrees, azimuth_A * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec2(hc.get_azimuth(),hc.get_altitude());

  matrix<double> resultant_vector2 = prod(bac::ha_declination_horizon<double, quantity<bud::plane_angle>, double>(phi2).get(),vec2.get());

  auto coordinates2 = bac::extract_coordinates(resultant_vector2).get_coordinates();
  auto theta2 = coordinates2.first;
  auto gama2 = coordinates2.second;

  BOOST_CHECK_CLOSE(theta2.value() * 180.0 / PI, 87.933334, 0.001);
  BOOST_CHECK_CLOSE(gama2.value() * 180.0 / PI, 23.219444, 0.001);
}

BOOST_AUTO_TEST_CASE(ha_declination_ra_declination) {

  /**
   * What was the Local Hour Angle of a star whose Right Ascension
   * was 18h 32m 21s on local calendar date 22 April 1980 when
   * observed in time zone −4 h from
   * Longitude 64◦ W at local time 14h 36m 51.67s?
   */
  double ra = decimal_hour(18,32,21).get() * 15.0;
  double declination = 23.21944444;

  std::string ts1("1980-04-22 14:36:51.67");
  ptime t1(time_from_string(ts1));

  decimal_hour d1 = LST(64,DIRECTION::WEST, t1);

  quantity<bud::plane_angle, double> ST = (d1.get() + 4) * 15 * bud::degree;

  equatorial_ra_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      era(ra * bud::degrees, declination * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec1(era.get_ra(),era.get_dec());

  matrix<double> resultant_vector1 = prod(bac::ha_declination_ra_declination<double, quantity<bud::plane_angle>, double>(ST).get(),vec1.get());

  auto coordinates1 = bac::extract_coordinates(resultant_vector1).get_coordinates();
  auto theta1 = coordinates1.first;
  auto gama1 = coordinates1.second;

  //Hour Angle converted from degree to hour
  BOOST_CHECK_CLOSE((theta1.value() * 180.0 / PI) / 15.0, 9.873239, 1);
  BOOST_CHECK_CLOSE(gama1.value() * 180.0 / PI, 23.219444, 0.001);

  /**
   * What was the Right Ascension of a star whose Local Hour Angle
   * was 9h 52m 23.66s on local calendar date 22 April 1980 when
   * observed in time zone −4 h from
   * Longitude 64◦ W at local time 14h 36m 51.67s?
   */
  double ha = decimal_hour(9,52,23.66).get() * 15.0;

  equatorial_ha_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      eha(ha * bud::degrees, declination * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec2(eha.get_ha(),eha.get_dec());

  matrix<double> resultant_vector2 = prod(bac::ha_declination_ra_declination<double, quantity<bud::plane_angle>, double>(ST).get(),vec2.get());

  auto coordinates2 = bac::extract_coordinates(resultant_vector2).get_coordinates();
  auto theta2 = coordinates2.first;
  auto gama2 = coordinates2.second;

  // Right Ascension converted from degree to hour
  // If Right Ascension negative, add 24.
  long double theta = (theta2.value() * 180.0 / PI) / 15.0;
  long double ra_result = ( theta ) < 0 ? theta + 24.0 : theta;

  BOOST_CHECK_CLOSE(ra_result, 18.539165, 1);
  BOOST_CHECK_CLOSE(gama2.value() * 180.0 / PI, 23.219444, 0.001);
}

BOOST_AUTO_TEST_CASE(ecliptic_to_ra_declination) {

  /**
   * What were the Right Ascension and the declination of a planet
   * whose ecliptic coordinates were longitude 139◦ 41′ 10′′ and
   * latitude 4◦ 52′ 31′′ on 6 July 2009?
   */
  double longitude = 139.6861111;
  double latitude = 4.87527778;

  std::string s("2009-07-6");
  date d(from_simple_string(s));

  auto obliquity = obliquity_of_ecliptic(d).get();
  BOOST_CHECK_CLOSE(obliquity.value() * 180.0 / PI, 23.43805531 , 0.001);

  ecliptic_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      ec(longitude * bud::degrees, latitude * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec(ec.get_lat(),ec.get_lon());

  matrix<double> resultant_vector1 = prod(bac::ecliptic_to_ra_declination<>(obliquity).get(),vec.get());

  auto coordinates = bac::extract_coordinates(resultant_vector1).get_coordinates();
  auto theta = coordinates.first;
  auto gama = coordinates.second;

  BOOST_CHECK_CLOSE((theta.value() * 180.0 / PI), 143.722173, 0.001);
  BOOST_CHECK_CLOSE(gama.value() * 180.0 / PI, 19.535003, 0.001);
}

BOOST_AUTO_TEST_CASE(ra_declination_to_ecliptic) {
  /**
   * What are the Ecliptic Coordinates of a planet whose
   * Right Ascension and Declination are given as
   * α = 9h 34m 53.32s and δ = 19◦ 32′ 6.01′′
   * when the Greenwich calendar date is 6July2009?
   */

  double ra = decimal_hour(9,34,53.32).get() * 15.0;
  double declination = 19.535003;

  std::string s("2009-07-6");
  date d(from_simple_string(s));

  auto obliquity = obliquity_of_ecliptic(d).get();
  BOOST_CHECK_CLOSE(obliquity.value() * 180.0 / PI, 23.43805531 , 0.001);

  equatorial_ra_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      era(ra * bud::degrees, declination * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec(era.get_ra(),era.get_dec());

  matrix<double> resultant_vector = prod(bac::ra_declination_to_ecliptic<>(obliquity).get(),vec.get());

  auto coordinates = bac::extract_coordinates(resultant_vector).get_coordinates();
  auto theta = coordinates.first;
  auto gama = coordinates.second;

  BOOST_CHECK_CLOSE((theta.value() * 180.0 / PI), 139.686106, 0.001);
  BOOST_CHECK_CLOSE(gama.value() * 180.0 / PI, 4.875276, 0.001);
}

BOOST_AUTO_TEST_CASE(ra_declination_to_galactic) {

  /**
   * What are the Galactic Coordinates of a star whose
   * right ascension and declination are
   * α = 10h 21m 00s and δ = 10◦ 03′ 11′′?
   */

  double ra = decimal_hour(10,21,0).get() * 15.0;
  double declination = 10.053056;

  equatorial_ra_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      era(ra * bud::degrees, declination * bud::degrees);

  bac::column_vector<double, quantity<bud::plane_angle>, double> vec(era.get_ra(),era.get_dec());

  matrix<double> resultant_vector = prod(bac::ra_declination_to_galactic<double>().get(),vec.get());

  auto coordinates = bac::extract_coordinates(resultant_vector).get_coordinates();
  auto theta = coordinates.first;
  auto gama = coordinates.second;

  long double longitude_result = theta.value() * 180.0 / PI;
  long double longitude = ( longitude_result ) < 0 ? longitude_result + 360.0 : longitude_result;

  BOOST_CHECK_CLOSE(longitude, 232.247881, 0.001);
  BOOST_CHECK_CLOSE(gama.value() * 180.0 / PI, 51.122268, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
