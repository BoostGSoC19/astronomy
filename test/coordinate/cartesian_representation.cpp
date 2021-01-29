/*=============================================================================
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE cartesian_representation_test

#include <boost/test/unit_test.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>
#include <boost/astronomy/coordinate/rep/representation.hpp>

using namespace std;
using namespace boost::astronomy::coordinate;
using namespace boost::units::si;
using namespace boost::geometry;
using namespace boost::units;
namespace bud = boost::units::degree;

BOOST_AUTO_TEST_SUITE(cartesian_representation_constructors)

BOOST_AUTO_TEST_CASE(cartesian_representation_default_constructor)
{
    //using set functions
    cartesian_representation<double, quantity<si::length>, quantity<si::length>,
    quantity<si::length>> point1;
    point1.set_x_y_z(2.5*meter, 91.0*meter, 12.0*meter);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 2.5, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 91.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 12, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_quantities_constructor)
{
    //checking construction from value
    auto point1 = make_cartesian_representation
    (1.5*meter, 9.0*si::kilo*meter, 3.0*si::centi*meter);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_z()), quantity<decltype(si::centi*meter)>>::value));

    cartesian_representation<double, quantity<si::length>, quantity<si::length>,
    quantity<si::length>> point2(1.5*meter, 9.0*meter, 3.0*meter);
    BOOST_CHECK_CLOSE(point2.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point2.get_y().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(point2.get_z().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_copy_constructor)
{
    //checking construction from value
    auto point1 = make_cartesian_representation
    (1.5*meter, 9.0*si::kilo*meter, 3.0*si::centi*meter);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_z()), quantity<decltype(si::centi*meter)>>::value));

    //copy constructor
    auto point2 = make_cartesian_representation(point1);
    BOOST_CHECK_CLOSE(point1.get_x().value(), point2.get_x().value(), 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), point2.get_y().value(), 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), point2.get_z().value(), 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_z()), quantity<decltype(si::centi*meter)>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_copy_constructor_with_different_units)
{
    //checking construction from value
    auto point1 = make_cartesian_representation
    (1.5*meter, 9.0*si::kilo*meter, 3.0*si::centi*meter);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_z()), quantity<decltype(si::centi*meter)>>::value));

    //Conversion from one unit type to other
    auto point2 = make_cartesian_representation
    <double, quantity<si::length>, quantity<si::length>, quantity<si::length>>(point1);
    BOOST_CHECK_CLOSE(point2.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point2.get_y().value(), 9000.0, 0.001);
    BOOST_CHECK_CLOSE(point2.get_z().value(), 0.03, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_geometry_point_constructor)
{
    //constructing from boost::geometry::model::point
    model::point<double, 3, cs::spherical<boost::geometry::degree>> model_point(30, 60, 1);
    auto point1 = make_cartesian_representation
    <double,quantity<si::length>,quantity<si::length>,quantity<si::length>>(model_point);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 0.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_z()), quantity<si::length>>::value));

    cartesian_representation<double, quantity<si::length>, quantity<si::length>,
    quantity<si::length>> point2(model_point);
    BOOST_CHECK_CLOSE(point2.get_x().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(point2.get_y().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(point2.get_z().value(), 0.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_conversion_from_spherical_representation)
{
    //constructing from spherical representation
    auto spherical_point = make_spherical_representation
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meter);
    auto point1 = make_cartesian_representation(spherical_point);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 0.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_conversion_from_spherical_equatorial_representation)
{
    //constructing from spherical_equitorial representation
    auto spherical_equatorial_point = make_spherical_equatorial_representation
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meter);
    auto point2 = make_cartesian_representation(spherical_equatorial_point);
    BOOST_CHECK_CLOSE(point2.get_x().value(), 0.433012646, 0.001);
    BOOST_CHECK_CLOSE(point2.get_y().value(), 0.250000097, 0.001);
    BOOST_CHECK_CLOSE(point2.get_z().value(), 0.866025405, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(cartesian_representation_operators)

BOOST_AUTO_TEST_CASE(cartesian_representation_addition_operator)
{
    auto point1 = make_cartesian_representation(11.0*meters, 15.0*meters, 19.0*meters);
    auto point2 = make_cartesian_representation
        (6.0*si::milli*meters, 10.0*si::centi*meters, 11.0*meters);

    auto sum = make_cartesian_representation(point1 + point2);

    BOOST_CHECK_CLOSE(sum.get_x().value(), 11.006, 0.001);
    BOOST_CHECK_CLOSE(sum.get_y().value(), 15.1, 0.001);
    BOOST_CHECK_CLOSE(sum.get_z().value(), 30, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(sum.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(cartesian_representation_arithmetic_functions)

BOOST_AUTO_TEST_CASE(cartesian_representation_cross_product)
{
    auto point1 = make_cartesian_representation
        (3.0 * meters, 5.0 * si::kilo *meters, 4.0 * si::mega * meters);
    auto point2 = make_cartesian_representation
        (3.0 * si::milli * meters, 5.0 * si::centi * meters, 4.0 * meters);
    //spherical_representation<degree> point2(45, 45, 3);

    auto result = cross(point1, point2);

    BOOST_CHECK_CLOSE(result.get_x().value(), -180.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 11.988, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), -1485.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity
        <bu::multiply_typeof_helper<decltype(si::kilo*meters), si::length>::type>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity
        <bu::multiply_typeof_helper<decltype(si::mega*meters),
        decltype(si::milli*meters)>::type>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity
        <bu::multiply_typeof_helper<decltype(si::centi*meters), si::length>::type>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_dot_product)
{
    auto point1 = make_cartesian_representation
        (3.0 * meters, 5.0 * si::kilo *meters, 4.0 * si::mega * meters);
    auto point2 = make_cartesian_representation
        (3.0 * si::milli * meters, 5.0 * si::centi * meters, 4.0 * meters);

    auto result = dot(point1, point2);

    BOOST_CHECK_CLOSE(result.value(), 16000250009.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result), quantity
        <bu::multiply_typeof_helper<decltype(si::milli*meters), si::length>::type>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_scalar_multiplication)
{
    auto point1 = make_cartesian_representation(25.0 * meter, 36.0 * si::centi * meter, 90.0 * si::kilo * meter);

    auto result = boost::astronomy::coordinate::multiply(point1, 2);

    BOOST_CHECK_CLOSE(result.get_x().value(), 50.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 72.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 180.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity<decltype(si::centi*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity<decltype(si::kilo*meter)>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_unit_vector)
{
    auto point1 = make_cartesian_representation(25.0*meter, 36.0*meter, 90.0*meter);

    auto result = boost::astronomy::coordinate::unit_vector(point1);

    BOOST_CHECK_CLOSE(result.get_x().value(), 0.2497379127153113, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 0.3596225943100483, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 0.8990564857751207, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_magnitude)
{
    auto point1 = make_cartesian_representation
        (25.0 * meter, 3600.0 * si::centi*meter, 90.0 * meter);

    auto result = boost::astronomy::coordinate::magnitude(point1);

    BOOST_CHECK_CLOSE(result.value(), 100.1049449328054, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result), quantity<si::length>>::value));

}

BOOST_AUTO_TEST_CASE(cartesian_representation_sum)
{
    auto point1 = make_cartesian_representation
        (10.0 * meter, 20.0 * si::kilo * meters, 30.0 * meter);
    auto point2 = make_cartesian_representation
        (50.0 * si::centi * meter, 60.0 * meter, 30.0 * meter);

    auto result = boost::astronomy::coordinate::sum(point1, point2);

    BOOST_CHECK_CLOSE(result.get_x().value(), 10.5, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 20.06, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 60, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_difference)
{
    auto point1 = make_cartesian_representation
        (10.0 * meter, 20.0 * si::kilo * meters, 30.0 * meter);
    auto point2 = make_cartesian_representation
        (50.0 * si::centi * meter, 60.0 * meter, 30.0 * meter);

    auto result = boost::astronomy::coordinate::difference(point1, point2);

    BOOST_CHECK_CLOSE(result.get_x().value(), 9.5, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 19.94, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_mean)
{
    auto point1 = make_cartesian_representation
        (10.0 * meter, 20.0 * si::kilo * meters, 30.0 * meter);
    auto point2 = make_cartesian_representation
        (50.0 * si::centi * meter, 60.0 * meter, 30.0 * meter);

    auto result = boost::astronomy::coordinate::mean(point1, point2);

    BOOST_CHECK_CLOSE(result.get_x().value(), 5.25, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 10.03, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 30.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_representation_matrix_multiplication)
{
    auto point = make_cartesian_representation
        (10.0 * meter, 20.0 * si::kilo * meters, 30.0 * si::centi * meter);

    matrix<double> m(3, 3);
    for (unsigned i = 0; i < m.size1(); ++i)
        for (unsigned j = 0; j < m.size2(); ++j)
            m(i, j) = 3 * i + j;

    auto result = boost::astronomy::coordinate::matrix_multiply(point, m);

    BOOST_CHECK_CLOSE(result.get_x().value(), 80.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 260.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 440.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_x()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_y()), quantity<decltype(si::kilo*meter)>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_z()), quantity<decltype(si::centi*meter)>>::value));
}

BOOST_AUTO_TEST_SUITE_END()
