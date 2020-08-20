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

namespace boost { namespace astronomy { namespace io {


// Forward declaration for proxy
template<typename DataType,typename Converter>
struct column_view; 


template<typename DataType,typename Converter>
struct Proxy {
private:
    int index;
    DataType value;
    column_view<DataType,Converter>* callback_ptr;
public:
    Proxy(DataType val, column_view<DataType,Converter>* callbck_ptr, int row) : value(val), callback_ptr(callbck_ptr),index(row){}

    operator DataType () const  { return value; }

    DataType operator = (DataType value)  {
        this->callback_ptr->update_value(index, value);
        return value;
    }


};


template<typename DataType,typename Converter>
struct column_view:column{
private:
    std::vector<std::vector<std::string>>* table_ref;
    std::unordered_map<int, DataType> cached_index;
public:
    // For storage purpose
    column_view() {}
    int get_column_number() { return this->index(); }
    std::size_t get_row_count() { return table_ref->size(); }

    class iterator {
        int index;
        column_view<DataType,Converter>& col_view;
    public:
        iterator(column_view<DataType,Converter>& col,int indx) :col_view(col),index(indx){}
        iterator operator ++() {
            index++;
            return *this;
        }
        bool operator != (const iterator& other) {
            return index != other.index;
        }
        Proxy<DataType,Converter> operator* () {
            return col_view[index];
        }

    };

    column_view(const column& column_metadata, std::vector<std::vector<std::string>>* tb_ref):
        column( column_metadata),
        table_ref(tb_ref)
        {}

    iterator begin() { return iterator(*this,0); }
    iterator end() { return iterator(*this, get_row_count()); }

    void update_value(int row, DataType new_value){
        cached_index[row] = new_value;
        (*table_ref)[row][this->index()-1] = Converter::serialize(new_value);
    }

  
    Proxy<DataType,Converter> operator [](int row_number) {

        auto cached_iter = cached_index.find(row_number);
        
        if (cached_iter != cached_index.end()) {
            return Proxy<DataType,Converter>(cached_iter->second, this,row_number);
        }

        auto raw_data_str = (*table_ref)[row_number][this->index()-1];
        cached_index.emplace(row_number, Converter::template deserialize_to<DataType>(raw_data_str,this->total_elements_per_field()));
        return Proxy<DataType,Converter>(cached_index.find(row_number)->second, this,row_number);
    }

};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_DATA_HPP
