/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP
#define BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <valarray>

#include <boost/astronomy/io/hdu.hpp>
#include <boost/astronomy/io/extension_hdu.hpp>
#include <boost/astronomy/io/image.hpp>
#include <boost/variant.hpp>

/**
 * @file    image_extension.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for image_extension structure
 */

namespace boost { namespace astronomy { namespace io {

/**
 * @brief       Stores the header information and data of image_extension
 * @details     For more information on Ascii_Table extension visit
 *              <A href="http://archive.stsci.edu/fits/users_guide/node41.html#SECTION00550000000000000000"> IMAGE_EXTENSION</A>
 * @author      Pranam Lashkari
 * @tparam      DataType Specifies the number of bits that represents a data value in image.
*/
template <bitpix DataType>
struct image_extension : public boost::astronomy::io::extension_hdu
{

    typedef boost::variant<
        image<bitpix::B8>,
        image<bitpix::B16>,
        image<bitpix::B32>,
        image<bitpix::_B32>,
        image<bitpix::_B64>
    > data_type;
    data_type data;
    image<DataType> data_;

public:

    image_extension(const header& other, const std::string& data_buffer) :extension_hdu(other) {
        instantiate_image(other.bitpix());
        set_image_data(data_buffer);
    } 
private:
    void set_image_data(const std::string& data_buffer) {
        read_image_visitor read_image_visit(data_buffer);
        boost::apply_visitor(read_image_visit, data);
    }

    void instantiate_image(bitpix element_type) {
        switch (element_type)
        {
        case boost::astronomy::io::bitpix::B8:
            data = image<bitpix::B8>();
            break;
        case boost::astronomy::io::bitpix::B16:
            data = image<bitpix::B16>();
            break;
        case boost::astronomy::io::bitpix::B32:
            data = image<bitpix::B32>();
            break;
        case boost::astronomy::io::bitpix::_B32:
            data = image<bitpix::_B32>();
            break;
        case boost::astronomy::io::bitpix::_B64:
            data = image<bitpix::_B64>();
            break;
        default:
            break;
        }
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP

