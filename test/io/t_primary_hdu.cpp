/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE primary_hdu_test

#include <boost/astronomy/io/primary_hdu.hpp>
#include <boost/test/unit_test.hpp>

class primary_hdu_fixture {
    std::fstream primary_hdu_sample_1;
    std::fstream primary_hdu_sample_2;

public:
    primary_hdu_fixture() {
        std::string samples_directory;
#ifdef SOURCE_DIR
        samples_directory =
            std::string((std::string(SOURCE_DIR) + "/fits_sample_files/"));
#else
        samples_directory = std::string(
            std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
            "/fits_sample_files/");
        ;
#endif

        primary_hdu_sample_1.open(samples_directory + "fits_sample1.fits");
        primary_hdu_sample_2.open(samples_directory + "fits_sample2.fits");


    }

    std::fstream& get_sample1() { return primary_hdu_sample_1; }
    std::fstream& get_sample2() { return primary_hdu_sample_2; }
};

using namespace boost::astronomy::io;

BOOST_AUTO_TEST_SUITE(primary_hdu_constructors)

BOOST_FIXTURE_TEST_CASE(primary_hdu_filestream_ctor, primary_hdu_fixture) {
    primary_hdu<bitpix::_B32> test_p_hdu(get_sample1());
    BOOST_REQUIRE_EQUAL(test_p_hdu.card_count(), 262);
    BOOST_REQUIRE_EQUAL(test_p_hdu.get_data().size(), 200 * 200 * 4);

    primary_hdu<bitpix::B8> test_p_hdu2(get_sample2());
    BOOST_REQUIRE_EQUAL(test_p_hdu2.card_count(), 7);
    BOOST_REQUIRE_EQUAL(test_p_hdu2.get_data().size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(primary_hdu_utlity_methods)

BOOST_FIXTURE_TEST_CASE(primary_hdu_get_data, primary_hdu_fixture) {
    // TODO : BETTER TESTS ARE REQUIRED
    primary_hdu<bitpix::_B32> test_p_hdu1(get_sample1());
    primary_hdu<bitpix::_B32> test_p_hdu2(get_sample2());

    BOOST_REQUIRE_EQUAL(test_p_hdu1.get_data().size(), 160000);
    BOOST_REQUIRE_EQUAL(test_p_hdu2.get_data().size(), 0);
}
BOOST_FIXTURE_TEST_CASE(primary_hdu_check_simple, primary_hdu_fixture) {
    // TODO : BETTER TESTS ARE REQUIRED
    primary_hdu<bitpix::_B32> test_p_hdu1(get_sample1());
    primary_hdu<bitpix::_B32> test_p_hdu2(get_sample2());

    BOOST_ASSERT(test_p_hdu1.is_simple());
    BOOST_ASSERT(test_p_hdu2.is_simple());
}
BOOST_FIXTURE_TEST_CASE(primary_hdu_check_extended, primary_hdu_fixture) {
    // TODO : BETTER TESTS ARE REQUIRED
    primary_hdu<bitpix::_B32> test_p_hdu1(get_sample1());
    primary_hdu<bitpix::_B32> test_p_hdu2(get_sample2());

    BOOST_ASSERT(test_p_hdu1.is_extended());
    BOOST_ASSERT(test_p_hdu2.is_extended());
}

BOOST_AUTO_TEST_SUITE_END()
