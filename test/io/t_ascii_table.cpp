/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE ascii_table_test

#include <boost/astronomy/io/ascii_table.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/test/unit_test.hpp>


class ascii_table_fixture {
    std::fstream ascii_table_sample1;
    std::fstream ascii_table_sample2;
    std::vector<char> sample1_data;
    std::vector<char> sample2_data;

public:
    ascii_table_fixture() {
        // Copy data into string

        std::string samples_directory;

#ifdef SOURCE_DIR
        samples_directory = std::string((std::string(SOURCE_DIR) +
            "/fits_sample_files/"));
#else
        samples_directory = std::string(
            std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
            "/fits_sample_files/");
        ;
#endif


        ascii_table_sample1.open(samples_directory + "fits_sample1.fits");
        ascii_table_sample2.open(samples_directory + "fits_sample2.fits");

        ascii_table_sample1.seekg(694080);
        ascii_table_sample2.seekg(11520);

        sample1_data.resize(3184);
        sample2_data.resize(75660);

        std::copy_n(std::istreambuf_iterator<char>(ascii_table_sample1), 3184,
            sample1_data.begin());
        std::copy_n(std::istreambuf_iterator<char>(ascii_table_sample2), 75660,
            sample2_data.begin());

        // Put to the starting of ASCII header
        ascii_table_sample1.seekg(665280);
        ascii_table_sample2.seekg(2880);
    }

    std::fstream& get_ascii_sample1() { return ascii_table_sample1; }
    std::fstream& get_ascii_sample2() { return ascii_table_sample2; }

    const std::vector<char>& get_ascii_sample1_data() { return sample1_data; }
    const std::vector<char>& get_ascii_sample2_data() { return sample2_data; }
};

using namespace boost::astronomy::io;

BOOST_AUTO_TEST_SUITE(ascii_table_ctors)

BOOST_FIXTURE_TEST_CASE(filestream_ctor, ascii_table_fixture) {
    ascii_table ascii_hdu1(get_ascii_sample1());
    ascii_table ascii_hdu2(get_ascii_sample2());

    BOOST_REQUIRE_EQUAL(ascii_hdu1.card_count(), 353);
    BOOST_REQUIRE_EQUAL(ascii_hdu1.get_data().size(), 3184);

    BOOST_REQUIRE_EQUAL(ascii_hdu2.card_count(), 105);
    BOOST_REQUIRE_EQUAL(ascii_hdu2.get_data().size(), 75660);
}
BOOST_FIXTURE_TEST_CASE(hdu_ctor, ascii_table_fixture) {
    ascii_table ascii_hdu1(get_ascii_sample1(), hdu(get_ascii_sample1()));
    ascii_table ascii_hdu2(get_ascii_sample2(), hdu(get_ascii_sample2()));

    BOOST_REQUIRE_EQUAL(ascii_hdu1.card_count(), 353);
    BOOST_REQUIRE_EQUAL(ascii_hdu1.get_data().size(), 3184);

    BOOST_REQUIRE_EQUAL(ascii_hdu2.card_count(), 105);
    BOOST_REQUIRE_EQUAL(ascii_hdu2.get_data().size(), 75660);
}
BOOST_FIXTURE_TEST_CASE(filestream_specific_pos_ctor, ascii_table_fixture) {
    get_ascii_sample1().seekg(0);
    get_ascii_sample2().seekg(0);

    ascii_table ascii_hdu1(get_ascii_sample1(), 665280);
    ascii_table ascii_hdu2(get_ascii_sample2(), 2880);

    BOOST_REQUIRE_EQUAL(ascii_hdu1.card_count(), 353);
    BOOST_REQUIRE_EQUAL(ascii_hdu1.get_data().size(), 3184);

    BOOST_REQUIRE_EQUAL(ascii_hdu2.card_count(), 105);
    BOOST_REQUIRE_EQUAL(ascii_hdu2.get_data().size(), 75660);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(convenience_methods)

BOOST_FIXTURE_TEST_CASE(ascii_table_fetch_data, ascii_table_fixture) {
    ascii_table ascii_hdu1(get_ascii_sample1());
    ascii_table ascii_hdu2(get_ascii_sample2());

    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        ascii_hdu1.get_data().begin(), ascii_hdu1.get_data().end(),
        get_ascii_sample1_data().begin(), get_ascii_sample1_data().end());

    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        ascii_hdu2.get_data().begin(), ascii_hdu2.get_data().end(),
        get_ascii_sample2_data().begin(), get_ascii_sample2_data().end());
}

BOOST_FIXTURE_TEST_CASE(
    ascii_table_get_column,
    ascii_table_fixture /*,*boost::unit_test::disabled()*/) {
    ascii_table ascii_hdu1(get_ascii_sample1());
    boost::float32_t mean_c200_data[] = { 0.3115222, 0.6534808, 0.7027547, 0.9687142 };

    auto mean_c200_col = boost::dynamic_pointer_cast<column_data<boost::float32_t>>(
        ascii_hdu1.get_column("MEANC200"));

    for (std::size_t i = 0; i < mean_c200_col->get_data().size(); i++) {

        BOOST_CHECK_CLOSE(mean_c200_col->get_data()[i], mean_c200_data[i], 0.001);

    }

    // Problem in GCC7.4
    //BOOST_REQUIRE_EQUAL_COLLECTIONS(mean_c200_col->get_data().begin(),
    //    mean_c200_col->get_data().end(),
    //    mean_c200_data, mean_c200_data + 4);

    ascii_table ascii_hdu2(get_ascii_sample2());

    auto pk_col = boost::dynamic_pointer_cast<column_data<std::string>>(
        ascii_hdu2.get_column("PK"));
    std::string pk[] = { "18+ 2.1  0", "19+ 6.1  0", "20+ 9.1  0" };

    BOOST_REQUIRE_EQUAL_COLLECTIONS(pk_col->get_data().begin(),
        pk_col->get_data().begin() + 3, pk, pk + 3);
}

BOOST_FIXTURE_TEST_CASE(ascii_table_get_column_size, ascii_table_fixture) {
    ascii_table ascii_hdu;
    BOOST_REQUIRE_EQUAL(ascii_hdu.column_size("D25.17"), 25);
    BOOST_REQUIRE_EQUAL(ascii_hdu.column_size("I5"), 5);
}
BOOST_FIXTURE_TEST_CASE(ascii_table_get_column_type, ascii_table_fixture) {
    ascii_table ascii_hdu;
    BOOST_REQUIRE_EQUAL(ascii_hdu.get_type("D25.17"), 'D');
    BOOST_REQUIRE_EQUAL(ascii_hdu.get_type("I20"), 'I');
}

BOOST_AUTO_TEST_SUITE_END()
