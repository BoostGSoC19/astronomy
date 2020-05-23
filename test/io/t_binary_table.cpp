/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE binary_table_test

#include <boost/astronomy/io/binary_table.hpp>
#include <boost/test/unit_test.hpp>

class binary_table_fixture {
    std::fstream binary_table_sample1;
    std::vector<char> bin_sample1_data;

public:
    binary_table_fixture() {
        std::string samples_directory;


#ifdef SOURCE_DIR
        samples_directory =
            std::string((std::string(SOURCE_DIR) + "/fits_sample_files/"));
#else
        samples_directory = std::string(
            std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
            "/fits_sample_files/");
#endif

        binary_table_sample1.open(samples_directory + "fits_sample3.fits",
            std::ios::in | std::ios::binary);


        // Copy data part of hdu
        binary_table_sample1.seekg(17280);
        bin_sample1_data.resize(1152480);

        char* buffer = new char[1152480];
        binary_table_sample1.read(buffer, 1152480);
        bin_sample1_data.assign(buffer, buffer + 1152480);

        binary_table_sample1.seekg(5760);
        delete[] buffer;
    }

    std::fstream& get_binary_table_sample1() { return binary_table_sample1; }
    const std::vector<char>& get_binary_table_sample1_data() {
        return bin_sample1_data;
    }
};

using namespace boost::astronomy::io;

BOOST_AUTO_TEST_SUITE(binary_table_ctors)

BOOST_FIXTURE_TEST_CASE(filestream_ctor, binary_table_fixture) {
    binary_table_extension binary_table1(get_binary_table_sample1());

    BOOST_REQUIRE_EQUAL(binary_table1.card_count(), 122);
    BOOST_REQUIRE_EQUAL(binary_table1.get_data().size(), 1152480);
}
BOOST_FIXTURE_TEST_CASE(hdu_ctor, binary_table_fixture) {
    binary_table_extension binary_table1(get_binary_table_sample1(),
        hdu(get_binary_table_sample1()));

    BOOST_REQUIRE_EQUAL(binary_table1.card_count(), 122);
    BOOST_REQUIRE_EQUAL(binary_table1.get_data().size(), 1152480);
}
BOOST_FIXTURE_TEST_CASE(filestream_specific_pos_ctor, binary_table_fixture) {
    get_binary_table_sample1().seekg(0);

    binary_table_extension binary_table1(get_binary_table_sample1(), 5760);

    BOOST_REQUIRE_EQUAL(binary_table1.card_count(), 122);
    BOOST_REQUIRE_EQUAL(binary_table1.get_data().size(), 1152480);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(convenience_methods)

BOOST_FIXTURE_TEST_CASE(binary_table_fetch_data, binary_table_fixture) {
    binary_table_extension binary_table1(get_binary_table_sample1());

    BOOST_REQUIRE_EQUAL_COLLECTIONS(binary_table1.get_data().begin(),
        binary_table1.get_data().end(),
        get_binary_table_sample1_data().begin(),
        get_binary_table_sample1_data().end());
}
BOOST_FIXTURE_TEST_CASE(binary_table_check_column_size, binary_table_fixture) {
    binary_table_extension binary_table1;

    BOOST_REQUIRE_EQUAL(binary_table1.column_size("144000I"), 288000);
    BOOST_REQUIRE_EQUAL(binary_table1.column_size("I"), 2);
}

BOOST_FIXTURE_TEST_CASE(binary_table_get_column, binary_table_fixture) {
    binary_table_extension binary_table1(get_binary_table_sample1());

    auto column_info =
        binary_table1.get_column<std::vector<std::float_t>>("DEL_TIME");

    BOOST_REQUIRE_CLOSE(column_info->get_data()[0][119], 595.0, 0.001);
}

BOOST_FIXTURE_TEST_CASE(binary_table_check_element_count,
    binary_table_fixture) {
    binary_table_extension binary_table1;

    BOOST_REQUIRE_EQUAL(binary_table1.element_count("300I"), 300);
    BOOST_REQUIRE_EQUAL(binary_table1.element_count("I"), 1);
}
BOOST_FIXTURE_TEST_CASE(binary_table_get_type, binary_table_fixture) {
    binary_table_extension binary_table1;

    BOOST_REQUIRE_EQUAL(binary_table1.get_type("242000I"), 'I');
}
BOOST_FIXTURE_TEST_CASE(binary_table_type_size, binary_table_fixture) {
    binary_table_extension binary_table1;

    BOOST_REQUIRE_EQUAL(binary_table1.type_size('A'), 1);
    BOOST_REQUIRE_THROW(binary_table1.type_size('G'),
        boost::astronomy::invalid_table_colum_format);
}

BOOST_AUTO_TEST_SUITE_END()
