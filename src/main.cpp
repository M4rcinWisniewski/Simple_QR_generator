#include <iostream>
#include "Generator.cpp"
#include <string>

int main(int argc, char *argv[]) {
    int num;
    
    // Validates users input argument
    if (argc == 2) {
        try {
            int num = std::stoi(argv[1]);
        } catch (const std::invalid_argument&) {
            std::cerr << "Not a valid integer!" << std::endl;
        } catch (const std::out_of_range&) {
            std::cerr << "Integer out of range!" << std::endl;
        }
    } else {
        std::cout << "Please provide one intiger." << std::endl;
    }


    Generator A(num);
    std::cout << A.qr_data() << "\n";
    std::cout << A.qr_data().length() << "\n";
    return 0;
}