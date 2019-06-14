#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>


using namespace std;
using namespace boost::astronomy::coordinate;
using namespace boost::units::si;
using namespace boost::geometry;
using namespace boost::units;


BOOST_AUTO_TEST_SUITE(representation_constructor)

BOOST_AUTO_TEST_CASE(cartesian)
{
    //checking construction from value
    auto point1 = make_cartesian_representation
    (1.5*meter, 9.0*kilo*meter, 3.0*centi*meter);
    BOOST_CHECK_CLOSE(point1.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), 9.0, 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), 3, 0.001);

    //copy constructor
    auto point2 = make_cartesian_representation(point1);
    BOOST_CHECK_CLOSE(point1.get_x().value(), point2.get_x().value(), 0.001);
    BOOST_CHECK_CLOSE(point1.get_y().value(), point2.get_y().value(), 0.001);
    BOOST_CHECK_CLOSE(point1.get_z().value(), point2.get_z().value(), 0.001);

    //constructing from boost::geometry::model::point
    model::point<double, 2, cs::spherical<degree>> model_point(30, 60);
    auto point3 = make_cartesian_representation
        <double, quantity<length>, quantity<length>, quantity<length>>(model_point);
    BOOST_CHECK_CLOSE(point3.get_x().value(), 0.75, 0.001);
    BOOST_CHECK_CLOSE(point3.get_y().value(), 0.4330127019, 0.001);
    BOOST_CHECK_CLOSE(point3.get_z().value(), 0.5, 0.001);

    //Conversion from one unit type to other
    auto point4 = make_cartesian_representation
        <double, quantity<length>, quantity<length>, quantity<length>>(point1);
    BOOST_CHECK_CLOSE(point4.get_x().value(), 1.5, 0.001);
    BOOST_CHECK_CLOSE(point4.get_y().value(), 9000.0, 0.001);
    BOOST_CHECK_CLOSE(point4.get_z().value(), 0.03, 0.001);

    ////constructing from another representation
    //spherical_representation<radian> spherical_point(0.523599, 1.047198, 1);
    //cartesian_representation point4(spherical_point);
    //BOOST_CHECK_CLOSE(point4.get_x(), 0.75, 0.001);
    //BOOST_CHECK_CLOSE(point4.get_y(), 0.4330127019, 0.001);
    //BOOST_CHECK_CLOSE(point4.get_z(), 0.5, 0.001);
}

//BOOST_AUTO_TEST_CASE(spherical)
//{
//    //checking construction from value
//    spherical_representation<degree> point1(45.0, 18, 3.5);
//    BOOST_CHECK_CLOSE(point1.get_lat(), 45.0, 0.001);
//    BOOST_CHECK_CLOSE(point1.get_lon(), 18.0, 0.001);
//    BOOST_CHECK_CLOSE(point1.get_dist(), 3.5, 0.001);
//
//    //copy constructor
//    spherical_representation<degree> point2(point1);
//    BOOST_CHECK_CLOSE(point1.get_lat(), point2.get_lat(), 0.001);
//    BOOST_CHECK_CLOSE(point1.get_lon(), point2.get_lon(), 0.001);
//    BOOST_CHECK_CLOSE(point1.get_dist(), point2.get_dist(), 0.001);
//
//    //constructing from boost::geometry::model::point
//    boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> model_point(50, 20, 30);
//    spherical_representation<radian> point3(model_point);
//    BOOST_CHECK_CLOSE(point3.get_lat(), 0.38050637711237, 0.001);
//    BOOST_CHECK_CLOSE(point3.get_lon(), 1.0625290806236, 0.001);
//    BOOST_CHECK_CLOSE(point3.get_dist(), 61.64414002969, 0.001);
//
//    //constructing from another representation
//    cartesian_representation cartesian_point(60, 45, 85);
//    spherical_representation<degree> point4(cartesian_point);
//    BOOST_CHECK_CLOSE(point4.get_lat(), 36.869897645844, 0.001);
//    BOOST_CHECK_CLOSE(point4.get_lon(), 41.423665625003, 0.001);
//    BOOST_CHECK_CLOSE(point4.get_dist(), 113.35784048755, 0.001);
//}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(representation_functions)

BOOST_AUTO_TEST_CASE(cross_product)
{
    auto point1 = make_cartesian_representation(3.0 * meters, 5.0 * kilo *meters, 4.0 * mega * meters);
    auto point2 = make_cartesian_representation(3.0 * milli * meters, 5.0 * centi * meters, 4.0 * meters);
    //spherical_representation<degree> point2(45, 45, 3);

    auto result = cross(point1, point2);

    BOOST_CHECK_CLOSE(result.get_x().value(), -180.0, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 11.988, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), -1485.0, 0.001);
}

BOOST_AUTO_TEST_CASE(dot_product)
{
    auto point1 = make_cartesian_representation(3.0 * meters, 5.0 * kilo *meters, 4.0 * mega * meters);
    auto point2 = make_cartesian_representation(3.0 * milli * meters, 5.0 * centi * meters, 4.0 * meters);

    auto result = dot(point1, point2);

    BOOST_CHECK_CLOSE(result.value(), 16000250009.0, 0.001);
}

BOOST_AUTO_TEST_CASE(unit_vector)
{
    auto point1 = make_cartesian_representation(25.0*meter, 36.0*meter, 90.0*meter);

    auto result = boost::astronomy::coordinate::unit_vector(point1);

    BOOST_CHECK_CLOSE(result.get_x().value(), 0.2497379127153113, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 0.3596225943100483, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 0.8990564857751207, 0.001);
}

BOOST_AUTO_TEST_CASE(magnitude)
{
    auto point1 = make_cartesian_representation(25.0 * meter, 3600.0 * centi*meter, 90.0 * meter);

    auto result = boost::astronomy::coordinate::magnitude(point1);

    BOOST_CHECK_CLOSE(result.value(), 100.1049449328054, 0.001);

}

BOOST_AUTO_TEST_CASE(sum)
{
    auto point1 = make_cartesian_representation(10.0 * meter, 20.0 * kilo * meters, 30.0 * meter);
    auto point2 = make_cartesian_representation(50.0 * centi * meter, 60.0 * meter, 30.0 * meter);

    auto result = point1 + point2;

    BOOST_CHECK_CLOSE(result.get_x().value(), 10.5, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 20.06, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 60, 0.001);
}

BOOST_AUTO_TEST_CASE(mean)
{
    auto point1 = make_cartesian_representation(10.0 * meter, 20.0 * kilo * meters, 30.0 * meter);
    auto point2 = make_cartesian_representation(50.0 * centi * meter, 60.0 * meter, 30.0 * meter);

    auto result = boost::astronomy::coordinate::mean(point1, point2);

    BOOST_CHECK_CLOSE(result.get_x().value(), 5.25, 0.001);
    BOOST_CHECK_CLOSE(result.get_y().value(), 10.03, 0.001);
    BOOST_CHECK_CLOSE(result.get_z().value(), 30.0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
