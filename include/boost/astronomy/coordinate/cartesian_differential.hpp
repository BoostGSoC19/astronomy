#ifndef BOOST_ASTRONOMY_COORDINATE_CARTESIAN_DIFFERENTIAL_HPP
#define BOOST_ASTRONOMY_COORDINATE_CARTESIAN_DIFFERENTIAL_HPP

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/static_assert.hpp>

#include <boost/astronomy/detail/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_differential.hpp>


namespace boost
{
    namespace astronomy
    {
        namespace coordinate
        {
            //! represents differential of cartesian representation
            //! uses three coordinate to represent a differential (dx, dy, dz)
            struct cartesian_differential : public boost::astronomy::coordinate::base_differential<CoordinateType,3, boost::geometry::cs::cartesian>
            {
            public:
                // default constructor no initialization
                cartesian_differential() {}

                //!constructs object from provided value of differential
                cartesian_differential(CoordinateType dx, CoordinateType dy, CoordinateType dz = 0.0)
                {
                    boost::geometry::set<0>(this->diff, dx);
                    boost::geometry::set<1>(this->diff, dy);
                    boost::geometry::set<2>(this->diff, dz);
                }

                //!constructs object from boost::geometry::model::point object
                template <std::size_t DimensionCount, typename CoordinateSystem>
                cartesian_differential(boost::geometry::model::point<CoordinateType, DimensionCount, CoordinateSystem> const& pointObject)
                {
                    boost::geometry::transform(pointObject, this->diff);
                }

                //!copy constructor
                cartesian_differential(cartesian_differential const& object)
                {
                    boost::geometry::transform(object.get_differential(), this->diff);
                }

                //!constructs object from any type of differential
                template <typename Differential>
                cartesian_differential(Differential const& other)
                {
                    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
                        <boost::astronomy::coordinate::base_differential, Differential>::value),
                        "No constructor found with given argument type");

                    boost::geometry::transform(other.get_differential(), this->diff);
                }

                //!returns the dx component of differential
                CoordinateType get_dx() const
                {
                    return boost::geometry::get<0>(this->diff);
                }

                //!returns the dy component of differential
                CoordinateType get_dy() const
                {
                    return boost::geometry::get<1>(this->diff);
                }

                //!returns the dz component of differential
                CoordinateType get_dz() const
                {
                    return boost::geometry::get<2>(this->diff);
                }

                //! returns the (dx, dy, dz) in the form of tuple
                std::tuple<CoordinateType, CoordinateType, CoordinateType> get_coordinates() const
                {
                    return std::make_tuple(get_dx(),get_dy(),get_dz());
                }

                //!set value of dx component of differential
                void set_dx(CoordinateType dx)
                {
                    boost::geometry::set<0>(this->diff, dx);
                }

                //!set value of dy component of differential
                void set_dy(CoordinateType dy)
                {
                    boost::geometry::set<1>(this->diff, dy);
                }

                //!set value of dz component of differential
                void set_dz(CoordinateType dz)
                {
                    boost::geometry::set<2>(this->diff, dz);
                }

                //!set value of (dx, dy, dz) in current object
                void set_coordinates(CoordinateType dx, CoordinateType dy, CoordinateType dz)
                {
                    boost::geometry::set<0>(this->diff, dx);
                    boost::geometry::set<1>(this->diff, dy);
                    boost::geometry::set<2>(this->diff, dz);
                }


                boost::astronomy::coordinate::cartesian_differential
                operator +(boost::astronomy::coordinate::cartesian_differential const& diff) const
                {
                    boost::astronomy::coordinate::cartesian_differential temp(this->diff);

                    temp.set_dx(temp.get_dx() + diff.get_dx());
                    temp.set_dy(temp.get_dy() + diff.get_dy());
                    temp.set_dz(temp.get_dz() + diff.get_dz());

                    return temp;
                }

                boost::astronomy::coordinate::cartesian_differential
                    operator *(double scalar) const
                {
                    boost::astronomy::coordinate::cartesian_differential temp(this->diff);

                    temp.set_dx(temp.get_dx() * scalar);
                    temp.set_dy(temp.get_dy() * scalar);
                    temp.set_dz(temp.get_dz() * scalar);

                    return temp;
                }
            };

        }// namespace coordinate
    } // namespace astronomy
} // namespace boost

#endif // !BOOST_ASTRONOMY_COORDINATE_cartesian_DIFFERENTIAL_HPP

