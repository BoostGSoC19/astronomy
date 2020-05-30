/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_COORDINATE_BASE_EQUATORIAL_FRAME_HPP
#define BOOST_ASTRONOMY_COORDINATE_BASE_EQUATORIAL_FRAME_HPP

#include <type_traits>
#include <boost/astronomy/coordinate/ref_frame/base_frame.hpp>
#include <boost/astronomy/coordinate/rep/spherical_representation.hpp>
#include <boost/astronomy/coordinate/diff/spherical_coslat_differential.hpp>

namespace boost { namespace astronomy { namespace coordinate {

namespace bu = boost::units;
namespace bg = boost::geometry;

template
<
    typename Representation, typename Differential
>
struct base_equatorial_frame : public base_frame<Representation, Differential>
{
    ///@cond INTERNAL
    BOOST_STATIC_ASSERT_MSG((std::is_base_of
            <spherical_representation<typename Representation::type,
            typename Representation::quantity1, typename Representation::quantity2,
            typename Representation::quantity3>, Representation>::value),
            "argument type is expected to be a spherical_representation class");
    BOOST_STATIC_ASSERT_MSG((std::is_base_of
            <spherical_coslat_differential<typename Differential::type,
            typename Differential::quantity1, typename Differential::quantity2,
            typename Differential::quantity3>, Differential>::value),
            "argument type is expected to be a spherical_coslat_differential class");
    ///@endcond

public:
    //default constructor no initialization
    base_equatorial_frame() {}

    //!constructs object from another representation object
    template <typename OtherRepresentation>
    base_equatorial_frame(OtherRepresentation const& representation_data)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, OtherRepresentation>::value),
            "argument type is expected to be a representation class");

        auto temp = make_spherical_representation
            <
                typename Representation::type,
                typename Representation::quantity1,
                typename Representation::quantity2,
                typename Representation::quantity3,
                typename OtherRepresentation::type,
                typename OtherRepresentation::quantity1,
                typename OtherRepresentation::quantity2,
                typename OtherRepresentation::quantity3
            >(representation_data);
        this->data = temp;
    }

    //!constructs object from provided components of representation
    base_equatorial_frame
    (
        typename Representation::quantity1 const& dec,
        typename Representation::quantity2 const& ra,
        typename Representation::quantity3 const& distance
    )
    {
        this->set_dec(dec);
        this->set_ra(ra);
        this->set_distance(distance);
    }

    //!constructs object from provided components of representation and differential
    base_equatorial_frame
    (
        typename Representation::quantity1 const& dec,
        typename Representation::quantity2 const& ra,
        typename Representation::quantity3 const& distance,
        typename Differential::quantity1 const& pm_dec,
        typename Differential::quantity2 const& pm_ra_cosdec,
        typename Differential::quantity3 const& radial_velocity
    ) : base_equatorial_frame(dec, ra, distance)
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_dec, pm_ra_cosdec, radial_velocity);
    }

    //!constructs object from other representation and differential
    template <typename OtherRepresentation, typename OtherDifferential>
    base_equatorial_frame
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    )
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, OtherRepresentation>::value),
            "argument type is expected to be a representation class");

        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, OtherDifferential>::value),
            "argument type is expected to be a differential class");

        auto rep_temp = make_spherical_representation
            <
                typename Representation::type,
                typename Representation::quantity1,
                typename Representation::quantity2,
                typename Representation::quantity3,
                typename OtherRepresentation::type,
                typename OtherRepresentation::quantity1,
                typename OtherRepresentation::quantity2,
                typename OtherRepresentation::quantity3
            >(representation_data);
        this->data = rep_temp;

        auto dif_temp = make_spherical_coslat_differential
            <
                typename Differential::type,
                typename Differential::quantity1,
                typename Differential::quantity2,
                typename Differential::quantity3,
                typename OtherDifferential::type,
                typename OtherDifferential::quantity1,
                typename OtherDifferential::quantity2,
                typename OtherDifferential::quantity3
            >(differential_data);
        this->motion = dif_temp;
    }

    //!returns Declination component of the coordinate
    typename Representation::quantity1 get_dec() const
    {
        return this->data.get_lat();
    }

    //!returns Right Ascension component of the coordinate
    typename Representation::quantity2 get_ra() const
    {
        return this->data.get_lon();
    }

    //!returns distance component of the coordinate
    typename Representation::quantity3 get_distance() const
    {
        return this->data.get_dist();
    }

    //!returns the (dec, ra, dist) in the form of tuple
    std::tuple
    <
        typename Representation::quantity1,
        typename Representation::quantity2,
        typename Representation::quantity3
    > get_dec_ra_dist() const
    {
        return this->data.get_lat_lon_dist();
    }

    //!returns proper motion in Declination
    typename Differential::quantity1 get_pm_dec() const
    {
        return this->motion.get_dlat();
    }

    //!returns proper motion in Right Ascension including cos(dec)
    typename Differential::quantity2 get_pm_ra_cosdec() const
    {
        return this->motion.get_dlon_coslat();
    }

    //!returns radial_velocity
    typename Differential::quantity3 get_radial_velocity() const
    {
        return this->motion.get_ddist();
    }

    //!returns the proper motion in form of tuple including cos(dec)
    std::tuple
    <
        typename Differential::quantity1,
        typename Differential::quantity2,
        typename Differential::quantity3
    > get_pm_dec_ra_radial() const
    {
        return this->motion.get_dlat_dlon_coslat_ddist();
    }

    //!sets value of Declination component of the coordinate
    void set_dec(typename Representation::quantity1 const& dec)
    {
        this->data.set_lat(dec);
    }

    //!sets value of Right Ascension component of the coordinate
    void set_ra(typename Representation::quantity2 const& ra)
    {
        this->data.set_lon(ra);
    }

    //!sets value of distance component of the coordinate
    void set_distance(typename Representation::quantity3 const& distance)
    {
        this->data.set_dist(distance);
    }

    //!sets values of all component of the coordinate
    void set_dec_ra_dist
    (
        typename Representation::quantity1 const& dec,
        typename Representation::quantity2 const& ra,
        typename Representation::quantity3 const& dist
    )
    {
        this->set_dec(dec);
        this->set_ra(ra);
        this->set_distance(dist);
    }

    //!sets the proper motion in Declination
    void set_pm_dec(typename Differential::quantity1 const& pm_dec)
    {
        this->motion.set_dlat(pm_dec);
    }

    //!sets the proper motion in Right Ascension including cos(dec)
    void set_pm_ra_cosdec(typename Differential::quantity2 const& pm_ra_cosdec)
    {
        this->motion.set_dlon_coslat(pm_ra_cosdec);
    }

    //!sets the radial_velocity
    void set_radial_velocity(typename Differential::quantity3 const& radial_velocity)
    {
        this->motion.set_ddist(radial_velocity);
    }

    //!set value of motion including cos(dec)
    void set_pm_dec_ra_radial
    (
        typename Differential::quantity1 const& pm_dec,
        typename Differential::quantity2 const& pm_ra_cosdec,
        typename Differential::quantity3 const& radial_velocity
    )
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_dec, pm_ra_cosdec, radial_velocity);
    }
}; //base_equatorial_frame

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_BASE_EQUATORIAL_FRAME_HPP

