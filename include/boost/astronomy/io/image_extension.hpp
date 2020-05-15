
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
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
protected:
    image<DataType> data;

public:

    /**
     * @brief       Constructs an image_extension object by reading the header information and data from the filestream
     * @param[in,out] file filestream set to open mode for reading
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
     * @todo        Why is std::accumulate passed with zero as the beginning?
    */
    image_extension(std::fstream &file) : extension_hdu(file)
    {
        //read image according to dimension specified by naxis
        switch (this->naxis())
        {
        case 0:
            break;
        case 1:
            data.read_image(file, this->naxis(1), 1);
            break;
        case 2:
            data.read_image(file, this->naxis(1), this->naxis(2));
            break;
        default:
            data.read_image(file, this->naxis(1), std::accumulate(this->naxis_.begin()+ 1,
                this->naxis_.end(), 0, std::multiplies<std::size_t>()));
            break;
        }
        set_unit_end(file);
    }

    /**
     * @brief       Constructs an image_extension from filstream and hdu object
     * @details     Constructs an image_extension object by reading the data from filestream
     *              and header information from given hdu object
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   other HDU object from where the header information is read
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
     * @todo        Why is std::accumulate passed with zero as the beginning
    */
    image_extension(std::fstream &file, hdu const& other) : extension_hdu(file, other)
    {
        //read image according to dimension specified by naxis
        switch (this->naxis())
        {
        case 0:
            break;
        case 1:
            data.read_image(file, this->naxis(1), 1);
            break;
        case 2:
            data.read_image(file, this->naxis(1), this->naxis(2));
            break;
        default:
            data.read_image(file, this->naxis(1), std::accumulate(this->naxis_.begin()+ 1,
                this->naxis_.end(), 0, std::multiplies<std::size_t>()));
            break;
        }
        set_unit_end(file);
    }

    /**
     * @brief       Constructs an image_extension by reading information from specified position in filestream 
     * @details     Constructs an image_extension object by reading the header information and data from specified
     *              position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in] pos File Pointer/cursor position from where the header information is to be read
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    image_extension(std::fstream &file, std::streampos pos) : extension_hdu(file, pos)
    {
        //read image according to dimension specified by naxis
        switch (this->naxis())
        {
        case 0:
            break;
        case 1:
            data.read_image(file, this->naxis(1), 1);
            break;
        case 2:
            data.read_image(file, this->naxis(1), this->naxis(2));
            break;
        default:
            data.read_image(file, this->naxis(1), std::accumulate(this->naxis_.begin()+ 1,
                this->naxis_.end(), 0, std::multiplies<std::size_t>()));
            break;
        }
        set_unit_end(file);
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP

