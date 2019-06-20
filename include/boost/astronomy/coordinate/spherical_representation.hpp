#ifndef BOOST_ASTRONOMY_COORDINATE_SPHERICAL_REPRESENTATION_HPP
#define BOOST_ASTRONOMY_COORDINATE_SPHERICAL_REPRESENTATION_HPP


#include <tuple>
#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>
#include <boost/units/physical_dimensions/plane_angle.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/dimensionless.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_representation.hpp>
#include <boost/astronomy/coordinate/cartesian_representation.hpp>


namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;


//!Represents the coordinate in spherical representation
//!Uses three components to represent a point/vector (latitude, longitude, distance)
/*!
\brief Spherical (polar) coordinate system, in degree or in radian
\details Defines the spherical coordinate system where points are
    defined in two angles
    and an optional radius usually known as r, theta, phi
\par Coordinates:
- coordinate 0:
    0 <= phi < 2pi is the angle between the positive x-axis and the
        line from the origin to the P projected onto the xy-plane.
- coordinate 1:
    0 <= theta <= pi is the angle between the positive z-axis and the
        line formed between the origin and P.
- coordinate 2 (if specified):
    r >= 0 is the distance from the origin to a given point P.

\see http://en.wikipedia.org/wiki/Spherical_coordinates
\ingroup cs
*/
template
<
    typename CoordinateType = double,
    typename LatQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename LonQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename DistQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>
>
struct spherical_representation : public base_representation
    <3, bg::cs::spherical<radian>, CoordinateType>
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG(
        ((std::is_same<typename bu::get_dimension<LatQuantity>::type,
            bu::plane_angle_dimension>::value) &&
            (std::is_same<typename bu::get_dimension<LonQuantity>::type,
            bu::plane_angle_dimension>::value)),
        "Latitude and Longitude must be of plane angle type");
    BOOST_STATIC_ASSERT_MSG((std::is_floating_point<CoordinateType>::value),
        "CoordinateType must be a floating-point type");
    ///@endcond

public:
    typedef LatQuantity quantity1;
    typedef LonQuantity quantity2;
    typedef DistQuantity quantity3;

    //default constructor no initialization
    spherical_representation() {}

    //!constructs object from provided value of coordinates (latitude, longitude, distance)
    spherical_representation
    (
        LatQuantity const& lat,
        LonQuantity const& lon,
        DistQuantity const& distance
    )
    {
        this->set_lat_lon_dist(lat, lon, distance);
    }

    //!constructs object from boost::geometry::model::point object
    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    spherical_representation
    (
        bg::model::point
        <
            OtherCoordinateType,
            OtherDimensionCount,
            OtherCoordinateSystem
        > const& pointObject
    )
    {
        bg::model::point<OtherCoordinateType, 3, bg::cs::cartesian> temp;
        bg::transform(pointObject, temp);
        bg::transform(temp, this->point);
    }

    //copy constructor
    spherical_representation
    (
        spherical_representation
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            DistQuantity
        > const& other)
    {
        this->point = other.get_point();
    }

    //!constructs object from any type of representation
    template <typename Representation>
    spherical_representation(Representation const& other)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, Representation>::value),
            "No constructor found with given argument type");

        auto temp = make_spherical_representation(other);
        bg::transform(temp.get_point(), this->point);
    }

    //! returns the (lat, lon, distance) in the form of tuple
    std::tuple<LatQuantity, LonQuantity, DistQuantity> get_lat_lon_dist() const
    {
        return std::make_tuple(this->get_lat(), this->get_lon(), this->get_dist());
    }

    //!returns the lat component of point
    LatQuantity get_lat() const
    {
        return static_cast<LatQuantity>
            (
                bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                    (bg::get<0>(this->point))
            );
    }

    //!returns the lon component of point
    LonQuantity get_lon() const
    {
        return static_cast<LonQuantity>
            (
                bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                    (bg::get<1>(this->point))
            );
    }

    //!returns the distance component of point
    DistQuantity get_dist() const
    {
        return DistQuantity::from_value(bg::get<2>(this->point));
    }

    //!set value of (lat, lon, distance) in current object
    void set_lat_lon_dist
    (
        LatQuantity const& lat,
        LonQuantity const& lon,
        DistQuantity const& distance
    )
    {
        this->set_lat(lat);
        this->set_lon(lon);
        this->set_dist(distance);
    }

    //!set value of lat component of point
    void set_lat(LatQuantity const& lat)
    {
        bg::set<0>
            (
            this->point,
            static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(lat).value()
            );
    }

    //!set value of lon component of point
    void set_lon(LonQuantity const& lon)
    {
        bg::set<1>
            (
            this->point,
            static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(lon).value()
            );
    }

    //!set value of distance component of point
    void set_dist(DistQuantity const& distance)
    {
        bg::set<2>(this->point, distance.value());
    }

    template<typename Addend>
    spherical_representation
    <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
    > 
    operator +(Addend const& addend) const
    {


        auto cartesian1 = make_cartesian_representation
            <CoordinateType, DistQuantity, DistQuantity, DistQuantity>(this->point);
        auto cartesian2 = make_cartesian_representation(addend);

        auto temp = cartesian1 + cartesian2;

        spherical_representation
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            DistQuantity
        > result = make_spherical_differential(temp);

        return result;
    }

}; //spherical_representation


//!Constructs object from provided quantities
//!Each quantity can have different units but with same datatypes
template
<
    typename CoordinateType,
    template <typename Unit1, typename CoordinateType_> class LatQuantity,
    template <typename Unit2, typename CoordinateType_> class LonQuantity,
    template <typename Unit3, typename CoordinateType_> class DistQuantity,
    typename Unit1,
    typename Unit2,
    typename Unit3
>
spherical_representation
<
    CoordinateType,
    LatQuantity<Unit1, CoordinateType>,
    LonQuantity<Unit2, CoordinateType>,
    DistQuantity<Unit3, CoordinateType>
> make_spherical_representation
(
    LatQuantity<Unit1, CoordinateType> const& lat,
    LonQuantity<Unit2, CoordinateType> const& lon,
    DistQuantity<Unit3, CoordinateType> const& dist
)
{
    return spherical_representation
        <
            CoordinateType,
            LatQuantity<Unit1, CoordinateType>,
            LonQuantity<Unit2, CoordinateType>,
            DistQuantity<Unit3, CoordinateType>
        >(lat, lon, dist);
}

//!Convert current quantities of spherical_representation to new quantities. 
template
<
    typename ReturnCoordinateType,
    typename ReturnLatQuantity,
    typename ReturnLonQuantity,
    typename ReturnDistQuantity,
    typename CoordinateType,
    typename LatQuantity,
    typename LonQuantity,
    typename DistQuantity
>
spherical_representation
<
    ReturnCoordinateType,
    ReturnLatQuantity,
    ReturnLonQuantity,
    ReturnDistQuantity
>
make_spherical_representation
(
    spherical_representation
    <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
    > const& other
)
{
    return make_cartesian_representation(
        static_cast<ReturnLatQuantity>(other.get_x()),
        static_cast<ReturnLonQuantity>(other.get_y()),
        static_cast<ReturnDistQuantity>(other.get_z())
    );
}

//!Create copy of spherical_representation
template
<
    typename CoordinateType,
    typename LatQuantity,
    typename LonQuantity,
    typename DistQuantity
>
spherical_representation<CoordinateType, LatQuantity, LonQuantity, DistQuantity>
make_spherical_representation
(
    spherical_representation
    <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
    > const& other
)
{
    return spherical_representation
        <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
        >(other);
}

//!Create spherical_representation from boost::geometry::point
//!Quantity types are to be specifed explicitly or angle is considered to be in radians
//!distance is taken dimensionless by default if not specified 
template
<
    typename CoordinateType=double,
    typename LatQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename LonQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename DistQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    std::size_t OtherDimensionCount,
    typename OtherCoordinateSystem,
    typename OtherCoordinateType
>
spherical_representation<CoordinateType, LatQuantity, LonQuantity, DistQuantity>
make_spherical_representation
(
    bg::model::point
    <
    OtherCoordinateType,
    OtherDimensionCount,
    OtherCoordinateSystem
    > const& pointObject)
{
    return spherical_representation
        <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
        >(pointObject);
}

//!Create cartesian_representation from other type of representations
template
<
    typename CoordinateType,
    typename XQuantity,
    typename YQuantity,
    typename ZQuantity
>
spherical_representation
<
    CoordinateType,
    bu::quantity<bu::si::plane_angle, CoordinateType>,
    bu::quantity<bu::si::plane_angle, CoordinateType>,
    XQuantity
> make_spherical_representation
(cartesian_representation<CoordinateType, XQuantity, YQuantity, ZQuantity> const& other)
{
    typedef cartesian_representation
        <
        CoordinateType,
        XQuantity,
        YQuantity,
        ZQuantity
        > cartesian_type;

    bg::model::point<typename cartesian_type::type, 3, bg::cs::cartesian> tempPoint;
    bg::model::point<typename cartesian_type::type, 3, bg::cs::spherical<radian>> result;

    bg::set<0>(tempPoint, other.get_x().value());
    bg::set<1>
    (
        tempPoint,
        static_cast<typename cartesian_type::quantity1>
        (other.get_y()).value()
    );
    bg::set<2>
    (
        tempPoint,
        static_cast<typename cartesian_type::quantity1>
        (other.get_z()).value()
    );

    bg::transform(tempPoint, result);

    return spherical_representation
        <
            typename cartesian_type::type,
            bu::quantity<bu::si::plane_angle, typename cartesian_type::type>,
            bu::quantity<bu::si::plane_angle, typename cartesian_type::type>,
            typename cartesian_type::quantity1
        >(result);
}

//!Create spherical_equatorial_representation from other type of representations
//A general implementation is used to avoid including spherical_equatorial_representation.hpp
//error: using spherical_equatorial_representation before it's declaration
template<typename OtherRepresentation>
spherical_representation
<
    typename OtherRepresentation::type,
    typename OtherRepresentation::quantity1,
    typename OtherRepresentation::quantity2,
    typename OtherRepresentation::quantity3
>
make_spherical_representation(OtherRepresentation const& other)
{
    return spherical_representation
        <
        typename OtherRepresentation::type,
        typename OtherRepresentation::quantity1,
        typename OtherRepresentation::quantity2,
        typename OtherRepresentation::quantity3
        >(other.get_point());
}

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_SPHERICAL_REPRESENTATION_HPP
