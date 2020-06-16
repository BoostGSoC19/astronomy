/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_FITS_HPP
#define BOOST_ASTRONOMY_IO_FITS_HPP

#include <boost/variant.hpp>
#include <boost/astronomy/io/fits_reader.hpp>


namespace boost { namespace astronomy {namespace io {
  


    enum class reading_options {
        read_only_headers,
        read_entire_hdus
    };

    template<typename FileReader, typename ExtensionsSupported>
    class basic_fits {
        typedef fits_reader<FileReader, ExtensionsSupported> fitsreader;
    public:
        basic_fits() {}
        static fitsreader open(const std::string& filepath, reading_options reading_option = reading_options::read_entire_hdus) {
            fitsreader f_reader(filepath);
            if (reading_option == reading_options::read_only_headers) {
                f_reader.read_only_headers();
            }
            else {
                f_reader.read_entire_hdus();
            }

            return f_reader;
        }
        template<typename ConversionType>
        static ConversionType& convert_to(typename ExtensionsSupported::Extension& extension) {
            return boost::get<ConversionType>(extension);
        }
    };

        }
    }
}

#endif // !BOOST_ASTRONOMY_IO_FITS_HPP

