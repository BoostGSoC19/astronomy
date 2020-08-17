/*=============================================================================
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
#define BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP

#include <string>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include <sstream>

#include <boost/static_assert.hpp>

#include <boost/astronomy/io/column.hpp>
#include <boost/astronomy/io/string_conversion_utility.hpp>

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
     * @brief   Creates a standalone object of column_data
    */
    column_data() {}
    /**
     * @brief   Copy Constructor for  initializing column metadata and column_data
     * @param   other column_data object from where the metadata and column data are copied 
    */
    column_data(const column_data& other) : column(other), column_data_(other.column_data_) {}

    /**
     * @brief  Creates a column_data object by copying column metadata from the given argument
     * @param  other column object that contains column metadata
    */
    column_data(const column& other) : column(other) {}

    /**
     * @brief   Returns the collection of current field value for every row in the table
     * @return  A vector<Type> containing the field value for all rows
    */
    const std::vector<Type>& get_data() const
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


// Forward declaration for proxy
template<typename DataType>
struct column_view; 


template<typename DataType>
struct Proxy {
private:
    int index;
    DataType value;
    column_view<DataType>* callback_ptr;
public:
    Proxy(DataType val, column_view<DataType>* callbck_ptr, int row) : value(val), callback_ptr(callbck_ptr),index(row){}

    operator DataType () const  { return value; }

    DataType operator = (DataType value)  {
        this->callback_ptr->update_value(index, value);
        return value;
    }


};


template<typename DataType>
struct column_view{
private:
    int column_number;
    std::vector<std::vector<std::string>>* table_ref;
    std::unordered_map<int, DataType> cached_index;
public:
    // For storage purpose
    column_view() {}
    int get_column_number() { return column_number; }
    std::size_t get_row_count() { return table_ref->size(); }

    class iterator {
        int index;
        column_view<DataType>& col_view;
    public:
        iterator(column_view<DataType>& col,int indx) :col_view(col),index(indx) {}
        iterator operator ++() {
            index++;
            return *this;
        }
        bool operator != (const iterator& other) {
            return index != other.index;
        }
        Proxy<DataType> operator* () {
            return col_view[index];
        }

    };
    column_view(int col_number, std::vector<std::vector<std::string>>* tb_ref): column_number( col_number), table_ref(tb_ref) {}

    iterator begin() { return iterator(*this,0); }
    iterator end() { return iterator(*this, get_row_count()); }

    void update_value(int index, DataType new_value){
        std::stringstream serialized_stream;
        serialized_stream << new_value;
        auto serialized_value = serialized_stream.str();
        cached_index[index] = new_value;
        (*table_ref)[index][column_number] = serialized_value;
    }

  
    Proxy<DataType> operator [](int index) {

        auto cached_iter = cached_index.find(index);
        
        if (cached_iter != cached_index.end()) {
            return Proxy<DataType>(cached_iter->second, this,index);
        }

        auto raw_data_str = (*table_ref)[index][column_number];
        cached_index.emplace(index, convert_to<DataType>(boost::algorithm::trim_copy(raw_data_str)));
        return Proxy<DataType>(cached_index.find(index)->second, this,index);
    }

};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
