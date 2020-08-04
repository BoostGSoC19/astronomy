/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE card_policy_test

#include <boost/test/unit_test.hpp>
#include <boost/astronomy/io/default_card_policy.hpp>


using namespace boost::astronomy::io;
class cardpolicy_fixture {

public:
    card_policy policy;

};


BOOST_AUTO_TEST_SUITE(validation_methods)

BOOST_FIXTURE_TEST_CASE(keyword_validation,cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_key_valid("SIMPLE  "), true); 
}

BOOST_FIXTURE_TEST_CASE(keyword_without_padding_validation, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_key_valid("SIMPLE"), true);
}

BOOST_FIXTURE_TEST_CASE(keyword_fail_validation, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_key_valid("GARBAGE KEYWORD"), false);

}

BOOST_FIXTURE_TEST_CASE(card_validation,cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("SIMPLE  =                    T / file does conform to FITS standard             "), true);
}

BOOST_FIXTURE_TEST_CASE(invalid_card_length_more_than_80, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("SIMPLE  =         garbageeqeqe          T / file does conform to FITS standard             "), false);
}

BOOST_FIXTURE_TEST_CASE(invalid_card_no_equals_and_space_in_col_9_10_for_non_reserved_keys, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("SIMPLE                       T / file does conform to FITS standard             "),false);
}

BOOST_FIXTURE_TEST_CASE(valid_card_no_equals_and_space_in_col_9_10_for_reserved_keys, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("COMMENT   FITS (Flexible Image Transport System) format is defined in 'Astronomy"), true);
}

BOOST_FIXTURE_TEST_CASE(valid_card_blank_field, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("          FITS (Flexible Image Transport System) format is defined in 'Astronomy"), true);
}

BOOST_FIXTURE_TEST_CASE(card_validation_split_params_all_string, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("SIMPLE", "T", ""), true);
}

BOOST_FIXTURE_TEST_CASE(card_invalid_split_params_large_value_length_and_no_comment, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("KEYWORD",std::string(71,'A'), ""), false);
}

BOOST_FIXTURE_TEST_CASE(card_invalid_split_params_large_value_length_with_comment, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.is_card_valid("KEYWORD", std::string(63, 'A'), std::string(6,'H')), false);
}

BOOST_FIXTURE_TEST_CASE(parse_value_to_type, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.parse_to<int>("123"),123);
}

BOOST_FIXTURE_TEST_CASE(raise_exception_on_parse_value_to_wrong_type, cardpolicy_fixture) {

    BOOST_REQUIRE_THROW(policy.parse_to<int>("ABC"),boost::astronomy::invalid_cast);
}

BOOST_FIXTURE_TEST_CASE(parse_value_to_type_complex_numbers, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.parse_to<std::complex<int>>("123 456"), std::complex<int>(123,456));
}

BOOST_FIXTURE_TEST_CASE(raise_exception_wrong_type_parse_value_to_type_complex_numbers, cardpolicy_fixture) {

    BOOST_REQUIRE_THROW(policy.parse_to<std::complex<int>>("123 4.5656"),boost::astronomy::invalid_cast);
}
BOOST_FIXTURE_TEST_CASE(parse_value_to_boolean, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.parse_to<bool>("T"),true);
}

BOOST_FIXTURE_TEST_CASE(serialize_bool, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.serialize_to_fits_format(true), std::string("T").insert(0, 19, ' '));
}

BOOST_FIXTURE_TEST_CASE(serialize_longlong, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.serialize_to_fits_format(1234343423222343421)," 1234343423222343421");
}

BOOST_FIXTURE_TEST_CASE(serialize_complex, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.serialize_to_fits_format(std::complex<int>(12,13)), std::string(18,' ')+"12"+std::string(18,' ')+ "13");
}

BOOST_FIXTURE_TEST_CASE(extract_keyword_from_card, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.extract_keyword("SIMPLE  =                    T / file does conform to FITS standard             "), "SIMPLE  ");

}

BOOST_FIXTURE_TEST_CASE(extract_value, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.extract_value("SIMPLE  =                    T / file does conform to FITS standard             "), "                    T");

}

BOOST_FIXTURE_TEST_CASE(format_keyword, cardpolicy_fixture) {

    BOOST_REQUIRE_EQUAL(policy.format_keyword("SIMPLE"), "SIMPLE  ");

}

BOOST_AUTO_TEST_SUITE_END()
