
/*=============================================================================
  Copyright 2019-2020 Sarthak Singhal < sarthak2007 - singhalsarthak2007@gmail.com >

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE cartesian_differential_test

#include <boost/test/unit_test.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>
#include <boost/astronomy/coordinate/differential.hpp>

using namespace std;
using namespace boost::astronomy::coordinate;
using namespace boost::units::si;
using namespace boost::geometry;
using namespace boost::units;
namespace bud = boost::units::degree;

BOOST_AUTO_TEST_SUITE(cartesian_differential_constructor)

BOOST_AUTO_TEST_CASE(cartesian_differential_default_constructor)
{
    //using set functions
    cartesian_differential<double, quantity<si::velocity>, quantity<si::velocity>,
    quantity<si::velocity>> motion1;
    motion1.set_dx_dy_dz(2.5*meters/seconds, 91.0*meters/seconds, 12.0*meters/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 2.5, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 91.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 12, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_quantities_constructor)
{
    //checking construction from value
    auto motion1 = make_cartesian_differential
    (1.5*meters/seconds, 9.0*si::kilo*meters/seconds, 3.0*meters/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dy()),
        quantity<bu::divide_typeof_helper<decltype(si::kilo*meters), si::time>::type>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dz()), quantity<si::velocity>>::value));
    
    cartesian_differential<double, quantity<si::velocity>, quantity<si::velocity>, quantity<si::velocity>>
        motion2(1.5*meters/seconds, 9.0*meters/seconds, 3.0*meters/seconds);
    BOOST_CHECK_CLOSE(motion2.get_dx().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dy().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dz().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_copy_constructor)
{
    //checking construction from value
    auto motion1 = make_cartesian_differential
    (1.5*meters/seconds, 9.0*si::kilo*meters/seconds, 3.0*meters/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dy()),
        quantity<bu::divide_typeof_helper<decltype(si::kilo*meters), si::time>::type>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dz()), quantity<si::velocity>>::value));

    //copy constructor
    auto motion2 = make_cartesian_differential(motion1);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), motion2.get_dx().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), motion2.get_dy().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), motion2.get_dz().value(), 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dy()),
        quantity<bu::divide_typeof_helper<decltype(si::kilo*meters), si::time>::type>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dz()), quantity<si::velocity>>::value));

    cartesian_differential<double, quantity<bu::divide_typeof_helper<si::length, si::time>::
        type>, quantity<bu::divide_typeof_helper<decltype(si::kilo*meters), si::time>::type>, quantity
        <si::velocity>> motion3(motion1);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), motion3.get_dx().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), motion3.get_dy().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), motion3.get_dz().value(), 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion3.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion3.get_dy()),
        quantity<bu::divide_typeof_helper<decltype(si::kilo*meters), si::time>::type>>::value));
    BOOST_TEST((std::is_same<decltype(motion3.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_copy_constructor_with_different_units)
{
    //checking construction from value
    auto motion1 = make_cartesian_differential
    (1.5*meters/seconds, 9.0*si::kilo*meters/seconds, 3.0*meters/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dy()),
        quantity<bu::divide_typeof_helper<decltype(si::kilo*meters), si::time>::type>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dz()), quantity<si::velocity>>::value));

    // //Conversion from one unit type to other
    auto motion2 = make_cartesian_differential
    <double, quantity<si::velocity>, quantity<si::velocity>, quantity<si::velocity>>(motion1);
    BOOST_CHECK_CLOSE(motion2.get_dx().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dy().value(), 9000.0, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dz().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dz()), quantity<si::velocity>>::value));  
}

BOOST_AUTO_TEST_CASE(cartesian_differential_geometry_point_constructor)
{
    //constructing from boost::geometry::model::motion
    model::point<double, 3, cs::spherical<boost::geometry::degree>> model_point(30, 60, 1);
    auto motion1 = make_cartesian_differential
    <double,quantity<si::velocity>,quantity<si::velocity>,quantity<si::velocity>>(model_point);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 0.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dz()), quantity<si::velocity>>::value));

    cartesian_differential<double, quantity<si::velocity>, quantity<si::velocity>,
    quantity<si::velocity>> motion2(model_point);
    BOOST_CHECK_CLOSE(motion2.get_dx().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dy().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dz().value(), 0.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_conversion_from_spherical_differential)
{
    //constructing from spherical differential
    auto spherical_motion = make_spherical_differential
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meters/seconds);
    auto motion1 = make_cartesian_differential(spherical_motion);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 0.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_conversion_from_spherical_equatorial_differential)
{
    //constructing from spherical_equitorial differential
    auto spherical_equatorial_motion = make_spherical_equatorial_differential
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meters/seconds);
    auto motion2 = make_cartesian_differential(spherical_equatorial_motion);
    BOOST_CHECK_CLOSE(motion2.get_dx().value(), 0.433012646, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dy().value(), 0.250000097, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dz().value(), 0.866025405, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_conversion_from_spherical_coslat_differential)
{
    //constructing from spherical_coslat differential
    auto spherical_coslat_motion = make_spherical_coslat_differential
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meters/seconds);
    auto motion3 = make_cartesian_differential(spherical_coslat_motion);
    BOOST_CHECK_CLOSE(motion3.get_dx().value(), 0.8100222, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_dy().value(), 0.467666778, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_dz().value(), 0.353768031, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion3.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion3.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(motion3.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(cartesian_differential_operators)

BOOST_AUTO_TEST_CASE(cartesian_differential_addition_operator)
{
    auto motion1 = make_cartesian_differential
        (11.0*meters/seconds, 15.0*meters/seconds, 19.0*meters/seconds);
    auto motion2 = make_cartesian_differential
        (6.0*si::milli*meters/seconds, 10.0*si::centi*meters/seconds, 11.0*meters/seconds);

    auto sum = make_cartesian_differential(motion1 + motion2);

    BOOST_CHECK_CLOSE(sum.get_dx().value(), 11.006, 0.001);
    BOOST_CHECK_CLOSE(sum.get_dy().value(), 15.1, 0.001);
    BOOST_CHECK_CLOSE(sum.get_dz().value(), 30, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(sum.get_dx()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_dy()), quantity<si::velocity>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_dz()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(cartesian_differential_multiplication_operator)
{
    auto motion1 = make_cartesian_differential
        (3.0*meters/seconds, 9.0*meters/seconds, 6.0*meters/seconds);

    auto product = make_cartesian_differential
        (motion1 * quantity<si::time>(5*seconds));

    BOOST_CHECK_CLOSE(product.get_dx().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(product.get_dy().value(), 45.0, 0.001);
    BOOST_CHECK_CLOSE(product.get_dz().value(), 30.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(product.get_dx()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(product.get_dy()), quantity<si::length>>::value));
    BOOST_TEST((std::is_same<decltype(product.get_dz()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

