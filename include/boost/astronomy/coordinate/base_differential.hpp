#ifndef BOOST_ASTRONOMY_COORDINATE_BASE_DIFFERENTIAL_HPP
#define BOOST_ASTRONOMY_COORDINATE_BASE_DIFFERENTIAL_HPP

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


//! structure which is the base for all the representation 
template
<
    std::size_t DimensionCount,
    typename CoordinateSystem,
    typename CoordinateType=double
>
struct base_differential
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG((DimensionCount == 2 || DimensionCount == 3),
        "DimensionCount is expected to be 2 or 3");
    ///@endcond
protected:
    bg::model::point<CoordinateType, DimensionCount, CoordinateSystem> diff;

public:

    typedef CoordinateSystem system;
    typedef CoordinateType type;

    //! returns the unit vector of current differential
    template <typename ReturnType>
    ReturnType unit_vector() const
    {
        /*!given coordinates/vectors are converted into cartesian and
        unit vector of it is returned by converting it into requested type*/

        /*checking return type if they both are not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, ReturnType>::value),
            "return type is expected to be a differential class");

        bg::model::point<CoordinateType, 3, bg::cs::cartesian> tempPoint;
        double mag = this->magnitude(); //magnitude of vector stored in current object
        
        //converting coordinate/vector into cartesian
        bg::transform(this->diff, tempPoint); 

        //performing calculations to find unit vector
        bg::set<0>(tempPoint, (bg::get<0>(tempPoint) / mag));
        bg::set<1>(tempPoint, (bg::get<1>(tempPoint) / mag));
        bg::set<2>(tempPoint, (bg::get<2>(tempPoint) / mag));

        return ReturnType(tempPoint);
    }

    //! magnitude of the current class is returned
    double magnitude() const
    {
        double result = 0.0;
        bg::model::point<CoordinateType, 3, bg::cs::cartesian> tempPoint;
        bg::transform(this->diff, tempPoint);

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

    //! converts current representation into specified representation
    template <typename ReturnType>
    ReturnType to_differential() const
    {
        /*checking return type if they both are not subclass of
        base_representaion then compile time erorr is generated*/
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, ReturnType>::value),
            "return type is expected to be a differential class");

        return ReturnType(this->diff);
    }

    //! returns the differential of calling object
    bg::model::point
    <
        CoordinateType,
        DimensionCount,
        CoordinateSystem
    >
    get_differential() const
    {
        return this->diff;
    }

    template
    <
        std::size_t OtherDimensionCount,
        typename OtherCoordinateSystem,
        typename OtherCoordinateType
    >
    bool operator==
    (
        base_differential
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
        bg::transform(this->diff, tempPoint1);
        bg::transform(other.get_differential(), tempPoint2);

        return (bg::get<0>(tempPoint1) == bg::get<0>(tempPoint2)) &&
            (bg::get<1>(tempPoint1) == bg::get<1>(tempPoint2)) &&
            (bg::get<2>(tempPoint1) == bg::get<2>(tempPoint2));
    }

}; //base_differential
}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_BASE_DIFFERENTIAL_HPP

