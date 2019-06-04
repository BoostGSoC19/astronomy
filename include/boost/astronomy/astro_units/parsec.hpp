/* 
  * 1 pc = 3.086e+16 meters
*/

#ifndef BOOST_ASTRONOMY_ASTRO_UNITS_PARSEC_HPP
#define BOOST_ASTRONOMY_ASTRO_UNITS_PARSEC_HPP

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
				typedef boost::units::scaled_base_unit< boost::units::si::meter_base_unit, boost::units::scale<30856778200000000, boost::units::static_rational<1> > > parsec;
				
			} //namespace astro_units
			 
		}//namespace Astronomy

	        template<>
            struct boost::units::base_unit_info<astronomy::astro_units::parsec>
            {
                static BOOST_CONSTEXPR const char* name()
                { 
                	return("Parsec"); 
                }
                static BOOST_CONSTEXPR const char* symbol()
                { 
                	return("pc"); 
                }
			};

	} //namespace Boost

#endif // !BOOST_ASTRONOMY_ASTRO_UNITS_PARSEC_HPP
