#ifndef BOOST_ASTRONOMY_COORDINATE_BASE_REPRESENTATION_HPP
#define BOOST_ASTRONOMY_COORDINATE_BASE_REPRESENTATION_HPP

#include <cstddef>
#include <type_traits>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
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
    BOOST_STATIC_ASSERT_MSG((std::is_arithmetic<CoordinateType>::value),
        "Coordinate Type must be an arithmetic type");
    ///@endcond

protected:
    bg::model::point<CoordinateType, DimensionCount, CoordinateSystem> point;

public:

    typedef CoordinateSystem system;
    typedef CoordinateType type;

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
