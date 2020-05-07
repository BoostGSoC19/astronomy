
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  Copyright 2019-2020 Sarthak Singhal < sarthak2007 - singhalsarthak2007@gmail.com >

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_COORDINATE_CIRS_HPP
#define BOOST_ASTRONOMY_COORDINATE_CIRS_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/astronomy/coordinate/ref_frame/base_equatorial_frame.hpp>

namespace boost { namespace astronomy { namespace coordinate {

template
<
    typename Representation, typename Differential
>
struct cirs : public base_equatorial_frame<Representation, Differential>
{

protected:
    //!time used to determine the position of earth at the time of observation
    boost::posix_time::ptime obs_time; 

public:
    //default constructor no initialization
    cirs() {}

    //!constructs object from another representation object
    template <typename OtherRepresentation>
    cirs(OtherRepresentation const& representation_data) : base_equatorial_frame
        <Representation, Differential>(representation_data) {}

    //!constructs object from provided components of representation
    cirs
    (
        typename Representation::quantity1 const& dec,
        typename Representation::quantity2 const& ra,
        typename Representation::quantity3 const& distance
    ) : base_equatorial_frame<Representation, Differential>(dec, ra, distance) {}

    //!constructs object from provided components of representation and differential
    cirs
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
    cirs
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    ) : base_equatorial_frame<Representation, Differential>
            (representation_data, differential_data) {}

    boost::posix_time::ptime get_obs_time() const
    {
        return this->obstime;
    }

    void set_obs_time(boost::posix_time::ptime const& time)
    {
        this->obstime = time;
    }
};

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_CIRS_HPP

