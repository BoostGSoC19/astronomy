/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE ascii_table_test

#include <boost/astronomy/io/ascii_table.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <boost/astronomy/io/fits_stream.hpp>
#include <boost/astronomy/io/default_card_policy.hpp>
#include "base_fixture.hpp"

using namespace boost::astronomy::io;

namespace fits_test {

    class ascii_table_fixture:public base_fixture<fits_stream,card_policy> {
    public:
        basic_ascii_table<card_policy> ascii_hdu1;
    
        ascii_table_fixture() {
#ifdef SOURCE_DIR
            samples_directory = std::string((std::string(SOURCE_DIR) +
                "/fits_sample_files/"));
#else
            samples_directory = std::string(
                std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
                "/fits_sample_files/");
#endif
            load_file("fits_sample1.fits");


            initialize_ascii_hdu(ascii_hdu1, "fits_sample1");
        }
    private:
         
        void initialize_ascii_hdu(basic_ascii_table<card_policy>& ascii_hdu, const std::string& sample_name) {
            hdu_store<card_policy>* raw_ascii_sample = get_raw_hdu(sample_name, "TABLE");
            if (raw_ascii_sample != nullptr) {
                ascii_hdu = basic_ascii_table<card_policy>(raw_ascii_sample->hdu_header, raw_ascii_sample->hdu_data_buffer);
            }
        }
    };
}

BOOST_AUTO_TEST_SUITE(convenience_methods)

BOOST_FIXTURE_TEST_CASE(ascii_table_get_data, fits_test::ascii_table_fixture) {
    auto total_rows = ascii_hdu1.get_data().size();
    auto total_columns = ascii_hdu1.get_data()[0].size();

    BOOST_REQUIRE_EQUAL(total_rows, 4);
    BOOST_REQUIRE_EQUAL(total_columns, 49);

 }

BOOST_FIXTURE_TEST_CASE(ascii_table_set_data, fits_test::ascii_table_fixture) {
    fits_test::hdu_store<card_policy>* raw_ascii_hdu = get_raw_hdu("fits_sample1", "TABLE");
    basic_ascii_table<card_policy> test_hdu(raw_ascii_hdu->hdu_header,"");

    test_hdu.set_data(raw_ascii_hdu->hdu_data_buffer);

    auto total_rows = test_hdu.get_data().size();
    auto total_columns = test_hdu.get_data()[0].size();

    BOOST_REQUIRE_EQUAL(total_rows, 4);
    BOOST_REQUIRE_EQUAL(total_columns, 49);

    auto mean_c200_col = ascii_hdu1.get_column<boost::float32_t>("MEANC200");
    BOOST_REQUIRE_CLOSE(static_cast<float>(mean_c200_col[0]), 0.3115222f, 0.001);
}

BOOST_FIXTURE_TEST_CASE(ascii_table_get_column,fits_test::ascii_table_fixture) {
    boost::float32_t backgrnd_col_data[] = { -0.367635f, 0.210143f, 0.476156f, 0.346646f };

    auto backgrnd_col = ascii_hdu1.get_column<boost::float32_t>("BACKGRND");
    for (int i = 0; i < 4; i++)
    BOOST_REQUIRE_CLOSE(static_cast<float>(backgrnd_col[i]), backgrnd_col_data[i], 0.001);
    float m = 0;
    for (auto element : backgrnd_col) {
         m=element;
    }
   
}

BOOST_FIXTURE_TEST_CASE(ascii_table_invalid_column_name, fits_test::ascii_table_fixture) {

    BOOST_REQUIRE_THROW(ascii_hdu1.get_column<boost::float32_t>("GARBAGE"),boost::astronomy::column_not_found_exception);  
}


BOOST_AUTO_TEST_CASE(ascii_table_get_column_size) {
    BOOST_REQUIRE_EQUAL(basic_ascii_table<card_policy>::column_size("D25.17"), 25);
}
BOOST_AUTO_TEST_CASE(ascii_table_get_column_type) {
    BOOST_REQUIRE_EQUAL(basic_ascii_table<card_policy>::get_type("D25.17"), 'D');
}

BOOST_AUTO_TEST_SUITE_END()
