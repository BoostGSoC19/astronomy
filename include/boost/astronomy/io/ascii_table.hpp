
/*=============================================================================
  Copyright 2019-2020 Sarthak Singhal < sarthak2007 - singhalsarthak2007@gmail.com >

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
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
    ascii_table(std::fstream &file, hdu const& other) : table_extension(file, other)
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
        for (std::size_t i = 0; i < this->tfields; i++)
        {
            col_metadata[i].index(i + 1);

            col_metadata[i].TFORM(
                value_of<std::string>("TFORM" + boost::lexical_cast<std::string>(i + 1))
            );

            col_metadata[i].TBCOL(
                value_of<std::size_t>("TBCOL" + boost::lexical_cast<std::string>(i + 1))
            );

            try {
                col_metadata[i].TTYPE(
                    value_of<std::string>("TTYPE" + boost::lexical_cast<std::string>(i + 1))
                );

                col_metadata[i].comment(
                    value_of<std::string>(col_metadata[i].TTYPE())
                );
            }
            catch (std::out_of_range e) {/*Do Nothing*/ }

            try {
                col_metadata[i].TUNIT(
                    value_of<std::string>("TUNIT" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range e) {/*Do Nothing*/ }

            try {
                col_metadata[i].TSCAL(
                    value_of<double>("TSCAL" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range e) {/*Do Nothing*/ }

            try {
                col_metadata[i].TZERO(
                    value_of<double>("TZERO" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range e) {/*Do Nothing*/ }
        }
    }

    /**
     * @brief       Reads the data associated with ASCII_Table extension HDU from the filestream
     * @param[in,out] file filestream set to open mode for reading
     * @note        After the reading the file pointer/cursor will be set to the end of logical HDU unit
    */
    void read_data(std::fstream &file)
    {
        file.read(&data[0], naxis(1)*naxis(2));
        set_unit_end(file);
    }

    /**
     * @brief       Gets the metadata along with value(field_value) for every row of specified field
     * @details     This methods takes a field name as argument and returns the metadata information
     *              of the field along with the field value for all the rows in the table.
     * @param[in]   name Name of the field
     * @return      Returns the metadata along with value for every row of specified field
     * @todo        From what i feel so far this function provides returns empty column
    */
    std::unique_ptr<column> get_column(std::string name) const
    {
        for (auto& col : col_metadata)
        {
            if (col.TTYPE() == name)
            {
                switch (get_type(col.TFORM()))
                {
                case 'A':
                {
                    auto result = std::make_unique<column_data<char>>(col);
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> char {
                            return *element;
                        }
                    );
                    return std::move(result);
                }
                case 'I':
                {
                    auto result = std::make_unique<column_data<std::int32_t>>(col);
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> std::int32_t {
                            return boost::endian::big_to_native(*reinterpret_cast<const std::int32_t*>(element));
                        }
                    );
                    return std::move(result);
                }
                case 'F':
                {
                    auto result = std::make_unique<column_data<float>>(col);
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> float {
                            float result = (element[3] << 0) | (element[2] << 8) |
                                            (element[1] << 16) | (element[0] << 24);
                            return result;
                        }
                    );
                    return std::move(result);
                }
                case 'E':
                {
                    auto result = std::make_unique<column_data<float>>(col);
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> float {
                            float result = (element[3] << 0) | (element[2] << 8) |
                                            (element[1] << 16) | (element[0] << 24);
                            return result;
                        }
                    );
                    return std::move(result);
                }
                case 'D':
                {
                    auto result = std::make_unique<column_data<double>>(col);
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> double {
                            double result = (element[7] << 0) | (element[6] << 8) |
                                            (element[5] << 16) | (element[4] << 24) |
                                            (element[3] << 32) | (element[2] << 40) |
                                            (element[1] << 48) | (element[0] << 56);
                            return result;
                        }
                    );
                    return std::move(result);
                }
                default:
                    throw invalid_table_colum_format();
                }
            }
        }

        return std::unique_ptr<column>(nullptr);
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
        int decimal = form.length();
        for(int i = 0; i < form.length(); i++)
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
    template<typename VectorType, typename Lambda>
    void fill_column 
    (
        std::vector<VectorType> &column_container,
        std::size_t start,
        std::size_t column_size,
        Lambda lambda
    ) const
    {
        column_container.reserve(naxis(2));
        for (std::size_t i = 0; i < naxis(2); i++)
        {
            column_container.emplace_back(lambda(this->data.data() + (i * naxis(1) + start)));
        }
    }

private:
    /**
     * @brief  Initializes the current object with  column metadata and table data
    */
    void set_ascii_table_info(std::fstream& file)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1) * naxis(2), std::back_inserter(data));
        
    }


};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

