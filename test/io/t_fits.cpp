/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE fits_external_interface


#include<boost/test/unit_test.hpp>
#include <boost/variant.hpp>
#include<string>
#include<fstream>
#include<stdexcept>

#include<boost/astronomy/io/stream_reader.hpp>
#include<boost/astronomy/io/default_hdus.hpp>
#include<boost/astronomy/io/default_card_policy.hpp>
#include<boost/astronomy/io/fits.hpp>
#include "base_fixture.hpp"



namespace fits_test {

    class fits_fixture:public base_fixture<fits_stream_reader> {
    public:
        std::string sample_1_path;
        std::string sample_2_path;
        std::string sample_3_path;

        fits_fixture() {
#ifdef SOURCE_DIR
            samples_directory = std::string((std::string(SOURCE_DIR) +
                "/fits_sample_files/"));
#else
            samples_directory = std::string(
                std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
                "/fits_sample_files/");
            ;
#endif
            sample_1_path = get_absolute_path("fits_sample1.fits");
            sample_2_path = get_absolute_path("fits_sample2.fits");
            sample_3_path = get_absolute_path("fits_sample3.fits");
        }
    };
}

using namespace boost::astronomy::io;
using fits = basic_fits<fits_stream_reader, default_hdu_manager<card_policy>>;


BOOST_FIXTURE_TEST_CASE(set_filename_on_valid_filepath,fits_test::fits_fixture) {
    auto astro_data = fits::open(sample_1_path);

    BOOST_REQUIRE_EQUAL(astro_data.get_control_block_info().filepath, sample_1_path);
}

BOOST_FIXTURE_TEST_CASE(raise_exception_on_invalid_filepath, fits_test::fits_fixture) {
    std::string filename = "Garbage file path";

    BOOST_REQUIRE_THROW(fits::open(filename), boost::astronomy::file_reading_exception );
}

BOOST_FIXTURE_TEST_CASE(open_read_only_headers, fits_test::fits_fixture) {

    auto astro_data = fits::open(sample_1_path,reading_options::read_only_headers);

    auto& prime_hdu = fits::convert_to<primary_hdu>(astro_data[0]);


   BOOST_REQUIRE_EQUAL(prime_hdu.get_header().card_count(), 262);
   BOOST_REQUIRE_EQUAL(prime_hdu.get_data < bitpix::_B32> ().size(), 0);

}

BOOST_FIXTURE_TEST_CASE(open_read_entire_hdus, fits_test::fits_fixture) {

    auto astro_data = fits::open(sample_1_path, reading_options::read_entire_hdus);

    auto& prime_hdu = fits::convert_to<primary_hdu>(astro_data[0]);

    BOOST_REQUIRE_EQUAL(prime_hdu.get_header().card_count(), 262);
    BOOST_REQUIRE_EQUAL(prime_hdu.get_data<bitpix::_B32>().size(), 160000);
}

