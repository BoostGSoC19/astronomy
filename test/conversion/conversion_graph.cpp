/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE conversion_graph

#include <iostream>

#include <boost/units/io.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

#include <boost/astronomy/coordinate/coord_sys/horizon_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/ecliptic_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/galactic_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/equatorial_ra_coord.hpp>
#include <boost/astronomy/coordinate/coord_sys/equatorial_ha_coord.hpp>

#include <boost/astronomy/coordinate/conversion/conversion_graph.hpp>

#include <boost/test/unit_test.hpp>

using namespace boost::units;
using namespace boost::units::si;
using namespace boost::astronomy::coordinate;

namespace bud = boost::units::degree;
namespace bac = boost::astronomy::coordinate;

BOOST_AUTO_TEST_SUITE(conversion_graph)

BOOST_AUTO_TEST_CASE(ecliptic_to_horizon) {

  ecliptic_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
    ec(97.638119 * bud::degrees, -17.857969 * bud::degrees);

  quantity<bud::plane_angle, double> phi = 52.175 * bud::degree;
  quantity<bud::plane_angle, double> st = 77.337 * bud::degree;
  quantity<bud::plane_angle, double> obliquity = 23.446 * bud::degree;

  matrix<double> ans = convert(COORDINATE_SYSTEM::ECLIPTIC,COORDINATE_SYSTEM::HORIZON,phi,st,obliquity,ec);

  auto theta = bac::extract_coordinates(ans).get_coordinates().first;
  auto gama = bac::extract_coordinates(ans).get_coordinates().second;

  BOOST_CHECK_CLOSE(theta.value() * 180.0 / PI, 153.491944, 0.001);
  BOOST_CHECK_CLOSE(gama.value() * 180.0 / PI, 40.399444, 0.001);
}

BOOST_AUTO_TEST_CASE(horizon_to_ecliptic) {

  horizon_coord<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>>
      hc(153.491944 * bud::degrees, 40.399444  * bud::degrees);

  quantity<bud::plane_angle, double> phi = 52.175 * bud::degree;
  quantity<bud::plane_angle, double> st = 77.337 * bud::degree;
  quantity<bud::plane_angle, double> obliquity = 23.446 * bud::degree;

  matrix<double> ans = convert(COORDINATE_SYSTEM::HORIZON,COORDINATE_SYSTEM::ECLIPTIC,phi,st,obliquity,hc);

  auto theta = bac::extract_coordinates(ans).get_coordinates().first;
  auto gama = bac::extract_coordinates(ans).get_coordinates().second;

  BOOST_CHECK_CLOSE(theta.value() * 180.0 / PI, 97.638119 , 0.001);
  BOOST_CHECK_CLOSE(gama.value() * 180.0 / PI, -17.857969, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

