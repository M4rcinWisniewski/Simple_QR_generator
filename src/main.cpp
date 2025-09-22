#include <iostream>
#include "Generator.cpp"
#include <string>
int main() {
    int num = 123;
    Generator A(num);
    std::cout << A.qr_data() << "\n";
    std::cout << A.qr_data().length() << "\n";
    return 0;
}