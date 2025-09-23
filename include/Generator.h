#include <string>
#include <vector>

class Generator {
    private: 
        int value;
        int version = 1;
        int mode = 1;
    public:
        // Constructor 
        Generator(int val, char ecc = 'L');

        std::string CCI();
        std::string mode_indicator();
        std::string encode_message();
        std::vector<std::vector<int>> qr_data();
        void save_as_img(std::vector<std::vector<int>> matrix) const;

        int determine_version(int num_digits);
        int get_value() const;
        int get_version() const;

};

