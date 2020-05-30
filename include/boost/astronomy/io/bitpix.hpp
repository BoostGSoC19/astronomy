/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_BITPIX_HPP
#define BOOST_ASTRONOMY_IO_BITPIX_HPP

/**
 * @file    bitpix.hpp
 * @author  Pranam Lashkari
 * @details Contains the enum declaration for representing bitpix values in header
 */

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Enum to represent different values of bitpix
 * @author  Pranam Lashkari
*/
enum class bitpix
{
    B8,     //! 8-bit unsigned integer
    B16,    //! 16-bit two's complement integer
    B32,    //! 32-bit two's complement integer
    _B32,   //! 32-bit IEEE single precision floating point
    _B64    //! 64-bit IEEE double precision floating point
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_BITPIX_HPP
