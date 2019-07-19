#ifndef BOOST_ASTRONOMY_COORDINATE_SUPERGALACTIC_HPP
#define BOOST_ASTRONOMY_COORDINATE_SUPERGALACTIC_HPP

#include <boost/astronomy/coordinate/base_frame.hpp>
#include <boost/astronomy/coordinate/spherical_representation.hpp>
#include <boost/astronomy/coordinate/spherical_coslat_differential.hpp>

namespace boost { namespace astronomy { namespace coordinate {

template <typename Representation, typename Differential>
struct supergalactic : public base_frame<Representation, Differential>
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
    supergalactic() {}

    //!creates coordinate in supergalactic frame using any subclass of base_representation
    template <typename OtherRepresentation>
    supergalactic(OtherRepresentation const& representation_data)
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
         <boost::astronomy::coordinate::base_representation, OtherRepresentation>::value),
         "Invalid representation class");

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
    //!sgb -> latitude, sgl -> longitude
    supergalactic
    (
        typename Representation::quantity1 const& sgb,
        typename Representation::quantity2 const& sgl,
        typename Representation::quantity3 const& distance
    )
    {
        this->data.set_lat_lon_dist(sgb, sgl, distance);
    }

    //!creates coordinate with motion from given values
    //!sgb -> latitude, sgl -> longitude
    //!pm_sgb -> proper motion in sgb, pm_sgl_cossgb -> proper motion in sgl including cos(sgb) 
    supergalactic
    (
        typename Representation::quantity1 const& sgb,
        typename Representation::quantity2 const& sgl,
        typename Representation::quantity3 const& distance,
        typename Differential::quantity1 const& pm_sgb,
        typename Differential::quantity2 const& pm_sgl_cossgb,
        typename Differential::quantity3 const& radial_velocity
    ) : supergalactic(sgb, sgl, distance)
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_sgb, pm_sgl_cossgb, radial_velocity);
    }

    //!creates coordinate with motion
    //!representation class is used for coordinate data
    //!differential class is used for motion data
    template <typename OtherRepresentation, typename OtherDifferential>
    supergalactic
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    )
    {
        BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
         <boost::astronomy::coordinate::base_representation, OtherRepresentation>::value),
         "argument type is expected to be a differential class");

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

    //copy constructor
    supergalactic(supergalactic<Representation, Differential> const& other)
    {
        this->data = other.get_data();
        this->motion = other.get_differential();
    }

    //!returns component sgb of the supergalactic coordinate
    typename Representation::quantity1 get_sgb() const
    {
        return this->data.get_lat();
    }

    //!returns component sgl of the supergalactic coordinate
    typename Representation::quantity2 get_sgl() const
    {
        return this->data.get_lon();
    }

    //!returns distance component of the supergalactic coordinate
    typename Representation::quantity3 get_distance() const
    {
        return this->data.get_dist();
    }

    //!returns the (sgb, sgl, dist) in the form of tuple
    std::tuple
    <
        typename Representation::quantity1,
        typename Representation::quantity2,
        typename Representation::quantity3
    > get_sgb_sgl_dist() const
    {
        return this->data.get_lat_lon_dist();
    }

    //!returns proper motion in supergalactic latitude
    typename Differential::quantity1 get_pm_sgb() const
    {
        return this->motion.get_dlat();
    }

    //!returns proper motion in supergalactic longitude including cos(b)
    typename Differential::quantity2 get_pm_sgl_cossgb() const
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
    > get_pm_sgb_sgl_radial() const
    {
        return this->motion.get_dlat_dlon_coslat_ddist();
    }

    //!sets value of component b of the supergalactic coordinate
    void set_sgb(typename Representation::quantity1 const& sgb)
    {
        this->data.set_lat(sgb);
    }

    //!sets value of component sgl of the supergalactic coordinate
    void set_sgl(typename Representation::quantity2 const& sgl) const
    {
        this->data.set_lon(sgl);
    }

    //!sets value of distance component of the supergalactic coordinate
    void set_distance(typename Representation::quantity3 const& distance)
    {
        this->data.set_dist(distance);
    }

    //!sets value of all component of the coordinate 
    void set_sgb_sgl_dist
    (
        typename Representation::quantity1 const& sgb,
        typename Representation::quantity2 const& sgl,
        typename Representation::quantity3 const& dist
    )
    {
        this->data.set_lat_lon_dist(sgb, sgl, dist);
    }

    //!sets the proper motion in supergalactic latitude
    void set_pm_sgb(typename Differential::quantity1 const& pm_sgb)
    {
        this->motion.set_dlat(pm_sgb);
    }

    //!sets the proper motion in supergalactic longitude including cos(b)
    void set_pm_sgl_cossgb(typename Differential::quantity2 const& pm_sgl_cossgb)
    {
        this->motion.set_dlon_coslat(pm_sgl_cossgb);
    }

    //!sets the radial_velocity
    void set_radial_velocity(typename Differential::quantity3 const& radial_velocity)
    {
        this->motion.set_ddist(radial_velocity);
    }

    //!set value of motion including cos(b)
    void set_pm_sgb_sgl_radial
    (
        typename Differential::quantity1 const& pm_sgb,
        typename Differential::quantity2 const& pm_sgl_cosb,
        typename Differential::quantity3 const& radial_velocity
    )
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_sgb, pm_sgl_cosb, radial_velocity);
    }

};

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_SUPERGALACTIC_HPP

