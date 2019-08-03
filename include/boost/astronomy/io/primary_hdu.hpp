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

namespace boost { namespace astronomy { namespace io {

template <bitpix DataType>
struct primary_hdu : public boost::astronomy::io::hdu
{
protected:
    bool simple; //!Stores the value of SIMPLE
    bool extend; //!Stores the value of EXTEND
    image<DataType> data; //!stores the image of primary HDU if any
                
public:
    primary_hdu() {}

    //!This constructore should be used when file is never read and boost::astronomy::io::hdu object is not created of the file
    primary_hdu(std::fstream &file) : hdu(file)
    {
        //primary header always starts at the begining of the file
        file.seekg(0);
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

    //!This constructore should be used when boost::astronomy::io::hdu object already exist for the file 
    primary_hdu(std::fstream &file, hdu const& other) : hdu(other)
    {
        //primary header always starts at the begining of the file
        file.seekg(0);
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

    //!returnes the stored data
    image<DataType> get_data() const
    {
        return this->data;
    }

    //!value of SIMPLE 
    bool is_simple() const
    {
        return this->simple;
    }

    //!value of EXTEND
    bool is_extended() const
    {
        return this->extend;
    }
};

}}} //namespace boost::astronomy::io


#endif // !BOOST_ASTRONOMY_IO_PRIMARY_HDU_HPP

