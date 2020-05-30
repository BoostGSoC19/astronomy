/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_COORDINATE_ICRS_HPP
#define BOOST_ASTRONOMY_COORDINATE_ICRS_HPP

#include <boost/astronomy/coordinate/ref_frame/base_equatorial_frame.hpp>

namespace boost { namespace astronomy { namespace coordinate {

template
<
    typename Representation, typename Differential
>
struct icrs : public base_equatorial_frame<Representation, Differential>
{

public:
    //default constructor no initialization
    icrs() {}

    //!constructs object from another representation object
    template <typename OtherRepresentation>
    icrs(OtherRepresentation const& representation_data) : base_equatorial_frame
        <Representation, Differential>(representation_data) {}

    //!constructs object from provided components of representation
    icrs
    (
        typename Representation::quantity1 const& dec,
        typename Representation::quantity2 const& ra,
        typename Representation::quantity3 const& distance
    ) : base_equatorial_frame<Representation, Differential>(dec, ra, distance) {}

    //!constructs object from provided components of representation and differential
    icrs
    (
        typename Representation::quantity1 const& dec,
        typename Representation::quantity2 const& ra,
        typename Representation::quantity3 const& distance,
        typename Differential::quantity1 const& pm_dec,
        typename Differential::quantity2 const& pm_ra_cosdec,
        typename Differential::quantity3 const& radial_velocity
    ) : base_equatorial_frame<Representation, Differential>
            (dec, ra, distance, pm_dec, pm_ra_cosdec, radial_velocity) {}

    //!constructs object from other representation and differential objects
    template <typename OtherRepresentation, typename OtherDifferential>
    icrs
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    ) : base_equatorial_frame<Representation, Differential>
            (representation_data, differential_data) {}
};

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_ICRS_HPP

