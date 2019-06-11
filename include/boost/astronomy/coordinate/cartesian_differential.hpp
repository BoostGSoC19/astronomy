#ifndef BOOST_ASTRONOMY_COORDINATE_CARTESIAN_DIFFERENTIAL_HPP
#define BOOST_ASTRONOMY_COORDINATE_CARTESIAN_DIFFERENTIAL_HPP

#include <tuple>
#include <type_traits>

#include <boost/geometry/strategies/strategy_transform.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/static_assert.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/systems/si/dimensionless.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_differential.hpp>

namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Represents differential of cartesian representation
//!Uses three coordinate to represent a differential (dx, dy, dz)
template
<
    typename CoordinateType = double,
    typename XQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    typename YQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>,
    typename ZQuantity = bu::quantity<bu::si::dimensionless, CoordinateType>
>
struct cartesian_differential: base_differential<3, bg::cs::cartesian, CoordinateType>
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

    // default constructor no initialization
    cartesian_differential() {}

    //!constructs object from provided components of differential
    cartesian_differential(XQuantity const& dx, YQuantity const& dy, ZQuantity const& dz)
    {
        this->set_dx_dy_dz(dx, dy, dz);
    }

    //!constructs object from boost::geometry::model::point object
    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    cartesian_differential
    (
        bg::model::point
        <
            OtherCoordinateType,
            OtherDimensionCount,
            OtherCoordinateSystem
        > const& pointObject
    )
    {
        bg::transform(pointObject, this->diff);
    }

    //!copy constructor
    cartesian_differential
    (
        cartesian_differential
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        > const& object
    )
    {
        this->diff = object.get_differential();
    }

    //!constructs object from any type of differential
    template <typename Differential>
    cartesian_differential(Differential const& other)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, Differential>::value),
            "No constructor found with given argument type");

        bg::transform(other.get_differential(), this->diff);
    }

    //! returns the (dx, dy, dz) in the form of tuple
    std::tuple<XQuantity, YQuantity, ZQuantity> get_dx_dy_dz() const
    {
        return std::make_tuple(this->get_dx(), this->get_dy(), this->get_dz());
    }

    //!returns the dx component of differential
    XQuantity get_dx() const
    {
        return XQuantity(bg::get<0>(this->diff) * typename XQuantity::unit_type());
    }

    //!returns the dy component of differential
    YQuantity get_dy() const
    {
        return YQuantity(bg::get<1>(this->diff) * typename YQuantity::unit_type());
    }
    //!returns the dz component of differential
    ZQuantity get_dz() const
    {
        return ZQuantity(bg::get<2>(this->diff) * typename ZQuantity::unit_type());
    }

    //!set value of (dx, dy, dz) in current object
    void set_dx_dy_dz(XQuantity const& dx,  YQuantity const& dy, ZQuantity const& dz)
    {
        this->set_dx(dx);
        this->set_dy(dy);
        this->set_dz(dz);
    }

    //!set value of dx component of differential
    void set_dx(XQuantity const& dx)
    {
        bg::set<0>(this->diff, dx.value());
    }

    //!set value of dy component of differential
    void set_dy(YQuantity const& dy)
    {
        bg::set<1>(this->diff, dy.value());
    }

    //!set value of dz component of differential
    void set_dz(ZQuantity const& dz)
    {
        bg::set<2>(this->diff, dz.value());
    }

    //!operator for addition of differential
    template
    <
        typename Addend
    >
    cartesian_differential
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

        auto cartesian = make_cartesian_differential(addend);

	    //performing calculation to find the sum
	    bg::set<0>(result, (this->get_dx().value() +
	        static_cast<quantity1>(cartesian.get_dx()).value()));
	    bg::set<1>(result, (this->get_dy().value() +
	        static_cast<quantity2>(cartesian.get_dy()).value()));
	    bg::set<2>(result, (this->get_dz().value() +
	        static_cast<quantity3>(cartesian.get_dz()).value()));

	    return cartesian_differential
	        <
	            CoordinateType,
	            XQuantity,
	            YQuantity,
	            ZQuantity
	        >(result);
    }

    //!operator for multiplication of differential
    template
    <
        typename OtherQuantity
    >
    auto operator *(OtherQuantity const& dt) const
    {

    	cartesian_differential<CoordinateType, XQuantity, YQuantity, ZQuantity>
            temp(this->diff);

        cartesian_differential
        <
        	CoordinateType,
        	bu::quantity<typename bu::multiply_typeof_helper
            <
                typename XQuantity::unit_type,
                typename OtherQuantity::unit_type>::type
			>,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename YQuantity::unit_type,
                typename OtherQuantity::unit_type>::type
            >,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename ZQuantity::unit_type,
                typename OtherQuantity::unit_type>::type
            >
        > product
        (
        	temp.get_dx() * dt,
        	temp.get_dy() * dt,
        	temp.get_dz() * dt
        );

        return product;
    }

}; //cartesian_differential

//!constructs object from provided components of differential
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
cartesian_differential
<
    CoordinateType,
    XQuantity<Unit1, CoordinateType>,
    YQuantity<Unit2, CoordinateType>,
    ZQuantity<Unit3, CoordinateType>
>
make_cartesian_differential
(
    XQuantity<Unit1, CoordinateType> const& dx,
    YQuantity<Unit2, CoordinateType> const& dy,
    ZQuantity<Unit3, CoordinateType> const& dz
)
{
    return cartesian_differential
        <
            CoordinateType,
            XQuantity<Unit1, CoordinateType>,
            YQuantity<Unit2, CoordinateType>,
            ZQuantity<Unit3, CoordinateType>
        >(dx, dy, dz);
}

//!constructs object from provided components of differential with different units
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
cartesian_differential
<
    ReturnCoordinateType,
    ReturnXQuantity,
    ReturnYQuantity,
    ReturnZQuantity
>
make_cartesian_differential
(
	cartesian_differential
	<
		CoordinateType,
		XQuantity,
		YQuantity,
		ZQuantity
	> const& other
)
{
    return make_cartesian_differential(
        static_cast<ReturnXQuantity>(other.get_dx()),
        static_cast<ReturnYQuantity>(other.get_dy()),
        static_cast<ReturnZQuantity>(other.get_dz())
    );
}

//!constructs object from provided differential
template
<
    typename CoordinateType,
    typename XQuantity,
    typename YQuantity,
    typename ZQuantity
>
cartesian_differential
<
	CoordinateType,
	XQuantity,
	YQuantity,
	ZQuantity
>
make_cartesian_differential
(
	cartesian_differential
	<
		CoordinateType,
		XQuantity,
		YQuantity,
		ZQuantity
	> const& other
)
{
    return cartesian_differential
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        >(other);
}

 //!constructs object from boost::geometry::model::point object
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
cartesian_differential
<
	CoordinateType,
	XQuantity,
	YQuantity,
	ZQuantity
>
make_cartesian_differential
(
    bg::model::point
    <
    	OtherCoordinateType,
    	OtherDimensionCount,
    	OtherCoordinateSystem
    > const& pointObject
)
{
    return cartesian_differential
        <
            CoordinateType,
            XQuantity,
            YQuantity,
            ZQuantity
        >(pointObject);
}

 //!constructs object from any type of differential
template <typename OtherDifferential>
cartesian_differential
<
    typename OtherDifferential::type,
    typename OtherDifferential::quantity3,
    typename OtherDifferential::quantity3,
    typename OtherDifferential::quantity3
>
make_cartesian_differential(OtherDifferential const& other)
{
    bg::model::point<typename OtherDifferential::type, 3, bg::cs::cartesian> result;
    bg::transform(other.get_differential(), result);

    return cartesian_differential
        <
        	typename OtherDifferential::type,
        	typename OtherDifferential::quantity3,
        	typename OtherDifferential::quantity3,
        	typename OtherDifferential::quantity3
        >(result);
}
}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_CARTESIAN_DIFFERENTIAL_HPP

