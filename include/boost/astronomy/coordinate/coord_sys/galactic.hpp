
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  Copyright 2019-2020 Sarthak Singhal < sarthak2007 - singhalsarthak2007@gmail.com >

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_COORDINATE_GALACTIC_HPP
#define BOOST_ASTRONOMY_COORDINATE_GALACTIC_HPP

#include <boost/astronomy/coordinate/ref_frame/base_frame.hpp>
#include <boost/astronomy/coordinate/rep/spherical_representation.hpp>
#include <boost/astronomy/coordinate/diff/spherical_coslat_differential.hpp>

namespace boost { namespace astronomy { namespace coordinate {

template <typename Representation, typename Differential>
struct galactic: public base_frame<Representation, Differential>
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
    galactic() {}

    //!creates coordinate in galactic frame using any subclass of base_representation
    template <typename OtherRepresentation>
    galactic(OtherRepresentation const& representation_data)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, OtherRepresentation>
            ::value), "argument type is expected to be a representation class");

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

    //!creates coordinate from given values
    //!b -> latitude, l -> longitude
    galactic
    (
        typename Representation::quantity1 const& b,
        typename Representation::quantity2 const& l,
        typename Representation::quantity3 const& distance
    )
    {
        this->data.set_lat_lon_dist(b, l, distance);
    }

    //!creates coordinate with motion from given values
    //!b -> latitude, l -> longitude
    //!pm_b -> proper motion in b, pm_l_cosb -> proper motion in l including cos(b) 
    galactic
    (
        typename Representation::quantity1 const& b,
        typename Representation::quantity2 const& l,
        typename Representation::quantity3 const& distance,
        typename Differential::quantity1 const& pm_b,
        typename Differential::quantity2 const& pm_l_cosb,
        typename Differential::quantity3 const& radial_velocity
    ) : galactic(b, l, distance)
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_b, pm_l_cosb, radial_velocity);
    }

    //!creates coordinate with motion
    //!representation class is used for coordinate data
    //!differential class is used for motion data
    template <typename OtherRepresentation, typename OtherDifferential>
    galactic
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    )
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_representation, OtherRepresentation>
            ::value), "argument type is expected to be a representation class");
        
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
            <boost::astronomy::coordinate::base_differential, OtherDifferential>
            ::value), "argument type is expected to be a differential class");

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

    //copy constructor
    galactic(galactic<Representation, Differential> const& other)
    {
        this->data = other.get_data();
        this->motion = other.get_differential();
    }

    //!returns component b of the galactic coordinate
    typename Representation::quantity1 get_b() const
    {
        return this->data.get_lat();
    }

    //!returns component l of the galactic coordinate
    typename Representation::quantity2 get_l() const
    {
        return this->data.get_lon();
    }

    //!returns distance component of the galactic coordinate
    typename Representation::quantity3 get_distance() const
    {
        return this->data.get_dist();
    }

    //!returns the (b, l, dist) in the form of tuple
    std::tuple
    <
        typename Representation::quantity1,
        typename Representation::quantity2,
        typename Representation::quantity3
    > get_b_l_dist() const
    {
        return this->data.get_lat_lon_dist();
    }

    //!returns proper motion in galactic latitude
    typename Differential::quantity1 get_pm_b() const
    {
        return this->motion.get_dlat();
    }

    //!returns proper motion in galactic longitude including cos(b)
    typename Differential::quantity2 get_pm_l_cosb() const
    {
        return this->motion.get_dlon_coslat();
    }

    //!returns radial_velocity
    typename Differential::quantity3 get_radial_velocity() const
    {
        return this->motion.get_ddist();
    }

    //!returns the proper motion in form of tuple including cos(b)
    std::tuple
    <
        typename Differential::quantity1,
        typename Differential::quantity2,
        typename Differential::quantity3
    > get_pm_b_l_radial() const
    {
        return this->motion.get_dlat_dlon_coslat_ddist();
    }

    //!sets value of component b of the galactic coordinate
    void set_b(typename Representation::quantity1 const& b)
    {
        this->data.set_lat(b);
    }

    //!sets value of component l of the galactic coordinate
    void set_l(typename Representation::quantity2 const& l)
    {
        this->data.set_lon(l);
    }

    //!sets value of distance component of the galactic coordinate
    void set_distance(typename Representation::quantity3 const& distance)
    {
        this->data.set_dist(distance);
    }

    //!sets values of all component of the coordinate
    void set_b_l_dist
    (
        typename Representation::quantity1 const& b,
        typename Representation::quantity2 const& l,
        typename Representation::quantity3 const& dist
    )
    {
        this->data.set_lat_lon_dist(b, l, dist);
    }

    //!sets the proper motion in galactic latitude
    void set_pm_b(typename Differential::quantity1 const& pm_b)
    {
        this->motion.set_dlat(pm_b);
    }

    //!sets the proper motion in galactic longitude including cos(b)
    void set_pm_l_cosb(typename Differential::quantity2 const& pm_l_cosb)
    {
        this->motion.set_dlon_coslat(pm_l_cosb);
    }

    //!sets the radial_velocity
    void set_radial_velocity(typename Differential::quantity3 const& radial_velocity)
    {
        this->motion.set_ddist(radial_velocity);
    }

    //!set value of motion including cos(b)
    void set_pm_b_l_radial
    (
        typename Differential::quantity1 const& pm_b,
        typename Differential::quantity2 const& pm_l_cosb,
        typename Differential::quantity3 const& radial_velocity
    )
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_b, pm_l_cosb, radial_velocity);
    }
};

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_GALACTIC_HPP

