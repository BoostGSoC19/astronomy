/*=============================================================================
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>
Copyright 2020 Nitin K         <konidalanitin@gmail.com>
Copyright 2020 Rohit Ranjan    <rohitrjn629@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

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

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/astronomy/coordinate/rep/base_representation.hpp>
#include <boost/astronomy/coordinate/rep/cartesian_representation.hpp>
#include <boost/astronomy/coordinate/rep/representation.hpp>
#include <boost/astronomy/coordinate/diff/differential.hpp>

namespace bg = boost::geometry;
namespace bu = boost::units;
using namespace boost::numeric::ublas;

namespace boost { namespace astronomy { namespace coordinate {

//!Returns the cross product of cartesian_representation(representation1) and representation2
template
<
    template<typename ...> class Representation2,
    typename ...Args1,
    typename ...Args2
>
auto cross
(
    cartesian_representation<Args1...> const& representation1,
    Representation2<Args2...> const& representation2
)
{
    /*!both the coordinates/vector are first converted into
    cartesian coordinate system then cross product of both cartesian
    vectors is converted into cartesian system type and returned*/

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

    typedef cartesian_representation<Args1...> representation1_type;
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

    auto cartesian1 = representation1;
    auto cartesian2 = make_cartesian_representation(representation2);

    typedef decltype(cartesian1) cartesian1_type;
    typedef decltype(cartesian2) cartesian2_type;


    bg::transform(representation1.get_point(), tempPoint1);
    bg::transform(representation2.get_point(), tempPoint2);


    bg::set<0>(result, (bg::get<1>(tempPoint1)*bg::get<2>(tempPoint2)) -
        ((bg::get<2>(tempPoint1)*
        bu::conversion_factor(typename cartesian1_type::quantity3::unit_type(),
        typename cartesian1_type::quantity2::unit_type()))*
        (bg::get<1>(tempPoint2)*
        bu::conversion_factor(typename cartesian2_type::quantity2::unit_type(),
        typename cartesian2_type::quantity3::unit_type()))));

    bg::set<1>(result, (bg::get<2>(tempPoint1)*bg::get<0>(tempPoint2)) -
        ((bg::get<0>(tempPoint1)*
        bu::conversion_factor(typename cartesian1_type::quantity1::unit_type(),
        typename cartesian1_type::quantity3::unit_type()))*
        (bg::get<2>(tempPoint2)*
        bu::conversion_factor(typename cartesian2_type::quantity3::unit_type(),
        typename cartesian2_type::quantity1::unit_type()))));

    bg::set<2>(result, (bg::get<0>(tempPoint1)*bg::get<1>(tempPoint2)) -
        ((bg::get<1>(tempPoint1)*
        bu::conversion_factor(typename cartesian1_type::quantity2::unit_type(),
        typename cartesian1_type::quantity1::unit_type()))*
        (bg::get<0>(tempPoint2)*
        bu::conversion_factor(typename cartesian2_type::quantity1::unit_type(),
        typename cartesian2_type::quantity2::unit_type()))));


    return
        cartesian_representation
        <
            typename cartesian1_type::type,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename cartesian1_type::quantity2::unit_type,
                typename cartesian2_type::quantity3::unit_type>::type
            >,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename cartesian1_type::quantity3::unit_type,
                typename cartesian2_type::quantity1::unit_type>::type
            >,
            bu::quantity<typename bu::multiply_typeof_helper
            <
                typename cartesian1_type::quantity1::unit_type,
                typename cartesian2_type::quantity2::unit_type>::type
            >
        >(result);
}

//!Returns the cross product of representation1(other than cartesian form) and representation2
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
    vectors is converted into requested system type and returned*/

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

    auto cartesian1 = make_cartesian_representation(representation1);
    auto cartesian2 = make_cartesian_representation(representation2);

    typedef decltype(cartesian1) cartesian1_type;
    typedef decltype(cartesian2) cartesian2_type;


    bg::transform(cartesian1.get_point(), tempPoint1);
    bg::transform(cartesian2.get_point(), tempPoint2);


    bg::set<0>(
        tempPoint2,
        bg::get<0>(tempPoint2)*
        bu::conversion_factor(typename cartesian2_type::quantity1::unit_type(),
        typename cartesian2_type::quantity1::unit_type())
    );
    bg::set<1>(
        tempPoint2,
        bg::get<1>(tempPoint2)*
        bu::conversion_factor(typename cartesian2_type::quantity2::unit_type(),
        typename cartesian2_type::quantity1::unit_type())
    );
    bg::set<2>(
        tempPoint2,
        bg::get<2>(tempPoint2)*
        bu::conversion_factor(typename cartesian2_type::quantity3::unit_type(),
        typename cartesian2_type::quantity1::unit_type())
    );

    result = bg::cross_product(tempPoint1,tempPoint2);

    return Representation1
        <
            typename representation1_type::type,
            typename representation1_type::quantity1,
            typename representation1_type::quantity2,
            bu::quantity<typename bu::multiply_typeof_helper
                <
                    typename cartesian1_type::quantity1::unit_type,
                    typename cartesian2_type::quantity1::unit_type>::type
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


//! Returns scalar product of the cartesian vector with a factor
template <typename ...Args>
cartesian_representation<Args...>
multiply(cartesian_representation<Args...> const& vector, double factor)
{
    bg::model::point
    <
        typename cartesian_representation<Args...>::type,
        3,
        bg::cs::cartesian
    > tempPoint;

    bg::set<0>(tempPoint, vector.get_x().value() * factor);
    bg::set<1>(tempPoint, vector.get_y().value() * factor);
    bg::set<2>(tempPoint, vector.get_z().value() * factor);

    return cartesian_representation<Args...>(tempPoint);
}


//! Returns scalar product of given vector other than cartesian with a factor
template <typename Coordinate>
auto multiply(Coordinate const& vector, double factor)
{
    Coordinate tempVector;

    tempVector.set_lat(vector.get_lat());
    tempVector.set_lon(vector.get_lon());
    tempVector.set_dist(vector.get_dist() * factor);

    return tempVector;
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


//! Returns unit vector of given vector
template <typename Coordinate>
auto unit_vector(Coordinate const& vector)
{
    auto mag = magnitude(vector); //magnitude of vector

    return multiply(vector, 1/mag.value());
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


//! Returns difference of representation1 and representation2
template<typename Representation1, typename Representation2>
Representation1 difference
(
    Representation1 const& representation1,
    Representation2 const& representation2
)
{
    /*!both the coordinates/vector are first converted into
    cartesian coordinate system then difference of cartesian
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
    bg::set<0>(result, (cartesian1.get_x().value() -
        static_cast<typename cartesian1_type::quantity1>(cartesian2.get_x()).value()));
    bg::set<1>(result, (cartesian1.get_y().value() -
        static_cast<typename cartesian1_type::quantity2>(cartesian2.get_y()).value()));
    bg::set<2>(result, (cartesian1.get_z().value() -
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


//! Returns multiplication of a matrix with the cartesian representation
template <typename ...Args, typename MatrixType = double>
cartesian_representation<Args...>
matrix_multiply(cartesian_representation<Args...> const& representation, matrix<MatrixType> mul)
{
    matrix<double> vec = matrix<double>(3,1);

    vec(0,0) = representation.get_x().value();
    vec(1,0) = representation.get_y().value();
    vec(2,0) = representation.get_z().value();

    vec = prod(mul, vec);

    bg::model::point
    <
        typename cartesian_representation<Args...>::type,
        3,
        bg::cs::cartesian
    > ans;
    bg::set<0>(ans, vec(0,0));
    bg::set<1>(ans, vec(1,0));
    bg::set<2>(ans, vec(2,0));

    return cartesian_representation<Args...>(ans);
}


}}} // namespace boost::astronomy::coordinate
#endif // !BOOST_ASTRONOMY_COORDINATE_ARITHMETIC_HPP
