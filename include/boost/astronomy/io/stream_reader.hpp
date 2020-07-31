/*=============================================================================
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_STREAM_READER_HPP
#define BOOST_ASTRONOMY_IO_STREAM_READER_HPP

#include<fstream>
#include<boost/astronomy/exception/fits_exception.hpp>

namespace boost {namespace astronomy {namespace io {

    /**
    * @brief This class provides convenience functions for reading fits file data
    * @note  file is a pointer in this class due to a bug in GCC 4.9 which does not
    *        implement move constructor for fstream class
    */
    class fits_stream_reader {

        std::fstream* file;

    public:
        /**
         * @brief Constructs a new FITS stream reader object

        */
        fits_stream_reader():file(new std::fstream) {}

        /**
         * @brief Default copy constructor
        */
        fits_stream_reader(fits_stream_reader && other):file(other.file) {
            other.file = nullptr;
        }

        /**
         * @brief Sets the file on which reading operations will take place
         * @param[in] path Location of the file
         * @throw file_reading_exception
        */
        void set_file(const std::string& path) {
            file->close();
            file->clear();
            this->file->exceptions(std::fstream::failbit | std::fstream::badbit);
            try {
                this->file->open(path, std::ios::binary | std::ios::in);
            }
            catch (std::fstream::failure& fstream_faliure) {
                throw file_reading_exception(fstream_faliure.what());
            }
        }

        /**
         * @brief Used to check whether the file is open or not
        */
        bool is_open() {
            return file->is_open();
        }

        /**
         * @brief Sets the file pointer to position for reading
         * @param[in] position The position to be set for reading
        */
        void set_reading_pos(std::size_t position) {
            this->file->seekg(position);
        }

        /**
         * @brief Get the value at current position of file pointer as a character
        */
        unsigned char get() {

            unsigned char byte = static_cast<unsigned char>(this->file->get());
            if (!this->file->eof()) { return byte; }
            return 0;
        }

        /**
         * @brief Gets the current position of file pointer
        */
        std::size_t get_current_pos() {
            return this->file->tellg();
        }

        /**
         * @brief Reads num_bytes from the file from current position as a string
         * @param[in] num_bytes Amount of data to be read in bytes
        */
        std::string read(std::size_t num_bytes) {
            std::string data(num_bytes, ' ');
            file->read(&data[0], num_bytes);
            return data;
        }

        /**
         * @brief Checks whether we are at the end of file
        */
        bool at_end() {

            return (file->peek(), file->eof());
        }

        /**
         * @brief Returns the internal file stream 
        */
        std::fstream& get_internal_stream() { return *file; }

        /**
         * @brief Sets the file pointer to the end of current logical record ( or beginning of next record )
        */
        void set_unit_end() {
            std::size_t current_pos = file->tellg();
            std::size_t logical_record_size = 2880;

            std::size_t offset = logical_record_size - (current_pos % logical_record_size);
            std::size_t newpos = (current_pos + offset);

            set_reading_pos(newpos);
        }

        /**
         * @brief Closes the file and Destroys the internal filestream
        */
        ~fits_stream_reader() {

            if (file != nullptr && file->is_open()) { file->close(); }
            delete file;
        }
    private:

        /**
         * @brief Sets the file exception flag to raise exception if file could not be read
        */
        void set_exception_flags() {
            this->file->exceptions(std::fstream::failbit | std::fstream::badbit);
        }
    };

        }
    }
}

#endif
