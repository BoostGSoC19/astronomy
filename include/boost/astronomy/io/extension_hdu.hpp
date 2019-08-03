#ifndef BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP
#define BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <valarray>

#include <boost/astronomy/io/hdu.hpp>

namespace boost { namespace astronomy { namespace io {

struct extension_hdu : public boost::astronomy::io::hdu
{
protected:
    int gcount = 1;
    int pcount = 0;

public:
    extension_hdu() {}

    extension_hdu(std::fstream &file) : hdu(file) 
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
    }

    extension_hdu(std::fstream &file, hdu const& other) : hdu(other)
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
    }

    extension_hdu(std::fstream &file, std::streampos pos) : hdu(file, pos)
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

