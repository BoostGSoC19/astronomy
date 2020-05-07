
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  Copyright 2019-2020 Sarthak Singhal < sarthak2007 - singhalsarthak2007@gmail.com >

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_COORDINATE_GEOCENTRIC_HPP
#define BOOST_ASTRONOMY_COORDINATE_GEOCENTRIC_HPP

#include <boost/astronomy/coordinate/base_ecliptic_frame.hpp>

namespace boost { namespace astronomy { namespace coordinate {

template
<
    typename Representation, typename Differential
>
struct geocentric : public base_ecliptic_frame<Representation, Differential>
{

public:
    //default constructor no initialization
    geocentric() {}

    //!constructs object from another representation object
    template <typename OtherRepresentation>
    geocentric(OtherRepresentation const& representation_data) : base_ecliptic_frame
        <Representation, Differential>(representation_data) {}

    //!constructs object from provided components of representation
    geocentric
    (
        typename Representation::quantity1 const& lat,
        typename Representation::quantity2 const& lon,
        typename Representation::quantity3 const& distance
    ) : base_ecliptic_frame<Representation, Differential>(lat, lon, distance) {}

    //!constructs object from provided components of representation and differential
    geocentric
    (
        typename Representation::quantity1 const& lat,
        typename Representation::quantity2 const& lon,
        typename Representation::quantity3 const& distance,
        typename Differential::quantity1 const& pm_lat,
        typename Differential::quantity2 const& pm_lon_coslat,
        typename Differential::quantity3 const& radial_velocity
    ) : base_ecliptic_frame<Representation, Differential>
            (lat, lon, distance, pm_lat, pm_lon_coslat, radial_velocity) {}

    //!constructs object from other representation and differential objects
    template <typename OtherRepresentation, typename OtherDifferential>
    geocentric
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    ) : base_ecliptic_frame<Representation, Differential>
            (representation_data, differential_data) {}
};

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_GEOCENTRIC_HPP

