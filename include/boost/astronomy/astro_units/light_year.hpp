/* 
  * 1 ly = 9.461e+15 meters
*/

#ifndef BOOST_ASTRONOMY_ASTRO_UNITS_LIGHT_YEAR_HPP
#define BOOST_ASTRONOMY_ASTRO_UNITS_LIGHT_YEAR_HPP

#include <boost/units/base_units/si/meter.hpp>
#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/scale.hpp>

namespace boost 
	{ 
		namespace astronomy 
		{ 
			namespace astro_units 
			{
				//Scaling from meter_base_unit
				typedef boost::units::scaled_base_unit< boost::units::si::meter_base_unit, boost::units::scale<9460730472580800, boost::units::static_rational<1> > > light_year;
				
			} //namespace astro_units 
		}//namespace Astronomy

	        template<>
            struct boost::units::base_unit_info<astronomy::astro_units::light_year>
            {
                static BOOST_CONSTEXPR const char* name()
                { 
                	return("Light year"); 
                }
                static BOOST_CONSTEXPR const char* symbol()
                { 
                	return("ly"); 
                }
			};

	} //namespace Boost

#endif // !BOOST_ASTRONOMY_ASTRO_UNITS_LIGHT_YEAR_HPP