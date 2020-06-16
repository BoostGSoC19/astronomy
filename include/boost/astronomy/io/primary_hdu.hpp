/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP
#define BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP

//#include <map>
#include <string>
#include <vector>
#include <cstddef>
#include <valarray>
#include <fstream>

#include <boost/astronomy/io/hdu.hpp>
#include <boost/astronomy/io/image.hpp>
#include <boost/variant.hpp>

/**
 * @file    primary_hdu.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for primary_hdu structure.
 */

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Structure used to access and manipulate primary_header data unit of FITS file
 * @details This structure holds the primary HDU's header information and image data (if any)
 *          and as of now provides methods for only querying the header information and data of
 *          primary HDU
 * @tparam  DataType    Specifies the number of bits that represents a data value in image.
 * @note    For more information on primary_hdu visit
 *          <a href="http://archive.stsci.edu/fits/users_guide/node19.html#SECTION00511000000000000000">Primary_HDU</a>
 * @todo    Consider renaming <strong>DataType</strong> to <strong>FieldSize</strong>
 */
struct primary_hdu
{
protected:
    bool simple; //!Stores the value of SIMPLE
    bool extend; //!Stores the value of EXTEND
    typedef boost::variant<
        image<bitpix::B8>,
        image<bitpix::B16>,
        image<bitpix::B32>,
        image<bitpix::_B32>,
        image<bitpix::_B64>
    > data_type;

    data_type data;
    header hdu_header;
public:
    /**
     * @brief   Default Constructor used to create a standalone object of primary_hdu
     */
    primary_hdu() {}

    primary_hdu( const header & other,const std::string& data_buffer):hdu_header(other) {

        instantiate_primary_hdu(hdu_header.bitpix());
        read_image_visitor read_image_visit(data_buffer);
        boost::apply_visitor(read_image_visit, data);
        init_primary_hdu();
    }

    header get_header() const {
        return hdu_header;
    }

    /**
     * @brief   Gets the image data associated with the primary HDU
     * @see     image.hpp
    */

    template<bitpix DT>
    image<DT> get_data() const { return *boost::get<image<DT>>(&this->data); }


    /**
     * @brief   Gets the value of FITS <strong>SIMPLE</strong> keyword
     * @return  Returns true if the file confirms to the FITS standard
    */
    bool is_simple() const
    {
        return simple;
    }

    /**
     * @brief   Indicates whether extentions are present in FITS file
     * @details Gets the value of <strong>EXTEND</strong> from the primary header
     * @note    This function may return true even if extentions are not present in the FITS file
    */
    bool is_extended() const
    {
        return extend;
    }
private:

    /**
    * @brief      Initializes the primary_hdu object(simple,extend) with image data
    * @param[in]  file filestream set to open mode for reading
   */
    void init_primary_hdu() {

        simple = hdu_header.value_of<bool>("SIMPLE");
        extend = hdu_header.value_of<bool>("EXTEND");
    }
    
    void instantiate_primary_hdu(bitpix element_type) {
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


#endif // !BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP

