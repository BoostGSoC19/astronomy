/*=============================================================================
Copyright 2020 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

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

namespace boost { namespace astronomy { namespace io {


// Forward declaration for proxy
template<typename DataType,typename Converter>
struct column_view; 

/**
 * @brief Manages the reading and writing of column values and updating the data accordingly
 * @author Gopi Krishna Menon
 * @tparam DataType Refers to the actual type of the data present in the associated column
 * @tparam Converter Policy for serializing and deserializing of data
*/
template<typename DataType,typename Converter>
struct Proxy {
private:
    int index;
    DataType value;
    column_view<DataType,Converter>* callback_ptr;
public:
    Proxy(DataType val, column_view<DataType,Converter>* callbck_ptr, int row) : value(val), callback_ptr(callbck_ptr),index(row){}

    /**
     * @brief Returns the value stored in the proxy
     * @note  This converter operator gets executed at the time of Reading a value ( Right Side of expr )
    */
    operator DataType () const  { return value; }

    /**
     * @brief Updates the value of field both in cache and main table structure
     * @param[in] value Replacement value for the old value
    */
    DataType operator = (DataType value)  {
        this->callback_ptr->update_value(index, value);
        return value;
    }
};


/**
 * @brief Generates an editable view of a perticular field in a table HDU
 * @author Gopi Krishna Menon
*/
template<typename DataType,typename Converter>
struct column_view:column{
private:
    std::vector<std::vector<std::string>>* table_ref;
    std::unordered_map<int, DataType> cached_index;
public:
    /**
     * @brief Default constructor for storage
    */ 
    column_view() {}

    /**
     * @brief Returns the column number to which the view is associated
    */
    int get_column_number() { return this->index(); }

    /**
     * @brief Gets the total number of rows in the table
    */
    std::size_t get_row_count() { return table_ref->size(); }

    /**
     * @brief For iterating the values inside the column
     * @author Gopi Krishna Menon
    */
    class iterator {
        int index;
        column_view<DataType,Converter>& col_view;
    public:
        /**
         * @brief Constructor used to initialize the object
         * @param[in,out] col Column view to which the iterator must be associated
         * @param[in] indx Row number of the element
        */
        iterator(column_view<DataType,Converter>& col,int indx) :col_view(col),index(indx){}

        /**
         * @brief Moves to the next item and returns a copy of this iterator
        */
        iterator operator ++() {
            index++;
            return *this;
        }

        /**
         * @brief Used to compare two column view iterators
         * @param[in] other The iterator to compare to
        */
        bool operator != (const iterator& other) {
            return index != other.index;
        }

        /**
         * @brief Returns a Proxy to the actual element for smart manipulation and reading
        */
        Proxy<DataType,Converter> operator* () {
            return col_view[index];
        }

    };

    /**
     * @brief Used to create and initialize the column view object
     * @param[in] column_metadata Refers to the column_metadata of the perticular column to which this view refers
     * @param[in,out] tb_ref Pointer to the actual table of the HDU for reading or making changes
    */
    column_view(const column& column_metadata, std::vector<std::vector<std::string>>* tb_ref):
        column( column_metadata),
        table_ref(tb_ref)
        {}

    /**
     * @brief Returns an iterator to the first element in the column
    */
    iterator begin() { return iterator(*this,0); }

    /**
     * @brief Returns an iterator to the one past the last column element
    */
    iterator end() { return iterator(*this, get_row_count()); }

    /**
     * @brief Updates the element at perticular row of the associated column in HDU table
    */
    void update_value(int row, DataType new_value){
        cached_index[row] = new_value;
        (*table_ref)[row][this->index()-1] = Converter::serialize(new_value);
    }

   /**
    * @brief Returns the element at specific row of the associated column as proxy object
    * @param[in] row_number The row in which the element resides
   */
    Proxy<DataType,Converter> operator [](int row_number) {

        auto cached_iter = cached_index.find(row_number);
        
        if (cached_iter != cached_index.end()) {
            return Proxy<DataType,Converter>(cached_iter->second, this,row_number);
        }

        auto raw_data_str = (*table_ref)[row_number][this->index()-1];
        cached_index.emplace(row_number, Converter::template deserialize_to<DataType>(raw_data_str,this->total_elements()));
        return Proxy<DataType,Converter>(cached_index.find(row_number)->second, this,row_number);
    }

};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
