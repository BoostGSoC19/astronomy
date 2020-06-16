
#ifndef BOOST_ASTRONOMY_IO_STREAM_READER_HPP
#define BOOST_ASTRONOMY_IO_STREAM_READER_HPP

#include<fstream>
#include<boost/astronomy/exception/fits_exception.hpp>

namespace boost {namespace astronomy {namespace io {
    class fits_stream_reader {

        std::fstream* file;

    public:
        fits_stream_reader():file(new std::fstream) {}
        fits_stream_reader(fits_stream_reader && other):file(other.file) {
            other.file = nullptr;
        }
        
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

        bool is_open() {
            return file->is_open();
        }
        void set_reading_pos(std::size_t position) {
            this->file->seekg(position);
        }
        unsigned char get() {

            unsigned char byte = static_cast<unsigned char>(this->file->get());
            if (!this->file->eof()) { return byte; }
            return 0;
        }
        std::size_t get_current_pos() {
            return this->file->tellg();
        }
        std::string read(std::size_t num_bytes) {
            std::string data(num_bytes, ' ');
            file->read(&data[0], num_bytes);
            return data;
        }
        bool at_end() {

            return (file->peek(), file->eof());
        }
        std::fstream& get_internal_stream() { return *file; }

        void set_unit_end() {
            std::size_t current_pos = file->tellg();
            std::size_t logical_record_size = 2880;

            std::size_t offset = logical_record_size - (current_pos % logical_record_size);
            std::size_t newpos = (current_pos + offset);

            set_reading_pos(newpos);
        }
        ~fits_stream_reader() {

            if (file != nullptr && file->is_open()) { file->close(); }
            delete file;
        }
    private:
        void set_exception_flags() {
            this->file->exceptions(std::fstream::failbit | std::fstream::badbit);
        }
    };

        }
    }
}

#endif
