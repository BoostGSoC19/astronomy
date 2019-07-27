#ifndef BOOST_ASTRONOMY_COORDINATE_ARITHMETIC_HPP
#define BOOST_ASTRONOMY_COORDINATE_ARITHMETIC_HPP

#include <type_traits>

#include <boost/units/quantity.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/arithmetic/cross_product.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/units/conversion.hpp>

#include <boost/astronomy/coordinate/base_representation.hpp>
#include <boost/astronomy/coordinate/cartesian_representation.hpp>


namespace boost { namespace astronomy { namespace coordinate {

namespace bg = boost::geometry;
namespace bu = boost::units;


//!Returns the cross product of representation1 and representation2
template
<
    template<typename ...> class Representation1,
    template<typename ...> class Representation2,
    typename ...Args1,
    typename ...Args2
>
auto cross
(
    Representation1<Args1...> const& representation1,
    Representation2<Args2...> const& representation2
)
{
    /*!both the coordinates/vector are first converted into
    cartesian coordinate system then cross product of both cartesian
    vectors is converted into requested type and returned*/

    /*checking types if it is not subclass of
    base_representaion then compile time erorr is generated*/
    //BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
    //    <
    //        boost::astronomy::coordinate::base_representation,
    //        Representation1<Args1...>
    //    >::value),
    //    "First argument type is expected to be a representation class");
    //BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
    //    <
    //        boost::astronomy::coordinate::base_representation,
    //        Representation2<Args2...>
    //    >::value),
    //    "Second argument type is expected to be a representation class");

    /*converting both coordinates/vector into cartesian system*/

    typedef Representation1<Args1...> representation1_type;
    typedef Representation2<Args2...> representation2_type;

    bg::model::point
    <
        typename std::conditional
        <
            sizeof(typename representation2_type::type) >=
                sizeof(typename representation1_type::type),
            typename representation2_type::type,
            typename representation1_type::type
        >::type,
        3,
        bg::cs::cartesian
    > tempPoint1, tempPoint2, result;

    bg::transform(representation1.get_point(), tempPoint1);
    bg::transform(representation2.get_point(), tempPoint2);

    bg::set<0>(result, (bg::get<1>(tempPoint1)*bg::get<2>(tempPoint2)) -
        ((bg::get<2>(tempPoint1)*
        bu::conversion_factor(typename representation1_type::quantity3::unit_type(),
        typename representation1_type::quantity2::unit_type()))*
        (bg::get<1>(tempPoint2)*
        bu::conversion_factor(typename representation2_type::quantity2::unit_type(),
        typename representation2_type::quantity3::unit_type()))));

    bg::set<1>(result, (bg::get<2>(tempPoint1)*bg::get<0>(tempPoint2)) -
        ((bg::get<0>(tempPoint1)*
        bu::conversion_factor(typename representation1_type::quantity1::unit_type(),
        typename representation1_type::quantity3::unit_type()))*
        (bg::get<2>(tempPoint2)*
        bu::conversion_factor(typename representation2_type::quantity3::unit_type(),
        typename representation2_type::quantity1::unit_type()))));

    bg::set<2>(result, (bg::get<0>(tempPoint1)*bg::get<1>(tempPoint2)) -
        ((bg::get<1>(tempPoint1)*
        bu::conversion_factor(typename representation1_type::quantity2::unit_type(),
        typename representation1_type::quantity1::unit_type()))*
        (bg::get<0>(tempPoint2)*
        bu::conversion_factor(typename representation2_type::quantity1::unit_type(),
        typename representation2_type::quantity2::unit_type()))));

    return Representation1
        <
            typename representation1_type::type,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename representation1_type::quantity2::unit_type,
                typename representation2_type::quantity3::unit_type>::type
            >,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename representation1_type::quantity3::unit_type,
                typename representation2_type::quantity1::unit_type>::type
            >,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename representation1_type::quantity1::unit_type,
                typename representation2_type::quantity2::unit_type>::type
            >
        >(result);
}


//! Returns dot product of representation1 and representation2
template<typename Representation1, typename Representation2>
auto dot(Representation1 const& representation1, Representation2 const& representation2)
{
    /*!both the coordinates/vector are first converted into
    cartesian coordinate system then dot product of both cartesian
    product is converted into requested type and returned*/

    /*converting both coordinates/vector into cartesian system*/
    bg::model::point
    <
        typename std::conditional
        <
            sizeof(typename Representation2::type) >=
                sizeof(typename Representation1::type),
            typename Representation2::type,
            typename Representation1::type
        >::type,
        3,
        bg::cs::cartesian
    > tempPoint1, tempPoint2;

    auto cartesian1 = make_cartesian_representation(representation1);
    auto cartesian2 = make_cartesian_representation(representation2);

    typedef decltype(cartesian1) cartesian1_type;
    typedef decltype(cartesian2) cartesian2_type;

    bg::set<0>(tempPoint1, cartesian1.get_x().value());
    bg::set<1>(tempPoint1,
        static_cast<typename cartesian1_type::quantity1>(cartesian1.get_y()).value());
    bg::set<2>(tempPoint1,
        static_cast<typename cartesian1_type::quantity1>(cartesian1.get_z()).value());

    bg::set<0>(tempPoint2, cartesian2.get_x().value());
    bg::set<1>(tempPoint2,
        static_cast<typename cartesian2_type::quantity1>(cartesian2.get_y()).value());
    bg::set<2>(tempPoint2,
        static_cast<typename cartesian2_type::quantity1>(cartesian2.get_z()).value());

    return bg::dot_product(tempPoint1, tempPoint2) *
        typename cartesian1_type::quantity1::unit_type() *
        typename cartesian2_type::quantity1::unit_type();
}


//! Returns magnitude of the cartesian vector
template
<
    typename CoordinateType,
    typename XQuantity,
    typename YQuantity,
    typename ZQuantity
>
auto magnitude
(
    cartesian_representation
    <
        CoordinateType,
        XQuantity,
        YQuantity,
        ZQuantity
    > const& vector
)
{
    CoordinateType result = 0;
    bg::model::point
    <
        CoordinateType,
        3,
        bg::cs::cartesian
    > tempPoint;

    bg::set<0>(tempPoint, vector.get_x().value());
    bg::set<1>(tempPoint, static_cast<XQuantity>(vector.get_y()).value());
    bg::set<2>(tempPoint, static_cast<XQuantity>(vector.get_z()).value());

    result += std::pow(bg::get<0>(tempPoint), 2) +
        std::pow(bg::get<1>(tempPoint), 2) +
        std::pow(bg::get<2>(tempPoint), 2);

    return std::sqrt(result) * typename XQuantity::unit_type();
}


//! Returns magnitude of the vector other than cartesian
template <typename Coordinate>
auto magnitude(Coordinate const& vector)
{
    return bg::get<2>(vector.get_point()) * typename Coordinate::quantity3::unit_type();
}


//! Returns the unit vector of vector given
template <typename ...Args>
cartesian_representation<Args...>
unit_vector(cartesian_representation<Args...> const& vector)
{
    bg::model::point
    <
        typename cartesian_representation<Args...>::type,
        3,
        bg::cs::cartesian
    > tempPoint;
    auto mag = magnitude(vector); //magnitude of vector

    //performing calculations to find unit vector
    bg::set<0>(tempPoint, vector.get_x().value() / mag.value());
    bg::set<1>(tempPoint,
        vector.get_y().value() /
        static_cast<typename cartesian_representation<Args...>::quantity2>(mag).value());
    bg::set<2>(tempPoint,
        vector.get_z().value() /
        static_cast<typename cartesian_representation<Args...>::quantity3>(mag).value());

    return cartesian_representation<Args...>(tempPoint);
}


//! Returns sum of representation1 and representation2 
template<typename Representation1, typename Representation2>
Representation1 sum
(
    Representation1 const& representation1,
    Representation2 const& representation2
)
{
    /*!both the coordinates/vector are first converted into
    cartesian coordinate system then sum of both cartesian
    vectors is converted into the type of first argument and returned*/

    /*checking types if it is not subclass of
    base_representaion then compile time erorr is generated*/
    //BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
    //    <
    //        boost::astronomy::coordinate::base_representation,
    //        Representation1
    //    >::value),
    //    "First argument type is expected to be a representation class");
    //BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
    //    <
    //        boost::astronomy::coordinate::base_representation,
    //        Representation2
    //    >::value),
    //    "Second argument type is expected to be a representation class");

    /*converting both coordinates/vector into cartesian system*/
    bg::model::point
    <
        typename std::conditional
        <
            sizeof(typename Representation2::type) >=
                sizeof(typename Representation1::type),
            typename Representation2::type,
            typename Representation1::type
        >::type,
        3,
        bg::cs::cartesian
    > result;

    auto cartesian1 = make_cartesian_representation(representation1);
    auto cartesian2 = make_cartesian_representation(representation2);

    typedef decltype(cartesian1) cartesian1_type;

    //performing calculation to find the sum
    bg::set<0>(result, (cartesian1.get_x().value() +
        static_cast<typename cartesian1_type::quantity1>(cartesian2.get_x()).value()));
    bg::set<1>(result, (cartesian1.get_y().value() +
        static_cast<typename cartesian1_type::quantity2>(cartesian2.get_y()).value()));
    bg::set<2>(result, (cartesian1.get_z().value() +
        static_cast<typename cartesian1_type::quantity3>(cartesian2.get_z()).value()));

    return Representation1(result);
}


//! Returns mean of representation1 and representation2
template<typename Representation1, typename Representation2>
Representation1 mean
(
    Representation1 const& representation1,
    Representation2 const& representation2
)
{

    /*!both the coordinates/vector are first converted into
    cartesian coordinate system then mean of both cartesian
    vectors is converted into the type of first argument and returned*/

    /*checking return type if it is not subclass of
    base_representaion then compile time erorr is generated*/
    //BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
    //    <boost::astronomy::coordinate::base_representation, ReturnType>::value),
    //    "return type is expected to be a representation class");

    /*converting both coordinates/vector into cartesian system*/
    bg::model::point
    <
        typename std::conditional
        <
            sizeof(typename Representation2::type) >=
                sizeof(typename Representation1::type),
            typename Representation2::type,
            typename Representation1::type
        >::type,
        3,
        bg::cs::cartesian
    > result;

    auto cartesian1 = make_cartesian_representation(representation1);
    auto cartesian2 = make_cartesian_representation(representation2);

    typedef decltype(cartesian1) cartesian1_type;

    //performing calculation to find the mean
    bg::set<0>(result, (cartesian1.get_x().value() +
        static_cast<typename cartesian1_type::quantity1>(cartesian2.get_x()).value())/2);
    bg::set<1>(result, (cartesian1.get_y().value() +
        static_cast<typename cartesian1_type::quantity2>(cartesian2.get_y()).value())/2);
    bg::set<2>(result, (cartesian1.get_z().value() +
        static_cast<typename cartesian1_type::quantity3>(cartesian2.get_z()).value())/2);

    return Representation1(result);
}

}}} // namespace boost::astronomy::coordinate
#endif // !BOOST_ASTRONOMY_COORDINATE_ARITHMETIC_HPP
