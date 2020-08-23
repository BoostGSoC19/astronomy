
#include<map>
#include<boost/astronomy/io/header.hpp>
#include<boost/astronomy/io/default_card_policy.hpp>


 namespace fits_test {

using namespace boost::astronomy::io;
template<typename CardPolicy>
struct hdu_store {
    header<CardPolicy> hdu_header;
    std::string hdu_data_buffer;
    hdu_store() {}
    hdu_store(const header<CardPolicy>& head, const std::string& data_buffer)
        :hdu_header(head), hdu_data_buffer(data_buffer) {}   
};

template<typename FileReader,typename CardPolicy=card_policy>
class base_fixture {
    typedef std::map<std::string, hdu_store<CardPolicy>> hdus_information;
    FileReader file_reader;
    std::map<std::string, hdus_information> samples;
protected:
    std::string samples_directory;
public:

    std::string get_samples_directory() { return samples_directory; }

    void load_file(const std::string& filename) {
        file_reader.set_file(samples_directory+filename);
        read_hdus(get_sample_name_from_filename(filename));
    }
    std::string get_absolute_path(const std::string& sample_name) {
        return samples_directory + sample_name;
    }
    hdu_store<CardPolicy>* get_raw_hdu(const std::string& sample_name, const std::string& hdu_name) {
        auto sample_pos = samples.find(sample_name);
        if (sample_pos != samples.end()) {
            auto hdu_pos = sample_pos->second.find(hdu_name);
            if (hdu_pos != sample_pos->second.end()) {
                return &hdu_pos->second;
            }
        }
        return nullptr;
    }
private:
    std::string get_sample_name_from_filename(const std::string& file_path) {
        size_t lastindex = file_path.find_last_of(".");
        std::string rawname = file_path.substr(0, lastindex);
        return rawname;
    }
    void read_hdus(const std::string& sample_name) {
        while (!file_reader.at_end()) {
            header<CardPolicy> hdu_header = extract_header();
            std::string hdu_data = extract_data_buffer(hdu_header);
            samples[sample_name][hdu_header.get_hdu_name()] = hdu_store<CardPolicy>(hdu_header, hdu_data);
        }
    }
    header<CardPolicy> extract_header() {
        header<CardPolicy> hdu_header;
        hdu_header.read_header(file_reader);
        file_reader.set_unit_end();
        return hdu_header;
    }
    std::string extract_data_buffer(header<CardPolicy>& hdu_header) {
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
} 
