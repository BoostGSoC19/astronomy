/*=============================================================================
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>
Copyright 2020 Nitin K         <konidalanitin@gmail.com>
Copyright 2020 Rohit Ranjan    <rohitrjn629@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE spherical_representation_test

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

BOOST_AUTO_TEST_SUITE(spherical_representation_constructors)

BOOST_AUTO_TEST_CASE(spherical_representation_default_constructor)
{
    //using set functions
    spherical_representation<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::length>> point1;
    point1.set_lat_lon_dist(45.0 * bud::degrees, 18.0 * bud::degrees, 3.5 * meters);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), 45.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), 18.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), 3.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_quantities_constructor)
{
    //checking construction from value
    auto point1 = make_spherical_representation
    (15.0 * bud::degrees, 39.0 * bud::degrees, 3.0*si::centi*meter);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), 39.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), 3.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_dist()), quantity<decltype(si::centi*meter)>>::value));

    spherical_representation<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::length>> point2(1.5 * bud::degrees, 9.0 * bud::degrees, 3.0 * meter);
    BOOST_CHECK_CLOSE(point2.get_lat().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point2.get_lon().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(point2.get_dist().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_copy_constructor)
{
    //checking construction from value
    auto point1 = make_spherical_representation
    (15.0 * bud::degrees, 30.0 * bud::degrees, 3.0*si::centi*meter);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), 30.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_dist()), quantity<decltype(si::centi*meter)>>::value));

    //copy constructor
    auto point2 = make_spherical_representation(point1);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), point2.get_lat().value(), 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), point2.get_lon().value(), 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), point2.get_dist().value(), 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_dist()), quantity<decltype(si::centi*meter)>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_copy_constructor_with_different_units)
{
    //checking construction from value
    auto point1 = make_spherical_representation
    (15.0 * bud::degrees, 10.0 * bud::degrees, 3.0*si::centi*meter);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), 10.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_dist()), quantity<decltype(si::centi*meter)>>::value));

    //Conversion from one unit type to other
    auto point2 = make_spherical_representation
    <double, quantity<bud::plane_angle>, quantity<bud::plane_angle>, quantity<si::length>>(point1);
    BOOST_CHECK_CLOSE(point2.get_lat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(point2.get_lon().value(), 10.0, 0.001);
    BOOST_CHECK_CLOSE(point2.get_dist().value(), 0.03, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_geometry_point_constructor)
{
    //constructing from boost::geometry::model::point
    model::point<double, 3, cs::cartesian> model_point(30, 60, 10);
    auto point1 = make_spherical_representation
    <double,quantity<bud::plane_angle>,quantity<bud::plane_angle>,quantity<si::length>>(model_point);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), 63.434948822922, 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), 81.521286852914, 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), 67.823299831253, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_dist()), quantity<si::length>>::value));

    spherical_representation<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::length>> point2(model_point);
    BOOST_CHECK_CLOSE(point2.get_lat().value(), 63.434948822922, 0.001);
    BOOST_CHECK_CLOSE(point2.get_lon().value(), 81.521286852914, 0.001);
    BOOST_CHECK_CLOSE(point2.get_dist().value(), 67.823299831253, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_conversion_from_cartesian_representation)
{
    //constructing from spherical representation
    auto cartesian_point = make_cartesian_representation(20.0 * meters, 60.0 * meters, 1.0 * meter);
    auto point1 = make_spherical_representation(cartesian_point);
    BOOST_CHECK_CLOSE(point1.get_lat().value(), 1.2490457723983, 0.001);
    BOOST_CHECK_CLOSE(point1.get_lon().value(), 1.5549862559121, 0.001);
    BOOST_CHECK_CLOSE(point1.get_dist().value(), 63.253458403474, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point1.get_lat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_lon()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point1.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_conversion_from_spherical_equatorial_representation)
{
    //constructing from spherical_equitorial representation
    auto spherical_equatorial_point = make_spherical_equatorial_representation
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meter);
    auto point2 = make_spherical_representation(spherical_equatorial_point);
    BOOST_CHECK_CLOSE(point2.get_lat().value(), 0.523599, 0.001);
    BOOST_CHECK_CLOSE(point2.get_lon().value(), 0.523598776, 0.001);
    BOOST_CHECK_CLOSE(point2.get_dist().value(), 1.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(point2.get_lat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_lon()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(point2.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(spherical_representation_operators)

BOOST_AUTO_TEST_CASE(spherical_representation_addition_operator)
{
    auto point1 = make_spherical_representation(15.0 * bud::degrees, 30.0 * bud::degrees, 10.0 * meters);
    auto point2 = make_spherical_representation(30.0 * bud::degrees, 45.0 * bud::degrees, 20.0 * meters);

    auto sum = make_spherical_representation(point1 + point2);

    BOOST_CHECK_CLOSE(sum.get_lat().value(), 26.097805456, 0.001);
    BOOST_CHECK_CLOSE(sum.get_lon().value(), 39.826115507, 0.001);
    BOOST_CHECK_CLOSE(sum.get_dist().value(), 29.6909332103, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(sum.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(spherical_representation_arithmetic_functions)

BOOST_AUTO_TEST_CASE(spherical_representation_cross_product)
{
    auto point1 = make_spherical_representation(3.0 * bud::degrees, 50.0 * bud::degrees, 40.0 * meters);
    auto point2 = make_spherical_representation(30.0 * bud::degrees, 45.0 * bud::degrees, 14.0 * meters);

    auto result = cross(point1, point2);

    BOOST_CHECK_CLOSE(result.get_lat().value(), -143.4774246228, 0.001);
    BOOST_CHECK_CLOSE(result.get_lon().value(), 45.186034054587, 0.001);
    BOOST_CHECK_CLOSE(result.get_dist().value(), 195.39050840581, 0.001);

    BOOST_TEST((std::is_same<decltype(result.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_dist()), quantity
            <bu::multiply_typeof_helper<si::length, si::length>::type>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_dot_product)
{
    auto point1 = make_spherical_representation(3.0 * bud::degrees, 50.0 * bud::degrees, 40.0 * meters);
    auto point2 = make_spherical_representation(30.0 * bud::degrees, 45.0 * bud::degrees, 14.0 * meters);

    auto result = dot(point1, point2);

    BOOST_CHECK_CLOSE(result.value(), 524.807154, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result), quantity
        <bu::multiply_typeof_helper<si::length, si::length>::type>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_scalar_multiplication)
{
    auto point1 = make_spherical_representation(25.0 * bud::degrees, 30.0 * bud::degrees, 90.0*meter);

    auto result = boost::astronomy::coordinate::multiply(point1, 2);

    BOOST_CHECK_CLOSE(result.get_lat().value(), 25.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_lon().value(), 30.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_dist().value(), 180.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_unit_vector)
{
    auto point1 = make_spherical_representation(25.0 * bud::degrees, 30.0 * bud::degrees, 90.0*meter);

    auto result = boost::astronomy::coordinate::unit_vector(point1);

    BOOST_CHECK_CLOSE(result.get_lat().value(), 25.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_lon().value(), 30.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_dist().value(), 1, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_magnitude)
{
    auto point1 = make_spherical_representation(25.0 * bud::degrees, 36.0 * bud::degrees, 9.0 * meters);

    auto result = boost::astronomy::coordinate::magnitude(point1);

    BOOST_CHECK_CLOSE(result.value(), 9, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result), quantity<si::length>>::value));

}

BOOST_AUTO_TEST_CASE(spherical_representation_sum)
{
    auto point1 = make_spherical_representation(15.0 * bud::degrees, 30.0 * bud::degrees, 10.0 * meters);
    auto point2 = make_spherical_representation(30.0 * bud::degrees, 45.0 * bud::degrees, 20.0 * meters);

    auto result = boost::astronomy::coordinate::sum(point1, point2);

    BOOST_CHECK_CLOSE(result.get_lat().value(), 26.097805456, 0.001);
    BOOST_CHECK_CLOSE(result.get_lon().value(), 39.826115507, 0.001);
    BOOST_CHECK_CLOSE(result.get_dist().value(), 29.6909332103, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_difference)
{
    auto point1 = make_spherical_representation(15.0 * bud::degrees, 30.0 * bud::degrees, 10.0 * meters);
    auto point2 = make_spherical_representation(30.0 * bud::degrees, 45.0 * bud::degrees, 20.0 * meters);

    auto result = boost::astronomy::coordinate::difference(point1, point2);

    BOOST_CHECK_CLOSE(result.get_lat().value(), -142.089, 0.001);
    BOOST_CHECK_CLOSE(result.get_lon().value(), 120.245, 0.001);
    BOOST_CHECK_CLOSE(result.get_dist().value(), 10.8834, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_representation_mean)
{
    auto point1 = make_spherical_representation(15.0 * bud::degrees, 30.0 * bud::degrees, 10.0 * meter);
    auto point2 = make_spherical_representation(30.0 * bud::degrees, 45.0 * bud::degrees, 20.0 * meter);

    auto result = boost::astronomy::coordinate::mean(point1, point2);

    BOOST_CHECK_CLOSE(result.get_lat().value(), 26.097805456543, 0.001);
    BOOST_CHECK_CLOSE(result.get_lon().value(), 39.826115384099, 0.001);
    BOOST_CHECK_CLOSE(result.get_dist().value(), 14.845466643593, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(result.get_lat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_lon()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(result.get_dist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()
