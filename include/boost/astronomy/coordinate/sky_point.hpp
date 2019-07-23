#ifndef BOOST_ASTRONOMY_COORDINATE_SKY_POINT_HPP
#define BOOST_ASTRONOMY_COORDINATE_SKY_POINT_HPP

#include <string>
#include <type_traits>
#include <cmath>

#include <boost/units/io.hpp>
#include <boost/static_assert.hpp>

#include <boost/astronomy/coordinate/frame.hpp>
#include <boost/astronomy/detail/is_base_template_of.hpp>

namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

/*!sky_point is used to represent a point(coordinate) in the sky*/
template <typename CoordinateSystem>
struct sky_point
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_frame_of
            <boost::astronomy::coordinate::base_frame, CoordinateSystem>::value),
            "Template argument is expected to be a frame class");
    ///@endcond
    
protected:
    CoordinateSystem point;

public:
    typedef CoordinateSystem system;
    
    //constructors  

    //!default constructor
    sky_point() {}

    //!create point with the given coordinates
    sky_point(CoordinateSystem const& object) : point(object) {}

    //!create point with providing representation and differential class object
    template <typename Representation, typename Differential>
    sky_point
    (
        Representation const& representation_data,
        Differential const& differential_data
    )
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, Representation>::value),
            "argument type is expected to be a representation class");

        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, Differential>::value),
            "argument type is expected to be a differential class");

        point = CoordinateSystem(representation_data, differential_data);
    }

    //!create point with direct values of representation and differential
    sky_point
    (
        typename CoordinateSystem::representation::quantity1 const& lat,
        typename CoordinateSystem::representation::quantity2 const& lon,
        typename CoordinateSystem::representation::quantity3 const& distance,
        typename CoordinateSystem::differential::quantity1 const& pm_lat,
        typename CoordinateSystem::differential::quantity2 const& pm_lon_coslat,
        typename CoordinateSystem::differential::quantity3 const& radian_velocity
    )
    {
        point = CoordinateSystem(lat, lon, distance, pm_lat, pm_lon_coslat,
                    radian_velocity);
    }

    template<class OtherCoordinateSystem>
    sky_point(sky_point<OtherCoordinateSystem> const& object);

    //!constructing from direct value of representation
    sky_point
    (
        typename CoordinateSystem::representation::quantity1 const& lat,
        typename CoordinateSystem::representation::quantity2 const& lon,
        typename CoordinateSystem::representation::quantity3 const& distance
    )
    {
        point = CoordinateSystem(lat, lon, distance);
    }

    //constructing from name of object if available in the calatoge
    sky_point(std::string const& name);

    std::string get_constillation();

    sky_point<CoordinateSystem> from_name(std::string const& name);

    //!angular separation between two coordinates in radians
    bu::quantity<bu::si::plane_angle> separation(sky_point<CoordinateSystem> const& 
        object) const
    {
        return this->point.get_angular_separation(object.get_point());
    }

    //!returns positional angle in the radian
    bu::quantity<bu::si::plane_angle> positional_angle(sky_point<CoordinateSystem>
        const& object) const
    {
        auto p1 = make_spherical_representation(this->point.get_data());
        auto p2 = make_spherical_representation(object.get_point().get_data());

        auto diff = p2.get_lon() - p1.get_lon();

        double temp_p1 = static_cast<bu::quantity<bu::si::plane_angle,
            typename CoordinateSystem::representation::type>>(p1.get_lat()).value();
        double temp_p2 = static_cast<bu::quantity<bu::si::plane_angle,
            typename CoordinateSystem::representation::type>>(p2.get_lat()).value();
        double temp_diff = static_cast<bu::quantity<bu::si::plane_angle,
            typename CoordinateSystem::representation::type>>(diff).value();

        double coslat = std::cos(temp_p2);

        double x = std::sin(temp_p2) * std::cos(temp_p1) - 
            coslat * std::sin(temp_p1) * std::cos(temp_diff);
        double y = std::sin(temp_diff) * coslat;

        return bu::quantity<bu::si::plane_angle>::from_value(std::atan2(x, y));
    }

    //!returns true if both coordinate systems are same else returns false
    template<class OtherCoordinateSystem>
    bool is_equivalent_system(sky_point<OtherCoordinateSystem> const& object)
    {
        return std::is_same<CoordinateSystem, OtherCoordinateSystem>::value;
    }

    template<class OtherCoordinateSystem>
    sky_point<OtherCoordinateSystem> transform_to();

    //!returns the point
    CoordinateSystem get_point() const
    {
        return this->point;
    }

    //!sets the point with given object
    void set_point(CoordinateSystem const& otherPoint)
    {
        this->point = otherPoint;
    }

}; //sky_point

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_SKY_POINT_HPP

