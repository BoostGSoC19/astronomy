#ifndef BOOST_ASTRONOMY_COORDINATE_ALT_AZ_HPP
#define BOOST_ASTRONOMY_COORDINATE_ALT_AZ_HPP

#include <type_traits>
#include <tuple>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>
#include <boost/units/quantity.hpp>
#include <boost/unis/systems/si/base.hpp>
#include <boost/units/systems/si/pressure.hpp>
#include <boost/units/systems/temperature/celsius.hpp>
#include <boost/units/system/angle/degree.hpp>
#include <boost/units/systems/si/volume.hpp>
#include <boost/units/systems/si/mass.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/static_assert.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_frame.hpp>
#include <boost/astronomy/coordinate/representation.hpp>
#include <boost/astronomy/coordinate/differential.hpp>


namespace boost { namespace astronomy { namespace coordinate {

template <typename Representation, typename Differential>
struct alt_az : public base_frame<Representation, Differential>
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

protected:
    //frame parameters
    spherical_equatorial_representation
        <
            double,
            bu::quantity<bu::degree::plane_angle>,
            bu::quantity<bu::degree::plane_angle>
        > earth_location;

    bu::quantity<bu::si::pressure> pressure = 0.0 * bu::si::pascals;
    bu::quantity<bu::celsius::temperature> temperature = 0.0 * bu::celsius::degrees;
    boost::posix_time::ptime obs_time;
    bu::quantity<bu::si::dimensionless> relative_humidity = 0.0;

public:
    alt_az() {}

    template <typename OtherRepresentation>
    alt_az(OtherRepresentation const& representation_data)
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

    alt_az
    (
        Representation::quantity1 const& alt,
        Representation::quantity2 const& az,
        Representation::quantity3 const& distance
    )
    {
        this->data.set_lat_lon_dist(alt, az, distance);
    }

    alt_az
    (
        Representation::quantity1 const& alt,
        Representation::quantity2 const& az,
        Representation::quantity3 const& distance,
        Differential::quantity1 const& pm_alt,
        Differential::quantity2 const& pm_az_cosalt,
        Differential::quantity3 const& radial_velocity) :
        alt_az(alt, az, distance)
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_alt, pm_az_cosalt, radial_velocity);
    }

    template <typename OtherRepresentation, typename OtherDifferential>
    alt_az
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

    alt_az(alt_az<Representation, Differential> const& other)
    {
        this->data = other.get_data();
        this->motion = other.get_differential();
        this->earth_location = other.get_location();
        this->obs_time = other.get_obs_time();
        this->pressure = other.get_pressure();
        this->temperature = other.get_temprature();
        this->relative_humidity = other.get_relative_humidity();
    }

    //!returns altitude component of the coordinate
    typename Representation::quantity1 get_alt() const
    {
        return this->data.get_lat();
    }

    //!returns azimuth component of the coordinate
    typename Representation::quantity2 get_az() const
    {
        return this->data.get_lon();
    }

    //!returns distance component of the coordinate
    typename Representation::quantity3 get_distance() const
    {
        return this->data.get_dist();
    }

    //!returns the (alt, az, dist) in the form of tuple
    std::tuple
    <
        typename Representation::quantity1,
        typename Representation::quantity2,
        typename Representation::quantity3
    > get_alt_az_dist() const
    {
        return this->data.get_lat_lon_dist();
    }

    //!returns proper motion in altitude
    typename Differential::quantity1 get_pm_alt() const
    {
        return this->motion.get_dlat();
    }

    //!returns proper motion in azimuth including cos(alt)
    typename Differential::quantity2 get_pm_az_cosalt() const
    {
        return this->motion.get_dlon_coslat();
    }

    //!returns radial_velocity
    typename Differential::quantity3 get_radial_velocity() const
    {
        return this->motion.get_ddist();
    }

    //!returns the proper motion in tuple form
    std::tuple
    <
        typename Differential::quantity1,
        typename Differential::quantity2,
        typename Differential::quantity3
    > get_pm_alt_az_radial() const
    {
        return this->motion.get_dlat_dlon_coslat_ddist();
    }

    //!sets value of altitude component of the coordinate
    void set_alt(typename Representation::quantity1 const& alt)
    {
        this->data.set_lat(alt);
    }

    //!sets value of azimuth component of the coordinate
    void set_az(typename Representation::quantity3 const& az)
    {
        this->data.set_lon(az);
    }

    //!sets value of distance component of the coordinate
    void set_distance(typename Representation::quantity3 const& distance)
    {
        this->data.set_dist(distance);
    }

    //!sets value of all component of the coordinate including cos(alt)
    void set_alt_az_dist
    (
        typename Representation::quantity1 const& alt,
        typename Representation::quantity2 const& az,
        typename Representation::quantity3 const& dist)
    {
        this->data.set_lat_lon_dist(alt, az, dist);
    }

    //!sets the proper motion in altitude
    void set_pm_alt(typename Differential::quantity1 const& pm_alt)
    {
        this->motion.set_dlat(pm_alt);
    }

    //!sets the proper motion in azimuth including cos(alt)
    void set_pm_az_cosalt(typename Differential::quantity2 const& pm_az_cosalt)
    {
        this->motion.set_dlon_coslat(pm_az_cosalt);
    }

    //!sets the radial_velocity
    void set_radial_velocity(typename Differential::quantity3 const& radial_velocity)
    {
        this->motion.set_ddist(radial_velocity);
    }

    //!set value of motion
    void set_pm_alt_az_radial
    (
        typename Differential::quantity1 const& pm_alt,
        typename Differential::quantity2 const& pm_az_cosalt,
        typename Differential::quantity3 const& radial_velocity
    )
    {
        this->motion.set_dlat_dlon_coslat_ddist(pm_alt, pm_az_cosalt, radial_velocity);
    }

    //!set all the perameters of current frame object
    void set_frame_parameters
    (
        spherical_equatorial_representation
        <
            double,
            bu::quantity<bu::degree::plane_angle>,
            bu::quantity<bu::degree::plane_angle>
        > const& location,
        bu::quantity<bu::si::pressure> const& pressure,
        bu::quantity<bu::celsius::temperature> const& temperature,
        boost::posix_time::ptime const& obs_time,
        bu::quantity<bu::si::dimensionless> const& relative_humidity
    )
    {
        this->earth_location = location;
        this->pressure = pressure;
        this->temperature = temperature;
        this->obs_time = obs_time;
        this->relative_humidity = relative_humidity;
    }

    //!get all the perameters of current frame object
    std::tuple<                
        spherical_equatorial_representation
        <
            double,
            bu::quantity<bu::degree::plane_angle>,
            bu::quantity<bu::degree::plane_angle>
        >,
        bu::quantity<bu::si::pressure>,
        bu::quantity<bu::celsius::temperature>,
        boost::posix_time::ptime,
        bu::quantity<bu::si::dimensionless>
    > get_frame_parameters() const
    {
        return std::make_tuple
        (
            this->earth_location,
            this->pressure,
            this->temperature,
            this->obs_time,
            this->relative_humidity
        );
    }

    //!get earth location of the current object
    spherical_equatorial_representation
    <
        double,
        bu::quantity<bu::degree::plane_angle>,
        bu::quantity<bu::degree::plane_angle>
    > get_location() const
    {
        return this->earth_location;
    }

    //!set earth location of the current object
    void set_location
    (
        spherical_equatorial_representation
        <
            double,
            bu::quantity<bu::degree::plane_angle>,
            bu::quantity<bu::degree::plane_angle>
        > const& location
    )
    {
        this->earth_location = location;
    }

    //!get atmospheric pressure
    bu::quantity<bu::si::pressure> get_pressure() const
    {
        return this->pressure;
    }

    //!set atmospheric pressure
    void set_pressure(bu::quantity<bu::si::pressure> const& pressure)
    {
        this->pressure = pressure;
    }

    //!get temperature of the location
    bu::quantity<bu::celsius::temperature> get_temprature() const
    {
        return this->temperature;
    }

    //!set temperature of the location
    void set_temprature(bu::quantity<bu::celsius::temperature> const& temperature)
    {
        this->temperature = temperature;
    }

    //!get observation time 
    boost::posix_time::ptime get_obs_time() const
    {
        return this->obs_time;
    }

    //!set observation time 
    void set_obs_time(boost::posix_time::ptime const& time)
    {
        this->obs_time = time;
    }

    //!get relative humidity
    bu::quantity<bu::si::dimensionless> get_relative_humidity() const
    {
        return this->relative_humidity;
    }

    //!set relative humidity
    void set_relative_humidity(bu::quantity<bu::si::dimensionless> const& humidity)
    {
        this->relative_humidity = humidity;
    }
};
}}} //namespace boost::astronomy::cordinate
#endif // !BOOST_ASTRONOMY_COORDINATE_ALT_AZ_HPP

