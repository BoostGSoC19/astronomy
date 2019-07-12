#ifndef BOOST_ASTRONOMY_COORDINATE_BASE_COORDINATE_SYSTEM_HPP
#define BOOST_ASTRONOMY_COORDINATE_BASE_COORDINATE_SYSTEM_HPP

#include <tuple>
#include <cmath>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>
#include <boost/static_assert.hpp>

#include <boost/astronomy/coordinate/representation.hpp>
#include <boost/astronomy/coordinate/differential.hpp>
#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>


namespace boost { namespace astronomy { namespace coordinate {

typedef boost::geometry::degree degree;
typedef boost::geometry::radian radian;

namespace bu = boost::units;
namespace bg = boost::geometry;

template <typename Representation, typename Differential>
struct base_frame
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
        <boost::astronomy::coordinate::base_representation, Representation>::value),
        "First template argument is expected to be a representation class");
    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
        <boost::astronomy::coordinate::base_differential, Differential>::value),
        "Second template argument is expected to be a differential class");
    ///@endcond

protected:
    Representation data;
    Differential motion;

public:
    typedef Representation representation;
    typedef Differential differential;

    //!returns 2d propermotion
    std::tuple
    <
        typename Differential::quantity1,
        typename Differential::quantity2
    > get_proper_motion() const
    {
        return std::make_tuple
        (
            Differential::quantity1::from_value(bg::get<0>(motion.get_differential())),
            Differential::quantity2::from_value(bg::get<1>(motion.get_differential())),
        );
    }

    //!returns radial velocity (line of sight velocity) of the object
    typename Differential::quantity3 get_radial_velocity() const
    {
        return Differential::quantity3::from_value(bg::get<2>(motion.get_differential()));
    }

    /*!returns differential data in form of boost::geometry::model::point 
    having components (pm_lat, pm_lon, radial_velocity) including cos(lat) component*/
    Differential get_differential() const
    {
        return motion;
    }

    //!set differential for the motion of the object
    void set_differential(Differential const& other)
    {
        this->motion = other;
    }

    /*!returns coordinate data in form of boost::geometry::model::point
    having components (lat, lon, distance)*/
    Representation get_data() const
    {
        return data;
    }

    //!returns data in specified subclass of base_representation
    //template <typename ReturnType>
    //ReturnType represent_as() const
    //{
    //    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
    //        <boost::astronomy::coordinate::base_representation, ReturnType>::value), 
    //        "return type is expected to be a representation class");

    //    return ReturnType(data);
    //}

    //!angular separation between two coordinates in radians
    bu::quantity<bu::si::dimensionless> separation(base_frame const& other) const
    {
        return bu::quantity<bu::si::dimensionless>::from_value(
            std::acos(dot(this->data, other.get_data()))
        );
    }
                
};
}}} //namespace boost::astronomy::coordinate
#endif  // !BOOST_ASTRONOMY_COORDINATE_BASE_COORDINATE_SYSTEM_HPP
