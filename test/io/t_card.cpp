/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE card_test

#include <boost/test/unit_test.hpp>
#include <boost/astronomy/io/card.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>


class card_fixture {

public:
    // Structure to store the card
    struct fixture_card {
        std::string key;
        std::string value;
        std::string comment;
        std::string raw_form; // its necessary to simulate  error conditions in some cases
    };

    card_fixture() {

        // Boolean Card
        std::string raw_boolean_card = "SIMPLE  =                    T /Standard FITS format                            ";
        fixture_card boolean_card;
        boolean_card.raw_form = raw_boolean_card;
        boolean_card.key = "SIMPLE";
        boolean_card.value = "T";
        boolean_card.comment = "Standard FITS format";
        card_list.emplace("boolean_card", boolean_card);

        // String Card
        std::string raw_string_card = "AUTHOR  = 'Acker et al.' /Catalog author(s)                                     ";
        fixture_card string_card;
        string_card.raw_form = raw_string_card;
        string_card.key = "AUTHOR";
        string_card.value = "Acker et al.";
        string_card.comment = "Catalog author(s)";
        card_list.emplace("string_card", string_card);

        // String Card without comment
        std::string raw_string_card_nocomment = "PHOTMODE= 'WFPC2,1,A2D7,LRF#4877.0,,CAL'                                        ";
        fixture_card string_card_nocomment;
        string_card_nocomment.raw_form = raw_string_card_nocomment;
        string_card_nocomment.key = "PHOTMODE";
        string_card_nocomment.value = "WFPC2,1,A2D7,LRF#4877.0,,CAL";
        string_card_nocomment.comment = "";
        card_list.emplace("string_card_nocomment", string_card_nocomment);

        //Numeric Cards

        // Integer Card
        std::string raw_integer_card = "OPSIZE  =                 2112 /PSIZE of original image                         ";
        fixture_card integer_card;
        integer_card.raw_form = raw_integer_card;
        integer_card.key = "OPSIZE";
        integer_card.value = "2112";
        integer_card.comment = "PSIZE of original image";
        card_list.emplace("integer_card", integer_card);

        // Integer Card without comment
        std::string raw_integer_card_nocomment = "DETECTOR=                    1                                                  ";
        fixture_card integer_card_nocomment;
        integer_card_nocomment.raw_form = raw_integer_card_nocomment;
        integer_card_nocomment.key = "DETECTOR";
        integer_card_nocomment.value = "1";
        integer_card_nocomment.comment = "";
        card_list.emplace("integer_card_nocomment", integer_card_nocomment);


        // Floating Point Card
        std::string raw_float_card =
            "SUNANGLE=              141.618 /angle between sun and V1 axis";
        raw_float_card.append(80 - raw_float_card.length(), ' ');


        fixture_card float_card;
        float_card.raw_form = raw_float_card;
        float_card.key = "SUNANGLE";
        float_card.value = "141.618";
        float_card.comment = "angle between sun and V1 axis";
        card_list.emplace("floating_card", float_card);

        // Floating Point Card without comment
        std::string raw_float_card_nocomment = "DEZERO  =             316.6452                                                  ";
        fixture_card float_card_nocomment;
        float_card_nocomment.raw_form = raw_float_card_nocomment;
        float_card_nocomment.key = "DEZERO";
        float_card_nocomment.value = "316.6452";
        float_card_nocomment.comment = "";
        card_list.emplace("floating_card_nocomment", float_card);

        // Invalid Card
        std::string  raw_invalid_card = "TFIELDS =" + std::string(19, ' ') + "16" + " /Number of data fields per record" + std::string(40, ' ');
        fixture_card invalid_card;
        invalid_card.raw_form = raw_invalid_card;
        invalid_card.key = "TFIELDS";
        invalid_card.value = "16";
        invalid_card.comment = "Number of data fields per record";
        card_list.emplace("invalid_card", invalid_card);


        //Comment only Card
        std::string raw_comment_card = "COMMENT  Morph_Flag: '<' for star-like appearance, '>' for fainter spherical   ";
        fixture_card comment_card;
        comment_card.raw_form = raw_comment_card;
        comment_card.key = "COMMENT";
        comment_card.value = "Morph_Flag: '<' for star-like appearance, '>' for fainter spherical";
        comment_card.comment = "";
        card_list.emplace("comment_card", comment_card);


        //Date ( This will be there in future )

        // Complex ( This will be there in future )



    }

 
    const fixture_card& get_card(const std::string& card_type) {

        return card_list[card_type];

    }

private:
    std::map<std::string, fixture_card> card_list;
};

using namespace boost::astronomy::io;

BOOST_FIXTURE_TEST_SUITE(card_constructors, card_fixture)

BOOST_AUTO_TEST_CASE(card_cstring_ctor) {

    card test_card(get_card("boolean_card").raw_form.c_str());

    BOOST_REQUIRE_EQUAL(test_card.key(), get_card("boolean_card").key);
    BOOST_REQUIRE_EQUAL(test_card.value<bool>(), get_card("boolean_card").value == "T" ? true : false);
}


BOOST_AUTO_TEST_CASE(card_string_ctor) {
    card test_card(get_card("string_card").raw_form);


    BOOST_REQUIRE_EQUAL(test_card.key(), get_card("string_card").key);
    BOOST_REQUIRE_EQUAL(test_card.value < std::string >(), get_card("string_card").value);

    // Case where the length is more than 80 ( Should throw exception )
    BOOST_REQUIRE_THROW((card(get_card("invalid_card").raw_form)), boost::astronomy::invalid_card_length_exception);

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(card_create_card_variants, card_fixture)


BOOST_AUTO_TEST_CASE(card_create_card) {
    card test_card;
    auto string_card = get_card("string_card");
    auto string_card_nocomment = get_card("string_card_nocomment");


    // String card with comment
    test_card.create_card(string_card.key, string_card.value, string_card.comment);


    BOOST_REQUIRE_EQUAL(test_card.raw_card(), string_card.raw_form);

    // String card without comment
    test_card.create_card(string_card_nocomment.key, string_card_nocomment.value);
    BOOST_REQUIRE_EQUAL(test_card.raw_card(), string_card_nocomment.raw_form);


    // Takes a random card and increases the keyword length to force an invalid_key exception
    BOOST_REQUIRE_THROW(
        test_card.create_card(string_card.key + "abcd", string_card.value, string_card.comment),
        boost::astronomy::invalid_key_length_exception
    );

    // Takes a random card and adds some spaces to the value to force an exception
    BOOST_REQUIRE_THROW(
        test_card.create_card(string_card.key,
            std::string(string_card.value).insert(string_card.value.length() - 1, std::string(52, ' ') + '\''),
            string_card.comment + "dummy"),
        boost::astronomy::invalid_value_length_exception
    );


}

BOOST_AUTO_TEST_CASE(card_create_card_bool) {
    auto boolean_card = get_card("boolean_card");
    card test_card;
    test_card.create_card(boolean_card.key, boolean_card.value == "T" ? true : false, boolean_card.comment);

    BOOST_REQUIRE_EQUAL(test_card.raw_card(), boolean_card.raw_form);
}

BOOST_AUTO_TEST_CASE(card_create_card_numeric) {
    auto fixture_integer_card = get_card("integer_card");
    auto fixture_floating_card = get_card("floating_card");

    card integer_card;
    card floating_card;

    integer_card.create_card(fixture_integer_card.key, std::atoi(fixture_integer_card.value.c_str()), fixture_integer_card.comment);
    floating_card.create_card(fixture_floating_card.key, std::atof(fixture_floating_card.value.c_str()), fixture_floating_card.comment);


    BOOST_REQUIRE_EQUAL(integer_card.raw_card(), fixture_integer_card.raw_form);
    BOOST_REQUIRE_EQUAL(floating_card.raw_card(), fixture_floating_card.raw_form);
}


BOOST_AUTO_TEST_CASE(card_create_card_complex) {
    // TODO : Write the tests after including support for complex numbers
}

BOOST_AUTO_TEST_CASE(card_create_commentary_card) {
    auto fixture_comment_card = get_card("comment_card");
    card comment_card;
    comment_card.create_commentary_card(fixture_comment_card.key, fixture_comment_card.value);
    BOOST_REQUIRE_EQUAL(comment_card.raw_card(), fixture_comment_card.raw_form);

}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(card_utility_methods, card_fixture)

BOOST_AUTO_TEST_CASE(card_key) {
    auto integer_card = card(get_card("integer_card").raw_form).key();
    auto floating_card = card(get_card("floating_card").raw_form).key();
    auto comment_card = card(get_card("comment_card").raw_form).key();


    BOOST_REQUIRE_EQUAL(integer_card, get_card("integer_card").key);
    BOOST_REQUIRE_EQUAL(floating_card, get_card("floating_card").key);
    BOOST_REQUIRE_EQUAL(comment_card, get_card("comment_card").key);
}

BOOST_AUTO_TEST_CASE(card_value) {
    auto fixture_integer_card = get_card("integer_card");
    auto fixture_floating_card = get_card("floating_card");
    auto fixture_comment_card = get_card("comment_card");

    card integer_card(fixture_integer_card.raw_form);
    card floating_card(fixture_floating_card.raw_form);
    card comment_card(fixture_comment_card.raw_form);

    BOOST_REQUIRE_EQUAL(integer_card.value<int>(), std::atoi(fixture_integer_card.value.c_str()));
    BOOST_CHECK_CLOSE(floating_card.value<float>(), std::atof(fixture_floating_card.value.c_str()), 0.001);
    BOOST_REQUIRE_EQUAL(comment_card.value<std::string>(), fixture_comment_card.value);


}

BOOST_AUTO_TEST_CASE(card_value_comment) {
    auto fixture_integer_card = get_card("integer_card");
    BOOST_REQUIRE_EQUAL(card(fixture_integer_card.raw_form).value_with_comment(),
        fixture_integer_card.value + " /" + fixture_integer_card.comment
    );

    auto fixture_floating_card = get_card("floating_card");
    BOOST_REQUIRE_EQUAL(card(fixture_floating_card.raw_form).value_with_comment(),
        fixture_floating_card.value + " /" + fixture_floating_card.comment
    );


}

BOOST_AUTO_TEST_CASE(card_setvalue) {
    // TODO : Problems with main codebase please check

}

BOOST_AUTO_TEST_SUITE_END()

