
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP
#define BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <valarray>

#include <boost/astronomy/io/hdu.hpp>

/**
 * @file    extension_hdu.hpp
 * @author  Pranam Lashkari
 * @details
 */

namespace boost { namespace astronomy { namespace io {

struct extension_hdu : public boost::astronomy::io::hdu
{
protected:
    std::string extname;
    int gcount = 1;
    int pcount = 0;

public:
    extension_hdu() {}

    extension_hdu(std::fstream &file) : hdu(file) 
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
        extname = this->value_of<std::string>("EXTNAME");
    }

    extension_hdu(std::fstream &file, hdu const& other) : hdu(other)
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
        extname = this->value_of<std::string>("EXTNAME");
    }

    extension_hdu(std::fstream &file, std::streampos pos) : hdu(file, pos)
    {
        gcount = this->value_of<int>("GCOUNT");
        pcount = this->value_of<int>("PCOUNT");
        extname = this->value_of<std::string>("EXTNAME");
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_EXTENSION_HDU_HPP

