#ifndef BOOST_ASTRONOMY_COORDINATE_BASE_REPRESENTATION_HPP
#define BOOST_ASTRONOMY_COORDINATE_BASE_REPRESENTATION_HPP

#include <cstddef>
#include <type_traits>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>
#include <boost/geometry/arithmetic/cross_product.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/static_assert.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>

namespace boost { namespace astronomy { namespace coordinate {

namespace bg = boost::geometry;
typedef bg::degree degree;
typedef bg::radian radian;

// structure which is the base for all the representation
template
<
    std::size_t DimensionCount,
    typename CoordinateSystem,
    typename CoordinateType=double
>
struct base_representation
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG((DimensionCount == 2 || DimensionCount == 3),
        "DimensionCount is expected to be 2 or 3");
    ///@endcond
protected:
    bg::model::point<CoordinateType, DimensionCount, CoordinateSystem> point;

public:

    typedef CoordinateSystem system;
    typedef CoordinateType type;

    /// cross prodct of current vector with specified vector
    template
    <
        typename ReturnType,
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    ReturnType cross
    (
        base_representation
        <
            OtherDimensionCount,
            OtherCoordinateSystem,
            OtherCoordinateType
        > const& other
    ) const
    {
        /*!both the coordinates/vector are first converted into
        cartesian coordinate system then cross product of both cartesian
        vectors is converted into requested type and returned*/

        /*checking return type if it is not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, ReturnType>::value),
            "return type is expected to be a representation class");

        /*converting both coordinates/vector into cartesian system*/
        bg::model::point
        <
            typename std::conditional
            <
                sizeof(OtherCoordinateType) >= sizeof(CoordinateType),
                OtherCoordinateType,
                CoordinateType
            >::type,
            3,
            bg::cs::cartesian
        > tempPoint1, tempPoint2;

        bg::transform(this->point, tempPoint1);
        bg::transform(other.get_point(), tempPoint2);

        /*creating object of the specified return type and returning it */
        return ReturnType
        (
            bg::cross_product
            <
                bg::model::point<typename ReturnType::type, 3, bg::cs::cartesian>
            >(tempPoint1, tempPoint2)
        );
    }

    //! dot prodct of current vector with specified vector
    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    double dot
    (
        base_representation
        <
            OtherDimensionCount,
            OtherCoordinateSystem,
            OtherCoordinateType
        >  const& other
    ) const
    {
        /*!both the coordinates/vector are first converted into
        cartesian coordinate system then dot product of both cartesian
        product is converted into requested type and returned*/

        /*converting both coordinates/vector into cartesian system*/
        bg::model::point
        <
            typename std::conditional
            <
                sizeof(OtherCoordinateType) >= sizeof(CoordinateType),
                OtherCoordinateType,
                CoordinateType
            >::type,
            3,
            bg::cs::cartesian
        > tempPoint1, tempPoint2;

        bg::transform(this->point, tempPoint1);
        bg::transform(other.get_point(), tempPoint2);

        return bg::dot_product(tempPoint1, tempPoint2);

    }

    //! returns the unit vector of current vector
    template <typename ReturnType>
    ReturnType unit_vector() const
    {
        /*!given coordinates/vectors are converted into cartesian and
        unit vector of it is returned by converting it into requested type*/

        /*checking return type if they both are not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, ReturnType>::value),
            "return type is expected to be a representation class");

        bg::model::point<CoordinateType, 3, bg::cs::cartesian> tempPoint;
        double mag = this->magnitude(); //magnitude of vector stored in current object

        //converting coordinate/vector into cartesian
        bg::transform(this->point, tempPoint);

        //performing calculations to find unit vector
        bg::set<0>(tempPoint, (bg::get<0>(tempPoint) / mag));
        bg::set<1>(tempPoint, (bg::get<1>(tempPoint) / mag));
        bg::set<2>(tempPoint, (bg::get<2>(tempPoint) / mag));

        return ReturnType(tempPoint);
    }

    //! converts current representation into specified representation
    template <typename ReturnType>
    ReturnType to_representation() const
    {
        /*checking return type if they both are not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, ReturnType>::value),
            "return type is expected to be a representation class");

        return ReturnType(this->point);
    }

    //! sum of current vector and specified vector
    template
    <
        typename ReturnType,
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    ReturnType sum
    (
        base_representation
        <
            OtherDimensionCount,
            OtherCoordinateSystem,
            OtherCoordinateType
        > const& other
    ) const
    {
        /*checking return type if it is not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, ReturnType>::value),
            "return type is expected to be a representation class");

        /*converting both coordinates/vector into cartesian system*/
        bg::model::point
        <
            typename std::conditional
            <
                sizeof(OtherCoordinateType) >= sizeof(CoordinateType),
                OtherCoordinateType,
                CoordinateType
            >::type,
            3,
            bg::cs::cartesian
        > tempPoint1, tempPoint2, result;

        bg::transform(this->point, tempPoint1);
        bg::transform(other.get_point(), tempPoint2);

        //performing calculation to find the sum
        bg::set<0>(result, (bg::get<0>(tempPoint1) + bg::get<0>(tempPoint2)));
        bg::set<1>(result, (bg::get<1>(tempPoint1) + bg::get<1>(tempPoint2)));
        bg::set<2>(result, (bg::get<2>(tempPoint1) + bg::get<2>(tempPoint2)));

        return ReturnType(result);
    }

    //! mean of current vector with specified vector
    template
    <
        typename ReturnType,
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    ReturnType mean
    (
        base_representation
        <
            OtherDimensionCount,
            OtherCoordinateSystem,
            OtherCoordinateType
        > const& other
    ) const
    {
        /*checking return type if it is not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, ReturnType>::value),
            "return type is expected to be a representation class");

        /*converting both coordinates/vector into cartesian system*/
        bg::model::point
        <
            typename std::conditional
            <
                sizeof(OtherCoordinateType) >= sizeof(CoordinateType),
                OtherCoordinateType,
                CoordinateType
            >::type,
            3,
            bg::cs::cartesian
        > tempPoint1, tempPoint2, result;

        bg::transform(this->point, tempPoint1);
        bg::transform(other.get_point(), tempPoint2);

        //performing calculation to find the mean
        bg::set<0>(result, ((bg::get<0>(tempPoint1) + bg::get<0>(tempPoint2)) / 2));
        bg::set<1>(result, ((bg::get<1>(tempPoint1) + bg::get<1>(tempPoint2)) / 2));
        bg::set<2>(result, ((bg::get<2>(tempPoint1) + bg::get<2>(tempPoint2)) / 2));

        return ReturnType(result);
    }

    //! magnitude of the current class is returned
    double magnitude() const
    {
        double result = 0.0;
        bg::model::point<CoordinateType, 3, bg::cs::cartesian> tempPoint;
        bg::transform(this->point, tempPoint);

        switch (DimensionCount)
        {
        case 2:
            result += std::pow(bg::get<0>(tempPoint), 2);
            result += std::pow(bg::get<1>(tempPoint), 2);
            break;
        case 3:
            result += std::pow(bg::get<0>(tempPoint), 2);
            result += std::pow(bg::get<1>(tempPoint), 2);
            result += std::pow(bg::get<2>(tempPoint), 2);
            break;
        default:
            return -1;
        }

        return std::sqrt(result);
    }

    //! returns the point/vector of calling object
    bg::model::point<CoordinateType, DimensionCount, CoordinateSystem> get_point() const
    {
        return this->point;
    }

    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    bool operator==
    (
        base_representation
        <
            OtherDimensionCount,
            OtherCoordinateSystem,
            OtherCoordinateType
        >  const& other
    ) const
    {
        /*converting both coordinates/vector into cartesian system*/
        bg::model::point
        <
            typename std::conditional
            <
                sizeof(OtherCoordinateType) >= sizeof(CoordinateType),
                OtherCoordinateType,
                CoordinateType
            >::type,
            3,
            bg::cs::cartesian
        > tempPoint1, tempPoint2;
        bg::transform(this->point, tempPoint1);
        bg::transform(other.get_point(), tempPoint2);

        return (bg::get<0>(tempPoint1) == bg::get<0>(tempPoint2)) &&
            (bg::get<1>(tempPoint1) == bg::get<1>(tempPoint2)) &&
            (bg::get<2>(tempPoint1) == bg::get<2>(tempPoint2));
    }

}; //base_representation
}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_BASE_REPRESENTATION_HPP
