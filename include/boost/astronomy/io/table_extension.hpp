/*=============================================================================
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_TABLE_EXTENSION_HPP
#define BOOST_ASTRONOMY_IO_TABLE_EXTENSION_HPP

#include <cstddef>
#include <fstream>
#include <string>
#include <boost/astronomy/io/hdu.hpp>
#include <boost/astronomy/io/extension_hdu.hpp>
#include <boost/astronomy/io/column.hpp>
#include <boost/astronomy/io/column_data.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>

/**
 * @file    table_extension.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for table_extension
 */

namespace boost { namespace astronomy { namespace io {

/**
 * @details The object of this structure stores header
 *          information and data of ASCII table extension or Binary Table Extension
*/
template<typename CardPolicy>
struct table_extension : public extension_hdu<CardPolicy>
{
protected:
    std::size_t tfields_;
    std::vector<column> col_metadata_;

    typedef std::vector<std::vector<std::string>> table_data;
    mutable table_data tb_data;

public:
    /**
     * @brief       Constructs a standalone object of table_extentsion
    */
    table_extension():tfields_(-1) {}

    
    /**
     * @brief       Constructs an table_extension from the hduobject passed as an argument
     * @details     This constructor accepts hdu_object as argument,creates a table_extension
     *              by copying the header information from hdu_object and reserves the space for storing
     *              information(metadata) of all the fields in the table
     * @param[in]   other hdu object containing the header information of the current extension HDU
    */
    table_extension(header<CardPolicy> const& other) : extension_hdu<CardPolicy>(other)
    {
        set_table_extension_info();
    }


    /**
     * @brief Returns the header associated with the currently held primary hdu
    */
    header<CardPolicy> get_header() const {
        return this->hdu_header;
    }

    column get_column_metadata(const std::string& column_name) const {
        auto pos = std::find_if(this->col_metadata_.begin(), col_metadata_.end(), [&](const column& col) {return column_name == col.TTYPE(); });
        if (pos != this->col_metadata_.end()) {
            return (*pos);
        }
        throw column_not_found_exception(column_name);
    }

    template<typename ColDataType, typename Converter>
    column_view<ColDataType, Converter> make_column_view(const std::string& column_name) const {
            return column_view<ColDataType,Converter>(get_column_metadata(column_name), &tb_data);
    
    }

    const table_data& get_data() const { return this->tb_data; }

    table_data& get_data() { return this->tb_data; }


    private:
    void set_table_extension_info() {
        this->tfields_ = this->hdu_header.template value_of<std::size_t>("TFIELDS");
        this->col_metadata_.resize(this->tfields_);
    }

};

}}} //namespace boost::astronomy::io
#endif // !BOOST_ASTRONOMY_IO_TABLE_EXTENSION
