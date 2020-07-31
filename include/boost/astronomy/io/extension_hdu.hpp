/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP
#define BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>

#include <boost/astronomy/io/hdu.hpp>

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Used to store header information of an extension Header Data Unit
 * @details In addition to hdu filling all the header information, this structure is only involved
 *          with setting the value of variables - extname, gcount,pcount
 * @see     boost::astronomy::io::hdu
*/
struct extension_hdu
{
protected:
    std::string extname;
    int gcount = 1;
    int pcount = 0;
    header hdu_header;

public:
    /**
     * @brief Default constructor to create a standalone object of extension_hdu
    */
    extension_hdu() {}

    
    /**
     * @brief     Constructs an extention_hdu from the hduobject passed as an argument
     * @details   This constructor accepts a hdu_object as argument and creates a extention_hdu
     *            by copying the header information from hdu_object
     * @param[in] other hdu object containing the header information
    */
    extension_hdu(header const& other) :hdu_header(other)
    {
        set_extension_info();
    }

    
private:
    /**
     * @brief       Sets the extension_hdu's info from the header
    */
    void set_extension_info() {
        gcount = hdu_header.value_of<int>("GCOUNT");
        pcount = hdu_header.value_of<int>("PCOUNT");
        extname = hdu_header.value_of<std::string>("EXTNAME");
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

