/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE data_conversions_test

#include<limits>
#include<complex>
#include<boost/test/unit_test.hpp>
#include<boost/astronomy/io/data_conversions.h>



class data_conversion_fixture {

public:

    template<typename OriginalType, typename AssumeType = OriginalType>
   static OriginalType reverse_endianess(OriginalType data) {

        AssumeType temp;
        std::memcpy(&temp, &data, sizeof(data));
        temp = boost::endian::endian_reverse(temp);
        std::memcpy(&data, &temp, sizeof(data));
        return data;
    }

    template<typename OriginalType, typename AssumeType = OriginalType>
    static std::vector<OriginalType> reverse_endianess(std::vector<OriginalType>& data) {
        std::vector<OriginalType> endianess_changed_data;
        endianess_changed_data.resize(data.size());
        std::transform(data.begin(), data.end(), endianess_changed_data.begin(), [](OriginalType element) { return reverse_endianess<OriginalType, AssumeType>(element); });
        return endianess_changed_data;
    }




    template<typename OriginalType, typename AssumeType = OriginalType>
    static std::complex<OriginalType> reverse_endianess(std::complex<OriginalType> data) {

        OriginalType o_real_part = data.real();
        OriginalType o_imag_part = data.imag();

        AssumeType a_real_part;
        AssumeType a_imag_part;

        std::memcpy(&a_real_part, &o_real_part, sizeof(o_real_part));
        std::memcpy(&a_imag_part, &o_imag_part, sizeof(o_imag_part));

        a_real_part = boost::endian::endian_reverse(a_real_part);
        a_imag_part = boost::endian::endian_reverse(a_imag_part);

        std::memcpy(&o_real_part, &a_real_part, sizeof(o_real_part));
        std::memcpy(&o_imag_part, &a_imag_part, sizeof(o_imag_part));

        return std::complex<OriginalType>(o_real_part, o_imag_part);
    }

    template<typename OriginalType, typename AssumeType = OriginalType>
    static std::vector<std::complex<OriginalType>> reverse_endianess(std::vector<std::complex<OriginalType>>& data) {
        std::vector<std::complex<OriginalType>> endianess_changed_data;
        endianess_changed_data.resize(data.size());
        std::transform(data.begin(), data.end(), endianess_changed_data.begin(), [](std::complex<OriginalType> element) { return reverse_endianess<OriginalType, AssumeType>(element); });
        return endianess_changed_data;
    }

    template<typename T>
    static std::string serialize(T data) {

        char* start_pos = reinterpret_cast<char*>(&data);
        char* end_pos = reinterpret_cast<char*>(start_pos + sizeof(T));
        return std::string(start_pos, end_pos);

    }
    template<typename T>
    static std::string serialize(std::complex<T>data) {
        T real_part = data.real();
        T imag_part = data.imag();
        return serialize(real_part) + serialize(imag_part);
    }

    template<typename T>
    static std::string serialize(std::vector<T> data) {

        std::string serialized_data = "";
        for (auto element : data) {
            serialized_data += serialize(element);
        }
        return serialized_data;
    }
};



using namespace boost::astronomy::io;
BOOST_AUTO_TEST_SUITE(single_element_parsing_methods)

BOOST_AUTO_TEST_SUITE(raw_serial_data_to_integer_variants)

BOOST_FIXTURE_TEST_CASE(raw_serial_data_to_boost_int8_t,data_conversion_fixture) {

    std::string serialized_data = serialize(reverse_endianess<boost::int8_t>(std::numeric_limits<boost::int8_t>::max()));

    boost::int8_t deserialized_data = data_conversions::element_to_numeric<boost::int8_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, std::numeric_limits<boost::int8_t>::max());
}

BOOST_FIXTURE_TEST_CASE(raw_serial_data_to_boost_int16_t, data_conversion_fixture) {

    std::string serialized_data = serialize(reverse_endianess<boost::int16_t>(std::numeric_limits<boost::int16_t>::max()));

    boost::int16_t deserialized_data = data_conversions::element_to_numeric<boost::int16_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, std::numeric_limits<boost::int16_t>::max());
}

BOOST_FIXTURE_TEST_CASE(raw_serial_data_to_boost_int32_t, data_conversion_fixture) {

    std::string serialized_data = serialize(reverse_endianess<boost::int32_t>(std::numeric_limits<boost::int32_t>::max()));

    boost::int32_t deserialized_data = data_conversions::element_to_numeric<boost::int32_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, std::numeric_limits<boost::int32_t>::max());
}

BOOST_FIXTURE_TEST_CASE(raw_serial_data_to_boost_int64_t, data_conversion_fixture) {

    std::string serialized_data = serialize(reverse_endianess<boost::int64_t>(std::numeric_limits<boost::int64_t>::max()));

    boost::int64_t deserialized_data = data_conversions::element_to_numeric<boost::int64_t>(serialized_data);
 
    BOOST_REQUIRE_EQUAL(deserialized_data, std::numeric_limits<boost::int64_t>::max());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(raw_serial_data_to_floating_variants)

BOOST_FIXTURE_TEST_CASE(raw_serial_data_to_boost_float32_t, data_conversion_fixture) {

    std::string serialized_data = serialize(reverse_endianess<boost::float32_t,boost::int32_t>(std::numeric_limits<boost::float32_t>::max()));

    boost::float32_t deserialized_data = data_conversions::element_to_numeric<boost::float32_t, boost::int32_t>(serialized_data);

    BOOST_REQUIRE_CLOSE(deserialized_data, std::numeric_limits<boost::float32_t>::max(),0.001);
}

BOOST_FIXTURE_TEST_CASE(raw_serial_data_to_boost_float64_t, data_conversion_fixture) {

    std::string serialized_data = serialize(reverse_endianess<boost::float64_t,boost::int64_t>(std::numeric_limits<boost::float64_t>::max()));

    boost::float64_t deserialized_data = data_conversions::element_to_numeric<boost::float64_t, boost::int64_t>(serialized_data);

    BOOST_REQUIRE_CLOSE(deserialized_data, std::numeric_limits<boost::float64_t>::max(),0.001);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(raw_serial_data_to_complex_integer_variants)

BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_8, data_conversion_fixture) {

    std::complex<boost::int8_t> test_data(std::numeric_limits<boost::int8_t>::max(), std::numeric_limits<boost::int8_t>::max());
    std::string serialized_data = serialize(reverse_endianess<boost::int8_t>(test_data));

    std::complex<boost::int8_t> deserialized_data = data_conversions::element_to_complex<boost::int8_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, test_data);
}
BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_16, data_conversion_fixture) {

    std::complex<boost::int16_t> test_data(std::numeric_limits<boost::int16_t>::max(), std::numeric_limits<boost::int16_t>::max());
    std::string serialized_data = serialize(reverse_endianess<boost::int16_t>(test_data));

    std::complex<boost::int16_t> deserialized_data = data_conversions::element_to_complex<boost::int16_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, test_data);
}
BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_32, data_conversion_fixture) {

    std::complex<boost::int32_t> test_data(std::numeric_limits<boost::int32_t>::max(), std::numeric_limits<boost::int32_t>::max());
    std::string serialized_data = serialize(reverse_endianess<boost::int32_t>(test_data));

    std::complex<boost::int32_t> deserialized_data = data_conversions::element_to_complex<boost::int32_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, test_data);
}
BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_64, data_conversion_fixture) {

    std::complex<boost::int64_t> test_data(std::numeric_limits<boost::int64_t>::max(), std::numeric_limits<boost::int64_t>::max());
    std::string serialized_data = serialize(reverse_endianess<boost::int64_t>(test_data));

    std::complex<boost::int64_t> deserialized_data = data_conversions::element_to_complex<boost::int64_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, test_data);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(raw_serial_data_to_complex_floating_variants)

BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_float_32, data_conversion_fixture) {

    std::complex<boost::float32_t> test_data(std::numeric_limits<boost::float32_t>::max(), std::numeric_limits<boost::float32_t>::max());
    std::string serialized_data = serialize(reverse_endianess<boost::float32_t,boost::int32_t>(test_data));

    std::complex<boost::float32_t> deserialized_data = data_conversions::element_to_complex<boost::float32_t,boost::int32_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, test_data);
}
BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_float_64, data_conversion_fixture) {

    std::complex<boost::float64_t> test_data(std::numeric_limits<boost::float64_t>::max(), std::numeric_limits<boost::float64_t>::max());
    std::string serialized_data = serialize(reverse_endianess<boost::float64_t, boost::int64_t>(test_data));

    std::complex<boost::float64_t> deserialized_data = data_conversions::element_to_complex<boost::float64_t,boost::int64_t>(serialized_data);

    BOOST_REQUIRE_EQUAL(deserialized_data, test_data);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(raw_serial_data_to_byte)

BOOST_FIXTURE_TEST_CASE(serial_data_to_char, data_conversion_fixture) {
    // Well its just a direct cast so the code shown below is really a joke
    BOOST_REQUIRE_EQUAL(data_conversions::element_to_byte<char>("A"),65);
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_int8_t, data_conversion_fixture) {

    BOOST_REQUIRE_EQUAL(data_conversions::element_to_byte<boost::int8_t>("D"), 68);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(multi_element_parsing_methods)

BOOST_AUTO_TEST_SUITE(serial_data_to_integral_collections)
BOOST_FIXTURE_TEST_CASE(serial_data_to_int8_collection, data_conversion_fixture) {

    std::vector<boost::int8_t> sample_data{ 1,2,3,4,5,6,7,8,9 };
    std::string serialized_data = serialize(sample_data);

    std::vector<boost::int8_t> deserialized_data = data_conversions::elements_to_numeric_collection<boost::int8_t>(serialized_data,sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
                                  sample_data.begin(), sample_data.end());
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_int16_collection, data_conversion_fixture) {

    std::vector<boost::int16_t> sample_data{ 2,4,3,1,2,3,54,23,3 };
    std::string serialized_data = serialize(reverse_endianess(sample_data));

    std::vector<boost::int16_t> deserialized_data = data_conversions::elements_to_numeric_collection<boost::int16_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_int32_collection, data_conversion_fixture) {

    std::vector<boost::int32_t> sample_data{ 34324332,432,324,3244,324,421,33 };
    std::string serialized_data = serialize(reverse_endianess(sample_data));

    std::vector<boost::int32_t> deserialized_data = data_conversions::elements_to_numeric_collection<boost::int32_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_int64_collection, data_conversion_fixture) {

    std::vector<boost::int64_t> sample_data{ 2324423,343243,3243,4234,3523,45234,452,2434223,42355245224 };
    reverse_endianess(sample_data);
    std::string serialized_data = serialize(reverse_endianess(sample_data));

    std::vector<boost::int64_t> deserialized_data = data_conversions::elements_to_numeric_collection<boost::int64_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(serial_data_to_floating_collections)

BOOST_FIXTURE_TEST_CASE(serial_data_to_float32_collection, data_conversion_fixture) {

    std::vector<boost::float32_t> sample_data{ 34324.2,4.32,3.24,324.4,32.4,4.21,3.3 };
    std::string serialized_data = serialize(reverse_endianess<boost::float32_t,boost::int32_t>(sample_data));

    std::vector<boost::float32_t> deserialized_data = data_conversions::elements_to_numeric_collection<boost::float32_t,boost::int32_t>(serialized_data, sample_data.size());

    for (std::size_t i = 0; i < sample_data.size(); i++) {
        BOOST_REQUIRE_CLOSE(sample_data[i], deserialized_data[i],0.001);
    }
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_float64_collection, data_conversion_fixture) {

    std::vector<boost::float64_t> sample_data{ 3433232324.2232,423.3223,323.23224,32234.4,233232.4,234.2321,3222.3 };
    std::string serialized_data = serialize(reverse_endianess<boost::float64_t, boost::int64_t>(sample_data));

    std::vector<boost::float64_t> deserialized_data = data_conversions::elements_to_numeric_collection<boost::float64_t, boost::int64_t>(serialized_data, sample_data.size());

    for (std::size_t i = 0; i < sample_data.size(); i++) {
        BOOST_REQUIRE_CLOSE(sample_data[i], deserialized_data[i], 0.001);
    }
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(serial_data_to_complex_integral_conversions)
BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_int_8_collection, data_conversion_fixture) {

    std::vector<std::complex<boost::int8_t>> sample_data;
    sample_data.push_back(std::complex<boost::int8_t>(1,2));
    sample_data.push_back(std::complex<boost::int8_t>(3, 4));
    sample_data.push_back(std::complex<boost::int8_t>(5, 6));
   
    std::string serialized_data = serialize(reverse_endianess <boost::int8_t>(sample_data));

    std::vector<std::complex<boost::int8_t>> deserialized_data = data_conversions::elements_to_complex_collection<boost::int8_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_int_16_collection, data_conversion_fixture) {

    std::vector<std::complex<boost::int16_t>> sample_data;
    sample_data.push_back(std::complex<boost::int16_t>(1232,232));
    sample_data.push_back(std::complex<boost::int16_t>(3233, 4234));
    sample_data.push_back(std::complex<boost::int16_t>(5233, 6233));

    std::string serialized_data = serialize(reverse_endianess<boost::int16_t>(sample_data));

    std::vector<std::complex<boost::int16_t>> deserialized_data = data_conversions::elements_to_complex_collection<boost::int16_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_int_32_collection, data_conversion_fixture) {

    std::vector<std::complex<boost::int32_t>> sample_data;
    sample_data.push_back(std::complex<boost::int32_t>(13334, 2324));
    sample_data.push_back(std::complex<boost::int32_t>(33224, 24344));
    sample_data.push_back(std::complex<boost::int32_t>(52131, 42312));

    std::string serialized_data = serialize(reverse_endianess <boost::int32_t>(sample_data));

    std::vector<std::complex<boost::int32_t>> deserialized_data = data_conversions::elements_to_complex_collection<boost::int32_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_int_64_collection, data_conversion_fixture) {

    std::vector<std::complex<boost::int64_t>> sample_data;
    sample_data.push_back(std::complex<boost::int64_t>(14233434, 2342344234322));
    sample_data.push_back(std::complex<boost::int64_t>(312321422, 423432432423));
    sample_data.push_back(std::complex<boost::int64_t>(324234235, 43253452546));

    std::string serialized_data = serialize(reverse_endianess <boost::int64_t>(sample_data));

    std::vector<std::complex<boost::int64_t>> deserialized_data = data_conversions::elements_to_complex_collection<boost::int64_t>(serialized_data, sample_data.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(),
        sample_data.begin(), sample_data.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(serial_data_to_floating_number_conversions)
BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_float32_collection, data_conversion_fixture) {

    std::vector<std::complex<boost::float32_t>> sample_data;
    sample_data.push_back(std::complex<boost::float32_t>(1333.2434f, 2.324234423f));
    sample_data.push_back(std::complex<boost::float32_t>(332432.4f, 23434.4f));
    sample_data.push_back(std::complex<boost::float32_t>(52.13123f, 42.1312312336f));

    std::string serialized_data = serialize(reverse_endianess<boost::float32_t, boost::int32_t>(sample_data));

    std::vector<std::complex<boost::float32_t>> deserialized_data = data_conversions::elements_to_complex_collection<boost::float32_t, boost::int32_t>(serialized_data, sample_data.size());

    for (std::size_t i = 0; i < sample_data.size(); i++) {
        BOOST_REQUIRE_CLOSE(deserialized_data[i].real(), sample_data[i].real(), 0.001);
        BOOST_REQUIRE_CLOSE(deserialized_data[i].imag(), sample_data[i].imag(), 0.001);
    }
}

BOOST_FIXTURE_TEST_CASE(serial_data_to_complex_float64_collection, data_conversion_fixture) {

    std::vector<std::complex<boost::float64_t>> sample_data;
    sample_data.push_back(std::complex<boost::float64_t>(1333.24343434, 34432.324234423));
    sample_data.push_back(std::complex<boost::float64_t>(332432.43434,3.23443344343443));
    sample_data.push_back(std::complex<boost::float64_t>(52.131233434, 42.131231233634));

    std::string serialized_data = serialize(reverse_endianess<boost::float64_t, boost::int64_t>(sample_data));

    std::vector<std::complex<boost::float64_t>> deserialized_data = data_conversions::elements_to_complex_collection<boost::float64_t, boost::int64_t>(serialized_data, sample_data.size());

    for (std::size_t i = 0; i < sample_data.size(); i++) {
        BOOST_REQUIRE_CLOSE(deserialized_data[i].real(), sample_data[i].real(), 0.001);
        BOOST_REQUIRE_CLOSE(deserialized_data[i].imag(), sample_data[i].imag(), 0.001);
    }
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(serial_data_to_byte_collection)

BOOST_FIXTURE_TEST_CASE(serial_data_to_int8_t_collection,data_conversion_fixture) {

    std::vector<boost::int8_t> sample_data{ 1,2,3,4 };
    std::string serialized_data = serialize(reverse_endianess(sample_data));

    std::vector<boost::int8_t> deserialized_data = data_conversions::elements_to_byte_collection<boost::int8_t>(serialized_data, sample_data.size());

    BOOST_REQUIRE_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(), sample_data.begin(), sample_data.end());

}

BOOST_FIXTURE_TEST_CASE(serial_data_to_char_t_collection, data_conversion_fixture) {

    std::vector<char> sample_data{ 2,23,32,41 };
    std::string serialized_data = serialize(reverse_endianess(sample_data));

    std::vector<char> deserialized_data = data_conversions::elements_to_byte_collection<char>(serialized_data, sample_data.size());

    BOOST_REQUIRE_EQUAL_COLLECTIONS(deserialized_data.begin(), deserialized_data.end(), sample_data.begin(), sample_data.end());
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
