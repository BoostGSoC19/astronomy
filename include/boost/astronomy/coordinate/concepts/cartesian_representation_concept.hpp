#ifndef BOOST_ASTRONOMY_COORDINATE_CONCEPTS_CARTESIAN_REPRESENTATION_CONCEPT_HPP
#define BOOST_ASTRONOMY_COORDINATE_CONCEPTS_CARTESIAN_REPRESENTATION_CONCEPT_HPP

//!Header for Cartesian representation concept checking class

#include <boost/concept_check.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/representation.hpp>
#include <boost/astronomy/coordinate/concepts/base_representation_concept.hpp>

namespace boost { namespace astronomy { namespace coordinate { namespace concepts {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Concept checking class for Cartesian Representation
template<typename Representation>
class Cartesian_Representation : Base_Representation<Representation>
{
private:

	Representation R;
	typedef decltype(R.get_point()) pointType;
	typedef typename bg::coordinate_system<pointType>::type cSystem;

	static const bool isCartesianPoint = std::is_same<cSystem,bg::cs::cartesian>::value;

public:

	/*Asserting all components to have the same dimension. This assertion differentiates cartesian
	from other representations*/
	BOOST_CONCEPT_ASSERT((Cartesian_Components<
		typename Representation::quantity1,
		typename Representation::quantity2,
		typename Representation::quantity3>));

	BOOST_CONCEPT_USAGE(Cartesian_Representation)
	{
		//Asserting the point as Cartesian
		BOOST_STATIC_ASSERT_MSG(isCartesianPoint,"Cartesian point required");
	}
};

}}}} //namespace boost::astronomy::coordinate::concepts

#endif // !BOOST_ASTRONOMY_COORDINATE_CONCEPTS_CARTESIAN_REPRESENTATION_CONCEPT_HPP