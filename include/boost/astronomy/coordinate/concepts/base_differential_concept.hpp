#ifndef BOOST_ASTRONOMY_COORDINATE_CONCEPTS_BASE_DIFFERENTIAL_CONCEPT_HPP
#define BOOST_ASTRONOMY_COORDINATE_CONCEPTS_BASE_DIFFERENTIAL_CONCEPT_HPP

//!Header for base differential concept checking

#include <boost/concept_check.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/coordinate_dimension.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_differential.hpp>
#include <boost/astronomy/coordinate/concepts/general.hpp>

namespace boost { namespace astronomy { namespace coordinate { namespace concepts {

namespace bu = boost::units;
namespace bg = boost::geometry;

//!Concept checking class for base differential class
template<typename Differential>
class Base_Differential
{
private:

	//Value to check if the Differential class is a sub class of base_differential
	static const bool isBaseDifferential = boost::astronomy::detail::is_base_template_of
        <boost::astronomy::coordinate::base_differential, Differential>::value;

    Differential R;
    typedef decltype(R.get_differential()) pointType;

    //point traits
    typedef typename bg::coordinate_type<pointType>::type cType;
    typedef typename bg::coordinate_system<pointType>::type cSystem;
    static const size_t dCount = bg::dimension<pointType>::value;

public:

	//Asserting pointType to model Point concept
	BOOST_CONCEPT_ASSERT((Point<cType,dCount,cSystem>));

	BOOST_CONCEPT_USAGE(Base_Differential)
	{
		//Asserting differential class to be a sub class of base_differential
		BOOST_STATIC_ASSERT_MSG(isBaseDifferential,"Not a valid differential class");	
	}
};

}}}} //namespace boost::astronomy::coordinate::concepts

#endif // !BOOST_ASTRONOMY_COORDINATE_CONCEPTS_BASE_DIFFERENTIAL_CONCEPT_HPP