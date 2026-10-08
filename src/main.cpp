#include "bitset.hpp"
#include <iostream>

int main() {
  const std::size_t UNIVERSE_SIZE = 100;

  sets::BitSet A(UNIVERSE_SIZE);
  for (std::size_t i : {1, 3, 5, 7, 9, 84, 65}) {
    A.set(i, true);
  }

  sets::BitSet B(UNIVERSE_SIZE);
  for (std::size_t i : {2, 3, 5, 8, 16, 10, 90, 65}) {
    B.set(i, true);
  }

  std::cout << "=== Исходные множества (U size = " << UNIVERSE_SIZE
            << ") ===\n";
  std::cout << std::endl;
  std::cout << "Множество A: " << A << std::endl;
  std::cout << std::endl;
  std::cout << "Множество B: " << B << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "=== Операции ===\n";
  std::cout << std::endl;
  std::cout << "Пересечение A∩B: " << (A & B) << std::endl;
  std::cout << std::endl;
  std::cout << "Объединение A∪B:: " << (A | B) << std::endl;
  std::cout << std::endl;
  std::cout << "Отрицание ¬A: " << ~A << std::endl;
  std::cout << std::endl;
  std::cout << "Отрицание ¬B: " << ~B << std::endl;

  return 0;
}
