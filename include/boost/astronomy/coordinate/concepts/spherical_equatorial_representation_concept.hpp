#ifndef BOOST_ASTRONOMY_COORDINATE_CONCEPTS_SPHERICAL_EQUATORIAL_REPRESENTATION_CONCEPT_HPP
#define BOOST_ASTRONOMY_COORDINATE_CONCEPTS_SPHERICAL_EQUATORIAL_REPRESENTATION_CONCEPT_HPP

//!Header for Spherical Equatorial representation concept checking class

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

//!Concept checking class for Spherical Equatorial Representation
template<typename Representation>
class Spherical_Equatorial_Representation : Base_Representation<Representation>
{
private:

	Representation R;
	typedef decltype(R.get_point()) pointType;
	typedef typename bg::coordinate_system<pointType>::type cSystem;

	static const bool isSphericalEquatorialPoint = 
		std::is_same<cSystem,bg::cs::spherical_equatorial<radian>>::value;

public:

	//Asserting components to have plane angle type and floating point type
	BOOST_CONCEPT_ASSERT((Spherical_Components<
		typename Representation::quantity1,
		typename Representation::quantity2>));

	BOOST_CONCEPT_USAGE(Spherical_Equatorial_Representation)
	{
		//Asserting the point as Spherical Equatorial
		BOOST_STATIC_ASSERT_MSG(isSphericalEquatorialPoint,
            "Spherical Equatorial point required");
	}
};

}}}} //namespace boost::astronomy::coordinate::concepts

#endif // !BOOST_ASTRONOMY_COORDINATE_CONCEPTS_SPHERICAL_EQUATORIAL_REPRESENTATION_CONCEPT_HPP