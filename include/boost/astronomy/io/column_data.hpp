
/*=============================================================================
  Copyright 2019-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
#define BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP

#include <string>
#include <cstddef>
#include <vector>

#include <boost/static_assert.hpp>

#include <boost/astronomy/io/column.hpp>

/**
 * @file    column_data.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for column_data structure
 */

namespace boost { namespace astronomy { namespace io {
/**
 * @brief   Stores the collection of current field value for every row in the specified type.
 * @details This class in addition to holding the current field information also stores
 *          the current field value for every row in the table as a collection
 * @tparam  Type Type for storing the field values
*/
template <typename Type>
struct column_data: public column
{
private:
    std::vector<Type> column_data_;

public:
    /**
     * @brief   Returns the collection of current field value for every row in the table
     * @return  A vector<Type> containing the field value for all rows
     * @todo    Ask from sir why this function returns by value 
    */
    std::vector<Type> get_data() const
    {
        return column_data_;
    }

    /**
     * @brief   Returns the collection of current field value for every row in the table
     * @return  A vector<Type> containing the field value for all rows  
    */
    std::vector<Type>& get_data()
    {
        return column_data_;
    }

};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
