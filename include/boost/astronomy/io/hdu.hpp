
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_HDU_HPP
#define BOOST_ASTRONOMY_IO_HDU_HPP

#include <string>
#include <fstream>
#include <vector>
#include <cstddef>
#include <unordered_map>
#include <memory>

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/astronomy/io/bitpix.hpp>
#include <boost/astronomy/io/image.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>
#include <boost/astronomy/io/card.hpp>
#include <boost/astronomy/io/column.hpp>

/**
 * @file    hdu.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for structure <strong>hdu</strong>
 */
namespace boost { namespace astronomy { namespace io {

struct column;

/**
 * @brief   Used to store Header Related Information of FITS HDU ( Header Data Unit ) 
 * @details This structure also provides additional methods for querying some of the common but important keyword values
 *          along with a general function ( value_of ) for querying the value associated with any  keyword in an HDU
 * @author  Pranam Lashkari
 * @author  Sarthak Singhal
 * @note    To learn more about HDU please refer
 *          <a href="http://archive.stsci.edu/fits/users_guide/node5.html#SECTION00320000000000000000">FITS</a>
 * @todo    Define a virtual destructor to avoid leaking memory for all subclasses
 */
struct hdu
{
protected:
    boost::astronomy::io::bitpix bitpix_value; //! stores the BITPIX value (enum bitpix)
    std::vector<std::size_t> naxis_; //! values of all naxis (NAXIS, NAXIS1, NAXIS2...)

    //! Stores the each card in header unit (80 char key value pair)
    std::vector<card> cards;

    //! stores the card-key index (used for faster searching)
    std::unordered_map<std::string, std::size_t> key_index; 

public:

    /**
     * @brief       Default constructor used to generate a standalone object of HDU
    */
    hdu() {}

    /**
     * @brief       Constructs an hdu object from the given file
     * @details     This method takes a filename as argument and constructs an hdu object
     *              by  reading the header information associated with the first HDU
     * @param[in]   file_name Path to the FITS file
     * @see         read_header(std::fstream &file)
    */
    hdu(std::string const& file_name)
    {
        std::fstream file(file_name, std::ios_base::in | std::ios_base::binary);
        read_header(file);
        file.close();
    }

    /**
     * @brief       Constructs an HDU object by reading the header information from the specified position
     *              of the given FITS file
     * @param[in]   file_name Path to the FITS file
     * @param[in]   pos Position from which the header information should be read
     * @note        Please make sure to set the file position to the beginning of an Header Data Unit
    */
    hdu(std::string const& file_name, std::streampos pos)
    {
        std::fstream file(file_name, std::ios_base::in | std::ios_base::binary);
        read_header(file, pos);
        file.close();
    }

    /**
     * @brief        Constructs an HDU object from a filestream passed as an argument to the method
     * @param[in,out] file filestream set to open mode for reading
     * @note        As a side effect the file pointer associated with the stream gets set to the end
     *              of the current HDU unit
    */
    hdu(std::fstream &file)
    {
        read_header(file);
    }


    /**
     * @brief       Constructs an HDU object by reading the header information from the specified position
     *              in the given file stream
     * @param[in]   file filestream set to open mode for reading
     * @param[in]   pos Position from which the header information should be read
     * @note        Please make sure to set the file position to the beginning of an Header Data Unit
    */
    hdu(std::fstream &file, std::streampos pos)
    {
        read_header(file, pos);
    }

    /**
     * @brief       Reads the header information of a Header Data Unit ( HDU  )
     * @details     This method takes a stream as argument and reads the header
     *              information ( cards of 80byte each )of an HDU from the current position
     *              until the card with <strong>END</strong> keyword is encountered
     * @param[in,out] file filestream set to open mode for reading
     * @note        As a side effect the file pointer gets set to the end of HDU
     * @throws      fits_exception() If <strong>bitpix</strong> has an invalid value
     * @throws      boost::bad_lexical_cast If the conversion of value to the specific type was not successul
    */
    void read_header(std::fstream &file)
    {
        cards.reserve(36); //reserves the space of atleast 1 HDU unit 
        char _80_char_from_file[80]; //used as buffer to read a card consisting of 80 char

        //reading file card by card until END card is found
        while (true)
        {
            //read from file and create push card into the vector
            file.read(_80_char_from_file, 80);
            cards.emplace_back(_80_char_from_file);

            //store the index of the card in map
            this->key_index[this->cards.back().key()] = this->cards.size() - 1;

            //check if end card is found
            if(this->cards.back().key(true) == "END     ")
            {
                break;
            }
        }
        set_unit_end(file);    //set cursor to the end of the HDU unit

        //finding and storing bitpix value
                    
        switch (cards[key_index["BITPIX"]].value<int>())
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
        naxis_.emplace_back(cards[key_index["NAXIS"]].value<std::size_t>());
        naxis_.reserve(naxis_[0]);
                    
        for (std::size_t i = 1; i <= naxis_[0]; i++)
        {
            naxis_.emplace_back(cards[key_index["NAXIS" +
                boost::lexical_cast<std::string>(i)]].value<std::size_t>());
        }
    }

    /**
     * @brief      Reads the header information of an HDU from specified position in stream 
     * @param[in]  pos position from the header should be read
     * @param[in,out] file filestream set to open mode for reading
     * @see        read_header(std::fstream &file)
    */
    void read_header(std::fstream &file, std::streampos pos)
    {
        file.seekg(pos);
        read_header(file);
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
        return this->naxis_[n];
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
        return this->cards[key_index.at(key)].value<ReturnType>();
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

    virtual std::unique_ptr<column> get_column(std::string name) const
    {
        throw wrong_extension_type();
    }
};
}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_HDU_HPP

