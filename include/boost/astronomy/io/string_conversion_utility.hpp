/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_STRING_CONVERSION_UTILITY_HPP
#define BOOST_ASTRONOMY_IO_STRING_CONVERSION_UTILITY_HPP

#include <sstream>
#include <limits>
#include <boost/lexical_cast.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>



namespace boost{namespace astronomy{namespace io{

    /**
     * @brief Used for serialization and deserialization of ASCII table's data
    */
    struct ascii_converter {

        /**
         * @brief Deserializes the ASCII data to the given type
         * @param[in] convert_str ASCII data to convert to given type
         * @tparam T type to deserialize the data to
         * @note The second parameter is basically the number of elements in given string
         *       but as ASCII table does not support multiple elements in single field it
         *       is left blank ( To comply to the compile time interface )
        */
        template<typename T>
        static T deserialize_to(const std::string& convert_str, int) {
            try {
                return boost::lexical_cast<T>(convert_str);
            }
            catch (bad_lexical_cast&) {
                throw invalid_cast("Cannot convert from String to the required Type");
            }
        }

        /**
         * @brief Serializes the value of given type to string
         * @param[in] value The value to be serialized
         * @tparam T Type of value which needs to be serialized
        */
        template<typename T>
        static std::string serialize(T value) {
            return
                boost::hana::eval_if(std::numeric_limits<T>::is_integer,
                    [&value] {return std::to_string(value); },
                    [&value](auto _) {
                        //to_string does not maintain the same rep for floating point numbers causing field size problems
                        //to_string dosent work for custom types as well(although here it is)
                        std::stringstream conversion_stream;
                        conversion_stream << value;
                        return conversion_stream.str();
                    });
        }

    };


} } }


#endif
