#ifndef BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
#define BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP

#include <string>
#include <cstddef>
#include <vector>

#include <boost/static_assert.hpp>

#include <boost/astronomy/io/column.hpp>

namespace boost { namespace astronomy { namespace io {

template <typename Type>
struct column_data: public column
{
private:
    std::vector<Type> column_data_;

public:
    std::vector<Type> get_data() const
    {
        return column_data_;
    }
};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
