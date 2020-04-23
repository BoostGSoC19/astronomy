#ifndef BOOST_ASTRONOMY_COORDINATE_CONCEPTS_GENERAL_HPP
#define BOOST_ASTRONOMY_COORDINATE_CONCEPTS_GENERAL_HPP

//!Header for general concept checking classes

#include <type_traits>

#include <boost/concept_check.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/concepts/point_concept.hpp>

#include <boost/units/quantity.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/is_quantity.hpp>
#include <boost/units/systems/si/dimensionless.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_representation.hpp>

namespace boost { namespace astronomy { namespace coordinate { namespace concepts {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Concept checking class to check validity of Cartesian components
template 
<
	typename XQuantity, 
	typename YQuantity, 
	typename ZQuantity
>
class Cartesian_Components
{
private:
	
	typedef typename bu::get_dimension<XQuantity>::type XDimension;
	typedef typename bu::get_dimension<YQuantity>::type YDimension;
	typedef typename bu::get_dimension<ZQuantity>::type ZDimension;

	//Value to check if the parameters are boost::units::quantity
	static const bool areQuantites = bu::is_quantity<XQuantity>::value &&
		bu::is_quantity<YQuantity>::value && bu::is_quantity<ZQuantity>::value;
	
	//Value to check if all the components have the same dimensions	
	static const bool sameDimensions = (std::is_same<XDimension,YDimension>::value) && 
		(std::is_same<YDimension,ZDimension>::value);

public:

	BOOST_CONCEPT_USAGE(Cartesian_Components)
	{	
		//Asserting dimensions and quantities check
		BOOST_STATIC_ASSERT(areQuantites);
		BOOST_STATIC_ASSERT_MSG(sameDimensions,"All components must have same dimensions");
	}
};

//!Concept checking class to check validity of latitude and longitude 
template
<
	typename LatQuantity,
	typename LonQuantity
>
class Spherical_Components
{
private:

	static const bool isPlaneAngle = (std::is_same<typename bu::get_dimension<LatQuantity>::type,
        bu::plane_angle_dimension>::value) && 
		(std::is_same<typename bu::get_dimension<LonQuantity>::type,
		bu::plane_angle_dimension>::value);

	static const bool areQuantites = bu::is_quantity<LatQuantity>::value &&
		bu::is_quantity<LonQuantity>::value;

	static const bool isFloatingPoint = 
		std::is_floating_point<typename LatQuantity::value_type>::value;

public:

	BOOST_CONCEPT_USAGE(Spherical_Components)
	{	
		//Asserting dimensions and quantities check
		BOOST_STATIC_ASSERT(areQuantites);
		BOOST_STATIC_ASSERT_MSG(isPlaneAngle,"Latitude and Longitude must be plane angle type");
		BOOST_STATIC_ASSERT_MSG(isFloatingPoint,"CoordinateType must be a floating-point type");
	}
};

//!Concept checking class to check validity of point class
template
<
    typename CoordinateType,
    std::size_t DimensionCount,
    typename CoordinateSystem
>
class Point
{
private:

	typedef bg::model::point
    <
        CoordinateType,
        DimensionCount,
        CoordinateSystem
    > pointType;

public:
	
    /*concept assert to check if the resulting point class is valid using predefined 
    Point concept from Boost geometry*/
	BOOST_CONCEPT_ASSERT((bg::concepts::Point<pointType>));

	BOOST_CONCEPT_USAGE(Point)
	{
		//Asserting no of dimensions to three 
		BOOST_STATIC_ASSERT_MSG(DimensionCount==3, "Three dimensional point required");
		//Asserting CoordinateType to be integer of floating point type
		BOOST_STATIC_ASSERT_MSG(std::is_arithmetic<CoordinateType>::value, 
			"Arithmetic CoordinateType required");
	}
};

}}}} //namespace boost::astronomy::coordinate::concepts

#endif // !BOOST_ASTRONOMY_COORDINATE_CONCEPTS_GENERAL_HPP