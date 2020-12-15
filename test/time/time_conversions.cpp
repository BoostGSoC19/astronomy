/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE time_conversions_test

#include <iostream>
#include <boost/astronomy/time/parser.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/astronomy/time/time_conversions.hpp>

#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(time_conversions)

BOOST_AUTO_TEST_CASE(time)
{
  //Make an object of universal time
  //What was the GST at 14h 36m 51.67s UT on Greenwich date 22 April 1980?
  //GST = 4h 40m 5.23s
  //Verified from Practical Astronomy with your Calculator
  //by Peter Duffett-Smith
  std::string ts1("1980-04-22 14:36:51.67");
  ptime t1(time_from_string(ts1));

  decimal_hour d1 = GST(t1);

  BOOST_CHECK_CLOSE(d1.get(), decimal_hour(4,40,5.23).get(), 1);

  //What is the local sidereal time on the longitude 64◦ W
  //when the GST is 4h 40m 5.23s OR
  //at 14h 36m 51.67s UT on Greenwich date 22 April 1980
  //Local Sidereal Time at Longitude 64.00°(W) = 0h 24m 5.23s
  decimal_hour d2 = LST(64, DIRECTION::WEST, t1);

  BOOST_CHECK_CLOSE(d2.get(), decimal_hour(0,24,5.23).get(), 1);

  //What was the GST at 19h 21m 0.0s UT on Greenwich date 10 April 1987?
  //GST = 8h 34m 57.0896s
  //Verified from Astronomical Algorithms 2nd Edition. by. Jean Meeus
  std::string ts2("1987-04-10 19:21:0");
  ptime t2(time_from_string(ts2));

  decimal_hour d3 = GST(t2);

  BOOST_CHECK_CLOSE(d3.get(), decimal_hour(8,34,57.0896).get(), 1);

  //What is the local sidereal time on the longitude 82◦ E
  //when the GST is 8h 36m 55.00s OR
  //at 19h 21m 0.00s UT on Greenwich date 10 April 1987
  //Local Sidereal Time at Longitude 82.00° (E) = 14h 05m 42s
  decimal_hour d4 = LST(82, DIRECTION::EAST, t2);

  BOOST_CHECK_CLOSE(d4.get(), decimal_hour(14,5,42).get(), 1);

  //What was the GST at 16h 44m 0.0s UT on Greenwich date 13 August 2020?
  //GST = 14h 14m 18.136s
  //Verified from http://neoprogrammics.com/sidereal_time_calculator/index.php
  std::string ts3("2020-08-13 16:44:0");
  ptime t3(time_from_string(ts3));

  decimal_hour d5 = GST(t3);

  BOOST_CHECK_CLOSE(d5.get(), decimal_hour(14, 14, 18.136).get(), 1);

  //What is the local sidereal time on the longitude 37◦ E
  //when the GST is 14h 14m 18.136s OR
  //at at 16h 44m 0.0s UT on Greenwich date 13 August 2020
  //Local Sidereal Time at Longitude 37.00° (E) = 16h 42m 19s
  decimal_hour d6 = LST(37, DIRECTION::EAST, t3);

  BOOST_CHECK_CLOSE(d6.get(), decimal_hour(16, 42, 19).get(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
