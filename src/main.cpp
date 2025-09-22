#include <iostream>
#include "Generator.cpp"
#include <string>
int main() {
    int num = 1234;
    Generator A(num);
    std::cout << A.mode_indicator() << " " << A.CCI() << " "
    << A.encode_message() << "\n";
    std::cout << A.get_version() << "\n";
    return 0;
}