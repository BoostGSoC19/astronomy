/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

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


namespace boost { namespace astronomy { namespace io {

/**
 * @brief       Stores the header information and data of image_extension
 * @details     For more information on Ascii_Table extension visit
 *              <A href="http://archive.stsci.edu/fits/users_guide/node41.html#SECTION00550000000000000000"> IMAGE_EXTENSION</A>
 * @author      Pranam Lashkari
*/
template <typename CardPolicy,typename Converter>
struct basic_image_extension : public boost::astronomy::io::extension_hdu<CardPolicy>
{

    typedef boost::variant<
        image<bitpix::B8,Converter>,
        image<bitpix::B16,Converter>,
        image<bitpix::B32,Converter>,
        image<bitpix::_B32,Converter>,
        image<bitpix::_B64,Converter>
    > data_type;
    data_type data;
    
public:

    /**
     * @brief Initializes the image extension object with header and data passed as argument
     * @param[in] other Header associated with Image HDU
     * @param[in] data_buffer Data associated with the Image HDU
    */
    basic_image_extension(const header<CardPolicy>& other, const std::string& data_buffer) :extension_hdu<CardPolicy>(other) {
        instantiate_image(other.bitpix());
        set_image_data(data_buffer);
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
    image<DT,Converter> get_data() const { return *boost::get<image<DT>>(&this->data); }




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

private:
    /**
     * @brief Sets the image data
     * @param[in] data_buffer Image data in binary form from the file
    */
    void set_image_data(const std::string& data_buffer) {
        read_image_visitor read_image_visit(data_buffer);
        boost::apply_visitor(read_image_visit, data);
    }

    /**
     * @brief Instantiates an image object
     * @param[in] element_type Size of element ( bits per pixel )
    */
    void instantiate_image(bitpix element_type) {
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

using image_extension = basic_image_extension<card_policy>;


}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP

