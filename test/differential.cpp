#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <boost/astronomy/coordinate/cartesian_differential.hpp>
#include <boost/astronomy/coordinate/spherical_differential.hpp>

using namespace std;
using namespace boost::astronomy::coordinate;
using namespace boost::units::si;
using namespace boost::units::degree;
using namespace boost::units::angle;
using namespace boost::geometry;
using namespace boost::units;

BOOST_AUTO_TEST_SUITE(differential_constructor)

BOOST_AUTO_TEST_CASE(cartesian)
{
    //checking construction from value
    auto motion1 = make_cartesian_differential
        (1.5*meters/seconds, 9.0*meters/seconds, 3.5*meters/seconds);

    BOOST_CHECK_CLOSE(motion1.get_dx().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), 3.5, 0.001);

    //copy constructor
    auto motion2 = make_cartesian_differential(motion1);
    BOOST_CHECK_CLOSE(motion1.get_dx().value(), motion2.get_dx().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dy().value(), motion2.get_dy().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dz().value(), motion2.get_dz().value(), 0.001);

    //constructing from boost::geometry::model::point
    model::point<double, 2, cs::spherical<boost::geometry::degree>> model_point(30, 60);
    auto motion3 = make_cartesian_differential
        <double, quantity<velocity>, quantity<velocity>, quantity<velocity>>(model_point);
    BOOST_CHECK_CLOSE(motion3.get_dx().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_dy().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_dz().value(), 0.5, 0.001);

    // constructing from another differential
    auto spherical_motion = make_spherical_differential
        (0.523599*radians, 1.047198*radians, 1.0*meters/seconds);
    auto motion4 = make_cartesian_differential(spherical_motion);
    BOOST_CHECK_CLOSE(motion4.get_dx().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(motion4.get_dy().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(motion4.get_dz().value(), 0.5, 0.001);
}

BOOST_AUTO_TEST_CASE(spherical)
{
    //checking construction from value
    auto motion1 = make_spherical_differential
        (45.0 * degrees, 18.0 * degrees, 3.5 * meters/seconds);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), 45.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon().value(), 18.0, 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), 3.5, 0.001);

    //copy constructor
    auto motion2 = make_spherical_differential(motion1);
    BOOST_CHECK_CLOSE(motion1.get_dlat().value(), motion2.get_dlat().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_dlon().value(), motion2.get_dlon().value(), 0.001);
    BOOST_CHECK_CLOSE(motion1.get_ddist().value(), motion2.get_ddist().value(), 0.001);

    //constructing from boost::geometry::model::point
    model::point<double, 3, boost::geometry::cs::cartesian> model_point(50, 20, 30);
    auto motion3 = make_spherical_differential(model_point);
    BOOST_CHECK_CLOSE(motion3.get_dlat().value(), 0.38050637711237, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_dlon().value(), 1.0625290806236, 0.001);
    BOOST_CHECK_CLOSE(motion3.get_ddist().value(), 61.64414002969, 0.001);

    //constructing from another representation
    auto cartesian_motion = make_cartesian_differential
        (60.0 * meters/seconds, 45.0 * meters/seconds, 85.0 * meters/seconds);
    auto motion4 = make_spherical_differential(cartesian_motion);
    BOOST_CHECK_CLOSE(motion4.get_dlat().value(), 0.64350110879328, 0.001);
    BOOST_CHECK_CLOSE(motion4.get_dlon().value(), 0.72297935340149, 0.001);
    BOOST_CHECK_CLOSE(motion4.get_ddist().value(), 113.35784048755, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

// BOOST_AUTO_TEST_SUITE(differential_functions)

// BOOST_AUTO_TEST_CASE(magnitude)
// {
//     cartesian_differential point1(25, 36, 90);

//     double result = point1.magnitude();

//     BOOST_CHECK_CLOSE(result, 100.1049449328054, 0.001);

// }

// BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(differential_operators)

BOOST_AUTO_TEST_CASE(addition)
{
    auto motion1 = make_cartesian_differential
        (11.0*meters/seconds, 15.0*meters/seconds, 19.0*meters/seconds);
    auto motion2 = make_cartesian_differential
        (6.0*meters/seconds, 10.0*meters/seconds, 11.0*meters/seconds);
    //point1 with motion1 and motion2 after one instance of time
    auto _1_instance = make_cartesian_differential(motion1 + motion2);

    BOOST_CHECK_CLOSE(_1_instance.get_dx().value(), 17.0, 0.001);
    BOOST_CHECK_CLOSE(_1_instance.get_dy().value(), 25, 0.001);
    BOOST_CHECK_CLOSE(_1_instance.get_dz().value(), 30.0, 0.001);
}

BOOST_AUTO_TEST_CASE(multiplication)
{
    auto motion1 = make_cartesian_differential
        (3.0*meters/seconds, 9.0*meters/seconds, 6.0*meters/seconds);

    //point1 with motion1 after 5 instance of time 
    auto _1_instance = make_cartesian_differential
        (motion1 * quantity<si::time>(5*seconds));

    BOOST_CHECK_CLOSE(_1_instance.get_dx().value(), 15.0, 0.001);
    BOOST_CHECK_CLOSE(_1_instance.get_dy().value(), 45.0, 0.001);
    BOOST_CHECK_CLOSE(_1_instance.get_dz().value(), 30.0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()