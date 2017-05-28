//codigo original => https://cboard.cprogramming.com/c-programming/117223-gaussian-noise-generator.html
#include <cmath>
#include <complex>
#include <iostream>
bool euler_flip(bool value)
{
    return std::pow
    (
        std::complex<float>(std::exp(1.0)), 
        std::complex<float>(0, 1) 
        * std::complex<float>(std::atan(1.0)
        *(1 << (value + 2)))
    ).real() < 0;
}
int main(){
    int i = euler_flip(1);
    std::cout << i <<std::endl;
    i = euler_flip(0);
    std::cout << i <<std::endl;
    i = euler_flip(1);
    std::cout << i <<std::endl;
    i = euler_flip(0);
    std::cout << i <<std::endl;
    i = euler_flip(0);
    std::cout << i <<std::endl;
    i = euler_flip(1);
    std::cout << i <<std::endl;
    i = euler_flip(0);
    std::cout << i <<std::endl;
    i = euler_flip(0);
    std::cout << i <<std::endl;
    return 0;
}