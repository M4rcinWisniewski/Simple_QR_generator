#include <string>


class Generator {
    private: 
        int value;
        char ecc;
        int version;
        int mode = 1;
    public:
        // Constructor 
        Generator(int val, char ecc = 'L');

        std::string CCI();
        std::string mode_indicator();
        std::string encode_message();

        int determine_version(int num_digits);
        int get_value() const;
        int get_version() const;

};

