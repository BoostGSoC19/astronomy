#ifndef BOOST_ASTRONOMY_COORDINATE_CONCEPTS_CARTESIAN_DIFFERENTIAL_CONCEPT_HPP
#define BOOST_ASTRONOMY_COORDINATE_CONCEPTS_CARTESIAN_DIFFERENTIAL_CONCEPT_HPP

//!Header for Cartesian differential concept checking class

#include <boost/concept_check.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/differential.hpp>
#include <boost/astronomy/coordinate/concepts/base_differential_concept.hpp>

namespace boost { namespace astronomy { namespace coordinate { namespace concepts {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Concept checking class for Cartesian Differential
template<typename Differential>
class Cartesian_Differential : Base_Differential<Differential>
{
private:

	Differential R;
	typedef decltype(R.get_differential()) pointType;
	typedef typename bg::coordinate_system<pointType>::type cSystem;

	static const bool isCartesianPoint = std::is_same<cSystem,bg::cs::cartesian>::value;

public:

	/*Asserting all components to have the same dimension. This assertion differentiates cartesian
	from other differentials*/
	BOOST_CONCEPT_ASSERT((Cartesian_Components<
		typename Differential::quantity1,
		typename Differential::quantity2,
		typename Differential::quantity3>));

	BOOST_CONCEPT_USAGE(Cartesian_Differential)
	{
		//Asserting the point as Cartesian
		BOOST_STATIC_ASSERT_MSG(isCartesianPoint,"Cartesian differential required");
	}
};

}}}} //namespace boost::astronomy::coordinate::concepts

#endif // !BOOST_ASTRONOMY_COORDINATE_CONCEPTS_CARTESIAN_DIFFERENTIAL_CONCEPT_HPP