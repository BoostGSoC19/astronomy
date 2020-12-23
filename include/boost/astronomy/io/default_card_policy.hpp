/*=============================================================================
Copyright 2020 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_DEFAULT_CARD_POLICY_HPP
#define BOOST_ASTRONOMY_DEFAULT_CARD_POLICY_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <complex>
#include <boost/lexical_cast.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>
#include <boost/astronomy/io/string_conversion_utility.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/blank.hpp>
#include <boost/mpl/vector.hpp>

namespace boost{namespace astronomy{namespace io{

class card_policy {

    std::vector<std::string> reserved_keywords;

public:
    typedef boost::mpl::vector<
        boost::blank,
        bool,
        int,
        float,
        double,
        unsigned int,
        long long,
        std::size_t,
        std::string,
        std::complex<int>,
        std::complex<double>,
        std::complex<float>,
        std::complex<std::size_t>
    >  supported_types;


    /**
     * @brief Constructs a default object of card and initializes the reserved keyword list
    */
    card_policy() {
        reserved_keywords.push_back("COMMENT");
        reserved_keywords.push_back("HISTORY");
        reserved_keywords.push_back("END");
    }


    /**
     * @brief Returns true if the keyword length is less than or equal to 8
     * @param[in] keyword Keyword string to be validated
    */
    bool is_key_valid(const std::string& keyword) const {
        return keyword.length() <= 8;
    }

    /**
     * @brief Returns true if the card follows the specification of FITS standard
     * @param[in] fits_card card string to be validated
    */
    bool is_card_valid(const std::string& fits_card) const {

        if (fits_card.length() <= 80)
        {
            if (in_proper_format(fits_card) || is_reserved_keyword(boost::trim_copy(fits_card.substr(0, 7)))|| is_blank(fits_card))
                return true;
        }
        return false;
    }

    /**
     * @brief Returns true if the parameters follow the specifications of being a card
    */
    bool is_card_valid(const std::string& keyword, const std::string& value, const std::string& comment) const  {

        if (is_key_valid(keyword)) {
            if (comment.empty()) { return value.length() <= 70; }
            else { return value.length() + comment.length() <= 68; }
        }
        return false;
    }

    /**
     * @brief Parses the values stored as string to the given type T
     * @param[in] value value to be parsed
    */
    template<typename T>
    T parse_to(const std::string& value) const  {
        return parse_to(value, boost::type<T>{});
    }


    /**
      * @brief Parses the values stored as string to the given type T
      * @param[in] value value to be parsed
      * @param[in] dummy Partial Specialization for special types
     */
    template<typename T>
    T parse_to(const std::string& value, boost::type <T>) const {
        return ascii_converter::deserialize_to<T>(value,0);
    }
    /**
     * @brief Overloaded case for boolean
    */
    bool parse_to(const std::string& value, boost::type<bool>) const {

        if (value == "T") { return true; }
        if (value == "F") { return false; }

        throw boost::astronomy::invalid_cast("Cannot convert to boolean value");

    }

    /**
      * @brief Parses the values stored as string to the given complex <T>
      * @param[in] value value to be parsed
     */
    template<typename T>
    std::complex<T> parse_to(const std::string& value, boost::type<std::complex<T>>) const {

        auto space_pos = std::find(value.begin(), value.end(), ' ');
        if (space_pos != value.end()) {
            std::string real_part(value.begin(), space_pos);
            std::string imag_part(space_pos + 1, value.end());

            return std::complex<T>(parse_to<T>(real_part), parse_to<T>(imag_part));
        }
        throw boost::astronomy::invalid_cast("Cannot convert to complex number");
    }

    /**
     * @brief Serializes the value of given type to string
     * @param[in] value The value to be serialized
    */
    template<typename T>
    std::string serialize_to_fits_format(T value) const {
        std::string val;

        std::stringstream value_stream;
        value_stream << value;
        val = value_stream.str();

        val.insert(0, 20 - val.length(), ' ');
        return val;
    }

    /**
     * @brief Pads the string according to the requirements in FITS standard
     * @param[in] value Value to be serialized and padded according to requirements
    */
    std::string serialize_to_fits_format(const std::string& value) const {
        std::string generated_string="";
        generated_string += '\'';
        generated_string += value;
        generated_string += '\'';
        return generated_string;
    }

    /**
     * @brief Serializes a boolean to string and pads it according to requirements in FITS standard
     * @param[in] value Boolean value to serialize to
    */
    std::string serialize_to_fits_format(bool value) const {

        return std::string(value == true ? "T" : "F").insert(0, 19, ' ');
    }

    /**
     * @brief Serializes a long long value to string and pads it according to requirements in FITS standard
     * @param[in] value long long value to serialize to
    */
    std::string serialize_to_fits_format(long long value)const {
        std::string val = std::to_string(value);
        return val.insert(0, 20 - val.length(), ' ');
    }

    /**
     * @brief Serializes a complex number to string and pads it according to requirements in FITS standard
     * @param[in] value complex number to serialize to
     * @tparam Type of complex number
    */
    template<typename T>
    std::string serialize_to_fits_format(std::complex<T> value)const {
        return serialize_to_fits_format(value.real())+ serialize_to_fits_format(value.imag());        
    }

    /**
     * @brief Extracts the key from the fits_card
     * @param[in] The card from which the keyword needs to be extracted
    */
    std::string extract_keyword(const std::string& fits_card)const {
        return fits_card.substr(0, 8);
    }

    /**
     * @brief Formats the keyword according to the requirements of FITS standard
     * @param[in] keyword Keyword to format
    */
    std::string format_keyword(const std::string& keyword) const {
        return std::string(keyword).append(8 - keyword.length(), ' ');
    }

    /**
     * @brief Returns the value associated with keyword from the fits_card
     * @param[in] fits_card fits card from where the value needs to be extracted
    */
    std::string extract_value(const std::string& fits_card) const {
        return fits_card.substr(9, fits_card.find('/') - 10);
    }

private:

    /**
     * @brief Checks whether the keyword is in list of reserved keywords
    */
    bool is_reserved_keyword(const std::string& keyword) const {
        return std::find(reserved_keywords.begin(), reserved_keywords.end(), keyword) != reserved_keywords.end();
    }

    /**
     * @brief Checks whether the card follows the standard specification for cards with non reserved keywords
    */
    bool in_proper_format(const std::string& fits_card)const {
        return fits_card.substr(8, 2) == "= ";
    }

    /**
     * @brief Checks whether the card has blank field
    */
    bool is_blank(const std::string& fits_card) const {
        return fits_card.substr(0, 8) == std::string(8, ' ');
    }
};

} } }

#endif
