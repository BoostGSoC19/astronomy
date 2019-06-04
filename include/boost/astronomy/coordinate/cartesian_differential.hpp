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
            struct cartesian_differential : public boost::astronomy::coordinate::base_differential
                <3, boost::geometry::cs::cartesian>
            {
            public:
                // default constructor no initialization
                cartesian_differential() {}

                //!constructs object from provided value of differential
                cartesian_differential(double dx, double dy, double dz = 0.0)
                {
                    boost::geometry::set<0>(this->diff, dx);
                    boost::geometry::set<1>(this->diff, dy);
                    boost::geometry::set<2>(this->diff, dz);
                }

                //!constructs object from boost::geometry::model::point object
                template <std::size_t DimensionCount, typename System>
                cartesian_differential(boost::geometry::model::point<double, DimensionCount, System> const& pointObject)
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

                //! returns the (dx, dy, dz) in the form of tuple
                std::tuple<double, double, double> get_dx_dy_dz() const
                {
                    return std::make_tuple(boost::geometry::get<0>(this->diff),
                        boost::geometry::get<1>(this->diff), boost::geometry::get<2>(this->diff));
                }

                //!returns the dx component of differential
                double get_dx() const
                {
                    return boost::geometry::get<0>(this->diff);
                }

                //!returns the dy component of differential
                double get_dy() const
                {
                    return boost::geometry::get<1>(this->diff);
                }

                //!returns the dz component of differential
                double get_dz() const
                {
                    return boost::geometry::get<2>(this->diff);
                }

                //!set value of (dx, dy, dz) in current object
                void set_dx_dy_dz(double dx, double dy, double dz)
                {
                    boost::geometry::set<0>(this->diff, dx);
                    boost::geometry::set<1>(this->diff, dy);
                    boost::geometry::set<2>(this->diff, dz);
                }

                //!set value of dx component of differential
                void set_dx(double dx)
                {
                    boost::geometry::set<0>(this->diff, dx);
                }

                //!set value of dy component of differential
                void set_dy(double dy)
                {
                    boost::geometry::set<1>(this->diff, dy);
                }

                //!set value of dz component of differential
                void set_dz(double dz)
                {
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
                    operator *(double multiplier) const
                {
                    boost::astronomy::coordinate::cartesian_differential temp(this->diff);

                    temp.set_dx(temp.get_dx() * multiplier);
                    temp.set_dy(temp.get_dy() * multiplier);
                    temp.set_dz(temp.get_dz() * multiplier);

                    return temp;
                }
            };

        }// namespace coordinate
    } // namespace astronomy
} // namespace boost

#endif // !BOOST_ASTRONOMY_COORDINATE_cartesian_DIFFERENTIAL_HPP

