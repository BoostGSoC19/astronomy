
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


namespace boost { namespace astronomy {  namespace io {

struct ascii_table : public table_extension
{
public:
    ascii_table(std::fstream &file) : table_extension(file)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1)*naxis(2), std::back_inserter(data));
        set_unit_end(file);
    }

    ascii_table(std::fstream &file, hdu const& other) : table_extension(file, other)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1)*naxis(2), std::back_inserter(data));
        set_unit_end(file);
    }

    ascii_table(std::fstream &file, std::streampos pos) : table_extension(file, pos)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1)*naxis(2), std::back_inserter(data));
        set_unit_end(file);
    }

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

    void read_data(std::fstream &file)
    {
        file.read(&data[0], naxis(1)*naxis(2));
        set_unit_end(file);
    }

    std::unique_ptr<column> get_column(std::string name) const
    {
        for (auto col : col_metadata)
        {
            if (col.TTYPE() == name)
            {
                switch (get_type(col.TFORM()))
                {
                case 'A':
                {
                    auto result = std::make_unique<column_data<char>>();
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> char {
                            return *element;
                        }
                    );
                    return std::move(result);
                }
                case 'I':
                {
                    auto result = std::make_unique<column_data<std::int32_t>>();
                    fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                        [](char const* element) -> std::int32_t {
                            return boost::endian::big_to_native(*reinterpret_cast<const std::int32_t*>(element));
                        }
                    );
                    return std::move(result);
                }
                case 'F':
                {
                    auto result = std::make_unique<column_data<float>>();
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
                    auto result = std::make_unique<column_data<float>>();
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
                    auto result = std::make_unique<column_data<double>>();
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

        std::unique_ptr<column>(nullptr);
    }

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

    char get_type(std::string format) const
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });

        return form[0];
    }

private:
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

};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_ASCII_TABLE_HPP

