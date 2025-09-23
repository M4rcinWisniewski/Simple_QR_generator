#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"
#include "../include/Generator.h"
#include <string>
#include <vector>
#include <iostream>
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

    std::string add_terminator(std::string data) {
        int max_capacity = 152;
        int data_len = data.length();
        std::string new_data = data;
        int zeros_to_add = max_capacity - data_len;
        if (zeros_to_add > 4) { zeros_to_add = 4; }

        if (max_capacity > data_len) {
            for (size_t i = 0; i < zeros_to_add; i++) {
                new_data += "0";
            }
        }
        //aligns the dat to be full byte (divisible by 8)
        data_len += zeros_to_add;
        if (data_len % 8 != 0) {
            int align = (8 - (data_len % 8)) % 8;
            for (size_t i = 0; i < align; i++) {
                new_data += "0";
            }
        }
        return new_data;
    }

    std::vector<std::vector<int>> fill_matrix(int n, int m) {
        std::vector<std::vector<int>> vec(n, std::vector<int>(m, 0));
        return vec;
    }

    void add_finder(std::vector<std::vector<int>> &matrix, int startRow, int startCol) {
        for (int r = 0; r < 7; r++) {
            for (int c = 0; c < 7; c++) {
                // Outer border
                if (r == 0 || r == 6 || c == 0 || c == 6)
                    matrix[startRow + r][startCol + c] = 1;
                // Inner 3x3 square
                else if (r >= 2 && r <= 4 && c >= 2 && c <= 4)
                    matrix[startRow + r][startCol + c] = 1;
                // else leave 0
            }
        }
    }

    bool is_reserved(int r, int c) {
        // Finder patterns
        if ((r <= 6 && c <= 6) ||           // top-left
            (r <= 6 && c >= 14) ||          // top-right
            (r >= 14 && c <= 6))            // bottom-left
            return true;

        // Timing patterns
        if (r == 6 || c == 6)
            return true;

        return false; // available for data
    }


    void add_code(std::vector<std::vector<int>> &matrix, std::string code) {
        //encoding code in zig-zag pattern from bottom-right corner with excluding reserved bits like finding patterns
        int bit_index = 0;

        for (int r = 20; r >= 0; r--) {                 
            for (int c = 20; c >= 0; c--) {             
                if (!is_reserved(r, c)) {
                    if (r == c || c == r - 1) {         
                        if (bit_index < code.size()) {
                            matrix[r][c] = code[bit_index++] - '0'; 
                        }
                    }
                }
            }
        }
    }

}

// Class methods
Generator::Generator(int val,char e) : value(val){};
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

    std::vector<std::string> groups = sliced_string(string_val);
    for (const auto& grp : groups) {
        int num_digits = grp.length();
        int bits_needed = (num_digits == 3) ? 10 : (num_digits == 2) ? 7 : 4;
        int val = std::stoi(grp);

        // Convert to binary string
        std::string bin = to_bin(val);

        // Pad on the left to reach exact bit length
        if (bin.length() < bits_needed) {
            bin = add_padding(bin, bits_needed);
        }

        result += bin;
    }

    return result;
}



std::vector<std::vector<int>> Generator::qr_data() {
    std::string code = add_terminator(mode_indicator() + CCI() + encode_message());
    std::vector<std::vector<int>> matrix(21, std::vector<int>(21, 0));

    // Add finders
    add_finder(matrix, 0, 0);     // top-left
    add_finder(matrix, 0, 14);    // top-right
    add_finder(matrix, 14, 0);    // bottom-left


    // top-left
    for (int r = -1; r <= 7; r++) {
        if (r >= 0 && r <= 6) matrix[r][7] = 0;
        if (r >= 0 && r <= 6) matrix[7][r] = 0;
    }
    // top-right
    for (int r = -1; r <= 7; r++) {
        if (r >= 0 && r <= 6) matrix[r][13] = 0;
        if (r >= 0 && r <= 6) matrix[7][14 + r] = 0;
    }
    // bottom-left
    for (int c = -1; c <= 7; c++) {
        if (c >= 0 && c <= 6) matrix[13][c] = 0;
        if (c >= 0 && c <= 6) matrix[14 + c][7] = 0;
    }

    for (int i = 8; i < 13; i++) {
        matrix[6][i] = i % 2 ? 0 : 1;   // horizontal
        matrix[i][6] = i % 2 ? 0 : 1;   // vertical
    }
    std::cout << code << '\n';
    add_code(matrix, code);
    return matrix;
}

void Generator::save_as_img(std::vector<std::vector<int>> matrix) const {
    int width = matrix.size();
    int height = matrix[0].size();
    std::vector<unsigned char> pixels(width * height * 3);

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x){
            unsigned char color = matrix[y][x] ? 0 : 255;
            pixels[3*(y*width + x) + 0] = color; // R
            pixels[3*(y*width + x) + 1] = color; // G
            pixels[3*(y*width + x) + 2] = color; // B
        }
    }

    stbi_write_png("qr.png", width, height, 3, pixels.data(), width*3);
}