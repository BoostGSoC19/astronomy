#ifndef BOOST_ASTRONOMY_HORIZON_COORD_HPP
#define BOOST_ASTRONOMY_HORIZON_COORD_HPP

#include <iostream>
#include <boost/static_assert.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/units/physical_dimensions/plane_angle.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/dimensionless.hpp>

namespace boost {
    namespace astronomy {
        namespace coordinate {

            namespace bu = boost::units;
            namespace bg = boost::geometry;

            template
                    <
                            typename CoordinateType = double,
                            typename AltitudeQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>,
                            typename AzimuthQuantity = bu::quantity<bu::si::plane_angle, CoordinateType>
                    >
            struct horizon_coord : public coord_sys
                    <2, bg::cs::spherical<degree>, CoordinateType> {
            public:
                typedef AltitudeQuantity quantity1;
                typedef AzimuthQuantity quantity2;

                //Default constructor
                horizon_coord() {}

                horizon_coord
                        (
                                AltitudeQuantity const &Altitude,
                                AzimuthQuantity const &Azimuth
                        )
                        {
                    this->set_Altitude_Azimuth(Altitude, Azimuth);
                }

                std::tuple<AltitudeQuantity, AzimuthQuantity> get_altitude_azimuth() const {
                    return std::make_tuple(this->get_Altitude(), this->get_Azimuth());
                }

                AltitudeQuantity get_Altitude() const {
                    return static_cast<AltitudeQuantity>
                    (
                            bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                                    (bg::get<0>(this->point))
                    );
                }

                AzimuthQuantity get_Azimuth() const {
                    return static_cast<AzimuthQuantity>
                    (
                            bu::quantity<bu::si::plane_angle, CoordinateType>::from_value
                                    (bg::get<1>(this->point))
                    );
                }

                //Set value of Altitude and Azimuth
                void set_Altitude_Azimuth
                        (
                                AltitudeQuantity const &Altitude,
                                AzimuthQuantity const &Azimuth
                        ) {
                    this->set_Altitude(Altitude);
                    this->set_Azimuth(Azimuth);
                }

                //Set Altitude
                void set_Altitude(AltitudeQuantity const &Altitude) {
                    bg::set<0>
                            (
                                    this->point,
                                    static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(Altitude).value()
                            );
                }

                //Set Azimuth
                void set_Azimuth(AzimuthQuantity const &Azimuth) {
                    bg::set<1>
                            (
                                    this->point,
                                    static_cast<bu::quantity<bu::si::plane_angle, CoordinateType>>(Azimuth).value()
                            );
                }

            }; //horizon_coord

            template
                    <
                            typename CoordinateType,
                            template<typename Unit1, typename CoordinateType_> class AltitudeQuantity,
                            template<typename Unit2, typename CoordinateType_> class AzimuthQuantity,
                            typename Unit1,
                            typename Unit2
                    >
            horizon_coord
                    <
                            CoordinateType,
                            AltitudeQuantity<Unit1, CoordinateType>,
                            AzimuthQuantity<Unit2, CoordinateType>
                    > make_horizon_coord
                    (
                            AltitudeQuantity<Unit1, CoordinateType> const &Altitude,
                            AzimuthQuantity<Unit2, CoordinateType> const &Azimuth
                    ) {
                return horizon_coord
                        <
                                CoordinateType,
                                AltitudeQuantity<Unit1, CoordinateType>,
                                AzimuthQuantity<Unit2, CoordinateType>
                        >(Altitude, Azimuth);
            }

            //Print Horizon Coordinates
            template
                    <
                            typename CoordinateType,
                            class AltitudeQuantity,
                            class AzimuthQuantity
                    >
            std::ostream& operator<< (std::ostream &out, horizon_coord
            <CoordinateType, AltitudeQuantity, AzimuthQuantity> const& point)
            {
                out << "Horizon Coordinate (Altitude: "
                    << point.get_Altitude() << " , Azimuth: "
                    << point.get_Azimuth() << ")";

                return out;
            }

        }
    }
}

#endif //BOOST_ASTRONOMY_HORIZON_COORD_HPP
