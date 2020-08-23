/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP
#define BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>

#include <boost/astronomy/io/hdu.hpp>
#include <boost/astronomy/io/image.hpp>
#include <boost/variant.hpp>
#include <boost/gil.hpp>

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Structure used to access and manipulate primary_header data unit of FITS file
 * @details This structure holds the primary HDU's header information and image data (if any)
 *          and as of now provides methods for only querying the header information and data of
 *          primary HDU
 * @note    For more information on primary_hdu visit
 *          <a href="http://archive.stsci.edu/fits/users_guide/node19.html#SECTION00511000000000000000">Primary_HDU</a>
 */
template<typename CardPolicy,typename Converter>
struct basic_primary_hdu
{
protected:
    bool simple; //!Stores the value of SIMPLE
    bool extend; //!Stores the value of EXTEND
    typedef boost::variant<
        image<bitpix::B8,Converter>,
        image<bitpix::B16,Converter>,
        image<bitpix::B32,Converter>,
        image<bitpix::_B32,Converter>,
        image<bitpix::_B64,Converter>
    > data_type;

    data_type data;
    header<CardPolicy> hdu_header;

public:
    /**
     * @brief   Default Constructor used to create a standalone object of primary_hdu
     */
    basic_primary_hdu() {}

    /**
     * @brief Initializes the primary hdu object with header and data passed as argument
     * @param[in] other Header associated with Primary HDU
     * @param[in] data_buffer Data associated with the Primary HDU
    */
    basic_primary_hdu( const header<CardPolicy> & other,const std::string& data_buffer):hdu_header(other) {

        instantiate_primary_hdu(hdu_header.bitpix());
        read_image_visitor read_image_visit(data_buffer);
        boost::apply_visitor(read_image_visit, data);
        init_primary_hdu();
    }


    /**
     * @brief Returns the header associated with the currently held primary hdu
    */
    header<CardPolicy> get_header() const {
        return hdu_header;
    }

    /**
     * @brief   Gets the image data associated with the primary HDU
     * @see     image.hpp
    */

    template<bitpix DT>
    image<DT,Converter> get_data() const { return *boost::get<image<DT,Converter>>(&this->data); }

    /**
     * @brief Writes the entire HDU ( header, image_data ) into the file
     * @param[in,out] file_writer Provides operations for writing data into the file
     * @tparam FileWriter Type of file_writer object.
    */
    template<typename FileWriter>
    void write_to(FileWriter& file_writer) {
        
        this->hdu_header.write_header(file_writer);
        write_image_visitor write_image_visit;
        auto image_data = boost::apply_visitor(write_image_visit, data);
        file_writer.write(image_data);
        auto current_write_pos = file_writer.get_current_pos();
        auto logical_record_end_pos = file_writer.find_unit_end();
        file_writer.write(std::string(logical_record_end_pos - current_write_pos, ' '));
    }


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

        simple = hdu_header.template value_of<bool>("SIMPLE");
        extend = hdu_header.template value_of<bool>("EXTEND");
    }

    /**
     * @brief Instantiates an image object
     * @param[in] element_type Size of element ( bits per pixel )
    */
    void instantiate_primary_hdu(bitpix element_type) {
        switch (element_type)
        {
        case boost::astronomy::io::bitpix::B8:
            data = image<bitpix::B8,Converter>();
            break;
        case boost::astronomy::io::bitpix::B16:
            data = image<bitpix::B16,Converter>();
            break;
        case boost::astronomy::io::bitpix::B32:
            data = image<bitpix::B32,Converter>();
            break;
        case boost::astronomy::io::bitpix::_B32:
            data = image<bitpix::_B32,Converter>();
            break;
        case boost::astronomy::io::bitpix::_B64:
            data = image<bitpix::_B64,Converter>();
            break;
        default:
            break;
        }
    }
};
}}} //namespace boost::astronomy::io


#endif // !BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP

