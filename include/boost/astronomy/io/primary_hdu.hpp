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
template <bitpix DataType>
struct primary_hdu : public boost::astronomy::io::hdu
{
protected:
    bool simple; //!Stores the value of SIMPLE
    bool extend; //!Stores the value of EXTEND
    image<DataType> data; //!stores the image of primary HDU if any

public:
    /**
     * @brief   Default Constructor used to create a standalone object of primary_hdu
     */
    primary_hdu() {}

    /**
     * @brief       Constructs a primary_hdu object and reads the primary header and image of primary HDU from the FITS file
     * @param[in,out] file filestream set to open mode for reading
     * @pre         file pointer should be at the beginning of the file
     * @note        This constructor should be used when file has not been read at all and boost::astronomy::io::hdu object is
     *              not created from the file. Also after the reading the cursor/file pointer is set to the end of HDU unit
     */
    primary_hdu(std::fstream &file) : hdu(file)
    {
        simple = this->value_of<bool>("SIMPLE");
        extend = this->value_of<bool>("EXTEND");

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
            data.read_image(file, this->naxis(1), std::accumulate(this->naxis_.begin()+1,
                this->naxis_.end(), 0, std::multiplies<std::size_t>()));
            break;
        }

        set_unit_end(file);    //set cursor to the end of the HDU unit
    }

    /**
     * @brief       Constructs a primary_hdu object based on the file and the hdu object passed
     * @details     This function constructs a primary_hdu object by copying the header information
     *              of primary hdu from <strong>other</strong> and reading the image( if any) from the
     *              FITS file
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   other HDU object that contains header information for the primary HDU
     * @note        Prefer this constructor to be used when boost::astronomy::io::hdu object already exist for the file
     *              Also the cursor/ filepointer is set to the end of HDU unit
    */
    primary_hdu(std::fstream &file, hdu const& other) : hdu(other)
    {
        simple = this->value_of<bool>("SIMPLE");
        extend = this->value_of<bool>("EXTEND");

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

        set_unit_end(file);    //set cursor to the end of the HDU unit
    }


    /**
     * @brief   Gets the image data associated with the primary HDU
     * @see     image.hpp
    */
    image<DataType> get_data() const
    {
        return this->data;
    }

    /**
     * @brief   Gets the value of FITS <strong>SIMPLE</strong> keyword
     * @return  Returns true if the file confirms to the FITS standard
    */
    bool is_simple() const
    {
        return this->simple;
    }

    //!value of EXTEND
    /**
     * @brief   Indicates whether extentions are present in FITS file
     * @details Gets the value of <strong>EXTEND</strong> from the primary header
     * @note    This function may return true even if extentions are not present in the FITS file
    */
    bool is_extended() const
    {
        return this->extend;
    }
};

}}} //namespace boost::astronomy::io


#endif // !BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP

