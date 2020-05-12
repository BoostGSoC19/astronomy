
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_TABLE_EXTENSION_HPP
#define BOOST_ASTRONOMY_IO_TABLE_EXTENSION_HPP

#include <cstddef>
#include <fstream>
#include <string>
#include <boost/astronomy/io/extension_hdu.hpp>
#include <boost/astronomy/io/column.hpp>

/**
 * @file    table_extension.hpp
 * @author  Pranam Lashkari
 * @details
 */

namespace boost { namespace astronomy { namespace io {


struct table_extension : public extension_hdu
{
protected:
    std::size_t tfields;
    std::vector<column> col_metadata;
    std::vector<char> data;

public:
    table_extension() {}

    table_extension(std::fstream &file) : extension_hdu(file)
    {
        tfields = this->value_of<std::size_t>("TFIELDS");
        col_metadata.reserve(tfields);
    }

    table_extension(std::fstream &file, hdu const& other) : extension_hdu(file, other)
    {
        tfields = this->value_of<std::size_t>("TFIELDS");
        col_metadata.reserve(tfields);
    }

    table_extension(std::fstream &file, std::streampos pos) : extension_hdu(file, pos)
    {
        tfields = this->value_of<std::size_t>("TFIELDS");
        col_metadata.reserve(tfields);
    }
};

}}} //namespace boost::astronomy::io
#endif // !BOOST_ASTRONOMY_IO_TABLE_EXTENSION
