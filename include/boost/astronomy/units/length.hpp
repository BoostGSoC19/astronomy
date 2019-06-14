#ifndef BOOST_ASTRONOMY_UNITS_UNITS_HPP
#define BOOST_ASTRONOMY_UNITS_UNITS_HPP

#include <boost/units/quantity.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/base_units/astronomical/astronomical_unit.hpp>
#include <boost/units/base_units/astronomical/light_day.hpp>
#include <boost/units/base_units/astronomical/light_hour.hpp>
#include <boost/units/base_units/astronomical/light_minute.hpp>
#include <boost/units/base_units/astronomical/light_second.hpp>
#include <boost/units/base_units/astronomical/light_year.hpp>
#include <boost/units/base_units/astronomical/parsec.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/cgs/length.hpp>


namespace boost { namespace astronomy { namespace units {

namespace bu = boost::units;
namespace bua = boost::units::astronomical;

typedef bua::astronomical_unit_base_unit::unit_type length_astronomical_unit;
BOOST_UNITS_STATIC_CONSTANT(astronomical_unit, length_astronomical_unit);
BOOST_UNITS_STATIC_CONSTANT(astronomical_units, length_astronomical_unit);

typedef bua::light_day_base_unit::unit_type length_light_day;
BOOST_UNITS_STATIC_CONSTANT(light_day, length_light_day);
BOOST_UNITS_STATIC_CONSTANT(light_days, length_light_day);

typedef bua::light_hour_base_unit::unit_type length_light_hour;
BOOST_UNITS_STATIC_CONSTANT(light_hour, length_light_hour);
BOOST_UNITS_STATIC_CONSTANT(light_hours, length_light_hour);

typedef bua::light_minute_base_unit::unit_type length_light_minute;
BOOST_UNITS_STATIC_CONSTANT(light_minute, length_light_minute);
BOOST_UNITS_STATIC_CONSTANT(light_minutes, length_light_minute);

typedef bua::light_second_base_unit::unit_type length_light_second;
BOOST_UNITS_STATIC_CONSTANT(light_second, length_light_second);
BOOST_UNITS_STATIC_CONSTANT(light_seconds, length_light_second);

typedef bua::light_year_base_unit::unit_type length_light_year;
BOOST_UNITS_STATIC_CONSTANT(light_year, length_light_year);
BOOST_UNITS_STATIC_CONSTANT(light_years, length_light_year);

typedef bua::parsec_base_unit::unit_type length_parsec;
BOOST_UNITS_STATIC_CONSTANT(parsec, length_parsec);
BOOST_UNITS_STATIC_CONSTANT(parsecs, length_parsec);

typedef bu::si::length length_meter;
typedef bu::si::length length_metre;
BOOST_UNITS_STATIC_CONSTANT(meter, length_meter);
BOOST_UNITS_STATIC_CONSTANT(meters, length_meter);
BOOST_UNITS_STATIC_CONSTANT(metre, length_meter);
BOOST_UNITS_STATIC_CONSTANT(metres, length_meter);

typedef bu::cgs::length length_centimeter;
typedef bu::cgs::length length_centimetre;
BOOST_UNITS_STATIC_CONSTANT(centimeter, length_centimeter);
BOOST_UNITS_STATIC_CONSTANT(centimeters, length_centimeter);
BOOST_UNITS_STATIC_CONSTANT(centimetre, length_centimeter);
BOOST_UNITS_STATIC_CONSTANT(centimetres, length_centimeter);

}}} //namespace boost::astronomy::units


#endif // !BOOST_ASTRONOMY_UNITS_UNITS_HPP
