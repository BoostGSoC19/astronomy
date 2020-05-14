
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP
#define BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <valarray>

#include <boost/astronomy/io/hdu.hpp>

/**
 * @file    extension_hdu.hpp
 * @author  Pranam Lashkari
 * @details Contains the definition for extension_hdu 
*/

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Used to store header information of an extension Header Data Unit
 * @details In addition to hdu filling all the header information, this structure is only involved
 *          with setting the value of variables - extname, gcount,pcount
 * @todo    Too much repetitive code for all constructors. Refactor them into a method
 * @see     boost::astronomy::io::hdu
*/
struct extension_hdu : public boost::astronomy::io::hdu
{
protected:
    std::string extname;
    int gcount = 1;
    int pcount = 0;

public:
    /**
     * @brief Default constructor to create a standalone object of extension_hdu
    */
    extension_hdu() {}

    /**
     * @brief   Constructs an extention_hdu object from the given filestream
     * @details This constructor constructs an extension_hdu object by reading
     *          the header information of the extention HDU from the given filestream
     * @param[in,out] file filestream set to open mode for reading
     * @note    After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    extension_hdu(std::fstream &file) : hdu(file) 
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
        extname = this->value_of<std::string>("EXTNAME");
    }

    /**
     * @brief     Constructs an extention_hdu from the filestream and hduobject passed as an argument
     * @details   This constructor accepts a file stream and hdu_object as argument and creates a extention_hdu
     *            by copying the header information from hdu_object
     * @param[in,out] file filestream set to open mode for reading
     * @param[in] other hdu object containing the header information
     * @note    After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    extension_hdu(std::fstream &file, hdu const& other) : hdu(other)
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
        extname = this->value_of<std::string>("EXTNAME");
    }

    /**
     * @brief     Constructs an extention_hdu from the specified pos in filestream passed as arguments
     * @details   This constructor accepts a filestream and streamposition as argument and creates a extention_hdu
     *            by extracting the header information from the given file
     * @param[in,out] file filestream set to open mode for reading
     * @param[in] pos File Pointer/cursor position from where the header information is to be read
     * @note    After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    extension_hdu(std::fstream &file, std::streampos pos) : hdu(file, pos)
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
        extname = this->value_of<std::string>("EXTNAME");
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

