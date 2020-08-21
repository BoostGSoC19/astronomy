/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE hdu_test

#include <boost/astronomy/io/hdu.hpp>
#include <boost/astronomy/io/fits_stream.hpp>
#include <boost/astronomy/io/default_card_policy.hpp>
#include <boost/test/unit_test.hpp>
#include "base_fixture.hpp"
#include <map>
#include <stdio.h>

using namespace boost::astronomy::io;
namespace fits_test {

    class hdu_fixture :public base_fixture<fits_stream,card_policy> {
    public:
        header<card_policy> sample_1;
        hdu_fixture() {
#ifdef SOURCE_DIR
            samples_directory = std::string((std::string(SOURCE_DIR) +
                "/fits_sample_files/"));
#else
            samples_directory = std::string(
                std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
                "/fits_sample_files/");
#endif
            load_file("fits_sample1.fits");
            initialize_header(sample_1, "fits_sample1", "primary_hdu");
        }
    private:

        void initialize_header(header<card_policy>& hdu_header, const std::string& sample_name, const std::string& hdu_name) {
            hdu_store<card_policy>* sample = get_raw_hdu(sample_name, hdu_name);
            if (sample != nullptr) {
                hdu_header = sample->hdu_header;
            }
        }
    };
}


BOOST_AUTO_TEST_SUITE(hdu_methods)

BOOST_FIXTURE_TEST_CASE(read_from_filereader,fits_test::hdu_fixture) {
    BOOST_REQUIRE_EQUAL(sample_1.card_count(), static_cast<std::size_t>(262));
}

BOOST_FIXTURE_TEST_CASE(write_header_to_file, fits_test::hdu_fixture) {

    fits_stream test_file;
    std::string file_path = get_absolute_path("test_file_hdu.fits");
    test_file.create_file(file_path);

    sample_1.write_header(test_file);

    // Reopen for reading
    test_file.set_file(file_path);
    header<card_policy> test_header;
    test_header.read_header(test_file);


    BOOST_CHECK(test_header == get_raw_hdu("fits_sample1", "primary_hdu")->hdu_header);


    test_file.close();
    remove(file_path.c_str());
}



BOOST_FIXTURE_TEST_CASE(bitpix_func, fits_test::hdu_fixture) {
    BOOST_CHECK_MESSAGE(sample_1.bitpix() == bitpix::_B32,
        "The bitpix values do not match");
}
BOOST_FIXTURE_TEST_CASE(total_dimensions, fits_test::hdu_fixture) {
    BOOST_REQUIRE_EQUAL(sample_1.total_dimensions(), static_cast<std::size_t>(3));
 
}

BOOST_FIXTURE_TEST_CASE(naxis_n, fits_test::hdu_fixture) {
    BOOST_REQUIRE_EQUAL(sample_1.naxis(1), static_cast<std::size_t>(200));
    BOOST_REQUIRE_EQUAL(sample_1.naxis(2), static_cast<std::size_t>(200));
    BOOST_REQUIRE_EQUAL(sample_1.naxis(3), static_cast<std::size_t>(4));

}

BOOST_FIXTURE_TEST_CASE(value_of, fits_test::hdu_fixture) {
    BOOST_REQUIRE_EQUAL(sample_1.value_of<std::string>("CTYPE2"),
        "DEC--TAN");
}
BOOST_FIXTURE_TEST_CASE(value_of_int, fits_test::hdu_fixture) {
    BOOST_REQUIRE_EQUAL(sample_1.value_of<int>("GPIXELS"), 632387);
}
BOOST_FIXTURE_TEST_CASE(value_of_floating, fits_test::hdu_fixture) {
    BOOST_CHECK_CLOSE(sample_1.value_of<double>("PHOTFLAM"), 3.44746E-16, 0.001);
}
BOOST_FIXTURE_TEST_CASE(card_count, fits_test::hdu_fixture) {
    BOOST_REQUIRE_EQUAL(sample_1.card_count(), static_cast<std::size_t>(262));
}
BOOST_AUTO_TEST_SUITE_END()
