#ifndef BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
#define BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP

#include <fstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <complex>
#include <utility>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/endian/conversion.hpp>

#include <boost/astronomy/io/table_extension.hpp>
#include <boost/astronomy/io/column.hpp>
#include <boost/astronomy/io/column_data.hpp>

namespace boost { namespace astronomy { namespace io {

struct binary_table_extension : table_extension
{
public:
    binary_table_extension(std::fstream &file) : table_extension(file)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1)*naxis(2), std::back_inserter(data));
        set_unit_end(file);
    }

    binary_table_extension(std::fstream &file, hdu const& other) : table_extension(file, other)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1)*naxis(2), std::back_inserter(data));
        set_unit_end(file);
    }

    binary_table_extension(std::fstream &file, std::streampos pos) : table_extension(file, pos)
    {
        populate_column_data();
        std::copy_n(std::istream_iterator<char>(file), naxis(1)*naxis(2), std::back_inserter(data));
        set_unit_end(file);
    }

    void populate_column_data()
    {
        std::size_t start = 0;
        for (std::size_t i = 0; i < this->tfields; i++)
        {
            col_metadata[i].index(i + 1);

            col_metadata[i].TFORM(
                value_of<std::string>("TFORM" + boost::lexical_cast<std::string>(i + 1))
            );

            col_metadata[i].TBCOL(start);

            start += column_size(col_metadata[i].TFORM());

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

            try {
                col_metadata[i].TDISP(
                    value_of<std::string>("TDISP" + boost::lexical_cast<std::string>(i + 1))
                );
            }
            catch (std::out_of_range e) {/*Do Nothing*/ }

            try {
                col_metadata[i].TDIM(
                    value_of<std::string>("TDIM" + boost::lexical_cast<std::string>(i + 1))
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
                if (element_count(col.TFORM()) == 1)
                {
                    switch (get_type(col.TFORM()))
                    {
                    case 'L':
                    {
                        auto result = std::make_unique<column_data<bool>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> bool {
                                if (*element == 'T') return true;
                                else return false;
                            }
                        );
                        return std::move(result);
                    }
                    case 'X':
                    {
                        auto result = std::make_unique<column_data<char>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> char {
                                return *element;
                            }
                        );
                        return std::move(result);
                    }
                    case 'B':
                    {
                        auto result = std::make_unique<column_data<std::uint8_t>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> std::uint8_t {
                                return static_cast<std::uint8_t>(*element);
                            }
                        );
                        return std::move(result);
                    }
                    case 'I':
                    {
                        auto result = std::make_unique<column_data<std::int16_t>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> std::int16_t {
                                return boost::endian::big_to_native(*reinterpret_cast<const std::int16_t*>(element));
                            }
                        );
                        return std::move(result);
                    }
                    case 'J':
                    {
                        auto result = std::make_unique<column_data<std::int32_t>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> std::int32_t {
                                return boost::endian::big_to_native(*reinterpret_cast<const std::int32_t*>(element));
                            }
                        );
                        return std::move(result);
                    }
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
                    case 'C':
                    {
                        auto result = std::make_unique<column_data<std::complex<float>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> std::complex<float> {
                                std::complex<float> result;
                                float temp = (element[3] << 0) | (element[2] << 8) |
                                                (element[1] << 16) | (element[0] << 24);
                                result.real(temp);

                                element += 4;
                                temp = (element[3] << 0) | (element[2] << 8) |
                                        (element[1] << 16) | (element[0] << 24);
                                result.imag(temp);
                                return result;
                            }
                        );
                        return std::move(result);
                    }
                    case 'M':
                    {
                        auto result = std::make_unique<column_data<std::complex<double>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> std::complex<double> {
                            std::complex<double> result;
                            double temp = (element[7] << 0) | (element[6] << 8) |
                                            (element[5] << 16) | (element[4] << 24) |
                                            (element[3] << 32) | (element[2] << 40) |
                                            (element[1] << 48) | (element[0] << 56);
                            result.real(temp);

                            element += 8;
                            temp = (element[7] << 0) | (element[6] << 8) |
                                    (element[5] << 16) | (element[4] << 24) |
                                    (element[3] << 32) | (element[2] << 40) |
                                    (element[1] << 48) | (element[0] << 56);
                            result.imag(temp);
                            return result;
                            }
                        );
                        return std::move(result);
                    }
                    case 'P':
                    {
                        auto result = std::make_unique<column_data<std::pair<std::int32_t, std::int32_t>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [](char const* element) -> std::pair<std::int32_t, std::int32_t> {
                                auto x = boost::endian::big_to_native(*reinterpret_cast<const std::int32_t*>(element));
                                auto y = boost::endian::big_to_native(*(reinterpret_cast<const std::int32_t*>(element)+1));
                                return std::make_pair(x,y);
                            }
                        );
                        return std::move(result);
                    }
                    default:
                        throw invalid_table_colum_format();
                    }
                }
                else
                {
                    std::size_t num_of_element = element_count(col.TFORM());
                    switch (get_type(col.TFORM()))
                    {
                    case 'L':
                    {
                        auto result = std::make_unique<column_data<std::vector<bool>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<bool> {
                                std::vector<bool> values;
                                values.reserve(num_of_element);
                                for (std::size_t i = 0; i < num_of_element; i++)
                                {
                                    if (*element == 'T') values.emplace_back(true);
                                    else values.emplace_back(false);
                                }
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'X':
                    {
                        auto result = std::make_unique<column_data<std::vector<char>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<char> {
                                return std::vector<char>(element, element + num_of_element);
                            }
                        );
                        return std::move(result);
                    }
                    case 'B':
                    {
                        auto result = std::make_unique<column_data<std::vector<std::uint8_t>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<std::uint8_t> {
                                return std::vector<std::uint8_t>(
                                    reinterpret_cast<const std::uint8_t*>(element), 
                                    reinterpret_cast<const std::uint8_t*>(element)+ num_of_element
                                    );
                            }
                        );
                        return std::move(result);
                    }
                    case 'I':
                    {
                        auto result = std::make_unique<column_data<std::vector<std::int16_t>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<std::int16_t> {
                                std::vector<std::int16_t> values;
                                values.reserve(num_of_element);
                                for (std::size_t i = 0; i < num_of_element; i++)
                                {
                                    values.emplace_back(
                                        boost::endian::big_to_native(
                                            *(reinterpret_cast<const std::int16_t*>(element) +
                                             num_of_element
                                        )));
                                }
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'J':
                    {
                        auto result = std::make_unique<column_data<std::vector<std::int32_t>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<std::int32_t> {
                                std::vector<std::int32_t> values;
                                values.reserve(num_of_element);
                                for (std::size_t i = 0; i < num_of_element; i++)
                                {
                                    values.emplace_back(
                                        boost::endian::big_to_native(
                                            *(reinterpret_cast<const std::int32_t*>(element) +
                                             num_of_element
                                        )));
                                }
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'A':
                    {
                        auto result = std::make_unique<column_data<std::vector<char>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<char> {
                                return std::vector<char>(element, element + num_of_element);
                            }
                        );
                        return std::move(result);
                    }
                    case 'E':
                    {
                        auto result = std::make_unique<column_data<std::vector<float>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<float> {
                                std::vector<float> values(
                                    reinterpret_cast<const float*>(element), 
                                    reinterpret_cast<const float*>(element) + num_of_element
                                );

                                std::transform(
                                    values.begin(), 
                                    values.end(), 
                                    values.begin(), 
                                    [](float var) -> float{
                                        char *native = reinterpret_cast<char*>(&var);
                                        return (native[3] << 0) | (native[2] << 8) |
                                                (native[1] << 16) | (native[0] << 24);
                                    });
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'D':
                    {
                        auto result = std::make_unique<column_data<std::vector<double>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<double> {
                                std::vector<double> values(
                                    reinterpret_cast<const double*>(element), 
                                    reinterpret_cast<const double*>(element) + num_of_element
                                );

                                std::transform(
                                    values.begin(), 
                                    values.end(), 
                                    values.begin(), 
                                    [](double var) -> float{
                                        char *native = reinterpret_cast<char*>(&var);
                                        return (native[7] << 0) | (native[6] << 8) |
                                                (native[5] << 16) | (native[4] << 24) |
                                                (native[3] << 32) | (native[2] << 40) |
                                                (native[1] << 48) | (native[0] << 56);
                                    });
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'C':
                    {
                        auto result = std::make_unique<column_data<std::vector<std::complex<float>>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<std::complex<float>> {
                                std::vector<std::complex<float>> values;
                                values.reserve(num_of_element);
                                for (auto i = reinterpret_cast<const float*>(element); i != i + num_of_element*2; i+=2)
                                {
                                    char const* char_ptr = reinterpret_cast<const char*>(i);
                                    float real = (char_ptr[3] << 0) | (char_ptr[2] << 8) |
                                                (char_ptr[1] << 16) | (char_ptr[0] << 24);
                                    
                                    char_ptr += 4;
                                    float img = (char_ptr[3] << 0) | (char_ptr[2] << 8) |
                                                (char_ptr[1] << 16) | (char_ptr[0] << 24);

                                    values.emplace_back(real, img);
                                }
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'M':
                    {
                        auto result = std::make_unique<column_data<std::vector<std::complex<double>>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<std::complex<double>> {
                            std::vector<std::complex<double>> values;
                            values.reserve(num_of_element);
                            for (auto i = reinterpret_cast<const double*>(element); i != i + num_of_element*2; i+=2)
                                {
                                    char const* char_ptr = reinterpret_cast<const char*>(i);
                                    double real = (char_ptr[3] << 0) | (char_ptr[2] << 8) |
                                                (char_ptr[1] << 16) | (char_ptr[0] << 24);
                                    
                                    char_ptr += 4;
                                    double img = (char_ptr[3] << 0) | (char_ptr[2] << 8) |
                                                (char_ptr[1] << 16) | (char_ptr[0] << 24);

                                    values.emplace_back(real, img);
                                }
                            return values;
                            }
                        );
                        return std::move(result);
                    }
                    case 'P':
                    {
                        auto result = std::make_unique<column_data<std::vector<std::pair<std::int32_t, std::int32_t>>>>();
                        fill_column(result->get_data(), col.TBCOL(), column_size(col.TFORM()),
                            [num_of_element](char const* element) -> std::vector<std::pair<std::int32_t, std::int32_t>> {
                                std::vector<std::pair<std::int32_t, std::int32_t>> values;
                                values.reserve(num_of_element);
                                for (std::size_t i = 0; i < num_of_element; i++)
                                {
                                    values.emplace_back(
                                        boost::endian::big_to_native(
                                            *(reinterpret_cast<const std::int32_t*>(element) + i)),
                                        boost::endian::big_to_native(
                                            *(reinterpret_cast<const std::int32_t*>(element)+ i +1))
                                        );
                                }
                                return values;
                            }
                        );
                        return std::move(result);
                    }
                    default:
                        throw invalid_table_colum_format();
                    }
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

        return form.length() > 1 ?
            boost::lexical_cast<std::size_t>(form.substr(0, form.length() - 1)) *
                type_size(form[form.length() - 1]) :
            type_size(form[0]);
    }

    std::size_t element_count(std::string format) const
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });

        return form.length() > 1 ?
            boost::lexical_cast<std::size_t>(form.substr(0, form.length() - 1)) :
            static_cast<std::size_t>(1);
    }

    char get_type(std::string format) const
    {
        std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                            return c == '\'' || c == ' ';
                        });

        return form[form.length() - 1];
    }

    std::size_t type_size(char type) const
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
#endif // BOOST_ASTRONOMY_IO_BINARY_TABLE_HPP
