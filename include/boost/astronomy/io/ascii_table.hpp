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
#include <boost/astronomy/io/table_extension.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/variant.hpp>
#include <boost/cstdfloat.hpp>



namespace boost { namespace astronomy {  namespace io {

/**
 * @brief           Stores the header information and data of ASCII_Table extension HDU
 * @details         This class provides a set of methods for creating,querying, and manipulation of ASCII_table extension HDU
 *                  For more information on Ascii_Table extension visit
 *                  <A href="http://archive.stsci.edu/fits/users_guide/node37.html#SECTION00540000000000000000">ASCII_TABLE</A>
 * @author          Sarthak Singhal,Gopi Krishna Menon, Pranam Lashkari
*/

template<typename CardPolicy,typename Converter>
struct basic_ascii_table : public table_extension<CardPolicy>
{
    mutable std::unordered_map<std::string,
        boost::variant<

        column_view<std::string, Converter>,
        column_view<long long, Converter>,
        column_view<float, Converter>,
        column_view<double, Converter>
        >
    > cached_columns;

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
     * @brief      Sets the data of ASCII Table from data_buffer
     * @param[in]  data_buffer Data of ASCII Table
    */
    void set_data(const std::string& data_buffer) {
        this->col_metadata_.clear();
        this->col_metadata_.resize(this->tfields_);
        set_ascii_table_info(data_buffer);
    }


    /**
     * @brief       Returns a editable view of the column
     * @param[in]   name Name of the field
     * @return      Returns a view of the column for reading or writing data
    */ 
    template<typename ColDataType>
    column_view<ColDataType,Converter>& get_column(const std::string& column_name) const {

        auto cache_entry = this->cached_columns.find(column_name);

        if (cache_entry != this->cached_columns.end()) {
            return boost::get<column_view<ColDataType,Converter>>(this->cached_columns[column_name]);
        }
        else {
            this->cached_columns.emplace(column_name, this->template make_column_view<ColDataType,Converter>(column_name));
            return boost::get<column_view<ColDataType,Converter>>(this->cached_columns[column_name]);
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

    template<typename FileWriter>
    void write_to(FileWriter& file_writer) {

        this->hdu_header.write_header(file_writer);

        for (int row = 0; row < this->tb_data.size(); row++) {

            std::string row_temp_buffer = "";

            for (int col = 0; col < this->tb_data[row].size(); col++) {
                std::string padding = std::string(this->column_size(this->col_metadata_[col].TFORM())-this->tb_data[row][col].length(),' ');
                std::string final_data = padding + this->tb_data[row][col];
                row_temp_buffer += final_data;
            }
            file_writer.write(row_temp_buffer);
        }
        auto current_write_pos = file_writer.get_current_pos();
        auto logical_record_end_pos = file_writer.find_unit_end();

        file_writer.write(std::string(logical_record_end_pos - current_write_pos, ' '));
    }

private:

  
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

            this->col_metadata_[i].total_elements_per_field(0);

        }
    }

    /**
     * @brief  Initializes the current object with  column metadata and table data
     * @param[in] data_buffer Data of the ASCII table
    */
    void set_ascii_table_info(const std::string& data_buffer) {
        populate_column_data();

        if (!data_buffer.empty()) {
          set_table_data(data_buffer);
        }

    }

    /**
     * @brief Converts the data_buffer into table_data format (2D matrix )
    */
    void set_table_data(const std::string& data_buffer) {

        auto total_rows = this->hdu_header.naxis(2);
        auto total_fields = this->tfields_;
        auto total_characters_per_row = this->hdu_header.naxis(1);

        // Initialize table data matrix with total_rows * total_columns elements of type string
        this->tb_data = typename table_extension<CardPolicy>::table_data(total_rows, std::vector<std::string>(total_fields, std::string()));

        auto current_row = 0;
        auto current_column = 0;

        while(current_row< total_rows) {
            auto starting_offset = data_buffer.begin()+ current_row * total_characters_per_row + this->col_metadata_[current_column].TBCOL() - 1;
            auto ending_offset = starting_offset + this->column_size(this->col_metadata_[current_column].TFORM());

            this->tb_data[current_row][current_column++] = boost::algorithm::trim_copy(std::string(starting_offset, ending_offset));

            if (current_column % this->tfields_ == 0) {
                current_row++;
                current_column = 0;
            }

        }
    }
};



}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

