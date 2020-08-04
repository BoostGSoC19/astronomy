/*=============================================================================
Copyright 2018 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_HDU_HPP
#define BOOST_ASTRONOMY_IO_HDU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <fstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/astronomy/io/bitpix.hpp>
#include <boost/astronomy/io/image.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>
#include <boost/astronomy/io/card.hpp>
#include <boost/astronomy/io/column.hpp>

namespace boost { namespace astronomy { namespace io {


/**
 * @brief   Used to store Header Related Information of FITS HDU ( Header Data Unit )
 * @details This structure also provides additional methods for querying some of the common but important keyword values
 *          along with a general function ( value_of ) for querying the value associated with any  keyword in an HDU
 * @author  Pranam Lashkari
 * @author  Sarthak Singhal
 * @note    To learn more about HDU please refer
 *          <a href="http://archive.stsci.edu/fits/users_guide/node5.html#SECTION00320000000000000000">FITS</a>
 */
template<typename CardPolicy>
struct header
{
protected:
    boost::astronomy::io::bitpix bitpix_value; //! stores the BITPIX value (enum bitpix)
    std::vector<std::size_t> naxis_; //! values of all naxis (NAXIS, NAXIS1, NAXIS2...)
    std::string hdu_name;

    //! Stores the each card in header unit (80 char key value pair)
    std::vector<card<CardPolicy>> cards;

    //! stores the card-key index (used for faster searching)
    std::unordered_map<std::string, std::size_t> key_index;

public:

    /**
     * @brief Reads the header portion of an HDU using file reader
     * @param[in] file_reader Reader used to access the FITS file
    */
    template<typename FileReader>
    void read_header(FileReader& file_reader) {

        cards.reserve(36); //reserves the space of atleast 1 HDU unit
        std::string card_buffer;

        //reading file card by card until END card is found
        while (true)
        {
            //read from file and create push card into the vector
            card_buffer = file_reader.read(80);
            cards.emplace_back(card_buffer);

            //store the index of the card in map
            this->key_index[this->cards.back().keyword()] = this->cards.size() - 1;

            //check if end card is found
            if (this->cards.back().keyword(true) == "END     ")
            {
                break;
            }
        }
        //finding and storing bitpix value

        switch (cards[key_index["BITPIX"]]. template value<int>())
        {
        case 8:
            this->bitpix_value = io::bitpix::B8;
            break;
        case 16:
            this->bitpix_value = io::bitpix::B16;
            break;
        case 32:
            this->bitpix_value = io::bitpix::B32;
            break;
        case -32:
            this->bitpix_value = io::bitpix::_B32;
            break;
        case -64:
            this->bitpix_value = io::bitpix::_B64;
            break;
        default:
            throw fits_exception();
            break;
        }

        //setting naxis values
        std::size_t total_dimensions = cards[key_index["NAXIS"]].template value<std::size_t>();
        naxis_.reserve(total_dimensions);

        for (std::size_t i = 1; i <= total_dimensions; i++)
        {
            naxis_.emplace_back(cards[key_index["NAXIS" +
                boost::lexical_cast<std::string>(i)]].template value<std::size_t>());
        }

    }

    /**
     * @brief Returns the name of the HDU
    */
    std::string get_hdu_name() {
        // Check if its a extension
        try {

            hdu_name = value_of < std::string>("XTENSION");
            return hdu_name;
        }
        catch (...) {}

        // Test if its primary header ( otherwise exception propagated)
        value_of<bool>("SIMPLE");
        return "primary_hdu";
    }

    bool contains_keyword(const std::string& keyword) {
        auto keyword_found = std::find_if(cards.begin(), cards.end(), [&keyword](const card<CardPolicy>& h_card) {
            return h_card.keyword() == keyword;
            });
        return keyword_found != cards.end();
    }

    /**
     * @brief       Gets the bitpix value associated with the HDU
     * @return      io::bitpix
    */
    io::bitpix bitpix() const
    {
        return this->bitpix_value;
    }

    /**
     * @brief       Gets the value(number of elements) of all dimensions associated with the HDU data
     * @return      Returns a vector<size_t> containing the number of elements for each  dimension
    */
    std::vector<std::size_t> all_naxis() const
    {
        return this->naxis_;
    }

    /**
     * @brief       Gets the number of elements for a perticular dimension in HDU data
     * @return      Returns a std::size_t containing the number of elements in a perticular dimension
    */
    std::size_t naxis(std::size_t n = 0) const
    {
        return this->naxis_[n - 1]; // 1 Based indexing
    }

    /**
     * @brief       Returns the total number of dimensions of an HDU data
    */
    std::size_t total_dimensions() const { return all_naxis().size(); }

    std::size_t data_size() {
        if (naxis_.empty()) { return 0; }
        return std::accumulate(naxis_.begin(), naxis_.end(), static_cast<std::size_t > (1), std::multiplies<std::size_t>());
    }

    /**
     * @brief       Gets the value associated with a perticular keyword
     * @param[in]   key Keyword whose value is to be queried
     * @tparam      ReturnType The type in which the value associated with the keyword needs to be obtained
     * @return      Returns the value associated with keyword in the required type
     * @throws      boost::bad_lexical_cast If the conversion of value to the specific type was not successul
    */
    template <typename ReturnType>
    ReturnType value_of(std::string const& key)// const
    {
        return this->cards[key_index.at(key)].template value<ReturnType>();
    }


    /**
     * @brief       Sets the file_pointer/cursor to the end of current HDU unit
     * @param[out] file filestream whose position needs to be set to current HDU boundary
    */
    void set_unit_end(std::fstream &file) const
    {
        //set cursor to the end of the HDU unit
        file.seekg((file.tellg() + (2880 - (file.tellg() % 2880))));
    }

    /**
     * @brief      Gets the number of cards in HDU header
     * @return     total number of cards in HDU header
    */
    std::size_t card_count() {
        return cards.size() - 1; // Last one is END Card ( It will not be counted )

    }
};
}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_HDU_HPP

