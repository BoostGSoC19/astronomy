#ifndef BOOST_ASTRONOMY_COORD_SYS_HPP
#define BOOST_ASTRONOMY_COORD_SYS_HPP

#include <cstddef>
#include <type_traits>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/static_assert.hpp>

namespace boost {
    namespace astronomy {
        namespace coordinate {

            namespace bg = boost::geometry;
            typedef bg::degree degree;
            typedef bg::radian radian;

            template
                    <
                            std::size_t DimensionCount,
                            typename CoordinateSystem,
                            typename CoordinateType=double
                    >
            struct coord_sys {

            protected:
                bg::model::point<CoordinateType, DimensionCount, CoordinateSystem> point;

            public:

                typedef CoordinateSystem system;
                typedef CoordinateType type;

                bg::model::point<CoordinateType, DimensionCount, CoordinateSystem> get_point() const {
                    return this->point;
                }
            }; //coord_sys
        }
    }
}
#endif //BOOST_ASTRONOMY_COORD_SYS_HPP
