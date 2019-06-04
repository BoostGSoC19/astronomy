/* 
  * So, An Astronomical Unit (AU) is the average distance between Earth and the Sun.
  * 1 AU = 149597870691 meters
*/

#ifndef BOOST_ASTRONOMY_ASTRO_UNITS_ASTRONOMICAL_UNIT_HPP
#define BOOST_ASTRONOMY_ASTRO_UNITS_ASTRONOMICAL_UNIT_HPP

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
				typedef boost::units::scaled_base_unit< boost::units::si::meter_base_unit, boost::units::scale<149597870691, boost::units::static_rational<1> > > astronomical_unit;

			} // namespace astro_units

		}// namespace Astronomy
	        template<>
            struct boost::units::base_unit_info<astronomy::astro_units::astronomical_unit>
            {
                static BOOST_CONSTEXPR const char* name()
                { 
                	return("Astronomical unit"); 
                }
                static BOOST_CONSTEXPR const char* symbol()
                { 
                	return("AU"); 
                }
			};
	} // namespace Boost

#endif // !BOOST_ASTRONOMY_ASTRO_UNITS_ASTRONOMICAL_UNIT_HPP
