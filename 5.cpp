#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include <cassert>



std::vector<int> sieve_of_eratosthenes(int n) {
    if (n < 2) {
        return {};
    }
    

    std::vector<bool> is_prime(n + 1, true);
    is_prime[0] = false;
    is_prime[1] = false;
    

    int sqrt_n = static_cast<int>(std::sqrt(n));
    
    for (int i = 2; i <= sqrt_n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    

    std::vector<int> primes;
    primes.reserve(static_cast<size_t>(n / std::log(n)));  
    
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    
    return primes;
}


std::vector<int> sieve_bitset(int n) {
    if (n < 2) return {};
    
    const int MAX_N = 10000000;
    static std::bitset<MAX_N + 1> is_prime;
    
    if (n > MAX_N) {

        return sieve_of_eratosthenes(n);
    }
    
    is_prime.set();  
    is_prime[0] = is_prime[1] = 0;
    
    int sqrt_n = static_cast<int>(std::sqrt(n));
    
    for (int i = 2; i <= sqrt_n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    
    std::vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    
    return primes;
}


bool is_prime_number(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    

    int sqrt_num = static_cast<int>(std::sqrt(num));
    for (int i = 5; i <= sqrt_num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

std::vector<int> trial_division(int n) {
    std::vector<int> primes;
    if (n < 2) return primes;
    
    if (n >= 2) primes.push_back(2);
    if (n >= 3) primes.push_back(3);
    

    for (int i = 5; i <= n; i += 6) {
        if (is_prime_number(i)) {
            primes.push_back(i);
        }
        if (i + 2 <= n && is_prime_number(i + 2)) {
            primes.push_back(i + 2);
        }
    }
    
    return primes;
}


void test_primes() {
    std::cout << "=== ТЕСТИРОВАНИЕ ФУНКЦИЙ ПОИСКА ПРОСТЫХ ЧИСЕЛ ===\n\n";
    
    int tests_passed = 0;
    int tests_failed = 0;
    

    struct TestCase {
        int n;
        std::vector<int> expected_primes;
        std::string description;
    };
    
    std::vector<TestCase> test_cases = {
        {0, {}, "N = 0"},
        {1, {}, "N = 1"},
        {2, {2}, "N = 2"},
        {3, {2, 3}, "N = 3"},
        {4, {2, 3}, "N = 4"},
        {5, {2, 3, 5}, "N = 5"},
        {10, {2, 3, 5, 7}, "N = 10"},
        {20, {2, 3, 5, 7, 11, 13, 17, 19}, "N = 20"},
        {30, {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}, "N = 30"},
        {50, {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47}, "N = 50"}
    };
    
    for (const auto& test : test_cases) {
        std::cout << "Тест: " << test.description << "\n";
        

        auto result1 = sieve_of_eratosthenes(test.n);
        auto result2 = sieve_bitset(test.n);
        auto result3 = trial_division(test.n);
        
        bool all_match = true;
        

        if (result1 != test.expected_primes) {
            std::cout << "  ✗ Метод 1 (решето) не совпадает\n";
            all_match = false;
        }
        

        if (result2 != test.expected_primes) {
            std::cout << "  ✗ Метод 2 (bitset) не совпадает\n";
            all_match = false;
        }
        

        if (result3 != test.expected_primes) {
            std::cout << "  ✗ Метод 3 (перебор) не совпадает\n";
            all_match = false;
        }
        
        if (all_match) {
            std::cout << "  ✓ Все методы верны\n";
            std::cout << "  Простые числа: ";
            for (int p : test.expected_primes) {
                std::cout << p << " ";
            }
            std::cout << "\n\n";
            tests_passed++;
        } else {
            tests_failed++;
        }
    }
    

    std::cout << "Тест: Проверка отдельных чисел\n";
    struct {
        int num;
        bool expected;
    } prime_tests[] = {
        {2, true}, {3, true}, {4, false}, {5, true},
        {9, false}, {11, true}, {15, false}, {17, true},
        {21, false}, {23, true}, {97, true}, {100, false},
        {997, true}, {1000, false}
    };
    
    int prime_test_ok = 0;
    for (const auto& pt : prime_tests) {
        if (is_prime_number(pt.num) == pt.expected) {
            prime_test_ok++;
        } else {
            std::cout << "  ✗ Ошибка: is_prime(" << pt.num << ") != " 
                      << (pt.expected ? "true" : "false") << "\n";
        }
    }
    
    std::cout << "  ✓ Правильно определено: " << prime_test_ok << "/" 
              << sizeof(prime_tests)/sizeof(prime_tests[0]) << " чисел\n\n";
    

    std::cout << "Тест: Согласованность методов на диапазоне 2..1000\n";
    auto res1 = sieve_of_eratosthenes(1000);
    auto res2 = sieve_bitset(1000);
    auto res3 = trial_division(1000);
    
    if (res1 == res2 && res2 == res3) {
        std::cout << "  ✓ Все методы дают одинаковый результат\n";
        std::cout << "  Найдено простых чисел до 1000: " << res1.size() << "\n\n";
        tests_passed++;
    } else {
        std::cout << "  ✗ Методы не согласованы!\n\n";
        tests_failed++;
    }
    
    // Итоги
    std::cout << "ИТОГИ ТЕСТИРОВАНИЯ:\n";
    std::cout << "  Пройдено: " << tests_passed << "\n";
    std::cout << "  Провалено: " << tests_failed << "\n";
}

void print_primes(const std::vector<int>& primes, int max_display = 50) {
    if (primes.empty()) {
        std::cout << "Простые числа не найдены.\n";
        return;
    }
    
    std::cout << "Найдено простых чисел: " << primes.size() << "\n";
    
    if (primes.size() <= static_cast<size_t>(max_display)) {
        std::cout << "Простые числа: ";
        for (size_t i = 0; i < primes.size(); ++i) {
            std::cout << primes[i];
            if (i < primes.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Первые " << max_display/2 << " чисел: ";
        for (int i = 0; i < max_display/2; ++i) {
            std::cout << primes[i] << " ";
        }
        std::cout << "\n...\nПоследние " << max_display/2 << " чисел: ";
        for (size_t i = primes.size() - max_display/2; i < primes.size(); ++i) {
            std::cout << primes[i] << " ";
        }
        std::cout << "\n";
    }
}


void compare_performance(int n) {
    std::cout << "=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ (N = " << n << ") ===\n\n";
    
    auto start1 = std::chrono::high_resolution_clock::now();
    auto primes1 = sieve_of_eratosthenes(n);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    std::cout << "Метод 1 (Решето Эратосфена):\n";
    std::cout << "  Время: " << duration1.count() << " мкс\n";
    std::cout << "  Найдено чисел: " << primes1.size() << "\n\n";
    
    auto start2 = std::chrono::high_resolution_clock::now();
    auto primes2 = sieve_bitset(n);
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "Метод 2 (Bitset):\n";
    std::cout << "  Время: " << duration2.count() << " мкс\n";
    std::cout << "  Найдено чисел: " << primes2.size() << "\n\n";
    
    auto start3 = std::chrono::high_resolution_clock::now();
    auto primes3 = trial_division(n);
    auto end3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3);
    
    std::cout << "Метод 3 (Перебор делителей):\n";
    std::cout << "  Время: " << duration3.count() << " мкс\n";
    std::cout << "  Найдено чисел: " << primes3.size() << "\n\n";
}


void prime_statistics(int n) {
    auto primes = sieve_of_eratosthenes(n);
    
    std::cout << "=== СТАТИСТИКА ПРОСТЫХ ЧИСЕЛ ДО " << n << " ===\n\n";
    std::cout << "Количество простых чисел: " << primes.size() << "\n";
    std::cout << "Плотность простых чисел: " << std::fixed << std::setprecision(4) 
              << (100.0 * primes.size() / n) << "%\n";
    
    if (!primes.empty()) {
        std::cout << "Наибольшее простое: " << primes.back() << "\n";
        std::cout << "Наименьшее простое: " << primes.front() << "\n";
        
        double approximation = n / std::log(n);
        std::cout << "Приближение n/ln(n): " << std::fixed << std::setprecision(0) 
                  << approximation << "\n";
        std::cout << "Отклонение от реального: " << std::setprecision(2) 
                  << std::abs(primes.size() - approximation) / primes.size() * 100 << "%\n";
    }
    
    if (primes.size() >= 2) {
        int twin_count = 0;
        for (size_t i = 0; i < primes.size() - 1; ++i) {
            if (primes[i + 1] - primes[i] == 2) {
                twin_count++;
            }
        }
        std::cout << "Пар простых чисел-близнецов: " << twin_count << "\n";
        if (twin_count > 0 && n >= 20) {
            std::cout << "Примеры близнецов: ";
            int shown = 0;
            for (size_t i = 0; i < primes.size() - 1 && shown < 5; ++i) {
                if (primes[i + 1] - primes[i] == 2) {
                    std::cout << "(" << primes[i] << ", " << primes[i + 1] << ") ";
                    shown++;
                }
            }
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}


int main() {
    std::cout << "╔════════════════════════════════╗\n";
    std::cout << "║     ПРОСТЫЕ ЧИСЛА ДО N        ║\n";
    std::cout << "║  Решето Эратосфена и другие   ║\n";
    std::cout << "╚════════════════════════════════╝\n\n";
    
    // 1. Запуск тестов
    test_primes();
    
    // 2. Демонстрация для разных N
    std::cout << "=== ДЕМОНСТРАЦИЯ ДЛЯ РАЗНЫХ N ===\n\n";
    
    int demo_values[] = {10, 50, 100};
    for (int n : demo_values) {
        std::cout << "--- Простые числа до " << n << " ---\n";
        auto primes = sieve_of_eratosthenes(n);
        print_primes(primes);
        std::cout << "\n";
    }
    
    // 3. Сравнение производительности
    compare_performance(100000);
    
    // 4. Статистика
    prime_statistics(1000);
    
    // 5. Интерактивный ввод
    std::cout << "=== ИНТЕРАКТИВНЫЙ РЕЖИМ ===\n";
    std::cout << "Введите N (или 0 для выхода): ";
    int n;
    std::cin >> n;
    
    while (n > 0) {
        if (n > 10000000) {
            std::cout << "N слишком большое. Максимум: 10000000\n";
        } else {
            auto primes = sieve_of_eratosthenes(n);
            std::cout << "\n";
            print_primes(primes, 40);
            
            if (n <= 100000) {
                prime_statistics(n);
            }
        }
        
        std::cout << "\nВведите N (или 0 для выхода): ";
        std::cin >> n;
    }
    
    std::cout << "Программа завершена.\n";
    return 0;
}