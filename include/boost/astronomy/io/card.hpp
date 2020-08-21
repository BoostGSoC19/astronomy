/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_CARD_HPP
#define BOOST_ASTRONOMY_IO_CARD_HPP

#include <string>
#include <sstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type.hpp>

#include <boost/astronomy/io/default_card_policy.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>

/**
 * @file    card.hpp
 * @author  Pranam Lashkari
 * @details Contains the definition for <strong>card</strong> structure
 */


namespace boost { namespace astronomy { namespace io {

/**
 * @brief   Represents the concept of <strong>card</strong> associated with the FITS standard.
 * @details This structure provides functions for storage, manipulation and access of FITS cards.
 * @author  Pranam Lashkari
 */
template<typename CardPolicy>
struct card: CardPolicy
{
private:
    std::string card_;

public:

    /**
     * @brief   Default Constructor to create a standalone object of <strong>card</strong>
     * @throws  std::bad_alloc If the request of 80 bytes of memory fails.
    */
    card()
    {
        this->card_.reserve(80);
    }

    

    /**
     * @brief       Takes a string as argument and creates a card object based on that string.
     * @details     This function accepts a string as an argument and stores it internally provided
                    the content inside the string complies with the FITS standard requirements.
     * @param[in]   str String that contains the key-value-comment data
    */
    card(std::string fits_card) : card()
    {
        if (this->is_card_valid(fits_card)) {
            this->card_ = fits_card;
        }
        else {
            throw invalid_card();
        }
    }


    /**
     * @brief       Constructs a card from key,value,comment supplied as the arguments to the function
     * @param[in]   key Keyword associated with the FITS card
     * @param[in]   value Value associated with the FITS card
     * @param[in]   comment Optional comment associated with the FITS card
    */
    card
    (
        std::string const& key,
        std::string const& value,
        std::string const& comment = ""
    )
    {
        create_card(key, value, comment);
    }

    /**
     * @brief Checks if contents of two cards with same card policy are equal
    */
    bool operator == (const card& other) const {
        return this->card_ == other.card_;
    }
    
    /**
     * @brief       Creates/Initializes the card object with a numeric value
     * @tparam      Value Represents a numeric type that has extraction (<<) operator defined
     * @param[in]   key  keyword associated with the FITS card
     * @param[in]   value Numeric value associated with FITS card
     * @param[in]   comment Optional comment that describes the content of card
    */
    template <typename Value>
    void create_card(std::string const& key, Value value, std::string const& comment = "")
    {
        create_card_impl(key, this->serialize_to_fits_format(value), comment);

    }


    
    /**
     * @brief       This function takes a key and value as argument and creates a card from it.
     * @details     This function is be used to create a <strong>comment,history,blank</strong> card.
     * @param[in]   key Keyword associated with FITS card
     * @param[in]   value Value associated with FITS card. This value can be a comment or history data or nothing
     * @pre         key.length()<=8 characters
     * @pre         value.length()<=70 characters
     * @note        If some value is present then it will be inserted from 11th position onwards
    */
    void create_commentary_card(std::string const& key, std::string const& value)
    {
        if (this->is_key_valid(key)) {
            std::string card_content = key + "  " + value;
            this->card_ = card_content;
            this->card_.append(80 - card_content.length(),' ');
            return;
        }

        throw invalid_card();
    }

    /**
     * @brief       Queries the keyword associated with current card object
     * @param[in]   whole Indicates whether the keyword should be returned with or without trailing spaces
     * @return      Returns the keyword part of the card
    */
    std::string keyword(bool whole = false) const
    {
        std::string keyword = this->extract_keyword(this->card_);
        if (whole)
        {
            return keyword;
        }
        return boost::algorithm::trim_copy(keyword);
    }

    /**
     * @brief   Returns the value associated with the card
     * @tparam  ReturnType  The type to which the value should be converted and returned
     * @pre     The ReturnType can be int, long long( If more than 10 digits) ,double,long double,float ,bool,string
     * @return  The value associated with the card
     * @throws  boost::bad_lexical_cast If the conversion could not succeed
     * @todo    Support for complex numbers and date is required
    */
    template <typename ReturnType>
    ReturnType value() const
    {
        return value_imp(boost::type<ReturnType>());
    }

    /**
     * @brief   Returns the value portion of card along with the comment
     * @return  Returns a std::string containing the value and comment
    */
    std::string value_with_comment() const
    {
        return boost::algorithm::trim_copy(this->card_.substr(10));
    }

    /**
     * @brief       Sets the value of current card
     * @param[in]   value Value to be set.
    */
    template<typename DataType>
    void set_value(DataType value)
    {

        std::string serialized_value = this->serialize_to_fits_format(value);

        if (this->is_card_valid(this->keyword(), serialized_value, "")) { 
            std::string key = this->keyword(true);
            this->card_.clear();
            this->card_.append(key);
            this->card_.append(serialized_value);
        }
    }

    /**
     * @brief   Returns the complete card as string
    */
    const std::string& raw_card() { return card_; }

private:

    /**
     * @brief Creates a card from key, value and comment supplied to the method
    */
    void create_card_impl(const std::string& key, const std::string& value, const std::string& comment="") {

        if (this->is_card_valid(key, value, comment)) {

            if (comment.empty()) {
                this->card_ = this->format_keyword(key) + "= " + value;
            }
            else {
                this->card_ = this->format_keyword(key) +"= "+value + " /" + comment;
            }

            this->card_.append(80 - this->card_.length(), ' ');
            return;
        }

        throw invalid_card();
    }

    /**
     * @brief   Returns the value associated with the card in required type.
     * @tparam  ReturnType Type to which the value needs to be converted.
     * @return  The value associated with card in the Required Type
    */
    template <typename ReturnType>
    ReturnType value_imp(boost::type<ReturnType>) const
    {
        std::string val = boost::algorithm::trim_copy(this->extract_value(this->card_));
        return this->template parse_to<ReturnType>(val);
    }

    /**
     * @brief   Returns the value associated with the card in string type
     * @return  string value
    */
    std::string value_imp(boost::type<std::string>) const {
        std::string val = boost::algorithm::trim_copy(this->extract_value(this->card_));
        if (val[0] == '\'') {
            return boost::algorithm::trim_copy(std::string(val.begin() + 1, val.end() - 1));
        }
        else {
            return val;
        }
    }
};


}}} //namespace boost
#endif // !BOOST_ASTRONOMY_IO_CARD_HPP

