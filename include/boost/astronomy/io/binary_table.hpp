/*=============================================================================
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
#define BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP

#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <complex>
#include <utility>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/cstdint.hpp>
#include <boost/cstdfloat.hpp>

#include <boost/astronomy/io/table_extension.hpp>
#include <boost/astronomy/io/column.hpp>
#include <boost/astronomy/io/column_data.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>


namespace boost { namespace astronomy { namespace io {
 /**
  * @brief           Stores the header information and data of Binary Table extension HDU
  * @details         This class provides a set of methods for creating,querying, and manipulation of Binary Table extension HDU
  *                  For more information on Binary_Table extension visit
  *                  <A href="http://archive.stsci.edu/fits/users_guide/node44.html#SECTION00560000000000000000">BINARY_TABLE</A>
  * @author          Pranam Lashkari
 */
template<typename CardPolicy, typename Converter>
struct basic_binary_table_extension : table_extension<CardPolicy>
{

    mutable std::unordered_map<std::string,
        boost::variant<
        column_view<bool, Converter>,
        column_view<std::vector<bool>, Converter>,

        column_view<boost::int16_t, Converter>,
        column_view<std::vector<boost::int16_t>, Converter>,

        column_view<boost::int32_t, Converter>,
        column_view<std::vector<boost::int32_t>, Converter>,

        column_view<boost::float32_t, Converter>,
        column_view<std::vector<boost::float32_t>, Converter>,

        column_view<boost::float64_t, Converter>,
        column_view<std::vector<boost::float64_t>, Converter>,

        column_view<std::pair<boost::int32_t, boost::int32_t>, Converter>,
        column_view<std::vector<std::pair<boost::int32_t, boost::int32_t>>, Converter>,

        column_view<std::complex<boost::float32_t>, Converter>,
        column_view<std::vector<std::complex<boost::float32_t>>, Converter>,

        column_view<std::complex<boost::float64_t>, Converter>,
        column_view<std::vector<std::complex<boost::float64_t>>, Converter>,

        column_view<std::uint8_t, Converter>,
        column_view<std::vector<std::uint8_t>, Converter>,

        column_view<char, Converter>,
        column_view<std::vector<char>, Converter>
         >
    > cached_columns;

public:

    /**
     * @brief       Creates a standalone object of binary_table_extension
    */
    basic_binary_table_extension() {}

    
    /**
    * @brief      Constructs an Binary Table object from header and data buffer
    * @param[in]  other Header part of Binary table
    * @param[in]  data_buffer Data part of Binary table
   */
    basic_binary_table_extension(header<CardPolicy> const& other, const std::string& data_buffer) : table_extension<CardPolicy>(other) {

        set_binary_table_info(data_buffer);

    }

    

    template<typename ColDataType>
    column_view<ColDataType, Converter>& get_column(const std::string& column_name) {
        auto cache_entry = this->cached_columns.find(column_name);

        if (cache_entry != this->cached_columns.end()) {
            return boost::get<column_view<ColDataType, Converter>>(this->cached_columns[column_name]);
        }
        else {
                this->cached_columns.emplace(column_name, this->template make_column_view<ColDataType, Converter>(column_name));
                return boost::get<column_view<ColDataType, Converter>>(this->cached_columns[column_name]);
        }
    }


    template<typename FileWriter>
    void write_to(FileWriter& file_writer) {

        this->hdu_header.write_header(file_writer);

        for (int row = 0; row < this->tb_data.size(); row++) {

            std::string row_temp_buffer = "";

            for (int col = 0; col < this->tb_data[row].size(); col++) {
                row_temp_buffer += this->tb_data[row][col];
            }

            file_writer.write(row_temp_buffer);
        }
        auto current_write_pos = file_writer.get_current_pos();
        auto logical_record_end_pos = file_writer.find_unit_end();

        file_writer.write(std::string(logical_record_end_pos - current_write_pos, ' '));
    }

    /**
     * @brief      Sets the data of Binary Table from data_buffer
     * @param[in]  data_buffer Data of Binary Table
    */
    void set_data(const std::string& data_buffer) {
        this->col_metadata_.clear();
        this->col_metadata_.resize(this->tfields_);
        set_binary_table_info(data_buffer);
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
        auto no_of_elements = element_count(form);
        auto size_type = type_size(get_type(form));
        return no_of_elements * size_type;
    }


    /**
     * @brief       Gets the number of elements present in field for which the format is specified
     * @param[in]   format  Format of field
     * @return      Number of elements in present in the field for which the format is specified
    */
    static std::size_t element_count(std::string format) 
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });

        return form.length() > 1 ?
            boost::lexical_cast<std::size_t>(form.substr(0, form.length() - 1)) :
            static_cast<std::size_t>(1);
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

        return form[form.length() - 1];
    }

    /**
     * @brief       Gets the size of a perticular type
     * @param[in]   type  Field type based on binary table extension
     * @return      Size of perticular type
    */
    static std::size_t type_size(char type) 
    {
        switch (type)
        {
        case 'L':
            return 1;
        case 'X':
            return 1;
        case 'B':
            return 1;
        case 'I':
            return 2;
        case 'J':
            return 4;
        case 'A':
            return 1;
        case 'E':
            return 4;
        case 'D':
            return 8;
        case 'C':
            return 8;
        case 'M':
            return 16;
        case 'P':
            return 8;
        default:
            throw invalid_table_colum_format();
        }
    }

 
    column& get_column_metadata(const std::string& column_name) {
        auto pos = std::find_if(this->col_metadata_.begin(), this->col_metadata_.end(), [&](const column& col) {return column_name == col.TTYPE(); });
            if (pos != this->col_metadata_.end()) {
                return (*pos);
            }
        throw column_not_found_exception(column_name);
    }

private:
   

    /**
     * @brief    Populates the metadata information for all fields of binary_table_extension
     * @details  This method populates the metadata information for all fields in a table
     *           for easy access to the data of binary_table_extention
     * @note     After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    void populate_column_data()
    {
        std::size_t start = 0;
        for (std::size_t i = 0; i < this->tfields_; i++)
        {
            this->col_metadata_[i].index(i + 1);

            this->col_metadata_[i].TFORM(
                this->hdu_header.template value_of<std::string>("TFORM" + boost::lexical_cast<std::string>(i + 1))
            );

            this->col_metadata_[i].TBCOL(start);

            start += column_size(this->col_metadata_[i].TFORM());

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

            try {
                this->col_metadata_[i].TDISP(
                    this->hdu_header.template value_of<std::string>("TDISP" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                this->col_metadata_[i].TDIM(
                    this->hdu_header.template value_of<std::string>("TDIM" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            this->col_metadata_[i].total_elements_per_field(element_count(this->col_metadata_[i].TFORM()));
        }
    }

    
    /**
     * @brief  Initializes the current object with  column metadata and table data
     * @param[in] data_buffer Data of the Binary table
    */
    void set_binary_table_info(const std::string& data_buffer) {
        populate_column_data();
        if (!data_buffer.empty()) {
            set_table_data(data_buffer);
        }
    }


    
    void set_table_data(const std::string& data_buffer) {

        auto total_rows = this->hdu_header.naxis(2);
        auto total_fields = this->tfields_;
        auto total_characters_per_row = this->hdu_header.naxis(1);

        // Initialize table data matrix with total_rows * total_columns elements of type string
        this->tb_data = typename table_extension<CardPolicy>::table_data(total_rows, std::vector<std::string>(total_fields, std::string()));

        auto current_row = 0;
        auto current_column = 0;


        auto starting_offset = data_buffer.begin();
        auto ending_offset = starting_offset;

        while (current_row<total_rows) {

            starting_offset = ending_offset;
            ending_offset = starting_offset + column_size(this->col_metadata_[current_column].TFORM());

            this->tb_data[current_row][current_column++] = boost::algorithm::trim_copy(std::string(starting_offset, ending_offset));
            if (current_column % this->tfields_ == 0) {
                current_row++;
                current_column = 0;
            }

        }
    }
};


}}} //namespace boost::astronomy::io
#endif // BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
