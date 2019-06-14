#define BOOST_TEST_MODULE unit_length_test

#include <boost/test/unit_test.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/cgs/length.hpp>
#include <boost/astronomy/units/length.hpp>

namespace bu = boost::units;
namespace bau = boost::astronomy::units;


BOOST_AUTO_TEST_SUITE(length_unit)

BOOST_AUTO_TEST_CASE(units_astronomical_unit)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::astronomical_unit_base_unit::unit_type>,
        bu::quantity<bau::length_astronomical_unit>>::value
        ));

    bu::quantity<
        bu::astronomical::astronomical_unit_base_unit::unit_type,
        int
    > q1 = 10 * bau::astronomical_unit;
    bu::quantity<bau::length_astronomical_unit, int> q2 = 10 * bau::astronomical_units;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_light_day)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::light_day_base_unit::unit_type>,
        bu::quantity<bau::length_light_day>>::value
        ));

    bu::quantity<bu::astronomical::light_day_base_unit::unit_type, int> q1 = 10 * bau::light_day;
    bu::quantity<bau::length_light_day, int> q2 = 10 * bau::light_days;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_light_hour)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::light_hour_base_unit::unit_type>,
        bu::quantity<bau::length_light_hour>>::value
        ));

    bu::quantity<bu::astronomical::light_hour_base_unit::unit_type, int> q1 = 10 * bau::light_hour;
    bu::quantity<bau::length_light_hour, int> q2 = 10 * bau::light_hour;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_light_minute)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::light_minute_base_unit::unit_type>,
        bu::quantity<bau::length_light_minute>>::value
        ));

    bu::quantity<bu::astronomical::light_minute_base_unit::unit_type> q1 = 10 * bau::light_minute;
    bu::quantity<bau::length_light_minute, int> q2 = 10 * bau::light_minutes;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_light_second)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::light_second_base_unit::unit_type>,
        bu::quantity<bau::length_light_second>>::value
        ));

    bu::quantity<bu::astronomical::light_second_base_unit::unit_type> q1 = 10 * bau::light_second;
    bu::quantity<bau::length_light_second, int> q2 = 10 * bau::light_seconds;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_light_year)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::light_year_base_unit::unit_type>,
        bu::quantity<bau::length_light_year>>::value
        ));

    bu::quantity<bu::astronomical::light_year_base_unit::unit_type, int> q1 = 10 * bau::light_year;
    bu::quantity<bau::length_light_year, int> q2 = 10 * bau::light_years;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_parsec)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::astronomical::parsec_base_unit::unit_type>,
        bu::quantity<bau::length_parsec>>::value
        ));

    bu::quantity<bu::astronomical::parsec_base_unit::unit_type, int> q1 = 10 * bau::parsec;
    bu::quantity<bau::length_parsec, int> q2 = 10 * bau::parsecs;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_meter)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::si::length>,
        bu::quantity<bau::length_meter>>::value
        ));
    BOOST_TEST((std::is_same<
        bu::quantity<bu::si::length>,
        bu::quantity<bau::length_metre>>::value
        ));

    bu::quantity<bu::si::length, int> q1 = 10 * bau::meter;
    bu::quantity<bau::length_meter, int> q2 = 10 * bau::meters;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));

    q1 = 10 * bau::metre;
    q2 = 10 * bau::metres;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_CASE(units_centimeter)
{
    BOOST_TEST((std::is_same<
        bu::quantity<bu::cgs::length>,
        bu::quantity<bau::length_centimeter>>::value
        ));
    BOOST_TEST((std::is_same<
        bu::quantity<bu::cgs::length>,
        bu::quantity<bau::length_centimetre>>::value
        ));

    bu::quantity<bu::cgs::length, int> q1 = 10 * bau::centimeter;
    bu::quantity<bau::length_centimeter, int> q2 = 10 * bau::centimeters;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));

    q1 = 10 * bau::centimetre;
    q2 = 10 * bau::centimetres;

    BOOST_TEST((q1.value() == 10));
    BOOST_TEST((q1 == q2));
}

BOOST_AUTO_TEST_SUITE_END()
