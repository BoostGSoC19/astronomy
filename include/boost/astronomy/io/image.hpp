/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_IMAGE_HPP
#define BOOST_ASTRONOMY_IO_IMAGE_HPP


#include <valarray>
#include <fstream>
#include <cstddef>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <string>
#include <cmath>
#include <numeric>

#include <boost/endian/conversion.hpp>
#include <boost/cstdfloat.hpp>
#include <boost/variant.hpp>

#include <boost/astronomy/io/bitpix.hpp>

/**
 * @file    image.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for image_buffer and image structure
 */

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Provides utility functions for working with image and is used to stores image data
 * @author  Pranam Lashkari
*/
template <typename PixelType>
struct image_buffer
{
protected:
    std::valarray<PixelType> data_; //! stores the image
    std::size_t width_; //! width of image
    std::size_t height_; //! height of image
    //std::fstream image_file; //! image file

    /**
     * @brief Only used for Type Punning ( Accessing bytes individually )
    */
    union pixel_data
    {
        PixelType pixel;
        std::uint8_t byte[sizeof(PixelType)];
        //char byte[sizeof(PixelType)];
    };

public:
    /**
     * @brief       Constructs an standalone object of image_buffer
    */
    image_buffer() {}

    /**
     * @brief       Constructs an image_buffer object by allocating width*height space for buffer
     * @param[in]   width Width of Image
     * @param[in]   height  Height of Image
    */
    image_buffer(std::size_t width, std::size_t height) : width_(width), height_(height)
    {
        this->data_.resize(width*height);
    }

    /**
     * @brief       virtual destructor allowing image_buffer to be a polymorphic base for derived classes
    */
    virtual ~image_buffer() {}

    /**
     * @brief   Gets the maximum value of all the pixels in the image
     * @return  PixelType containing the maximum pixel value
    */
    PixelType max() const
    {
        return this->data_.max();
    }

    /**
     * @brief   Gets the minimum value of all the pixels in the image
     * @return  PixelType containing the minimum pixel value
    */
    PixelType min() const
    {
        return this->data_.min();
    }


    /**
     * @brief   Gets the mean value of all the pixels in the image
     * @return  mean value of pixels in the image
    */
    double mean() const
    {
        if (this->data_.size() == 0)
        {
            return 0;
        }

        return (std::accumulate(std::begin(this->data_),
            std::end(this->data_), 0.0) / this->data_.size());
    }

    /**
     * @brief   Gets the median of all the pixel values in the image
     * @note    This method uses additional space of order O(n) where n is the number of total pixels
     * @return  median of all the pixel values in the image
    */
    PixelType median() const
    {
        std::valarray<PixelType> sorted_array = this->data_;
        std::nth_element(std::begin(sorted_array),
            std::begin(sorted_array) + sorted_array.size() / 2, std::end(sorted_array));

        return sorted_array[sorted_array.size() / 2];
    }

   /**
     * @brief   Gets the standard deviation of all the pixel values in the image
     * @note    This method uses additional space of order O(n) where n is the number of total pixels
     * @return  standard deviation of all the pixel values in the image
    */
    double std_dev() const
    {
        if (this->data_.size() == 0)
        {
            return 0;
        }

        double avg = this->mean();

        std::valarray<double> diff(this->data_.size());
        for (size_t i = 0; i < diff.size(); i++)
        {
            diff[i] = this->data_[i] - avg;
        }

        diff *= diff;
        return std::sqrt(diff.sum() / (diff.size() - 1));
    }

    /**
     * @brief       Gets the pixel value at specified position
     * @param[in]   x x position of pixel
     * @param[in]   y y position of pixel
    */
    PixelType operator() (std::size_t x, std::size_t y)
    {
        return this->data_[(x*this->width_) + y];
    }

    /**
     * @brief       Returns the size of image
    */
    std::size_t size() { return data_.size(); }
};




struct read_image_visitor :public boost::static_visitor<> {

    const std::string& data_buffer;

    read_image_visitor(const std::string& data_buf) :data_buffer(data_buf) {}

    template<typename Image_Type>
    void operator()(Image_Type& type) { type.read_image(data_buffer); }
};

/**
 * @brief   Stores image data associated with the perticular HDU
 * @tparam  args Specifies the number of bits that represents a data value in image.
 * @author  Pranam Lashkari, Gopi Krishna Menon
 * @see     image_buffer
*/
template<bitpix bitpix_val>
struct image:public image_buffer<typename bitpix_type<bitpix_val>::underlying_type> {

public:
    void read_image(const std::string& data_buffer) {

        if (data_buffer.empty()) { return; }

        auto element_size = get_element_size_from_bitpix(bitpix_val);
        std::string::const_iterator raw_data_iter=data_buffer.begin();

        this->data_.resize(data_buffer.size() / element_size);


        for (auto& element : this->data_) {
            std::string data(raw_data_iter, raw_data_iter + element_size);
            //Boost allows only endian  conversion with integral types
            // This is a workaround ( Largest integral type for storing all other types data temporarily)
            boost::int64_t temp_holder = 0;
            std::memcpy(&temp_holder, data.c_str(), element_size);
            temp_holder = boost::endian::big_to_native(temp_holder);
            std::memcpy(&element, &temp_holder, element_size);
            raw_data_iter += element_size;
        }
    }
};
}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_IMAGE_HPP
