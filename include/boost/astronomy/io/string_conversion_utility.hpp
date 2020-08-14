/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_STRING_CONVERSION_UTILITY_HPP
#define BOOST_ASTRONOMY_IO_STRING_CONVERSION_UTILITY_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/type.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>



namespace boost{namespace astronomy{namespace io{


   template<typename T>
   void spirit_type(boost::type<T>dummy) {}

   boost::spirit::qi::int_type spirit_type(boost::type<int>) { return boost::spirit::qi::int_;  }
   boost::spirit::qi::uint_type spirit_type(boost::type<unsigned int>) { return boost::spirit::qi::uint_; }
   boost::spirit::qi::float_type spirit_type(boost::type<float>) { return boost::spirit::qi::float_; }
   boost::spirit::qi::double_type spirit_type(boost::type<double>) { return boost::spirit::qi::double_; }
   boost::spirit::qi::long_long_type spirit_type(boost::type<long long>) { return boost::spirit::qi::long_long; }
   boost::spirit::qi::ulong_long_type spirit_type(boost::type<long unsigned int>) {return boost::spirit::qi::ulong_long ;}
   boost::spirit::qi::ulong_long_type spirit_type(boost::type<unsigned long long>) { return boost::spirit::qi::ulong_long; }

    template<typename T>
    T convert_to(const std::string& convert_str) {

        T converted_value;
        auto iter_pos = convert_str.begin();
        boost::spirit::qi::parse(iter_pos, convert_str.end(),spirit_type(boost::type<T>()),converted_value);
        if (iter_pos != convert_str.end()) {

            throw invalid_cast("Cannot convert from String to the required Type");
        }

        return converted_value;
    }


} } }





#endif
