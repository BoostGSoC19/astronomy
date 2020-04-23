#ifndef BOOST_ASTRONOMY_COORDINATE_CONCEPTS_SPHERICAL_DIFFERENTIAL_CONCEPT_HPP
#define BOOST_ASTRONOMY_COORDINATE_CONCEPTS_SPHERICAL_DIFFERENTIAL_CONCEPT_HPP

//!Header for Spherical differential concept checking class

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

//!Concept checking class for Spherical Differential
template<typename Differential>
class Spherical_Differential : Base_Differential<Differential>
{
private:

	Differential R;
	typedef decltype(R.get_differential()) pointType;
	typedef typename bg::coordinate_system<pointType>::type cSystem;

	static const bool isSphericalPoint = std::is_same<cSystem,bg::cs::spherical<radian>>::value;

public:

	//Asserting components to have plane angle type and floating point type
	BOOST_CONCEPT_ASSERT((Spherical_Components<
		typename Differential::quantity1,
		typename Differential::quantity2>));

	BOOST_CONCEPT_USAGE(Spherical_Differential)
	{
		//Asserting the point as Spherical
		BOOST_STATIC_ASSERT_MSG(isSphericalPoint,"Spherical differential required");
	}
};

}}}} //namespace boost::astronomy::coordinate::concepts

#endif // !BOOST_ASTRONOMY_COORDINATE_CONCEPTS_SPHERICAL_DIFFERENTIAL_CONCEPT_HPP