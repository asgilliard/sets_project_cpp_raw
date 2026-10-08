#pragma once
#include <cstddef> // для std::size_t (размерный тип)
#include <cstdint> // для std::uint32_t (тип фиксированной ширины)
#include <iosfwd> // для std::ostream (вывод)

namespace sets {
class BitSet {
private:
  using block_type = std::uint32_t;
  // static - константа общая для всех экземпляров класса,
  // constexpr - константа вычисляется на этапе компиляции
  static constexpr std::size_t BLOCK_SIZE = sizeof(block_type) * 8;
  std::size_t usize_;
  std::size_t blocks_count_;
  block_type* data_;

public:
  // Конструктор
  // explicit - функция, ожидающая BitSet, не создаст его сама неявно из числа
  explicit BitSet(std::size_t usize = 0);

  // Перемещающий конструктор (C++11)
  BitSet(BitSet&& other) noexcept;

  // Конструктор копирования
  BitSet(const BitSet& other);

  // Деструктор
  ~BitSet();

  void set(std::size_t idx, bool value);
  bool contains(std::size_t idx) const;

  void intersect_with(const BitSet& other); // AND (&=)
  void union_with(const BitSet& other);     // OR (|=)
  void invert();                            // NOT (~)

  // operator=
  BitSet& operator=(const BitSet& other);

  // Перемещающий operator= (C++11)
  // noexcept - метод никогда не бросает исключение
  BitSet& operator=(BitSet&& other) noexcept;

  // Унарные операторы-модификаторы (возвращают ссылку на себя *this)
  BitSet& operator&=(const BitSet& other);
  BitSet& operator|=(const BitSet& other);

  // Унарное отрицание (возвращает новый объект)
  BitSet operator~() const;

  // Бинарные операторы (возвращают новый объект)
  BitSet operator&(const BitSet& other) const;
  BitSet operator|(const BitSet& other) const;

  // friend даёт доступ к приватным полям не-методу
  friend std::ostream& operator<<(std::ostream& os, const BitSet& set);
};
} // namespace sets
