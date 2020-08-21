/*=============================================================================
Copyright 2020 Pranam Lashkari <plashkari628@gmail.com>
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
    /**
     * @brief Stores cache related information for every HDU to control and improve performance
     * @author Gopi Krishna Menon
    */
    struct control_block {
        std::string filepath;
        struct info {
            std::size_t header_location;
            std::size_t data_location;
            std::size_t hdu_index;
            bool read_status;
            info() = default;
            info(std::size_t header_loc, std::size_t data_loc, std::size_t index, std::size_t status) :header_location(header_loc),data_location(data_loc), hdu_index(index), read_status(status) {}
        };
        std::map<std::string, info> hdus_info;

        /**
         * @brief Returns the total number of HDU's present in file
        */
        std::size_t total_hdus() {
            return hdus_info.size();
        }

        /**
         * @brief Clears the control block object
        */
        void clear() {
            filepath.clear();
            hdus_info.clear();

        }
    };

    /**
     * @brief This class provides services for accessing and manipulating different HDU objects
     * @tparam FileReader Represents the reader class for reading related operations
     * @tparam ExtensionsSupported Contains the list of extensions along with their construction methods   
    */
    template<typename FileReader, typename ExtensionsSupported>
    struct fits_io {
    private:
        FileReader file_reader;
        std::vector<typename ExtensionsSupported::Extension> hdu_list;
        control_block hdus_control_block;
    public:
        /**
         * @brief Creates a default object of fits_reader
        */
        fits_io() {}

        /**
         * @brief Creates a fits_reader object and initializes it with the location of the file
         * @param[in] filepath Location of file
        */
        fits_io(const std::string& filepath) {
            initialize(filepath);
        }

        /**
         * @brief Initializes the file_reader and hdu control block
         * @param[in] filepath Location of file
        */
        void initialize(const std::string& filepath) {
            file_reader.set_file(filepath);
            hdus_control_block.clear();
            hdus_control_block.filepath = filepath;
        }

        /**
         * @brief Reads only the HDU's header portion and caches the header location and data location
        */
        void read_only_headers() {
            while (!file_reader.at_end()) {
                auto hdu_position = file_reader.get_current_pos();

                typename ExtensionsSupported::header_type hdu_header = extract_header();

                hdus_control_block.hdus_info[hdu_header.get_hdu_name()] = control_block::info(hdu_position,file_reader.get_current_pos(), hdu_list.size(), false);

                auto hdu_instance = ExtensionsSupported::construct_hdu(hdu_header, "");

                hdu_list.push_back(hdu_instance);

                if (hdu_header.data_size() != 0) {

                    auto total_elements = hdu_header.data_size();
                    auto element_size = get_element_size_from_bitpix(hdu_header.bitpix());
                    file_reader.set_reading_pos(file_reader.get_current_pos() + (total_elements*element_size));
                    file_reader.set_unit_end();
                }
            }
        }


        /**
         * @brief Reads both the HDU's header portion and data portion
         * @note  Similar to the read_only_headers() method this method also caches the hdu information
        */
        void read_entire_hdus() {
            while (!file_reader.at_end()) {
                auto header_loc = file_reader.get_current_pos();
                typename ExtensionsSupported::header_type hdu_header = extract_header();
                auto data_loc = file_reader.get_current_pos();
                std::string hdu_data = extract_data_buffer(hdu_header);
                hdus_control_block.hdus_info[hdu_header.get_hdu_name()] = control_block::info(header_loc,data_loc, hdu_list.size(), false);
                auto hdu_instance = ExtensionsSupported::construct_hdu(hdu_header, hdu_data);
                hdu_list.push_back(hdu_instance);
            }
        }

        /**
         * @brief Writes all the HDU's Header and Data information to the file
         * @param[in] file_path Path where the file resides
         * @note The HDUs are written in the order they are stored
        */
        void write_to(const std::string& file_path) {
            FileReader file_writer;
            file_writer.create_file(file_path);
            typename ExtensionsSupported::template writer_visitor<FileReader> writer_visitor(file_writer);

            for (auto& hdu : hdu_list) {
            boost::apply_visitor(writer_visitor, hdu);
            }
        }




        /**
         * @brief Returns the HDU at given index
        */
        typename ExtensionsSupported::Extension& operator [](int index) {
            return hdu_list.at(index);
        }

        /**
         * @brief Returns the HDU based on the hdu_name 
        */
        typename ExtensionsSupported::Extension& operator [](const std::string& hdu_name) {
            return hdu_list[hdus_control_block.hdus_info.at(hdu_name).hdu_index];
        }

        /**
         * @brief Returns the list of hdu objects associated with a FITS file
        */
        std::vector<typename ExtensionsSupported::Extension> get_hdu_list() {
            return hdu_list;
        }

        /**
         * @brief Returns the control block containing the hdu cache information
        */
        control_block get_control_block_info() {
            return hdus_control_block;
        }


    private:
        /**
         * @brief Extracts the header from the FITS file
        */
        typename ExtensionsSupported::header_type extract_header() {
            typename ExtensionsSupported::header_type hdu_header;
            hdu_header.read_header(file_reader);
            file_reader.set_unit_end();
            return hdu_header;
        }

        /**
         * @brief Extracts the data associated with a perticular HDU from the FITS file
         * @param[in] hdu_header Header information related to the current HDU
        */
        std::string extract_data_buffer(typename ExtensionsSupported::header_type& hdu_header) {
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
