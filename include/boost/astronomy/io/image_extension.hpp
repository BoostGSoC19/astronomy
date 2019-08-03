#ifndef BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP
#define BOOST_ASTRONOMY_IO_IMAGE_EXTENSION_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <valarray>

#include <boost/astronomy/io/hdu.hpp>
#include <boost/astronomy/io/extension_hdu.hpp>
#include <boost/astronomy/io/image.hpp>

namespace boost { namespace astronomy { namespace io {

template <bitpix DataType>
struct image_extension : public boost::astronomy::io::extension_hdu
{
protected:
    image<DataType> data;

public:
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

