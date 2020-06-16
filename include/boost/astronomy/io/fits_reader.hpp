/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_FITS_READER_HPP
#define BOOST_ASTRONOMY_IO_FITS_READER_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include <map>

#include <boost/astronomy/io/hdu.hpp>

namespace boost { namespace astronomy { namespace io {

    struct control_block {
        std::string filepath;
        struct info {
            std::size_t header_location;
            std::size_t data_location;
            std::size_t hdu_index;
            bool read_status;
            info() = default;
            info(std::size_t loc, std::size_t index, std::size_t status) :header_location(loc), hdu_index(index), read_status(status) {}
        };
        std::map<std::string, info> hdus_info;

        std::size_t total_hdus() {
            return hdus_info.size();
        }
        void clear() {
            filepath.clear();
            hdus_info.clear();

        }
    };

    template<typename FileReader, typename ExtensionsSupported>
    class fits_reader {
        FileReader file_reader;
        std::vector<typename ExtensionsSupported::Extension> hdu_list;
        control_block hdus_control_block;
    public:
        fits_reader() {}
        fits_reader(const std::string& filepath) {
            initialize(filepath);
        }
        void initialize(const std::string& filepath) {
            file_reader.set_file(filepath);
            hdus_control_block.clear();
            hdus_control_block.filepath = filepath;
        }
        void read_only_headers() {
            while (!file_reader.at_end()) {
                auto hdu_position = file_reader.get_current_pos();
                header hdu_header = extract_header();
                hdus_control_block.hdus_info[hdu_header.get_hdu_name()] = control_block::info(hdu_position, hdu_list.size(), false);
                auto hdu_instance = ExtensionsSupported::construct_hdu(hdu_header, "");
                hdu_list.push_back(hdu_instance);
                if (hdu_header.data_size() != 0) {
                    file_reader.set_reading_pos(file_reader.get_current_pos() + hdu_header.data_size());
                    file_reader.set_unit_end();
                }
            }
        }
        void read_entire_hdus() {
            while (!file_reader.at_end()) {
                auto hdu_position = file_reader.get_current_pos();
                header hdu_header = extract_header();
                std::string hdu_data = extract_data_buffer(hdu_header);
                hdus_control_block.hdus_info[hdu_header.get_hdu_name()] = control_block::info(hdu_position, hdu_list.size(), false);
                auto hdu_instance = ExtensionsSupported::construct_hdu(hdu_header, hdu_data);
                hdu_list.push_back(hdu_instance);
            }
        }
        typename ExtensionsSupported::Extension& operator [](int index) {
            return hdu_list.at(index);
        }
        typename ExtensionsSupported::Extension& operator [](const std::string& hdu_name) {
            return hdu_list[hdus_control_block.hdus_info.at(hdu_name).hdu_index];
        }
        std::vector<typename ExtensionsSupported::Extension> get_hdu_list() {
            return hdu_list;
        }
        control_block get_control_block_info() {
            return hdus_control_block;
        }


    private:
        header extract_header() {
            header hdu_header;
            hdu_header.read_header(file_reader);
            file_reader.set_unit_end();
            return hdu_header;
        }
        std::string extract_data_buffer(header& hdu_header) {
            if (hdu_header.data_size() == 0) {
                return "";
            }
            else {
                auto total_elements = hdu_header.data_size();
                auto element_size = get_element_size_from_bitpix(hdu_header.bitpix());
                std::string data_buffer = file_reader.read(total_elements * element_size);
                file_reader.set_unit_end();
                return data_buffer;
            }
        }
    };
}}}






#endif
