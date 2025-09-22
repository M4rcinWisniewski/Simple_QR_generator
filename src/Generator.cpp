#include "../include/Generator.h"
#include <string>
#include <vector>

constexpr int capacities_numeric_L[41] = {
    0,
    41, 77, 127, 187, 255, 322, 370, 461, 552, 652,
    772, 883, 1022, 1101, 1250, 1408, 1548, 1725, 1903, 2061,
    2232, 2409, 2620, 2812, 3057, 3283, 3517, 3669, 3909, 4158,
    4417, 4686, 4965, 5253, 5529, 5836, 6153, 6470, 6743, 7089
};

//Helper functions 
namespace {
    std::string to_bin(int n) { 
        if (n == 0) return "0"; 
        std::string result; 
        while (n > 0) { 
            std::string bit = std::to_string(n % 2); 
            result = bit + result; n /= 2; 
        } return result; 
    } 
    
    std::string add_padding(std::string string, int expected_length) { 
        int string_length = string.length(); 
        for (int i = 0; i < expected_length - string_length; i++) { 
            string = "0" + string; 
        } return string; 
    } 
    
    //character count indicator 
    int cci_width(const int mode, int version) { 
        if (version < 1 || version > 40) throw std::invalid_argument("version"); 
        if (mode == 1 ) return (version <= 9) ? 10 : (version <= 26) ? 12 : 14; // Numeric 
        if (mode == 2 ) return (version <= 9) ? 9 : (version <= 26) ? 11 : 13; // Alphanumeric 
        if (mode == 3 ) return (version <= 9) ? 8 : 16; // Byte 
        if (mode == 4 ) return (version <= 9) ? 8 : (version <= 26) ? 10 : 12; // Kanji 
        throw std::invalid_argument("mode"); 
    } 

    std::vector<std::string> sliced_string(std::string str) {
        std::vector<std::string> result;
        if (str.length() < 3) {
            result.push_back(str);
            return result;
        }
        std::string el = "";
        for (int i = 0; i < str.length(); i++) {
            
            if (i != 0 && i % 3 == 0) {
                result.push_back(el);
                el = "";
            }
            el.push_back(str[i]);
        }
        if (el.length() > 0) {
            result.push_back(el);
           
        }
        return result;
    }
}

// Class methods
Generator::Generator(int val,char e) : value(val), ecc(e) {};
int Generator::get_value() const { return value; } 
int Generator::get_version() const { return version; }


int Generator::determine_version(int num_digits) { 
    for (int N = 1; N <= 40; N++) { 
        if (num_digits <= capacities_numeric_L[N]) { 
            return N; 
        } 
    } return -1; // too big 
} 

std::string Generator::mode_indicator() {
    std::string indicators[4] = {"0001", "0010", "0100", "1000"};

    return indicators[mode - 1];

}

std::string Generator::CCI() { 
    int num_digits = std::to_string(value).length(); 
    version = determine_version(num_digits); 
    int width = cci_width(mode, version); 
    std::string cci; 
    if (num_digits < width) { 
        cci = add_padding(to_bin(num_digits), width); 
    } else{ 
        cci = to_bin(num_digits); 
    } 
    return cci; 
}


std::string Generator::encode_message() {
    std::string result;
    std::string string_val = std::to_string(value);
    int num_digits = string_val.length();
    int num_of_bits[3] = {4, 7, 10};

    std::vector sliced_val = sliced_string(string_val);
    for (size_t i = 0; i < sliced_val.size(); i++) {
        result += add_padding(to_bin(std::stoi(sliced_val[i])), num_of_bits[sliced_val[i].length() - 1]) + " "; 
    }
    return result;
} 