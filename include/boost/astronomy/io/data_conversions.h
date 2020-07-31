#ifndef BOOST_ASTRONOMY_IO_DATA_CONVERSIONS_HPP
#define BOOST_ASTRONOMY_IO_DATA_CONVERSIONS_HPP

#include<vector>
#include<string>
#include<cstring>
#include<complex>
#include<boost/cstdint.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/cstdfloat.hpp>
#include<boost/endian/conversion.hpp>
#include<boost/astronomy/io/column.hpp>
#include<boost/lexical_cast.hpp>


namespace boost{namespace astronomy{namespace io{
    class data_conversions {

    public:
        template <typename NumericType, typename AssumeType = NumericType>
        static NumericType element_to_numeric(const std::string& element) {
            AssumeType temp = boost::endian::big_to_native(
                *reinterpret_cast<const AssumeType*>(element.c_str()));

            NumericType value;
            std::memcpy(&value, &temp, sizeof(NumericType));
            return value;
        }

        template <typename NumericType, typename AssumeType = NumericType>
        static std::vector<NumericType> elements_to_numeric_collection(
            const std::string& elements, std::size_t no_elements) {
            std::vector<NumericType> values;
            values.resize(no_elements);

            auto start = reinterpret_cast<const NumericType*>(elements.c_str());
            auto end_s = reinterpret_cast<const NumericType*>(elements.c_str()) + no_elements;

            std::transform(start, end_s, values.begin(), [](NumericType element) {
                AssumeType temp;
                // Do not reinterpret cast here, strict aliasing rules will break
                std::memcpy(&temp, &element, sizeof(AssumeType));
                temp = boost::endian::big_to_native(temp);

                NumericType value;
                std::memcpy(&value, &temp, sizeof(NumericType));
                return value;
                });

            return values;
        }

        template <typename ComplexType, typename AssumeType = ComplexType>
        static std::complex<ComplexType> element_to_complex(const std::string& element) {
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
        static std::vector<std::complex<ComplexType>> elements_to_complex_collection(
            const std::string& elements, std::size_t no_elements) {

            std::vector<std::complex<ComplexType>> values;
            values.reserve(no_elements);

            const char* starting_offset = elements.c_str();
            const char* ending_offset = starting_offset + sizeof(ComplexType) * 2;

            for (std::size_t i = 0; i < no_elements; i++) {

                std::string raw_complex_no(starting_offset, ending_offset);
                starting_offset = ending_offset;
                ending_offset = starting_offset + sizeof(ComplexType) * 2;
                values.push_back(element_to_complex<ComplexType, AssumeType>(raw_complex_no));
            }
            return values;
        }
        template <typename ByteType>
        static ByteType element_to_byte(const std::string& element) {
            return boost::lexical_cast<ByteType>(element);
        }

        template <typename ByteType>
        static std::vector<ByteType> elements_to_byte_collection(const std::string& elements,
            std::size_t no_elements) {
            return std::vector<ByteType>(elements.begin(),
                elements.begin() + no_elements);
        }

        static std::size_t element_count(std::string format) {
            std::string form = boost::trim_copy_if(format, [](char c) -> bool {
                return c == '\'' || c == ' ';
                });

            return form.length() > 1 ?
                boost::lexical_cast<std::size_t>(form.substr(0, form.length() - 1)) :
                static_cast<std::size_t>(1);
        }

        template<typename T>
        static T convert(const std::string&, const column&) {
            return T();
        }
    };

    template<>
    bool data_conversions::convert(const std::string& element, const column&) {
        return element[0] == 'T';
    }
    template<>
    std::vector<bool> data_conversions::convert(const std::string& elements, const column&) {
        std::vector<bool> values;
        for (auto element : elements) {
            values.emplace_back(element == 'T');
        }
        return values;
    }
    template<>
    boost::int16_t data_conversions::convert(const std::string& element, const column&) {
        return element_to_numeric<boost::int16_t>(element);
    }

    template<>
    std::vector<boost::int16_t> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return elements_to_numeric_collection<boost::int16_t>(
            elements, element_count(column_metadata.TFORM()));
    }

    template<>
    boost::int32_t data_conversions::convert(const std::string& element, const column&) {
        return element_to_numeric<boost::int32_t>(element);
    }
    template<>
    std::vector<boost::int32_t> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return data_conversions::elements_to_numeric_collection<boost::int32_t>(
            elements, element_count(column_metadata.TFORM()));
    }

    template<>
    boost::float32_t data_conversions::convert(const std::string& element, const column&) {
        return element_to_numeric<boost::float32_t, boost::int32_t>(element);
    }
    template<>
    std::vector<boost::float32_t> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return elements_to_numeric_collection<boost::float32_t, boost::int32_t>(
            elements, element_count(column_metadata.TFORM()));
    }
    template<>
    boost::float64_t data_conversions::convert(const std::string& element, const column&) {
        return data_conversions::element_to_numeric<boost::float64_t, boost::int64_t>(element);
    }
    template<>
    std::vector<boost::float64_t> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return data_conversions::elements_to_numeric_collection<boost::float64_t, boost::int64_t>(
            elements, element_count(column_metadata.TFORM()));
    }
    template<>
    std::pair<boost::int32_t, boost::int32_t> data_conversions::convert(const std::string& element, const column&) {
        auto x = boost::endian::big_to_native(
            *reinterpret_cast<const boost::int32_t*>(element.c_str()));
        auto y = boost::endian::big_to_native(
            *(reinterpret_cast<const boost::int32_t*>(element.c_str()) + 1));
        return std::make_pair(x, y);
    }
    template<>
    std::vector<std::pair<boost::int32_t, boost::int32_t>> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        std::vector<std::pair<boost::int32_t, boost::int32_t>> values;
        auto no_elements = element_count(column_metadata.TFORM());
        values.reserve(element_count(column_metadata.TFORM()));
        for (std::size_t i = 0; i < no_elements; i++) {
            values.emplace_back(
                boost::endian::big_to_native(
                    *(reinterpret_cast<const boost::int32_t*>(elements.c_str()) + i)),
                boost::endian::big_to_native(
                    *(reinterpret_cast<const boost::int32_t*>(elements.c_str()) + i +
                        1)));
        }
        return values;
    }
    template<>
    std::complex<boost::float32_t> data_conversions::convert(const std::string& element, const column&) {
        return element_to_complex<boost::float32_t, boost::int32_t>(element);
    }
    template<>
    std::vector<std::complex<boost::float32_t>> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return elements_to_complex_collection<boost::float32_t, boost::int32_t>(
            elements, element_count(column_metadata.TFORM()));
    }
    template<>
    std::complex<boost::float64_t>data_conversions::convert(const std::string& element, const column&) {
        return element_to_complex<boost::float64_t, boost::int64_t>(element);
    }
    template<>
    std::vector<std::complex<boost::float64_t>> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return elements_to_complex_collection<boost::float64_t, boost::int64_t>(
            elements, element_count(column_metadata.TFORM()));
    }
    template<>
    std::uint8_t data_conversions::convert(const std::string& element, const column&) {
        return element_to_byte<std::uint8_t>(element);
    }
    template<>
    std::vector<std::uint8_t> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return elements_to_byte_collection<std::uint8_t>(
            elements, element_count(column_metadata.TFORM()));
    }
    template<>
    char data_conversions::convert(const std::string& element, const column&) {
        return data_conversions::element_to_byte<char>(element);
    }
    template<>
    std::vector<char> data_conversions::convert(const std::string& elements, const column& column_metadata) {
        return data_conversions::elements_to_byte_collection<char>(
            elements, element_count(column_metadata.TFORM()));
    }
} } }

#endif
