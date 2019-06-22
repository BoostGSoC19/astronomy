#ifndef BOOST_ASTRONOMY_COORDINATE_SPHERICAL_EQUATORIAL_DIFFERENTIAL_HPP
#define BOOST_ASTRONOMY_COORDINATE_SPHERICAL_EQUATORIAL_DIFFERENTIAL_HPP

#include <tuple>
#include <type_traits>

#include <boost/geometry/strategies/strategy_transform.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>
#include <boost/static_assert.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/systems/si/dimensionless.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_differential.hpp>
#include <boost/astronomy/coordinate/cartesian_differential.hpp>

namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Represents the differential in spherical equatorial representation
//!Uses three components to represent a differential (dlatitude, dlongitude, ddistance)

template
<
    typename CoordinateType = double,
    typename LatQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename LonQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename DistQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>
>
struct spherical_equatorial_differential : public base_differential
    <3, bg::cs::spherical_equatorial<radian>, CoordinateType>
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
    spherical_equatorial_differential() {}

    //!constructs object from provided components of differential
    spherical_equatorial_differential
    (
        LatQuantity const& dlat,
        LonQuantity const& dlon,
        DistQuantity const& ddistance
    )
    {
        this->set_dlat_dlon_ddist(dlat, dlon, ddistance);
    }

    //!constructs object from boost::geometry::model::point object
    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    spherical_equatorial_differential
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
        bg::transform(temp, this->diff);
    }

    //copy constructor
    spherical_equatorial_differential
    (
        spherical_equatorial_differential
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            DistQuantity
        > const& object
    )
    {
        this->diff = object.get_differential();
    }

    //!constructs object from any type of differential
    template <typename Differential>
    spherical_equatorial_differential(Differential const& other)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, Differential>::value),
            "No constructor found with given argument type");

        auto temp = make_spherical_equatorial_differential(other);
        bg::transform(temp.get_differential(), this->diff);

    }
    //! returns the (dlat, dlon, ddistance) in the form of tuple
    std::tuple<LatQuantity, LonQuantity, DistQuantity> get_dlat_dlon_ddist() const
    {
        return std::make_tuple(this->get_dlat(), this->get_dlon(), this->get_ddist());
    }

    //!returns the dlat component of differential
    LatQuantity get_dlat() const
    {
        return static_cast<LatQuantity>
            (
                bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                    (bg::get<0>(this->diff))
            );
    }

    //!returns the dlon component of differential
    LonQuantity get_dlon() const
    {
        return static_cast<LonQuantity>
            (
                bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                    (bg::get<1>(this->diff))
            );
    }

    //!returns the ddistance component of differential
    DistQuantity get_ddist() const
    {
        return DistQuantity::from_value(bg::get<2>(this->diff));
    }

    //!set value of (dlat, dlon, ddistance) in current object
    void set_dlat_dlon_ddist
    (
        LatQuantity const& dlat,
        LonQuantity const& dlon,
        DistQuantity const& ddistance
    )
    {
        this->set_dlat(dlat);
        this->set_dlon(dlon);
        this->set_ddist(ddistance);
    }

    //!set value of dlat component of differential
    void set_dlat(LatQuantity const& dlat)
    {
        bg::set<0>
            (
            this->diff,
            static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(dlat).value()
            );
    }

    //!set value of dlon component of differential
    void set_dlon(LonQuantity const& dlon)
    {
        bg::set<1>
            (
            this->diff,
            static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(dlon).value()
            );
    }

    //!set value of ddistance component of differential
    void set_ddist(DistQuantity const& ddistance)
    {
        bg::set<2>(this->diff, ddistance.value());
    }

    //!operator for addition of differential
    template
    <
        typename Addend
    >
    spherical_equatorial_differential
    <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
    >
    operator +(Addend const& addend) const
    {
        auto cartesian1 = make_cartesian_differential
            <CoordinateType, DistQuantity, DistQuantity, DistQuantity>(this->diff);
        auto cartesian2 = make_cartesian_differential(addend);

        auto temp = cartesian1 + cartesian2;

        spherical_equatorial_differential
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            DistQuantity
        > result = make_spherical_equatorial_differential(temp);

        return result;
    }

    //!operator for multiplication of differential
    template
    <
        typename OtherQuantity
    >
    auto operator *(OtherQuantity const& dt) const
    {
        
        spherical_equatorial_differential
            <CoordinateType, LatQuantity, LonQuantity, DistQuantity> temp(this->diff);

        spherical_equatorial_differential
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename DistQuantity::unit_type,
                typename OtherQuantity::unit_type>::type,
                CoordinateType
            >
        > product
        (
            temp.get_dlat(),
            temp.get_dlon(),
            temp.get_ddist() * dt
        );

        return product;
    }
}; //spherical_equatorial_differential

//!constructs object from provided components of differential
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
spherical_equatorial_differential
<
    CoordinateType,
    LatQuantity<Unit1, CoordinateType>,
    LonQuantity<Unit2, CoordinateType>,
    DistQuantity<Unit3, CoordinateType>
>
make_spherical_equatorial_differential
(
    LatQuantity<Unit1, CoordinateType> const& dlat,
    LonQuantity<Unit2, CoordinateType> const& dlon,
    DistQuantity<Unit3, CoordinateType> const& ddist
)
{
    return spherical_equatorial_differential
        <
            CoordinateType,
            LatQuantity<Unit1, CoordinateType>,
            LonQuantity<Unit2, CoordinateType>,
            DistQuantity<Unit3, CoordinateType>
        >(dlat, dlon, ddist);
}

//!constructs object from provided components of differential with different units
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
spherical_equatorial_differential
<
    ReturnCoordinateType,
    ReturnLatQuantity,
    ReturnLonQuantity,
    ReturnDistQuantity
>
make_spherical_equatorial_differential
(
    spherical_equatorial_differential
    <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
    > const& other
)
{
    return make_spherical_equatorial_differential(
        static_cast<ReturnLatQuantity>(other.get_dlat()),
        static_cast<ReturnLonQuantity>(other.get_dlon()),
        static_cast<ReturnDistQuantity>(other.get_ddist())
    );
}

//!constructs object from provided differential
template
<
    typename CoordinateType,
    typename LatQuantity,
    typename LonQuantity,
    typename DistQuantity
>
spherical_equatorial_differential
<
    CoordinateType,
    LatQuantity,
    LonQuantity,
    DistQuantity
>
make_spherical_equatorial_differential
(
    spherical_equatorial_differential
    <
        CoordinateType,
        LatQuantity,
        LonQuantity,
        DistQuantity
    > const& other
)
{
    return spherical_equatorial_differential
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            DistQuantity
        >(other);
}

//!constructs object from boost::geometry::model::point object
template
<
    typename CoordinateType = double,
    typename LatQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename LonQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
    typename DistQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    std::size_t OtherDimensionCount,
    typename OtherCoordinateSystem,
    typename OtherCoordinateType
>
spherical_equatorial_differential
<
    CoordinateType,
    LatQuantity,
    LonQuantity,
    DistQuantity
>
make_spherical_equatorial_differential
(
    bg::model::point
    <
        OtherCoordinateType,
        OtherDimensionCount,
        OtherCoordinateSystem
    > const& pointObject
)
{
    return spherical_equatorial_differential
        <
            CoordinateType,
            LatQuantity,
            LonQuantity,
            DistQuantity
        >(pointObject);
}

//!constructs object from any type of differential
template 
<
    typename OtherDifferential
>
auto make_spherical_equatorial_differential
(
    OtherDifferential const& other
)
{
    auto temp = make_cartesian_differential(other);
    typedef decltype(temp) cartesian_type;

    bg::model::point
        <
            typename cartesian_type::type,
            3,
            bg::cs::cartesian
        > tempPoint;

    bg::set<0>(tempPoint, temp.get_dx().value());
    bg::set<1>(tempPoint, static_cast<typename cartesian_type::quantity1>
        (temp.get_dy()).value());
    bg::set<2>(tempPoint, static_cast<typename cartesian_type::quantity1>
        (temp.get_dz()).value());

    bg::model::point<typename cartesian_type::type, 3, bg::cs::spherical_equatorial
        <radian>> result;
    bg::transform(tempPoint, result);

    return spherical_equatorial_differential
        <
            typename cartesian_type::type,
            bu::quantity<bu::si::plane_angle, typename cartesian_type::type>,
            bu::quantity<bu::si::plane_angle, typename cartesian_type::type>,
            typename cartesian_type::quantity1
        >(result);
}

}}}//namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_SPHERICAL_EQUATORIAL_DIFFERENTIAL_HPP


