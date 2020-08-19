/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE string_conversions

#include <boost/test/unit_test.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>
#include <boost/astronomy/io/string_conversion_utility.hpp>

using namespace boost::astronomy::io;

BOOST_AUTO_TEST_SUITE(conversion_tests)

BOOST_AUTO_TEST_CASE(string_to_int) {

    BOOST_REQUIRE_EQUAL(ascii_converter::deserialize_to<int>("1234",0),1234 );
}

BOOST_AUTO_TEST_CASE(string_to_unsigned_int) {

    BOOST_REQUIRE_EQUAL(ascii_converter::deserialize_to<unsigned int>("4294967295",0), 4294967295);
}

BOOST_AUTO_TEST_CASE(string_to_float) {

    BOOST_REQUIRE_CLOSE(ascii_converter::deserialize_to<float>("1.4",0), 1.4, 0.001);
}

BOOST_AUTO_TEST_CASE(string_to_double) {

    BOOST_REQUIRE_CLOSE(ascii_converter::deserialize_to<double>("1234323.2334242",0), 1234323.2334242,0.001);
}

BOOST_AUTO_TEST_CASE(string_to_long_long) {

    BOOST_REQUIRE_EQUAL(ascii_converter::deserialize_to<long long>("123444345345543345",0),123444345345543345);
}

BOOST_AUTO_TEST_CASE(string_to_size_t) {

    BOOST_REQUIRE_EQUAL(ascii_converter::deserialize_to<std::size_t>("18446",0), 18446);
}


BOOST_AUTO_TEST_CASE(raise_exception_on_invalid_value) {

    BOOST_REQUIRE_THROW(ascii_converter::deserialize_to<int>("123a",0), boost::astronomy::invalid_cast);
}

BOOST_AUTO_TEST_CASE(raise_exception_on_incompatible_type) {

    BOOST_REQUIRE_THROW(ascii_converter::deserialize_to<int>("14.23",0), boost::astronomy::invalid_cast);
}

BOOST_AUTO_TEST_SUITE_END()


