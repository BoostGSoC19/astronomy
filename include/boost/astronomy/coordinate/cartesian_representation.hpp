#ifndef BOOST_ASTRONOMY_COORDINATE_CARTESIAN_REPRESENTATION_HPP
#define BOOST_ASTRONOMY_COORDINATE_CARTESIAN_REPRESENTATION_HPP


#include <tuple>
#include <type_traits>

#include <boost/static_assert.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/strategies/strategy_transform.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/systems/si/dimensionless.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_representation.hpp>


namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Represents the coordinate in cartesian representation
//!Uses three components to represent a point/vector (x, y, z)
template
<
    typename CoordinateType=double,
    typename XQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    typename YQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    typename ZQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>
>
struct cartesian_representation: base_representation<3, bg::cs::cartesian, CoordinateType>
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG(
        ((std::is_same<typename bu::get_dimension<XQuantity>::type,
        typename bu::get_dimension<YQuantity>::type>::value) &&
        (std::is_same<typename bu::get_dimension<YQuantity>::type,
        typename bu::get_dimension<ZQuantity>::type>::value)),
        "All components must have same dimensions");
    ///@endcond

public:

    typedef XQuantity quantity1;
    typedef YQuantity quantity2;
    typedef ZQuantity quantity3;
    //typename cartesian_representation
    //    <CoordinateType, XQuantity, YQuantity, ZQuantity> this_type;

    //default constructoer no initialization
    cartesian_representation(){}

    //!Constructs object from provided quantities
    //!Each quantity can have different units but with same datatypes
    cartesian_representation(XQuantity const& x, YQuantity const& y, ZQuantity const& z)
    {
        this->set_x_y_z(x, y, z);
    }

    //!Constructs object from boost::geometry::model::point object
    //!Quantities have to be specified explicitly
    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    cartesian_representation
    (
        bg::model::point
        <
            OtherCoordinateType,
            OtherDimensionCount,
            OtherCoordinateSystem
        > const& pointObject
    )
    {
        bg::transform(pointObject, this->point);

    }

    //!Copy constructor
    cartesian_representation
    (
        cartesian_representation
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        > const& object
    )
    {
        this->point = object.get_point();
    }

    //!Constructs object from any type of representation
    //!Quantities have to be specified explicitly
    template <typename Representation>
    cartesian_representation(Representation const& other)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, Representation>::value),
            "No constructor found with given argument type");

        bg::transform(other.get_point(), this->point);
    }

    //! Returns the (x, y, z) in the form of tuple
    std::tuple<XQuantity, YQuantity, ZQuantity> get_x_y_z() const
    {
        return std::make_tuple(this->get_x(), this->get_y(), this->get_z());
    }

    //!Returns the x quantity component of point
    XQuantity get_x() const
    {
        return XQuantity::from_value(bg::get<0>(this->point));
    }

    //!Returns the y quantity component of point
    YQuantity get_y() const
    {
        return YQuantity::from_value(bg::get<1>(this->point));
    }

    //!Returns the z quantity component of point
    ZQuantity get_z() const
    {
        return ZQuantity::from_value(bg::get<2>(this->point));
    }

    //!Set value of (x, y, z) in current object
    void set_x_y_z(XQuantity const& x, YQuantity const& y, ZQuantity const& z)
    {
        this->set_x(x);
        this->set_y(y);
        this->set_z(z);
    }

    //!Set value of x quantity component of point
    void set_x(XQuantity const& x)
    {
        bg::set<0>(this->point, x.value());
    }

    //!Set value of y quantity component of point
    void set_y(YQuantity const& y)
    {
        bg::set<1>(this->point, y.value());
    }

    //!Set value of z quantity component of point
    void set_z(ZQuantity const& z)
    {
        bg::set<2>(this->point, z.value());
    }

    //!"+" operator to add any representation or differential
    template
    <
        typename Addend
    >
    cartesian_representation
    <
        CoordinateType,
        XQuantity,
        YQuantity,
        ZQuantity
    >
    operator +(Addend const& addend) const
    {
        bg::model::point
        <
            typename std::conditional
            <
                sizeof(typename Addend::type) >= sizeof(CoordinateType),
                typename Addend::type,
                CoordinateType
            >::type,
            3,
            bg::cs::cartesian
        > result;

    auto cartesian = make_cartesian_representation(addend);

    //performing calculation to find the sum
    bg::set<0>(result, (this->get_x().value() +
        static_cast<quantity1>(cartesian.get_x()).value()));
    bg::set<1>(result, (this->get_y().value() +
        static_cast<quantity2>(cartesian.get_y()).value()));
    bg::set<2>(result, (this->get_z().value() +
        static_cast<quantity3>(cartesian.get_z()).value()));

    return cartesian_representation
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        >(result);
    }

}; //cartesian_representation


//!Constructs object from provided quantities
//!Each quantity can have different units but with same datatypes
template
<
    typename CoordinateType,
    template <typename Unit1, typename CoordinateType_> class XQuantity,
    template <typename Unit2, typename CoordinateType_> class YQuantity,
    template <typename Unit3, typename CoordinateType_> class ZQuantity,
    typename Unit1,
    typename Unit2,
    typename Unit3
>
cartesian_representation
<
    CoordinateType,
    XQuantity<Unit1, CoordinateType>,
    YQuantity<Unit2, CoordinateType>,
    ZQuantity<Unit3, CoordinateType>
> make_cartesian_representation
(
    XQuantity<Unit1, CoordinateType> const& x,
    YQuantity<Unit2, CoordinateType> const& y,
    ZQuantity<Unit3, CoordinateType> const& z
)
{
    return cartesian_representation
        <
            CoordinateType,
            XQuantity<Unit1, CoordinateType>,
            YQuantity<Unit2, CoordinateType>,
            ZQuantity<Unit3, CoordinateType>
        >(x, y, z);
}


//!Convert current quantities of cartesian_representation to new quantities. 
template
<
    typename ReturnCoordinateType,
    typename ReturnXQuantity,
    typename ReturnYQuantity,
    typename ReturnZQuantity,
    typename CoordinateType,
    typename XQuantity,
    typename YQuantity,
    typename ZQuantity
>
cartesian_representation
<
    ReturnCoordinateType,
    ReturnXQuantity,
    ReturnYQuantity,
    ReturnZQuantity
>
make_cartesian_representation
(cartesian_representation<CoordinateType, XQuantity, YQuantity, ZQuantity> const& other)
{
    return make_cartesian_representation(
        static_cast<ReturnXQuantity>(other.get_x()),
        static_cast<ReturnYQuantity>(other.get_y()),
        static_cast<ReturnZQuantity>(other.get_z())
    );
}


//!Create copy of cartesian_representation
template
<
    typename CoordinateType,
    typename XQuantity,
    typename YQuantity,
    typename ZQuantity
>
cartesian_representation<CoordinateType, XQuantity, YQuantity, ZQuantity>
make_cartesian_representation
(cartesian_representation<CoordinateType, XQuantity, YQuantity, ZQuantity> const& other)
{
    return cartesian_representation
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        >(other);
}

//!Create cartesian_representation from boost::geometry::point
//!Quantity types are to be specifed explicitly or dimensionless is taken by default
template
<
    typename CoordinateType = double,
    typename XQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    typename YQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    typename ZQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    std::size_t OtherDimensionCount,
    typename OtherCoordinateSystem,
    typename OtherCoordinateType
>
cartesian_representation<CoordinateType, XQuantity, YQuantity, ZQuantity>
make_cartesian_representation
(
    bg::model::point
    <
    OtherCoordinateType,
    OtherDimensionCount,
    OtherCoordinateSystem
    > const& pointObject)
{
    return cartesian_representation
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        >(pointObject);
}


//!Create cartesian_representation from other type of representations
template <typename OtherRepresentation>
cartesian_representation
<
    typename OtherRepresentation::type,
    typename OtherRepresentation::quantity3,
    typename OtherRepresentation::quantity3,
    typename OtherRepresentation::quantity3
>
make_cartesian_representation(OtherRepresentation const& other)
{
    bg::model::point<typename OtherRepresentation::type, 3, bg::cs::cartesian> result;
    bg::transform(other.get_point(), result);

    return cartesian_representation
        <
        typename OtherRepresentation::type,
        typename OtherRepresentation::quantity3,
        typename OtherRepresentation::quantity3,
        typename OtherRepresentation::quantity3
        >(result);
}
}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_CARTESIAN_REPRESENTATION_HPP
