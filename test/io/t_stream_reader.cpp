/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE stream_reader_test

#include <stdio.h>
#include <boost/test/unit_test.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>
#include <boost/astronomy/io/fits_stream.hpp>
#include "base_fixture.hpp"

using namespace boost::astronomy::io;

namespace fits_test {

class stream_reader_fixture {

public:
    std::string samples_directory;
    std::string sample_path;
    fits_stream sample_reader;
    stream_reader_fixture() {
#ifdef SOURCE_DIR
        samples_directory = std::string((std::string(SOURCE_DIR) +
            "/fits_sample_files/"));
#else
        samples_directory = std::string(
            std::string(boost::unit_test::framework::master_test_suite().argv[1]) +
            "/fits_sample_files/");
#endif

        sample_path = samples_directory + "test_random_file.txt";
        sample_reader.set_file(sample_path);
    }


};
}



BOOST_AUTO_TEST_SUITE(initialization)

BOOST_FIXTURE_TEST_CASE(initialize_stream_reader,fits_test::stream_reader_fixture) {
    fits_stream test_reader;

    test_reader.set_file(sample_path);

    BOOST_REQUIRE_EQUAL(test_reader.is_open(), true);
}

BOOST_FIXTURE_TEST_CASE(return_false_on_bad_path, fits_test::stream_reader_fixture) {
    fits_stream test_reader;

    BOOST_REQUIRE_THROW(test_reader.set_file("GarbagePath"), boost::astronomy::file_reading_exception);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(convenience_methods)

BOOST_FIXTURE_TEST_CASE(is_file_accessible, fits_test::stream_reader_fixture) {
    BOOST_REQUIRE_EQUAL(sample_reader.is_open(), true);
}

BOOST_FIXTURE_TEST_CASE(set_reading_position, fits_test::stream_reader_fixture) {
    sample_reader.set_reading_pos(6);

    BOOST_REQUIRE_EQUAL(sample_reader.read(6),"World!");

}

BOOST_FIXTURE_TEST_CASE(get_character_from_file, fits_test::stream_reader_fixture) {
    BOOST_REQUIRE_EQUAL(sample_reader.get(), 'H');
}

BOOST_FIXTURE_TEST_CASE(get_current_file_ptr_position, fits_test::stream_reader_fixture) {
    sample_reader.set_reading_pos(6);

    BOOST_REQUIRE_EQUAL(sample_reader.get_current_pos(), 6);
}

BOOST_FIXTURE_TEST_CASE(file_ptr_at_endoffile, fits_test::stream_reader_fixture) {
    sample_reader.set_reading_pos(75); // At end of file

    BOOST_REQUIRE_EQUAL(sample_reader.at_end(), true);
}

BOOST_FIXTURE_TEST_CASE(create_file, fits_test::stream_reader_fixture) {

    sample_reader.create_file("test.txt");
    sample_reader.get_internal_stream() << "Hello World";

    sample_reader.set_file("test.txt");

    bool file_successfully_opened = sample_reader.is_open();
    BOOST_REQUIRE_EQUAL(file_successfully_opened, true);
    BOOST_REQUIRE_EQUAL(sample_reader.read(5), "Hello");

    sample_reader.close();
    remove("test.txt");

}


BOOST_FIXTURE_TEST_CASE(write_to_file, fits_test::stream_reader_fixture) {

    sample_reader.create_file("test_w.txt");
    sample_reader.write("Astronomy");

    sample_reader.set_file("test_w.txt");
    BOOST_REQUIRE_EQUAL(sample_reader.read(9), "Astronomy");

    sample_reader.close();
    remove("test.txt");
}

BOOST_AUTO_TEST_SUITE_END()
