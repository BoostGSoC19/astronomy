#ifndef BOOST_ASTRONOMY_IO_CARD_HPP
#define BOOST_ASTRONOMY_IO_CARD_HPP

#include <string>
#include <sstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type.hpp>

#include <boost/astronomy/exception/fits_exception.hpp>

namespace boost { namespace astronomy { namespace io {

//!structure to store a card (80 byte key value pairs as well as comments and history cards)
struct card
{
private:
    std::string card_;

public:
    card()
    {
        this->card_.reserve(80);
    }
    //! creating card from const char*
    //! it will read 80 char from provided pointer
    card(char const* c) : card()
    {
        this->card_.assign(c, 80);
    }

    //!a string is expected with lenght no more than 80 chars 
    //!this string will be directly stored in the card
    //!string must follow all the standerd of the key, value and comment for card
    card(std::string str) : card()
    {
        if (str.length() > 80)
        {
            throw invalid_card_length_exception();
        }
        this->card_ = str.append(80 - str.length(), ' ');
    }

    //!key, value and optional comments are expected
    //!all the values will be directly stored as provided so necessary spaces befor values must be provided
    //!spaces after the keyword and value will be taken care implicitly
    card(std::string const& key, std::string const& value, std::string const& comment = "")
    {
        if (key.length() > 8)
        {
            throw invalid_key_length_exception();
        }
        if ((comment.length() > 0) && (value.length() + comment.length() > 68))
        {
            throw invalid_value_length_exception();
        }
        else if (value.length() > 70)
        {
            throw invalid_value_length_exception();
        }

        if (comment.length())
        {
            this->card_ = std::string(key).append(8 - key.length(), ' ') + "= " + value + " /" + comment +
                std::string("").append(68 - value.length() + comment.length(), ' ');
        }
        else
        {
            this->card_ = std::string(key).append(8 - key.length(), ' ') + "= " + std::string(value).append(70 - key.length(), ' ');
        }
    }

    //!this overload supports date and string types
    //!key, value and optional comments are expected
    //!all the values will be directly stored as provided so necessary spaces befor values must be provided
    //!spaces after the keyword and value will be taken care implicitly
    void create_card(std::string const& key, std::string const& value, std::string const& comment = "")
    {
        if (key.length() > 8)
        {
            throw invalid_key_length_exception();
        }
        if ((comment.length() > 0) && (value.length() + comment.length() > 68))
        {
            throw invalid_value_length_exception();
        }
        else if (value.length() > 70)
        {
            throw invalid_value_length_exception();
        }

        if (comment.length())
        {
            this->card_ = std::string(key).append(8 - key.length(), ' ') + "= " + value + " /" + comment +
                std::string("").append(68 - value.length() + comment.length(), ' ');
        }
        else
        {
            this->card_ = std::string(key).append(8 - key.length(), ' ') + "= " + std::string(value).append(70 - key.length(), ' ');
        }
    }

    //!create card with boolean value
    void create_card(std::string const& key, bool value, std::string const& comment = "")
    {
        if (value)
        {
            create_card(key, std::string("T").insert(0, ' ', 19), comment);
        }
        else
        {
            create_card(key, std::string("F").insert(0, ' ', 19), comment);
        }
    }

    //!create card with numeric value
    template <typename Value>
    void create_card(std::string const& key, Value value, std::string const& comment = "")
    {
        std::ostringstream stream;
        stream << value;

        std::string val = stream.str();
        val.insert(0, ' ', 20 - val.length());
        create_card(key, val, comment);
    }

    //!create card for complex value
    template <typename Real, typename Imaginary>
    void create_card(std::string const& key, Real real, Imaginary imaginary, std::string const& comment = "")
    {
        std::ostringstream stream;
        stream << real << ", " << imaginary;

        std::string value = "(" + stream.str() + ")";

        create_card(key, value, comment);
    }

    //! to create comment, history or balnk keyword cards
    //! value will be put from column 11
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

        this->card_ = std::string(key).append(8 - key.length(), ' ') + "  " + std::string(value).append(70 - key.length(), ' ');
    }

    //!if whole value is set to true then string is returned with trailing spaces
    std::string key(bool whole = false) const
    {
        if (whole)
        {
            return this->card_.substr(0, 8);
        }
        return boost::algorithm::trim_copy(this->card_.substr(0, 8));
    }

    //!return types can be int, float, double, bool, string (date and complex numbers are returned as string surrounded in single quotes or in brackets)
    template <typename ReturnType>
    ReturnType value() const
    {
        return value_imp(boost::type<ReturnType>());
    }

    //!returns value portion of card with comment as std::string 
    std::string value_with_comment() const
    {
        return this->card_.substr(10);
    }

    //!set value of current card
    void value(std::string const& value)
    {
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

    template <typename ReturnType>
    ReturnType value_imp(boost::type<ReturnType>) const
    {
        std::string val = boost::algorithm::trim_copy(this->card_.substr(10, this->card_.find('/') - 10));
        return boost::lexical_cast<ReturnType>(val);
    }

    bool value_imp(boost::type<bool>) const
    {
        std::string val = boost::algorithm::trim_copy(this->card_.substr(10, this->card_.find('/') - 10));
        if (val == "T")
        {
            return true;
        }
        return false;
    }

};

}}} //namespace boost
#endif // !BOOST_ASTRONOMY_IO_CARD_HPP

