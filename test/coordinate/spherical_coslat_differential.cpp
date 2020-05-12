#define BOOST_TEST_MODULE spherical_coslat_differential_test

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

BOOST_AUTO_TEST_SUITE(spherical_coslat_differential_constructors)

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_default_constructor)
{
    //using set functions
    spherical_coslat_differential<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::velocity>> motion1;
    motion1.set_dlat_dlon_coslat_ddist(45.0 * bud::degrees, 18.0 * bud::degrees, 3.5 * meters/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 45.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), 18.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 3.5, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_quantities_constructor)
{
    //checking construction from value
    auto motion1 = make_spherical_coslat_differential
    (15.0 * bud::degrees, 39.0 * bud::degrees, 3.0*si::centi*meter/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), 39.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 3.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_ddist()),
        quantity<bu::divide_typeof_helper<decltype(si::centi*meters), si::time>::type>>::value));

    spherical_coslat_differential<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::velocity>> motion2(1.5 * bud::degrees, 9.0 * bud::degrees, 3.0 * meter/seconds);
    BOOST_CHECK_CLOSE(motion2.get_dlat().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dlon_coslat().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_ddist().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_copy_constructor)
{
    //checking construction from value
    auto motion1 = make_spherical_coslat_differential
    (15.0 * bud::degrees, 30.0 * bud::degrees, 3.0*si::centi*meter/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), 30.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_ddist()),
        quantity<bu::divide_typeof_helper<decltype(si::centi*meters), si::time>::type>>::value));

    //copy constructor
    auto motion2 = make_spherical_coslat_differential(motion1);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), motion2.get_dlat().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), motion2.get_dlon_coslat().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), motion2.get_ddist().value(), 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_ddist()),
        quantity<bu::divide_typeof_helper<decltype(si::centi*meters), si::time>::type>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_copy_constructor_with_different_units)
{
    //checking construction from value
    auto motion1 = make_spherical_coslat_differential
    (15.0 * bud::degrees, 10.0 * bud::degrees, 3.0*si::centi*meter/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), 10.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 3, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_ddist()),
        quantity<bu::divide_typeof_helper<decltype(si::centi*meters), si::time>::type>>::value));

    //Conversion from one unit type to other
    auto motion2 = make_spherical_coslat_differential
    <double, quantity<bud::plane_angle>, quantity<bud::plane_angle>, quantity<si::velocity>>(motion1);
    BOOST_CHECK_CLOSE(motion2.get_dlat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dlon_coslat().value(), 10.0, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_ddist().value(), 0.03, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_geometry_point_constructor)
{
    //constructing from boost::geometry::model::motion
    model::point<double, 3, cs::cartesian> model_point(30, 60, 10);
    auto motion1 = make_spherical_coslat_differential
    <double,quantity<bud::plane_angle>,quantity<bud::plane_angle>,quantity<si::velocity>>(model_point);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 63.434948822922, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), 81.521286852914, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 67.823299831253, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_ddist()), quantity<si::velocity>>::value));

    spherical_coslat_differential<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::velocity>> motion2(model_point);
    BOOST_CHECK_CLOSE(motion2.get_dlat().value(), 63.434948822922, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dlon_coslat().value(), 81.521286852914, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_ddist().value(), 67.823299831253, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_ddist()), quantity<si::velocity>>::value));
} 
    
BOOST_AUTO_TEST_CASE(spherical_coslat_differential_conversion_from_cartesian_differential)
{   
    //constructing from spherical differential
    auto cartesian_motion = make_cartesian_differential(20.0 * meters/seconds,
        60.0 * meters/seconds, 1.0 * meter/seconds);
    auto motion1 = make_spherical_coslat_differential(cartesian_motion);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 1.2490457723983, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon_coslat().value(), 0.49173, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 63.253458403474, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion1.get_dlat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_dlon_coslat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion1.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_conversion_from_spherical_equatorial_differential)
{   
    //constructing from spherical_equitorial differential
    auto spherical_equatorial_motion = make_spherical_equatorial_differential
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meter/seconds);
    auto motion2 = make_spherical_coslat_differential(spherical_equatorial_motion);
    BOOST_CHECK_CLOSE(motion2.get_dlat().value(), 0.523599, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_dlon_coslat().value(), 0.45345, 0.001);
    BOOST_CHECK_CLOSE(motion2.get_ddist().value(), 1.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion2.get_dlat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_dlon_coslat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion2.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_conversion_from_spherical_differential)
{
    //constructing from spherical_coslat differential
    auto spherical_motion = make_spherical_differential
    (0.523599 * si::radian, 60.0 * bud::degrees, 1.0 * meters/seconds);
    auto motion3 = make_spherical_coslat_differential(spherical_motion);
    BOOST_CHECK_CLOSE(motion3.get_dlat().value(), 0.523599, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_dlon_coslat().value(), 0.9069, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_ddist().value(), 1, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(motion3.get_dlat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion3.get_dlon_coslat()), quantity<si::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(motion3.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(spherical_coslat_differential_operators)

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_addition_operator)
{
    auto motion1 = make_spherical_coslat_differential(15.0 * bud::degrees, 30.0 * bud::degrees, 10.0 * meters/seconds);
    auto motion2 = make_spherical_coslat_differential(30.0 * bud::degrees, 45.0 * bud::degrees, 20.0 * meters/seconds);

    auto sum = make_spherical_coslat_differential(motion1 + motion2);

    BOOST_CHECK_CLOSE(sum.get_dlat().value(), 26.4022, 0.001);
    BOOST_CHECK_CLOSE(sum.get_dlon_coslat().value(), 39.86, 0.001);
    BOOST_CHECK_CLOSE(sum.get_ddist().value(), 29.4212, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(sum.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(sum.get_ddist()), quantity<si::velocity>>::value));
}

BOOST_AUTO_TEST_CASE(spherical_coslat_differential_multiplication_operator)
{
    auto motion1 = make_spherical_coslat_differential(15.0 * bud::degrees, 30.0 * bud::degrees, 10.0 * meters/seconds);

    spherical_coslat_differential<double, quantity<bud::plane_angle>, quantity<bud::plane_angle>,
    quantity<si::length>> product = make_spherical_coslat_differential(motion1 * quantity<si::time>(5.0*seconds));

    BOOST_CHECK_CLOSE(product.get_dlat().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(product.get_dlon_coslat().value(), 30.0, 0.001);
    BOOST_CHECK_CLOSE(product.get_ddist().value(), 50.0, 0.001);

    //checking whether quantity stored is as expected or not
    BOOST_TEST((std::is_same<decltype(product.get_dlat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(product.get_dlon_coslat()), quantity<bud::plane_angle>>::value));
    BOOST_TEST((std::is_same<decltype(product.get_ddist()), quantity<si::length>>::value));
}

BOOST_AUTO_TEST_SUITE_END()