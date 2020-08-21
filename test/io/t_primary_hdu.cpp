/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE primary_hdu_test

#include <boost/astronomy/io/primary_hdu.hpp>
#include <boost/astronomy/io/fits_stream.hpp>
#include "base_fixture.hpp"
#include <boost/test/unit_test.hpp>

using namespace boost::astronomy::io;

namespace fits_test {

    class primary_hdu_fixture:public base_fixture<fits_stream,card_policy> {
    public:
        basic_primary_hdu<card_policy> test_p_hdu1;
        primary_hdu_fixture() {
#ifdef SOURCE_DIR
            samples_directory =
                std::string((std::string(SOURCE_DIR) + "/fits_sample_files/"));
#else
            samples_directory = std::string(
                std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
                "/fits_sample_files/");           
#endif
            load_file("fits_sample1.fits");  
            initialize_primary_hdu(test_p_hdu1, "fits_sample1");
        }
    private:
        void initialize_primary_hdu(basic_primary_hdu<card_policy>& prime_hdu, const std::string& sample_name) {
            hdu_store<card_policy>* raw_primary_hdu_sample = get_raw_hdu(sample_name, "primary_hdu");
            if (raw_primary_hdu_sample != nullptr) {
                prime_hdu = basic_primary_hdu<card_policy>(raw_primary_hdu_sample->hdu_header, raw_primary_hdu_sample->hdu_data_buffer);
            }
        }
    };

}

BOOST_AUTO_TEST_SUITE(primary_hdu_constructors)

BOOST_FIXTURE_TEST_CASE(primary_hdu_ctor, fits_test::primary_hdu_fixture) {

    fits_test::hdu_store<card_policy>* raw_prime_hdu = get_raw_hdu("fits_sample1", "primary_hdu");
   
    basic_primary_hdu<card_policy> prime_hdu_1(raw_prime_hdu->hdu_header,raw_prime_hdu->hdu_data_buffer);
    BOOST_REQUIRE_EQUAL(prime_hdu_1.get_header().card_count(), 262);     
    BOOST_REQUIRE_EQUAL(prime_hdu_1.get_data<bitpix::_B32>().size(), 200 * 200 * 4);

    
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(primary_hdu_utlity_methods)

BOOST_FIXTURE_TEST_CASE(primary_hdu_get_data, fits_test::primary_hdu_fixture) {
    BOOST_REQUIRE_EQUAL(test_p_hdu1.get_data<bitpix::_B32>().size(), 160000);
}
BOOST_FIXTURE_TEST_CASE(primary_hdu_check_simple, fits_test::primary_hdu_fixture) {
    BOOST_ASSERT(test_p_hdu1.is_simple());
}
BOOST_FIXTURE_TEST_CASE(primary_hdu_check_extended, fits_test::primary_hdu_fixture) {
    BOOST_ASSERT(test_p_hdu1.is_extended());
 }

BOOST_AUTO_TEST_SUITE_END()
