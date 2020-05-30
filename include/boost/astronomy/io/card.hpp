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
struct card
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
     * @brief       Constructs a card from a C-style string
     * @details     This functions copies the first 80 characters from the array of characters pointed by c and
     *              stores them internally.
     * @param[in]   c Pointer to an array of characters.
     * @pre         The C-Style string supplied to the constructor must contain the card information ( key, value,
                    comment ) and should comply with the format of FITS standard
                    validation of data ( A policy class would work here)
     * @throws      std::bad_alloc If the request of 80bytes of memory allocation fails.
     * @todo        Provide validation features inside this constructor to prevent the user being concerned with
    */
    card(char const* c) : card()
    {
        this->card_.assign(c, 80);
    }


    /**
     * @brief       Takes a string as argument and creates a card object based on that string.
     * @details     This function accepts a string as an argument and stores it internally provided
                    the content inside the string complies with the FITS standard requirements.
     * @param[in]   str String that contains the key-value-comment data
     * @pre         The string provided as an argument must not be more than 80 characters in length
                    and should follow the FITS standard requirements for ( key,value,comments)
     * @throws      invalid_card_length_exception If the length of string is more than 80 characters
     * @throws      std::bad_alloc If the request of 80bytes of memory allocation fails.
     * @todo        This constructor also requires some validation features to make things simpler for user
    */
    card(std::string str) : card()
    {
        if (str.length() > 80)
        {
            throw invalid_card_length_exception();
        }
        this->card_ = str.append(80 - str.length(), ' ');
    }


    /**
     * @brief       Constructs a card from key,value,comment supplied as the arguments to the function
     * @param[in]   key Keyword associated with the FITS card
     * @param[in]   value Value associated with the FITS card
     * @param[in]   comment Optional comment associated with the FITS card
     * @pre         The keyword length should be greater than 8 characters
     * @pre         If comment is present then <strong>value.length()+comment.length()<=68 (The forward slash(/) and blank
                    are accounted for already else value should be of no more than 70 characters
     * @throws      invalid_key_length_exception If keyword length > 8
     * @throws      invalid_value_length_exception If value+comment length >68 or  value length >70 ( No comments)
     * @throws      std::bad_alloc If the request of 80bytes of memory allocation fails.
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
     * @brief Constructs a card from key,value,comment(optional) supplied as the argument to the function
     * @see   card (std::string const& key,std::string const& value,std::string const& comment = "")
    */
    void create_card
    (
        std::string const& key,
        std::string const& value,
        std::string const& comment = ""
    )
    {
        if (key.length() > 8)
        {
            throw invalid_key_length_exception();
        }
        if (!comment.empty() && (value.length() + comment.length() > 68))
        {
            throw invalid_value_length_exception();
        }
        else if (value.length() > 70)
        {
            throw invalid_value_length_exception();
        }

        if (!comment.empty())
        {
            this->card_ = std::string(key).append(8 - key.length(), ' ') +
                "= " + value + " /" + comment +
                std::string("").append(68 - value.length() + comment.length(), ' ');
        }
        else
        {
            this->card_ = std::string(key).append(8 - key.length(), ' ') +
                "= " + std::string(value).append(70 - key.length(), ' ');
        }
    }

    /**
     * @brief    Creates/Initializes the card object with a boolean value
     * @see      create_card(std::string const& key,std::string const& value,std::string const& comment = "")
     *           for more information on exception specification
    */
    void create_card(std::string const& key, bool value, std::string const& comment = "")
    {
        if (value)
        {
            create_card(key, std::string("T").insert(0, 19, ' '), comment);
        }
        else
        {
            create_card(key, std::string("F").insert(0, 19, ' '), comment);
        }
    }

    /**
     * @brief       Creates/Initializes the card object with a numeric value
     * @tparam      Value Represents a numeric type that has extraction (<<) operator defined
     * @param[in]   key  keyword associated with the FITS card
     * @param[in]   value Numeric value associated with FITS card
     * @param[in]   comment Optional comment that describes the content of card
     * @see         create_card(std::string const& key,std::string const& value,std::string const& comment = "")
     *              for more information on exception specification
    */
    template <typename Value>
    void create_card(std::string const& key, Value value, std::string const& comment = "")
    {
        std::string val = std::to_string(value);
        val.insert(0, ' ', 20 - val.length());
        create_card(key, val, comment);
    }


    /**
     * @brief       Creates/Initializes the card object with a complex value
     * @tparam      Real Any Numeric type representing <strong>real</strong> part of complex number
     * @tparam      Imaginary Any Numeric type representing <strong>imaginary</strong> part of complex number
     * @param[in]   key Keyword associated with FITS card
     * @param[in]   real Real part of complex number
     * @param[in]   imaginary Imaginary part of complex number
     * @param[in]   comment Represents an optional comment that describes the FITS card
     * @see         create_card(std::string const& key,std::string const& value,std::string const& comment = "")
     *              for more information on exception specification
    */
    template <typename Real, typename Imaginary>
    void create_card
    (
        // Use complex class. Its much better
        std::string const& key,
        Real real,
        Imaginary imaginary,
        std::string const& comment = ""
    )
    {
       std::string value = "(" + std::to_string(real)+','+std::to_string(imaginary) + ")";

        create_card(key, value, comment);
    }

    /**
     * @brief       This function takes a key and value as argument and creates a card from it.
     * @details     This function is be used to create a <strong>comment,history,blank</strong> card.
     * @param[in]   key Keyword associated with FITS card
     * @param[in]   value Value associated with FITS card. This value can be a comment or history data or nothing
     * @pre         key.length()<=8 characters
     * @pre         value.length()<=70 characters
     * @note        If some value is present then it will be inserted from 11th position onwards
     * @throws      invalid_key_length_exception If keyword.length()>8
     * @throws      invalid_value_length_exception  If value.length()>70
    */
    void create_commentary_card(std::string const& key, std::string const& value)
    {

        if (key.length() > 8)
        {
            throw invalid_key_length_exception();
        }
        if (value.length() > 70)
        {
            throw invalid_value_length_exception();
        }

        this->card_ = std::string(key).append(8 - key.length(), ' ') +
            "  " + std::string(value).append(70 - key.length(), ' ');
    }

    /**
     * @brief       Queries the keyword associated with current card object
     * @param[in]   whole Indicates whether the keyword should be returned with or without trailing spaces
     * @return      Returns the keyword part of the card
    */
    std::string key(bool whole = false) const
    {
        if (whole)
        {
            return this->card_.substr(0, 8);
        }
        return boost::algorithm::trim_copy(this->card_.substr(0, 8));
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
        return this->card_.substr(10);
    }

    /**
     * @brief       Sets the value of current card
     * @param[in]   value Value to be set.
     * @throws      key_not_defined_exception If keyword length of current card object is 0
     * @throws      invalid_value_length_exception If the value.length()>70
    */
    void value(std::string const& value)
    {
        // Just put empty instead of length please
        if (this->key().length() == 0)
        {
            throw key_not_defined_exception();
        }
        if (value.length() > 70)
        {
            throw invalid_value_length_exception();
        }
        this->card_.append(70 - value.length(), ' ');
    }

private:

    /**
     * @brief   Returns the value associated with the card in required type.
     * @tparam  ReturnType Type to which the value needs to be converted.
     * @return  The value associated with card in the Required Type
     * @throws  boost::bad_lexical_cast If the conversion could not succeed
    */
    template <typename ReturnType>
    ReturnType value_imp(boost::type<ReturnType>) const
    {
        std::string val = boost::algorithm::trim_copy(
            this->card_.substr(10, this->card_.find('/') - 10));
        return boost::lexical_cast<ReturnType>(val);
    }

    /**
     * @brief   Returns the value associated with the card in logical type
     * @return  boolean value
    */
    bool value_imp(boost::type<bool>) const
    {
        std::string val = boost::algorithm::trim_copy(
            this->card_.substr(10, this->card_.find('/') - 10));
        if (val == "T")
        {
            return true;
        }
        return false;
    }

};

}}} //namespace boost
#endif // !BOOST_ASTRONOMY_IO_CARD_HPP

