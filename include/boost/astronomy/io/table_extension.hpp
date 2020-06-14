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
struct table_extension : public extension_hdu
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
     * @brief       Constructs a table_extension object by reading header information from given filestream
     * @details     This constructor reads the header information of HDU from filestream and reserves the
     *              space for storing information( metadata ) of all the fields in the table
     * @param[in,out] file filestream set to open mode for reading
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    table_extension(std::fstream &file) : extension_hdu(file)
    {
        set_table_extension_info();
    }

    /**
     * @brief       Constructs an table_extension from the hduobject passed as an argument
     * @details     This constructor accepts hdu_object as argument,creates a table_extension
     *              by copying the header information from hdu_object and reserves the space for storing
     *              information(metadata) of all the fields in the table
     * @param[in]   other hdu object containing the header information of the current extension HDU
    */
    table_extension(hdu const& other) : extension_hdu(other)
    {
        set_table_extension_info();
    }

    /**
    * @brief     Constructs an table_extension from the specified pos in filestream passed as arguments
    * @details   This constructor accepts a filestream and streamposition as argument and creates a table_extension
    *            by extracting the header information from the given file. It also reserves the space for
    *            storing information(metadata) of all fields in the table
    * @param[in,out] file filestream set to open mode for reading
    * @param[in] pos File Pointer/cursor position from where the header information is to be read
    * @note    After the reading the file pointer/cursor will be set to the end of logical HDU unit
   */
    table_extension(std::fstream &file, std::streampos pos) : extension_hdu(file, pos)
    {
        set_table_extension_info();
    }
private:
    void set_table_extension_info() {
        tfields_ = this->value_of<std::size_t>("TFIELDS");
        col_metadata_.resize(tfields_);
    }
    
};

}}} //namespace boost::astronomy::io
#endif // !BOOST_ASTRONOMY_IO_TABLE_EXTENSION
