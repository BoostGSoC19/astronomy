/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_BITPIX_HPP
#define BOOST_ASTRONOMY_IO_BITPIX_HPP

#include<boost/cstdint.hpp>
#include<boost/cstdfloat.hpp>

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


 int get_element_size_from_bitpix(bitpix bitpix_value) {
    switch (bitpix_value)
    {
    case boost::astronomy::io::bitpix::B8:
        return 1;
        break;
    case boost::astronomy::io::bitpix::B16:
        return 2;
        break;
    case boost::astronomy::io::bitpix::B32:
        return 4;
        break;
    case boost::astronomy::io::bitpix::_B32:
        return 4;
        break;
    case boost::astronomy::io::bitpix::_B64:
        return 8;
        break;
    default:
        return 0;
    }
}

 // Base template
 template<bitpix type>
 struct bitpix_type;

 template<>
 struct bitpix_type<bitpix::B8> {
     typedef typename boost::int8_t underlying_type;
 };

 template<>
 struct bitpix_type<bitpix::B16> {
     typedef typename boost::int16_t underlying_type;
 };
 template<>
 struct bitpix_type<bitpix::B32> {
     typedef typename boost::int32_t underlying_type;
 };
 template<>
 struct bitpix_type<bitpix::_B32> {
     typedef typename boost::float32_t underlying_type;
 };
 template<>
 struct bitpix_type<bitpix::_B64> {
     typedef typename boost::float64_t underlying_type;
 };






}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_BITPIX_HPP
