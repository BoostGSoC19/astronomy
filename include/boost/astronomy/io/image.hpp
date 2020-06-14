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


/**
 * @brief   Stores image data associated with the perticular HDU
 * @tparam  args Specifies the number of bits that represents a data value in image.
 * @author  Pranam Lashkari
 * @see     image_buffer
*/
template<bitpix args>
struct image {};


/**
 * @brief   Image specialization for 8 bit data value
*/
template <>
struct image<bitpix::B8> : public image_buffer<std::uint8_t>
{
public:
    /**
     * @brief   Default constructor used to create a standalone object of image
    */
    image() {}

    /**
     * @brief       Constructs an image object from the file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::string const& file, std::size_t width, std::size_t height, std::streamoff start) :
        image_buffer<std::uint8_t>(width, height)
    {
        std::fstream image_file(file);
        image_file.seekg(start);
        read_image_logic(image_file);
        image_file.close();
    }

    /**
     * @brief       Constructs an image object from the given file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from given file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::string const& file, std::size_t width, std::size_t height) :
        image_buffer<std::uint8_t>(width, height)
    {
        std::fstream image_file(file);
        read_image_logic(image_file);
        image_file.close();
    }

    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        read_image(file, width, height, start);
    }

    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from  filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height);
    }

    /**
     * @brief       Reads the 8bit image data from the given filestream
     * @param[in,out] file filestream set to open mode for reading
    */
    void read_image_logic(std::fstream &image_file)
    {
        image_file.read((char*)std::begin(data_), width_*height_);
        //std::copy_n(std::istreambuf_iterator<char>(file.rdbuf()), width*height, std::begin(data));
    }

    /**
     * @brief       Reads the image data from given file
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    void read_image
    (
        std::string const& file,
        std::size_t width,
        std::size_t height,
        std::streamoff start
    )
    {
        std::fstream image_file(file);
        data_.resize(width*height);
        image_file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(image_file);

        image_file.close();
    }

    /**
     * @brief       Reads the image data from given file
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::string const& file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, 0);
    }


    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        data_.resize(width*height);
        file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(file);
    }

    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from current position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, file.tellg());
    }
};

/**
 * @brief   Image specialization for 16 bit data value
*/
template <>
struct image<bitpix::B16> : public image_buffer<std::int16_t>
{
public:
    /**
     * @brief   Default constructor used to create a standalone object of image
    */
    image() {}

    /**
     * @brief       Constructs an image object from the file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::string const& file, std::size_t width, std::size_t height, std::streamoff start) :
        image_buffer<std::int16_t>(width, height)
    {
        std::fstream image_file(file);
        image_file.seekg(start);
        read_image_logic(image_file);
        image_file.close();
    }

    /**
     * @brief       Constructs an image object from the given file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from given file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::string const& file, std::size_t width, std::size_t height) :
        image_buffer<std::int16_t>(width, height)
    {
        std::fstream image_file(file);
        read_image_logic(image_file);
        image_file.close();
    }

    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        read_image(file, width, height, start);
    }

    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from  filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height);
    }

    /**
     * @brief       Reads the 8bit image data from the given filestream
     * @param[in,out] file filestream set to open mode for reading
    */
    void read_image_logic(std::fstream &image_file)
    {
        for (std::size_t i = 0; i < height_*width_; i++)
        {
            image_file.read((char*)&data_[i], 2);
            data_[i] = boost::endian::big_to_native(data_[i]);
        }
    }

    /**
     * @brief       Reads the image data from given file
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    void read_image
    (
        std::string const& file,
        std::size_t width,
        std::size_t height,
        std::streamoff start
    )
    {
        std::fstream image_file(file);
        image_file.open(file);
        data_.resize(width*height);
        image_file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(image_file);

        image_file.close();
    }
    /**
     * @brief       Reads the image data from given file
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::string const& file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, 0);
    }

    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        data_.resize(width*height);
        file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(file);
    }
    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from current position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, file.tellg());
    }
};


/**
 * @brief   Image specialization for 32 bit data value
*/
template <>
struct image<bitpix::B32> : public image_buffer<std::int32_t>
{
public:
    /**
     * @brief   Default constructor used to create a standalone object of image
    */
    image() {}

    /**
     * @brief       Constructs an image object from the file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::string const& file, std::size_t width, std::size_t height, std::streamoff start) :
        image_buffer<std::int32_t>(width, height)
    {
        std::fstream image_file(file);
        image_file.seekg(start);
        read_image_logic(image_file);
        image_file.close();
    }

    /**
     * @brief       Constructs an image object from the given file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from given file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::string const& file, std::size_t width, std::size_t height) :
        image_buffer<std::int32_t>(width, height)
    {
        std::fstream image_file(file);
        read_image_logic(image_file);
        image_file.close();
    }
    /**
    * @brief       Constructs an image object from the given filestream
    * @details     This constructor reserves width*height space for image buffer
    *              and reads the image data from specific position in filestream
    * @param[in,out] file filestream set to open mode for reading
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
    * @param[in]   start Starting position of image data
   */
    image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        read_image(file, width, height, start);
    }

    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from  filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height);
    }

    /**
     * @brief       Reads the 8bit image data from the given filestream
     * @param[in,out] file filestream set to open mode for reading
    */
    void read_image_logic(std::fstream &image_file)
    {
        for (std::size_t i = 0; i < height_*width_; i++)
        {
            image_file.read((char*)&data_[i], 4);
            data_[i] = boost::endian::big_to_native(data_[i]);
        }
    }

    /**
     * @brief       Reads the image data from given file
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    void read_image
    (
        std::string const& file,
        std::size_t width,
        std::size_t height,
        std::streamoff start
    )
    {
        std::fstream image_file(file);
        data_.resize(width*height);
        image_file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(image_file);

        image_file.close();
    }

    /**
    * @brief       Reads the image data from given file
    * @details     This method reserves width*height space for image buffer
    *              and reads the image data from specific position in file
    * @param[in]   file path where the file resides
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
   */
    void read_image(std::string const& file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, 0);
    }

    /**
    * @brief       Reads the image data from the given filestream
    * @details     This method reserves width*height space for image buffer
    *              and reads the image data from specific position in filestream
    * @param[in,out] file filestream set to open mode for reading
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
    * @param[in]   start Starting position of image data
   */
    void read_image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        data_.resize(width*height);
        file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(file);
    }
    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from current position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, file.tellg());
    }
};

/**
 * @brief   Image specialization for 32-bit IEEE single precision floating point data value
*/
template <>
struct image<bitpix::_B32> : public image_buffer<boost::float32_t>
{
public:
    /**
     * @brief   Default constructor used to create a standalone object of image
    */
    image() {}

    /**
     * @brief       Constructs an image object from the file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::string const& file, std::size_t width, std::size_t height, std::streamoff start) :
        image_buffer<boost::float32_t>(width, height)
    {
        std::fstream image_file(file);
        image_file.seekg(start);
        read_image_logic(image_file);
        image_file.close();
    }

    /**
     * @brief       Constructs an image object from the given file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from given file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::string const& file, std::size_t width, std::size_t height) :
        image_buffer<boost::float32_t>(width, height)
    {
        std::fstream image_file(file);
        read_image_logic(image_file);
        image_file.close();
    }
    /**
   * @brief       Constructs an image object from the given filestream
   * @details     This constructor reserves width*height space for image buffer
   *              and reads the image data from specific position in filestream
   * @param[in,out] file filestream set to open mode for reading
   * @param[in]   width Width of the image
   * @param[in]   height Height of the image
   * @param[in]   start Starting position of image data
  */
    image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        read_image(file, width, height, start);
    }
    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from  filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height);
    }

    /**
    * @brief       Reads the 8bit image data from the given filestream
    * @param[in,out] file filestream set to open mode for reading
   */
    void read_image_logic(std::fstream &image_file)
    {
        pixel_data single_pixel;
        for (std::size_t i = 0; i < height_ * width_; i++) {
            image_file.read((char*)single_pixel.byte, 4);
            data_[i] = static_cast<boost::float32_t>(
                (single_pixel.byte[3] << 0) |
                (single_pixel.byte[2] << 8) |
                (single_pixel.byte[1] << 16) |
                (single_pixel.byte[0] << 24));
        }
    }
    /**
     * @brief       Reads the image data from given file
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    void read_image
    (
        std::string const& file,
        std::size_t width,
        std::size_t height,
        std::streamoff start
    )
    {
        std::fstream image_file(file);
        data_.resize(width*height);
        image_file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(image_file);
        image_file.close();
    }
    /**
    * @brief       Reads the image data from given file
    * @details     This method reserves width*height space for image buffer
    *              and reads the image data from specific position in file
    * @param[in]   file path where the file resides
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
   */
    void read_image(std::string const& file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, 0);
    }

    /**
    * @brief       Reads the image data from the given filestream
    * @details     This method reserves width*height space for image buffer
    *              and reads the image data from specific position in filestream
    * @param[in,out] file filestream set to open mode for reading
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
    * @param[in]   start Starting position of image data
   */
    void read_image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        data_.resize(width*height);
        file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(file);
    }
    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from current position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, file.tellg());
    }
};

/**
 * @brief   Image specialization for 64-bit IEEE double precision floating point data value
*/
template <>
struct image<bitpix::_B64> : public image_buffer<boost::float64_t>
{
public:
    /**
     * @brief   Default constructor used to create a standalone object of image
    */
    image() {}

    /**
     * @brief       Constructs an image object from the file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from specific position in file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
     * @param[in]   start Starting position of image data
    */
    image(std::string const& file, std::size_t width, std::size_t height, std::streamoff start) :
        image_buffer<boost::float64_t>(width, height)
    {
        std::fstream image_file(file);
        image_file.seekg(start);
        read_image_logic(image_file);
        image_file.close();
    }
    /**
     * @brief       Constructs an image object from the given file
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from given file
     * @param[in]   file path where the file resides
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::string const& file, std::size_t width, std::size_t height) :
        image_buffer<boost::float64_t>(width, height)
    {
        std::fstream image_file(file);
        read_image_logic(image_file);
        image_file.close();
    }
    /**
  * @brief       Constructs an image object from the given filestream
  * @details     This constructor reserves width*height space for image buffer
  *              and reads the image data from specific position in filestream
  * @param[in,out] file filestream set to open mode for reading
  * @param[in]   width Width of the image
  * @param[in]   height Height of the image
  * @param[in]   start Starting position of image data
 */
    image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        read_image(file, width, height, start);
    }
    /**
     * @brief       Constructs an image object from the given filestream
     * @details     This constructor reserves width*height space for image buffer
     *              and reads the image data from  filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height);
    }

    /**
    * @brief       Reads the 8bit image data from the given filestream
    * @param[in,out] file filestream set to open mode for reading
   */
    void read_image_logic(std::fstream &image_file)
    {
        pixel_data single_pixel;
        for (std::size_t i = 0; i < height_ * width_; i++) {
            image_file.read((char*)single_pixel.byte, 8);

            std::int64_t data;
            data = (static_cast<std::int64_t>(single_pixel.byte[7]) << 0) |
                (static_cast<std::int64_t>(single_pixel.byte[6]) << 8) |
                (static_cast<std::int64_t>(single_pixel.byte[5]) << 16) |
                (static_cast<std::int64_t>(single_pixel.byte[4]) << 24) |
                (static_cast<std::int64_t>(single_pixel.byte[3]) << 32) |
                (static_cast<std::int64_t>(single_pixel.byte[2]) << 40) |
                (static_cast<std::int64_t>(single_pixel.byte[1]) << 48) |
                (static_cast<std::int64_t>(single_pixel.byte[0]) << 56);
            std::memcpy(&data_[i], &data, sizeof(boost::float64_t));

        }
    }

    /**
      * @brief       Reads the image data from given file
      * @details     This method reserves width*height space for image buffer
      *              and reads the image data from specific position in file
      * @param[in]   file path where the file resides
      * @param[in]   width Width of the image
      * @param[in]   height Height of the image
      * @param[in]   start Starting position of image data
     */
    void read_image
    (
        std::string const& file,
        std::size_t width,
        std::size_t height,
        std::streamoff start
    )
    {
        std::fstream image_file(file);
        data_.resize(width*height);
        image_file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(image_file);

        image_file.close();
    }

    /**
    * @brief       Reads the image data from given file
    * @details     This method reserves width*height space for image buffer
    *              and reads the image data from specific position in file
    * @param[in]   file path where the file resides
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
   */
    void read_image(std::string const& file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, 0);
    }

    /**
    * @brief       Reads the image data from the given filestream
    * @details     This method reserves width*height space for image buffer
    *              and reads the image data from specific position in filestream
    * @param[in,out] file filestream set to open mode for reading
    * @param[in]   width Width of the image
    * @param[in]   height Height of the image
    * @param[in]   start Starting position of image data
   */
    void read_image(std::fstream &file, std::size_t width, std::size_t height, std::streamoff start)
    {
        data_.resize(width*height);
        file.seekg(start);
        this->width_ = width;
        this->height_ = height;
        read_image_logic(file);
    }
    /**
     * @brief       Reads the image data from the given filestream
     * @details     This method reserves width*height space for image buffer
     *              and reads the image data from current position in filestream
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   width Width of the image
     * @param[in]   height Height of the image
    */
    void read_image(std::fstream &file, std::size_t width, std::size_t height)
    {
        read_image(file, width, height, file.tellg());
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_IMAGE_HPP
