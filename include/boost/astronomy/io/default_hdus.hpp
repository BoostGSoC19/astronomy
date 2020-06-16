#ifndef BOOST_ASTRONOMY_IO_DEFAULT_HDUS_HPP
#define BOOST_ASTRONOMY_IO_DEFAULT_HDUS_HPP


#include<boost/astronomy/io/hdu.hpp>
#include<boost/astronomy/io/primary_hdu.hpp>
#include<boost/astronomy/io/binary_table.hpp>
#include<boost/astronomy/io/ascii_table.hpp>
#include<boost/variant.hpp>



namespace boost { namespace astronomy {namespace io {
    struct default_hdu_manager{

        typedef boost::variant<
            boost::blank,
            primary_hdu,   
            binary_table_extension,
            ascii_table

        >  Extension;

        static Extension construct_hdu(header& hdu_header, const std::string& data_buffer = "") {
            if (hdu_header.contains_keyword("SIMPLE")) {
                // ITS PRIMARY HDU
                return generate_primary_hdu(hdu_header, data_buffer);
            }
            else {
                // FOR ALL EXTENSION HDU's
                return generate_extension_hdu(hdu_header, data_buffer);
            }
        }

        static Extension generate_extension_hdu(header& hdu_header, const std::string& data_buffer) {
            std::string extension_name = hdu_header.value_of<std::string>("XTENSION");
            if (extension_name == "TABLE") { return ascii_table(hdu_header, data_buffer); }
            else if (extension_name == "BINTABLE") { return binary_table_extension(hdu_header, data_buffer); }
            else { return boost::blank{}; }
        }
        static Extension generate_primary_hdu(header& prime_header, const std::string& buffer) {
            Extension prime_hdu;
            prime_hdu = primary_hdu(prime_header, buffer);
            return prime_hdu;
        }
    };
        }
    }
}
#endif
