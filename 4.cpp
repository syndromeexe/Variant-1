#include <iostream>
#include <cmath>
#include <vector>

double vector_length(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double x : vec) {
        sum += x * x;
    }
    return std::sqrt(sum);
}

void test() {
    std::cout << "=== Тесты ===\n";
    
    double len1 = vector_length({3.0, 4.0});
    std::cout << "|(3,4)| = " << len1 
              << " (ожидается 5) - " 
              << (std::abs(len1 - 5.0) < 1e-10 ? "OK" : "FAIL") << '\n';
    
    double len2 = vector_length({1.0, 2.0, 3.0});
    std::cout << "|(1,2,3)| = " << len2 
              << " (ожидается " << std::sqrt(14.0) << ") - " 
              << (std::abs(len2 - std::sqrt(14.0)) < 1e-10 ? "OK" : "FAIL") << '\n';
    
    double len3 = vector_length({0.0, 0.0, 0.0});
    std::cout << "|(0,0,0)| = " << len3 
              << " (ожидается 0) - " 
              << (len3 == 0.0 ? "OK" : "FAIL") << '\n';
    
    double len4 = vector_length({1.0, 0.0, 0.0});
    std::cout << "|(1,0,0)| = " << len4 
              << " (ожидается 1) - " 
              << (std::abs(len4 - 1.0) < 1e-10 ? "OK" : "FAIL") << '\n';
}

int main() {
    std::cout << "ДЛИНА ВЕКТОРА (Евклидова норма)\n\n";
    
    test();
    
    std::cout << "\nВведите размерность вектора: ";
    size_t n;
    std::cin >> n;
    
    std::vector<double> vec(n);
    std::cout << "Введите " << n << " компонент вектора:\n";
    for (size_t i = 0; i < n; ++i) {
        std::cout << "  v[" << i << "] = ";
        std::cin >> vec[i];
    }
    
    double length = vector_length(vec);
    std::cout << "\nДлина вектора |v| = " << length << '\n';
    
    return 0;
}