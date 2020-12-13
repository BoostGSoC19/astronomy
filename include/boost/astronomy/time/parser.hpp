/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_PARSER_HPP
#define BOOST_ASTRONOMY_PARSER_HPP

#include <string>
#include <iostream>

using namespace std;

struct decimal_hour {
private:
    double dh = 0;

    double time = 0;
    long int hours = 0;
    double minutesRemainder = 0;
    long int minutes = 0;
    double secondsRemainder = 0;
    double seconds = 0;

public:
 decimal_hour(double const& d){
      dh = d;

      time = dh;
      hours = (long int)time;
      minutesRemainder = (time - hours) * 60;
      minutes = (long int)minutesRemainder;
      secondsRemainder = (minutesRemainder - minutes) * 60;
      seconds = secondsRemainder;
    }

    decimal_hour(double const& h,double const& m,double const& s){
      dh = h + m/60 + s/(60*60);
    }

    double get() const{
        return dh;
    }

    long int get_hours() const{
        return hours;
    }

    long int get_minutes() const{
        return minutes;
    }

    double get_seconds() const{
        return seconds;
    }
};

std::ostream& operator << (std::ostream &out, decimal_hour const& dh)
{
    std::string time_string = std::to_string(dh.get_hours()) + "h " + std::to_string(dh.get_minutes()) + "m " + std::to_string(dh.get_seconds()) + "s ";
    return out << "Hours: " << time_string;
}

#endif //BOOST_ASTRONOMY_PARSER_HPP
