/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE fits_reader_test

#include <boost/test/unit_test.hpp>
#include <boost/variant.hpp>
#include <boost/astronomy/io/fits_reader.hpp>
#include <boost/astronomy/io/fits_stream.hpp>
#include <boost/astronomy/io/default_hdus.hpp>
#include <boost/astronomy/io/primary_hdu.hpp>
#include <boost/astronomy/io/fits.hpp>
#include <boost/astronomy/io/string_conversion_utility.hpp>
#include <boost/astronomy/io/data_conversions.hpp>

using namespace boost::astronomy::io;

namespace fits_test {

    class fits_reader_fixture {
        std::string samples_directory;
    public:
        std::string sample1_path;
        fits_io<fits_stream, default_hdu_manager<card_policy,ascii_converter,data_conversions>> reader;
        fits_reader_fixture() {

#ifdef SOURCE_DIR
            samples_directory = std::string((std::string(SOURCE_DIR) +
                "/fits_sample_files/"));
#else
            samples_directory = std::string(
                std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
                "/fits_sample_files/");
#endif
            sample1_path = samples_directory + "fits_sample1.fits";
            reader.initialize(sample1_path);
        }

    };
    struct fetch_data_size :boost::static_visitor <std::size_t> {
    template<typename T>
     std::size_t operator ()(const T& hdu) const {
        return hdu.get_data().size();
    }
   
};
    template<>
     std::size_t fits_test::fetch_data_size::operator()(const boost::blank&)const {
        return 0;
    }
    template<>
     std::size_t fits_test::fetch_data_size::operator()(const basic_primary_hdu<card_policy>& prime_hdu)const {
        return prime_hdu.get_data<bitpix::_B32>().size();
    }
}



BOOST_AUTO_TEST_SUITE(convenience_functions)

BOOST_FIXTURE_TEST_CASE(read_only_fits_headers,fits_test::fits_reader_fixture) {
    reader.read_only_headers();

    for (auto& hdu : reader.get_hdu_list()) {
        auto hdu_data_size = boost::apply_visitor(fits_test::fetch_data_size(), hdu);
        BOOST_REQUIRE_EQUAL(hdu_data_size, 0);
    } 
}
BOOST_FIXTURE_TEST_CASE(read_entire_hdus, fits_test::fits_reader_fixture) {
    reader.read_entire_hdus();

    BOOST_REQUIRE_EQUAL(
        boost::apply_visitor(fits_test::fetch_data_size(), reader["primary_hdu"]),
        200 * 200 * 4
    );
}
BOOST_FIXTURE_TEST_CASE(get_hdu_by_index, fits_test::fits_reader_fixture) {
    reader.read_only_headers();

    BOOST_NOEXCEPT_OR_NOTHROW(
        fits::convert_to<basic_primary_hdu<card_policy>>(reader[0])
    );
}
BOOST_FIXTURE_TEST_CASE(invalid_index, fits_test::fits_reader_fixture) {
    reader.read_only_headers();

    BOOST_REQUIRE_THROW(reader[10],std::out_of_range);
}
BOOST_FIXTURE_TEST_CASE(get_hdu_by_name, fits_test::fits_reader_fixture) {
    reader.read_only_headers();

    BOOST_NOEXCEPT_OR_NOTHROW(
        fits::convert_to<basic_primary_hdu<card_policy>>(reader["primary_hdu"])
    );
}
BOOST_FIXTURE_TEST_CASE(invalid_name, fits_test::fits_reader_fixture) {
    reader.read_only_headers();

    BOOST_REQUIRE_THROW(reader["test_hdu"], std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
