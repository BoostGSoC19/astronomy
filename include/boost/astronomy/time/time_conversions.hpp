/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_TIME_CONVERSIONS
#define BOOST_ASTRONOMY_TIME_CONVERSIONS

#include <string>
#include <iostream>
#include <exception>
#include <boost/astronomy/time/parser.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

/**
 * Universal time (UT), and therefore the local civil time in any
 * part of the world, is related to the apparent motion of the Sun
 * around the Earth.
 *
 * Sidereal Time (ST) is a time scale that is based on Earth's rate
 * of rotation measured relative to the fixed stars.
 */

/**
 * The Greenwich Sidereal Time(GST) is the sidereal time correct for observations
 * made on the Greenwich meridian, longitude 0◦.
 */
decimal_hour GST(ptime t)
{
    //Get date from UT
    date d = t.date();

    //Get Julian Day Number
    double JD = d.julian_day(); //Ambiguity in Julian precision.

    double S = JD - 2451545.0;

    double T = S/36525.0;

    double T0 = 6.697374558 + (2400.051336 * T) + (0.000025862 * T * T);

    //Reduce the result to the range 0 to 24 by adding or subtracting multiples of 24
    T0 = T0 - 24.0 * floor(T0/24.0);

    //Convert UT to decimal hours
    double UT = ((t.time_of_day().seconds())/60.0 + t.time_of_day().minutes())/60.0 + t.time_of_day().hours();\

    //Multiply UT by 1.002737909
    double A = UT * 1.002737909;

    T0 += A;

    //Add this to T0 and reduce to the range 0 to 24 if necessary by subtracting or adding 24. This is the GST.
    T0 = T0 - 24.0 * floor(T0/24.0);

    //Return GST in decimal hours
    return {T0};
}

enum class DIRECTION {WEST, EAST};

decimal_hour compute_LST(double longitude, DIRECTION direction, double gst)
{
  //Convert longitude to hours
  double long_hours = longitude / 15.0;

  switch(direction)
  {
    case DIRECTION::WEST:
      //Multiply with direction
      long_hours = -1 * long_hours;
      break;
    case DIRECTION::EAST:
      //Multiply with direction
      long_hours = 1 * long_hours;
      break;
  }

  long_hours = long_hours + gst;

  //Bring the result into the range 0 to 24 by adding or subtracting 24 if necessary.
  //This is the local sidereal time (LST).
  long_hours = long_hours - 24.0 * floor(long_hours/24.0);

  return {long_hours};
}

//Local Sidereal Time (LST)
decimal_hour LST(double longitude, DIRECTION direction, ptime t)
{
  double gst = GST(t).get();

  if(longitude == 0)
    return {gst};

  return compute_LST(longitude,direction,gst);
}

decimal_hour LST(double longitude, DIRECTION direction, double gst)
{
  if(longitude == 0)
    return {gst};

  return compute_LST(longitude,direction,gst);
}

#endif //BOOST_ASTRONOMY_TIME_CONVERSIONS
