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
#include <boost/astronomy/io/extension_hdu.hpp>
#include <boost/astronomy/io/column.hpp>

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
    std::vector<char> data_;

public:
    /**
     * @brief       Constructs a standalone object of table_extentsion
    */
    table_extension() {}

    
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

    private:
    void set_table_extension_info() {
        this->tfields_ = this->hdu_header.template value_of<std::size_t>("TFIELDS");
        this->col_metadata_.resize(this->tfields_);
    }
    
};

}}} //namespace boost::astronomy::io
#endif // !BOOST_ASTRONOMY_IO_TABLE_EXTENSION
