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
#include <boost/astronomy/io/default_card_policy.hpp>
#include <boost/astronomy/io/data_conversions.h>



namespace boost { namespace astronomy { namespace io {
 /**
  * @brief           Stores the header information and data of Binary Table extension HDU
  * @details         This class provides a set of methods for creating,querying, and manipulation of Binary Table extension HDU
  *                  For more information on Binary_Table extension visit
  *                  <A href="http://archive.stsci.edu/fits/users_guide/node44.html#SECTION00560000000000000000">BINARY_TABLE</A>
  * @author          Pranam Lashkari
 */
template<typename CardPolicy=card_policy>
struct basic_binary_table_extension : table_extension<CardPolicy>
{
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
        }
    }

    /**
     * @brief       Gets the metadata along with value(field_value) for every row of specified field
     * @details     This methods takes a field name as argument and returns the
     *              metadata information of the field along with the field value for all the
     *              rows in the table.
     * @param[in]   name Name of the field
     * @return      Returns the metadata along with value for every row of
     *              specified field
     */
    template <typename ColDataType>
    std::unique_ptr<column_data<ColDataType>> get_column(const std::string name) {
        auto column_info = std::find_if(
            this->col_metadata_.begin(), this->col_metadata_.end(),
            [&name](const column& col) { return col.TTYPE() == name; }
        );

        if (column_info == this->col_metadata_.end()) { return nullptr; }

        return parse_to<ColDataType>(*column_info);
    }

    /**
     * @brief       Returns the data of Binary Table
    */
    std::vector<char>& get_data() { return this->data_; }

    /**
     * @brief       Returns the data of Binary table (const version)
    */
    const std::vector<char>& get_data() const { return this->data_; }

    /**
     * @brief      Sets the data of Binary Table from data_buffer
     * @param[in]  data_buffer Data of Binary Table
    */
    void set_data(const std::string& data_buffer) {
        this->data_.clear();
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

private:
    /**
     * @brief       Populates the container of given type with field_value for every row of specified field
     * @param[in,out] column_container Container that stores the field value for every row of specified field
     * @param[in]   col_metadata Column Metadata ( Some problems exists otherwise this param is not needed )
     * @param[in]   lambda Lambda function for fetching the field data from data buffer
    */
    template <typename VectorType, typename Lambda>
    void fill_col(
        std::vector<VectorType>& column_container,
        const column& col_metadata,
        Lambda lambda
                 ) const {
        auto is_single_element = element_count(col_metadata.TFORM()) > 1;
        column_container.reserve(this->hdu_header.naxis(2));
        for (std::size_t i = 0; i < this->hdu_header.naxis(2); i++) {

            std::string raw_data;

            auto start_off = this->data_.data() + (i * this->hdu_header.naxis(1) + col_metadata.TBCOL());
            auto ending_off = this->data_.data() +(i * this->hdu_header.naxis(1) + col_metadata.TBCOL()) +
                column_size(col_metadata.TFORM());

            if (is_single_element) {
                raw_data.assign(start_off, ending_off);
                column_container.push_back(lambda(raw_data,col_metadata));
            }
            else {
                ending_off = start_off + (ending_off - start_off) * element_count(col_metadata.TFORM());
                raw_data.assign(start_off, ending_off);
                column_container.push_back(lambda(raw_data,col_metadata));
            }
        }
    }

    /**
     * @brief  Initializes the current object with  column metadata and table data
     * @param[in] data_buffer Data of the Binary table
    */
    void set_binary_table_info(const std::string& data_buffer) {
        populate_column_data();
        this->data_.assign(data_buffer.begin(), data_buffer.end());
    }

    /**
     * @brief  Converts raw binary table field data to the given type and returns the column data
     * @param[in] col_metadata Column metadata used for parsing
    */
    template <typename T>
    std::unique_ptr<column_data<T>> parse_to(const column& col_metadata) {
        auto result = std::make_unique<column_data<T>>(col_metadata);
        fill_col(result->get_data(), col_metadata, data_conversions::convert<T>);
        return result;
    }
};

using binary_table = basic_binary_table_extension<card_policy>;


}}} //namespace boost::astronomy::io
#endif // BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
