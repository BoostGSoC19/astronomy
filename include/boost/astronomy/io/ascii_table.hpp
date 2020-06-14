/*=============================================================================
Copyright 2019 Sarthak Singhal <singhalsarthak2007@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP
#define BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

#include <valarray>
#include <fstream>
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
#include <boost/cstdfloat.hpp>

/**
 * @file    ascii_table.hpp
 * @author  Sarthak Singhal
 * @details This file contains definition for ascii_table structure
 */

namespace boost { namespace astronomy {  namespace io {

/**
 * @brief           Stores the header information and data of ASCII_Table extension HDU
 * @details         This class provides a set of methods for creating,querying, and manipulation of ASCII_table extension HDU
 *                  For more information on Ascii_Table extension visit
 *                  <A href="http://archive.stsci.edu/fits/users_guide/node37.html#SECTION00540000000000000000">ASCII_TABLE</A>
 * @author          Sarthak Singhal
*/

struct ascii_table : public table_extension
{
public:

    /**
     * @brief      Creates a standalone ascii_table object
    */
    ascii_table() {}

    /**

    /**
     * @brief       Constructs an ascii_table object from the given filestream
     * @details     This constructor constructs an ASCII_table object by reading the
     *              header information,data from the filestream and populates the field
     *              information that can be used for easy access to table data
     * @param[in,out] file filestream set to open mode for reading
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    ascii_table(std::fstream &file) : table_extension(file)
    {
        set_ascii_table_info(file);
        set_unit_end(file);
    }

    /**
     * @brief       Constructs an ascii_table object from the given filestream and hdu object
     * @details     Constructs an  ascii_table object by reading the data from filestream
     *              and header information from hdu object passed as an argument
     * @param[in,out] file filestream set to open mode for reading
     * @param[in]   other hdu object containing the header information of the current extension HDU
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    ascii_table(std::fstream &file, hdu const& other) : table_extension(other)
    {
        set_ascii_table_info(file);
        set_unit_end(file);
    }

    /**
     * @brief       Constructs an ascii_table object from the given position in filestream
     * @details     Constructs an ascii_table object by reading the HDU information from the
     *              given  filestream, starting at pos
     * @param[in,out] file filestream set to open mode for reading
     * @param[in] pos File Pointer/cursor position from where the header information is to be read
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    ascii_table(std::fstream &file, std::streampos pos) : table_extension(file, pos)
    {
        set_ascii_table_info(file);
        set_unit_end(file);
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
            col_metadata_[i].index(i + 1);

            col_metadata_[i].TFORM(
                value_of<std::string>("TFORM" + boost::lexical_cast<std::string>(i + 1))
            );

            col_metadata_[i].TBCOL(
                value_of<std::size_t>("TBCOL" + boost::lexical_cast<std::string>(i + 1))
            );

            try {
                col_metadata_[i].TTYPE(
                    value_of<std::string>("TTYPE" + boost::lexical_cast<std::string>(i + 1))
                );

                col_metadata_[i].comment(
                    value_of<std::string>(col_metadata_[i].TTYPE())
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                col_metadata_[i].TUNIT(
                    value_of<std::string>("TUNIT" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                col_metadata_[i].TSCAL(
                    value_of<double>("TSCAL" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }

            try {
                col_metadata_[i].TZERO(
                    value_of<double>("TZERO" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range&) {/*Do Nothing*/ }
        }
    }

    /**
     * @brief       Returns the data of ASCII table
    */
    std::vector<char>& get_data() { return data_; }

    /**
     * @brief       Returns the data of ASCII table (const version)
    */
    const std::vector<char>& get_data() const { return data_; }

    /**
     * @brief       Gets the metadata along with value(field_value) for every row of specified field
     * @details     This methods takes a field name as argument and returns the metadata information
     *              of the field along with the field value for all the rows in the table.
     * @param[in]   name Name of the field
     * @return      Returns the metadata along with value for every row of specified field
    */
    std::unique_ptr<column> get_column(std::string column_name) const {
        for (auto col : col_metadata_) {
            if (col.TTYPE() == column_name) {
                switch (get_type(col.TFORM())) {
                case 'A': {
                    auto result = std::make_unique<column_data<std::string>>(col);
                    fill_column<std::string>(result->get_data(), col);
                    return std::move(result);
                }
                case 'I': {
                    auto result = std::make_unique<column_data<boost::int32_t>>(col);
                    fill_column<boost::int32_t>(result->get_data(), col);
                    return std::move(result);
                }
                case 'F':  // Floating Point only hence fallthrough
                case 'E': {
                    auto result = std::make_unique<column_data<boost::float32_t>>(col);
                    fill_column<boost::float32_t>(result->get_data(), col);
                    return std::move(result);
                }
                case 'D': {
                    auto result = std::make_unique<column_data<boost::float64_t>>(col);
                    fill_column<boost::float64_t>(result->get_data(), col);
                    return std::move(result);
                }
                }
            }
        }
        return nullptr;
    }

    /**
     * @brief     Returns the field width based on the specified format
     * @param[in] format Field format
     * @return    Returns the width of the field
    */
    std::size_t column_size(std::string format) const
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
    char get_type(std::string format) const
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });

        return form[0];
    }

private:

    /**
     * @brief         Populates the container of given type with field_value for every row of specified field
     * @param[in,out] column_container Container that stores the field value for every row of specified field
     * @param[in]     start Position where column begins for the field
     * @param[in]     column_size Total size of the field
     * @param[in]     lambda Lambda function for fetching the field data from data buffer 
     * @todo          Why is column size present there
    */
    template<typename ColDataType, typename VectorType>
    void fill_column
    (
        std::vector<VectorType> &column_container,
        const column& col_metadata
    ) const
    {
        column_container.reserve(naxis(2));
        for (std::size_t i = 0; i < naxis(2); i++) {
            auto starting_offset =
                this->data_.begin() + i * naxis(1) + col_metadata.TBCOL();

            auto ending_offset = starting_offset + column_size(col_metadata.TFORM());

            std::string row_data_str(starting_offset, ending_offset + 1);

            ColDataType row_data = boost::lexical_cast<ColDataType>(
                boost::algorithm::trim_copy(row_data_str));

            column_container.emplace_back(row_data);
        }
    }

    /**
     * @brief  Initializes the current object with  column metadata and table data
    */
    void set_ascii_table_info(std::fstream& file)
    {
        populate_column_data();
        std::copy_n(std::istreambuf_iterator<char>(file), naxis(1) * naxis(2),
            std::back_inserter(data_));
        
    }


};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

