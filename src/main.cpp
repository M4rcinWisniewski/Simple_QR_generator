#include <iostream>
#include "Generator.cpp"
#include <string>

int main(int argc, char *argv[]) {
    int num;

    // Validates users input argument
    if (argc == 2) {
        try {
            num = std::stoi(argv[1]);
        } catch (const std::invalid_argument&) {
            std::cerr << "Not a valid integer!" << std::endl;
        } catch (const std::out_of_range&) {
            std::cerr << "Integer out of range!" << std::endl;
        }
    } else {
        std::cout << "Please provide one intiger." << std::endl;
    }


    Generator A(num);
    for (auto sub_vec : A.qr_data()) {
        for (auto digit : sub_vec) {
            std::cout << digit << " ";
        }
        std::cout << "\n";
    }
    A.save_as_img(A.qr_data());
    
    return 0;
}

