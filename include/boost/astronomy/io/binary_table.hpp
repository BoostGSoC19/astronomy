/*=============================================================================
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
#define BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP

#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/astronomy/io/column.hpp>
#include <boost/astronomy/io/column_data.hpp>
#include <boost/astronomy/io/table_extension.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include <boost/cstdint.hpp>
#include <boost/cstdfloat.hpp>
#include <complex>
#include <fstream>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <utility>

/**
 * @file    binary_table.hpp
 * @author  Pranam Lashkari
 * @details This file contains definition for binary_table_extension structure
 */

namespace boost {
    namespace astronomy {
        namespace io {

            using namespace std::placeholders;

            /**
             * @brief           Stores the header information and data of Binary Table
             * extension HDU
             * @details         This class provides a set of methods for creating,querying,
             * and manipulation of Binary Table extension HDU For more information on
             * Binary_Table extension visit <A
             * href="http://archive.stsci.edu/fits/users_guide/node44.html#SECTION00560000000000000000">BINARY_TABLE</A>
             * @author          Pranam Lashkari
             */
            struct binary_table_extension : table_extension {
            public:
                /**
                 * @brief       Creates a standalone object of binary_table_extension

                */
                binary_table_extension() {}

                /**
                 * @brief       Constructs an binary_table_extension object from the given
                 * filestream
                 * @details     This constructor constructs an binary_table_extension object
                 * by reading the header information,data from the filestream and populates
                 * the field information that can be used for easy access to table data
                 * @param[in,out] file filestream set to open mode for reading
                 * @note        After the reading the file pointer/cursor will be set to the
                 * end of logical HDU unit
                 */
                binary_table_extension(std::fstream& file) : table_extension(file) {
                    set_binary_table_info(file);
                    set_unit_end(file);
                }

                /**
                 * @brief       Constructs an binary_table_extension object from the given
                 * filestream and hdu object
                 * @details     Constructs an  binary_table_extension object by reading the
                 * data from filestream and header information from hdu object passed as an
                 * argument
                 * @param[in,out] file filestream set to open mode for reading
                 * @param[in]   other hdu object containing the header information of the
                 * current extension HDU
                 * @note        After the reading the file pointer/cursor will be set to the
                 * end of logical HDU unit
                 */
                binary_table_extension(std::fstream& file, hdu const& other)
                    : table_extension(other) {
                    set_binary_table_info(file);
                    set_unit_end(file);
                }

                /**
                 * @brief       Constructs an binary_table_extension object from the given
                 * position in filestream
                 * @details     Constructs an binary_table_extension object by reading the HDU
                 * information from the given  filestream, starting at pos
                 * @param[in,out] file filestream set to open mode for reading
                 * @param[in] pos File Pointer/cursor position from where the header
                 * information is to be read
                 * @note        After the reading the file pointer/cursor will be set to the
                 * end of logical HDU unit
                 */
                binary_table_extension(std::fstream& file, std::streampos pos)
                    : table_extension(file, pos) {
                    set_binary_table_info(file);
                    set_unit_end(file);
                }

                /**
                 * @brief    Populates the metadata information for all fields of
                 * binary_table_extension
                 * @details  This method populates the metadata information for all fields in
                 * a table for easy access to the data of binary_table_extention
                 * @note     After the reading the file pointer/cursor will be set to the end
                 * of logical HDU unit
                 */
                void populate_column_data() {
                    std::size_t start = 0;
                    for (std::size_t i = 0; i < this->tfields_; i++) {
                        col_metadata_[i].index(i + 1);

                        col_metadata_[i].TFORM(value_of<std::string>(
                            "TFORM" + boost::lexical_cast<std::string>(i + 1)));

                        col_metadata_[i].TBCOL(start);

                        start += column_size(col_metadata_[i].TFORM());

                        try {
                            col_metadata_[i].TTYPE(value_of<std::string>(
                                "TTYPE" + boost::lexical_cast<std::string>(i + 1)));

                            col_metadata_[i].comment(
                                value_of<std::string>(col_metadata_[i].TTYPE()));
                        }
                        catch (std::out_of_range&) { /*Do Nothing*/
                        }

                        try {
                            col_metadata_[i].TUNIT(value_of<std::string>(
                                "TUNIT" + boost::lexical_cast<std::string>(i + 1)));
                        }
                        catch (std::out_of_range&) { /*Do Nothing*/
                        }

                        try {
                            col_metadata_[i].TSCAL(value_of<double>(
                                "TSCAL" + boost::lexical_cast<std::string>(i + 1)));
                        }
                        catch (std::out_of_range&) { /*Do Nothing*/
                        }

                        try {
                            col_metadata_[i].TZERO(value_of<double>(
                                "TZERO" + boost::lexical_cast<std::string>(i + 1)));
                        }
                        catch (std::out_of_range&) { /*Do Nothing*/
                        }

                        try {
                            col_metadata_[i].TDISP(value_of<std::string>(
                                "TDISP" + boost::lexical_cast<std::string>(i + 1)));
                        }
                        catch (std::out_of_range&) { /*Do Nothing*/
                        }

                        try {
                            col_metadata_[i].TDIM(value_of<std::string>(
                                "TDIM" + boost::lexical_cast<std::string>(i + 1)));
                        }
                        catch (std::out_of_range&) { /*Do Nothing*/
                        }
                    }
                }
                /**
                 * @brief       Gets the metadata along with value(field_value) for every row
                 * of specified field
                 * @details     This methods takes a field name as argument and returns the
                 * metadata information of the field along with the field value for all the
                 * rows in the table.
                 * @param[in]   name Name of the field
                 * @return      Returns the metadata along with value for every row of
                 * specified field
                 * @todo        From what i feel so far this function provides returns empty
                 * column
                 */

                template <typename ColDataType>
                std::unique_ptr<column_data<ColDataType>> get_column(const std::string name) {
                    auto column_info = std::find_if(
                        col_metadata_.begin(), col_metadata_.end(),
                        [&name](const column& col) { return col.TTYPE() == name; });

                    return parse_to<ColDataType>(*column_info);
                }

                /**
                 * @brief       Returns the data of Binary Table
                 */
                std::vector<char>& get_data() { return data_; }

                /**
                 * @brief       Returns the data of Binary table (const version)
                 */
                const std::vector<char>& get_data() const { return data_; }

                /**
                 * @brief     Returns the field width based on the specified format
                 * @param[in] format Field format
                 * @return    Returns the width of the field
                 */
                std::size_t column_size(std::string format) const {
                    std::string form = boost::trim_copy_if(
                        format, [](char c) -> bool { return c == '\'' || c == ' '; });

                    auto no_of_elements = element_count(form);
                    auto size_type = type_size(get_type(form));

                    return no_of_elements * size_type;
                }

                /**
                 * @brief       Gets the number of elements present in field for which the
                 * format is specified
                 * @param[in]   format  Format of field
                 * @return      Number of elements in present in the field for which the
                 * format is specified
                 */
                std::size_t element_count(std::string format) const {
                    std::string form = boost::trim_copy_if(
                        format, [](char c) -> bool { return c == '\'' || c == ' '; });

                    return form.length() > 1 ? boost::lexical_cast<std::size_t>(
                        form.substr(0, form.length() - 1))
                        : static_cast<std::size_t>(1);
                }

                /**
                 * @brief       Gets the type of value stored in field based on the format
                 * specified
                 * @param[in]   format  Format of field
                 * @return      Type of value stored
                 */
                char get_type(std::string format) const {
                    std::string form = boost::trim_copy_if(
                        format, [](char c) -> bool { return c == '\'' || c == ' '; });

                    return form[form.length() - 1];
                }

                /**
                 * @brief       Gets the size of a perticular type
                 * @param[in]   type  Field type based on binary table extension
                 * @return      Size of perticular type
                 */
                std::size_t type_size(char type) const {
                    switch (type) {
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
                 * @brief       Populates the container of given type with field_value for
                 * every row of specified field
                 * @param[in,out] column_container Container that stores the field value for
                 * every row of specified field
                 * @param[in]   start Position where column begins for the field
                 * @param[in]   column_size Total size of the field
                 * @param[in]   lambda Lambda function for fetching the field data from data
                 * buffer
                 * @todo        Why is column size present there
                 */

                void set_binary_table_info(std::fstream& file) {
                    populate_column_data();

                    char* data_buffer = new char[naxis(1) * naxis(2)];
                    file.read(data_buffer, naxis(1) * naxis(2));
                    data_.assign(data_buffer, data_buffer + naxis(1) * naxis(2));
                    delete[] data_buffer;
                }

                template <typename VectorType, typename Lambda>
                void fill_col(std::vector<VectorType>& column_container,
                    const column& col_metadata, Lambda lambda) const {
                    auto is_single_element = element_count(col_metadata.TFORM()) > 1;
                    column_container.reserve(naxis(2));
                    for (std::size_t i = 0; i < naxis(2); i++) {
                        std::string raw_data;
                        auto start_off =
                            this->data_.data() + (i * naxis(1) + col_metadata.TBCOL());
                        auto ending_off = this->data_.data() +
                            (i * naxis(1) + col_metadata.TBCOL()) +
                            column_size(col_metadata.TFORM());

                        if (is_single_element) {
                            raw_data.assign(start_off, ending_off);
                            column_container.push_back(lambda(raw_data));

                        }
                        else {
                            ending_off = start_off + (ending_off - start_off) *
                                element_count(col_metadata.TFORM());
                            raw_data.assign(start_off, ending_off);
                            column_container.push_back(lambda(raw_data));
                        }
                    }
                }

                template <typename T>
                std::unique_ptr<column_data<T>> parse_to(const column& col_metadata) {
                    return nullptr;
                }
                // TODO : Refactor  it into a seperate class if required
                template <typename NumericType, typename AssumeType = NumericType>
                NumericType element_to_numeric(const std::string& element) {
                    AssumeType temp = boost::endian::big_to_native(
                        *reinterpret_cast<const AssumeType*>(element.c_str()));

                    NumericType value;
                    std::memcpy(&value, &temp, sizeof(NumericType));
                    return value;
                }

                template <typename NumericType, typename AssumeType = NumericType>
                std::vector<NumericType> elements_to_numeric_collection(
                    const std::string& elements, std::size_t no_elements) {
                    std::vector<NumericType> values;
                    values.resize(no_elements);

                    auto start = reinterpret_cast<const NumericType*>(elements.c_str());
                    auto end_s =
                        reinterpret_cast<const NumericType*>(elements.c_str()) + no_elements;

                    std::transform(start, end_s, values.begin(), [](NumericType element) {
                        AssumeType temp;
                        std::memcpy(&temp, &element, sizeof(AssumeType));
                        temp = boost::endian::big_to_native(temp);

                        NumericType value;
                        std::memcpy(&value, &temp, sizeof(NumericType));
                        return value;
                        });

                    return values;
                }
                template <typename ComplexType, typename AssumeType = ComplexType>
                std::complex<ComplexType> element_to_complex(const std::string& element) {
                    AssumeType temp_real = boost::endian::big_to_native(
                        *reinterpret_cast<const AssumeType*>(element.c_str()));

                    ComplexType real;
                    std::memcpy(&real, &temp_real, sizeof(ComplexType));

                    AssumeType temp_imaginary = boost::endian::big_to_native(
                        *(reinterpret_cast<const AssumeType*>(element.c_str()) + 1));
                    ComplexType imaginary;
                    std::memcpy(&imaginary, &temp_imaginary, sizeof(ComplexType));

                    return std::complex<ComplexType>(real, imaginary);
                }

                template <typename ComplexType, typename AssumeType = ComplexType>
                std::vector<std::complex<ComplexType>> elements_to_complex_collection(
                    const std::string& elements, std::size_t no_elements) {
                    std::vector<std::complex<ComplexType>> values;
                    values.reserve(no_elements);

                    for (std::size_t i = 0; i < no_elements; i++) {
                        AssumeType temp_real =
                            static_cast<AssumeType>(boost::endian::big_to_native(
                                *reinterpret_cast<const AssumeType*>(elements.c_str()) + 2 * i));
                        ComplexType real;
                        std::memcpy(&real, &temp_real, sizeof(ComplexType));

                        AssumeType temp_imaginary =
                            static_cast<AssumeType>(boost::endian::big_to_native(
                                *reinterpret_cast<const AssumeType*>(elements.c_str()) + 2 * i +
                                1));
                        ComplexType imaginary;
                        std::memcpy(&imaginary, &temp_imaginary, sizeof(ComplexType));
                        values.emplace_back(std::complex<ComplexType>(real, imaginary));
                    }
                    return values;
                }
                template <typename ByteType>
                ByteType element_to_byte(const std::string& element) {
                    return boost::lexical_cast<ByteType>(element);
                }

                template <typename ByteType>
                std::vector<ByteType> elements_to_byte_collection(const std::string& elements,
                    std::size_t no_elements) {
                    return std::vector<ByteType>(elements.begin(),
                        elements.begin() + no_elements);
                }
            };

            template <>
            std::unique_ptr<column_data<bool>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<bool>>(col_metadata);

                auto single_bool = [](const std::string& element) {
                    return element[0] == 'T';
                };

                fill_col(result->get_data(), col_metadata, single_bool);

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<bool>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result = std::make_unique<column_data<std::vector<bool>>>(col_metadata);
                auto multi_bool = [](const std::string& element_coll) {
                    std::vector<bool> values;
                    for (auto element : element_coll) {
                        values.emplace_back(element == 'T');
                    }
                    return values;
                };
                fill_col(result->get_data(), col_metadata, multi_bool);
                return result;
            }
            template <>
            std::unique_ptr<column_data<boost::int16_t>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<boost::int16_t>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_numeric<boost::int16_t>(element);
                    });
                /* std::bind(&binary_table_extension::element_to_numeric<boost::int16_t>,
                           this, _1));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<boost::int16_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<boost::int16_t>>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_numeric_collection<boost::int16_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });

                /* std::bind(
                     &binary_table_extension::elements_to_numeric_collection<boost::int16_t>,
                     this, _1, element_count(col_metadata.TFORM())));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<boost::int32_t>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<boost::int32_t>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_numeric<boost::int32_t>(element);
                    });
                /*  std::bind(&binary_table_extension::element_to_numeric<boost::int32_t>,
                            this, _1));*/
                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<boost::int32_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<boost::int32_t>>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_numeric_collection<boost::int32_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /*std::bind(
                    &binary_table_extension::elements_to_numeric_collection<boost::int32_t>,
                    this, _1, element_count(col_metadata.TFORM())));*/
                return result;
            }
            template <>
            std::unique_ptr<column_data<boost::float32_t>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<boost::float32_t>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_numeric<boost::float32_t, boost::int32_t>(element);
                    });
                /*  std::bind(&binary_table_extension::element_to_numeric<boost::float32_t,
                                                                        boost::int32_t>,
                            this, _1));*/
                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<boost::float32_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<boost::float32_t>>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_numeric_collection<boost::float32_t, boost::int32_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /*std::bind(&binary_table_extension::elements_to_numeric_collection<
                              boost::float32_t, boost::int32_t>,
                          this, _1, element_count(col_metadata.TFORM())));*/
                return result;
            }
            template <>
            std::unique_ptr<column_data<boost::float64_t>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<boost::float64_t>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_numeric<boost::float64_t, boost::int64_t>(element);
                    });
                /*  std::bind(&binary_table_extension::element_to_numeric<boost::float64_t,
                                                                        boost::int64_t>,
                            this, _1));*/
                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<boost::float64_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<boost::float64_t>>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_numeric_collection<boost::float64_t, boost::int64_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /*  std::bind(&binary_table_extension::elements_to_numeric_collection<
                                boost::float64_t, boost::int64_t>,
                            this, _1, element_count(col_metadata.TFORM())));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::pair<boost::int32_t, boost::int32_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::pair<boost::int32_t, boost::int32_t>>>(
                        col_metadata);

                auto element_to_descriptor = [](const std::string& element) {
                    auto x = boost::endian::big_to_native(
                        *reinterpret_cast<const boost::int32_t*>(element.c_str()));
                    auto y = boost::endian::big_to_native(
                        *(reinterpret_cast<const boost::int32_t*>(element.c_str()) + 1));
                    return std::make_pair(x, y);
                };

                fill_col(result->get_data(), col_metadata, element_to_descriptor);
                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<std::pair<boost::int32_t, boost::int32_t>>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result = std::make_unique<
                    column_data<std::vector<std::pair<boost::int32_t, boost::int32_t>>>>(
                        col_metadata);

                auto no_elements = element_count(col_metadata.TFORM());
                auto elements_to_descriptors = [no_elements](const std::string& elements) {
                    std::vector<std::pair<boost::int32_t, boost::int32_t>> values;
                    values.reserve(no_elements);
                    for (std::size_t i = 0; i < no_elements; i++) {
                        values.emplace_back(
                            boost::endian::big_to_native(
                                *(reinterpret_cast<const boost::int32_t*>(elements.c_str()) + i)),
                            boost::endian::big_to_native(
                                *(reinterpret_cast<const boost::int32_t*>(elements.c_str()) + i +
                                    1)));
                    }
                    return values;
                };

                fill_col(result->get_data(), col_metadata, elements_to_descriptors);

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::complex<boost::float32_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::complex<boost::float32_t>>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_complex<boost::float32_t, boost::int32_t>(element);
                    });
                // std::bind(&binary_table_extension::element_to_complex<boost::float32_t,
                //                                                      boost::int32_t>,
                //          this, _1));

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<std::complex<boost::float32_t>>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<std::complex<boost::float32_t>>>>(
                        col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_complex_collection<boost::float32_t, boost::int32_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /*  std::bind(&binary_table_extension::elements_to_complex_collection<
                                boost::float32_t, boost::int32_t>,
                            this, _1, element_count(col_metadata.TFORM())));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::complex<boost::float64_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::complex<boost::float64_t>>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_complex<boost::float64_t, boost::int64_t>(element);
                    });
                /*  std::bind(&binary_table_extension::element_to_complex<boost::float64_t,
                                                                        boost::int64_t>,
                            this, _1));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<std::complex<boost::float64_t>>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<std::complex<boost::float64_t>>>>(
                        col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_complex_collection<boost::float64_t, boost::int64_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /*std::bind(&binary_table_extension::elements_to_complex_collection<
                              boost::float64_t, boost::int64_t>,
                          this, _1, element_count(col_metadata.TFORM())));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::uint8_t>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<std::uint8_t>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_byte<std::uint8_t>(element);
                    });
                /*     std::bind(&binary_table_extension::element_to_byte<std::uint8_t>,
                               this, _1));*/
                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<std::uint8_t>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result =
                    std::make_unique<column_data<std::vector<std::uint8_t>>>(col_metadata);
                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_byte_collection<std::uint8_t>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /* std::bind(
                     &binary_table_extension::elements_to_byte_collection<std::uint8_t>,
                     this, _1, element_count(col_metadata.TFORM())));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<char>> binary_table_extension::parse_to(
                const column& col_metadata) {
                auto result = std::make_unique<column_data<char>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this](const std::string& element) {
                        return element_to_byte<char>(element);
                    });
                /*     std::bind(&binary_table_extension::element_to_byte<char>, this, _1));*/

                return result;
            }
            template <>
            std::unique_ptr<column_data<std::vector<char>>>
                binary_table_extension::parse_to(const column& col_metadata) {
                auto result = std::make_unique<column_data<std::vector<char>>>(col_metadata);

                fill_col(result->get_data(), col_metadata,
                    [this, &col_metadata](const std::string& elements) {
                        return elements_to_byte_collection<char>(
                            elements, element_count(col_metadata.TFORM()));
                    });
                /*      std::bind(&binary_table_extension::elements_to_byte_collection<char>,
                                this, _1, element_count(col_metadata.TFORM())));*/

                return result;
            }

        }  // namespace io
    }  // namespace astronomy
}  // namespace boost
#endif  // BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
