/*=============================================================================
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP
#define BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

#include <cstddef>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <string>
#include <cmath>
#include <numeric>

#include <boost/astronomy/io/column.hpp>
#include <boost/astronomy/io/column_data.hpp>
#include <boost/astronomy/io/default_card_policy.hpp>
#include <boost/astronomy/io/table_extension.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/cstdfloat.hpp>



namespace boost { namespace astronomy {  namespace io {

/**
 * @brief           Stores the header information and data of ASCII_Table extension HDU
 * @details         This class provides a set of methods for creating,querying, and manipulation of ASCII_table extension HDU
 *                  For more information on Ascii_Table extension visit
 *                  <A href="http://archive.stsci.edu/fits/users_guide/node37.html#SECTION00540000000000000000">ASCII_TABLE</A>
 * @author          Sarthak Singhal,Gopi Krishna Menon, Pranam Lashkari
*/

template<typename CardPolicy>
struct basic_ascii_table : public table_extension<CardPolicy>
{
public:

    /**
     * @brief      Creates a standalone ascii_table object
    */
    basic_ascii_table() {}

    /**
     * @brief      Constructs an ASCII Table object from header and data buffer
     * @param[in]  other Header part of ASCII table
     * @param[in]  data_buffer Data part of ASCII table
    */
    basic_ascii_table(header<CardPolicy> const& other, const std::string& data_buffer) : table_extension<CardPolicy>(other) {
        set_ascii_table_info(data_buffer);
    }

    /**
     * @brief    Populates the metadata information for all fields of ASCII_Table extension
     * @details  This method populates the metadata information for all fields in a table
     *           for easy access to the data of ASCII table extention extension
    */
    void populate_column_data()
    {
        for (std::size_t i = 0; i < this->tfields_; i++)
        {
            this->col_metadata_[i].index(i + 1);

            this->col_metadata_[i].TFORM(
                this->hdu_header.template value_of<std::string>("TFORM" + boost::lexical_cast<std::string>(i + 1))
            );

            this->col_metadata_[i].TBCOL(
                this->hdu_header.template value_of<std::size_t>("TBCOL" + boost::lexical_cast<std::string>(i + 1))
            );

            try {
                this->col_metadata_[i].TTYPE(
                    this->hdu_header.template value_of<std::string>("TTYPE" + boost::lexical_cast<std::string>(i + 1))
                );

                this->col_metadata_[i].comment(
                    this->hdu_header.template value_of<std::string>(this->col_metadata_[i].TTYPE())
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                this->col_metadata_[i].TUNIT(
                    this->hdu_header.template value_of<std::string>("TUNIT" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                this->col_metadata_[i].TSCAL(
                    this->hdu_header.template value_of<double>("TSCAL" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                this->col_metadata_[i].TZERO(
                    this->hdu_header.template value_of<double>("TZERO" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }
        }
    }

    /**
     * @brief      Sets the data of ASCII Table from data_buffer
     * @param[in]  data_buffer Data of ASCII Table
    */
    void set_data(const std::string& data_buffer) {
        this->col_metadata_.clear();
        this->data_.clear();
        this->col_metadata_.resize(this->tfields_);
        set_ascii_table_info(data_buffer);
    }


    /**
     * @brief       Returns the data of ASCII table
    */
    std::vector<char>& get_data() { return this->data_; }

    /**
     * @brief       Returns the data of ASCII table (const version)
    */
    const std::vector<char>& get_data() const { return this->data_; }

    /**
     * @brief       Gets the metadata along with value(field_value) for every row of specified field
     * @details     This methods takes a field name as argument and returns the metadata information
     *              of the field along with the field value for all the rows in the table.
     * @param[in]   name Name of the field
     * @return      Returns the metadata along with value for every row of specified field
    */
    template<typename ColDataType>
    std::unique_ptr<column_data<ColDataType>> get_column(const std::string& column_name) const {

        auto column_info = std::find_if(this->col_metadata_.begin(), this->col_metadata_.end(), [&column_name](const column& col) {
            return column_name == col.TTYPE();
            });

        
        if (column_info != this->col_metadata_.end()) {
            auto result = std::make_unique<column_data<ColDataType>>(*column_info);
            fill_column<ColDataType>(result);
            return result;
        }else {
            return nullptr;
        }
    }

    /**
     * @brief     Returns the field width based on the specified format
     * @param[in] format Field format
     * @return    Returns the width of the field
    */
    static std::size_t column_size(std::string format)
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });
        int decimal = static_cast<int>(form.length());
        for(int i = 0; i < static_cast<int>(form.length()); i++)
        {
        	if(form[i] == '.')
        	{
        		decimal = i;
        		break;
        	}
        }

        return boost::lexical_cast<std::size_t>(form.substr(1, decimal - 1));
    }

    /**
     * @brief       Gets the type of value stored in field based on the format specified
     * @param[in]   format  Format of field
     * @return      Type of value stored
    */
    static char get_type(std::string format)
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });

        return form[0];
    }

private:

    /**
     * @brief      Fills a perticular column with column data from internal buffer
     * @details    This method converts the raw column elements to ColDataType and fills
                   the column_data object passed as argument
     * @param[in]  column_ptr Pointer to column container 
    */
    template<typename ColDataType>
    void fill_column(std::unique_ptr< column_data<ColDataType>>& column_ptr) const
    {        
        column_ptr->get_data().reserve(this->hdu_header.naxis(2));
        for (std::size_t i = 0; i < this->hdu_header.naxis(2); i++) {
            auto starting_offset =
                this->data_.begin() + i * this->hdu_header.naxis(1) + column_ptr->TBCOL();

            auto ending_offset = starting_offset + column_size(column_ptr->TFORM());

            std::string row_data_str(starting_offset, ending_offset);

            ColDataType row_data = boost::lexical_cast<ColDataType>(
                boost::algorithm::trim_copy(row_data_str));

            column_ptr->get_data().emplace_back(row_data);
        }
       
    }
   
    /**
     * @brief  Initializes the current object with  column metadata and table data
     * @param[in] data_buffer Data of the ASCII table
    */
    void set_ascii_table_info(const std::string& data_buffer) {
        populate_column_data();
        this->data_.assign(data_buffer.begin(), data_buffer.end());
    }

};

using ascii_table = basic_ascii_table<card_policy>;


}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

