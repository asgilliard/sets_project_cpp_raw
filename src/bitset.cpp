#include "bitset.hpp"
#include <ostream>   // для вывода в std::cout
#include <stdexcept> // для обработки ошибок

namespace sets {
BitSet::BitSet(std::size_t usize) : usize_(usize) {
  blocks_count_ = (usize + BLOCK_SIZE - 1) / BLOCK_SIZE;
  data_ = (blocks_count_ > 0) ? new block_type[blocks_count_]{} : nullptr;
}

BitSet::BitSet(BitSet&& other) noexcept
    : usize_(other.usize_), blocks_count_(other.blocks_count_),
      data_(other.data_) {
  other.data_ = nullptr;
  other.blocks_count_ = 0;
  other.usize_ = 0;
};

BitSet::BitSet(const BitSet& other)
    : usize_(other.usize_), blocks_count_(other.blocks_count_) {
  data_ = (blocks_count_ > 0) ? new block_type[blocks_count_] : nullptr;
  for (std::size_t i = 0; i < blocks_count_; ++i) {
    data_[i] = other.data_[i];
  }
}

BitSet::~BitSet() {
  delete[] data_;
  data_ = nullptr;
}

BitSet& BitSet::operator=(const BitSet& other) {
  if (this == &other) {
    return *this;
  }

  delete[] data_;

  usize_ = other.usize_;
  blocks_count_ = other.blocks_count_;
  data_ = (blocks_count_ > 0) ? new block_type[blocks_count_] : nullptr;
  for (std::size_t i = 0; i < blocks_count_; ++i) {
    data_[i] = other.data_[i];
  }
  return *this;
}

BitSet& BitSet::operator=(BitSet&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  delete[] data_;

  usize_ = other.usize_;
  blocks_count_ = other.blocks_count_;
  data_ = other.data_;

  other.data_ = nullptr;
  other.blocks_count_ = 0;
  other.usize_ = 0;

  return *this;
}

void BitSet::set(std::size_t idx, bool value) {
  if (idx >= usize_) {
    throw std::out_of_range("Index out of Universe size");
  }
  auto block = idx / BLOCK_SIZE;
  auto shift = idx % BLOCK_SIZE;
  auto mask = block_type{1} << shift;
  if (value == true) {
    data_[block] |= mask;
  } else {
    data_[block] &= ~mask;
  }
}

bool BitSet::contains(std::size_t idx) const {
  if (idx >= usize_) {
    throw std::out_of_range("Index out of Universe size");
  }
  auto block = idx / BLOCK_SIZE;
  auto shift = idx % BLOCK_SIZE;
  auto mask = block_type{1} << shift;

  return (data_[block] & mask) != 0;
}

void BitSet::intersect_with(const BitSet& other) {
  auto min_blocks = (blocks_count_ < other.blocks_count_) ? blocks_count_
                                                          : other.blocks_count_;
  // Пересечение на совпадающих участках
  for (std::size_t i = 0; i < min_blocks; ++i) {
    data_[i] &= other.data_[i];
  }

  // Если other был меньше - остальное нули
  for (std::size_t i = min_blocks; i < blocks_count_; ++i) {
    data_[i] = 0;
  }
}

void BitSet::union_with(const BitSet& other) {
  // Если other был больше - ресайзим исходник и дописываем из other
  if (other.usize_ > usize_) {
    std::size_t old_blocks_count = blocks_count_;
    usize_ = other.usize_;
    blocks_count_ = other.blocks_count_;

    block_type* temp_data = new block_type[blocks_count_];
    for (std::size_t i = 0; i < old_blocks_count; ++i) {
      temp_data[i] = data_[i];
    }
    for (std::size_t i = old_blocks_count; i < blocks_count_; ++i) {
      temp_data[i] = 0;
    }

    delete[] data_;
    data_ = temp_data;
  }
  // Объединение на совпадающих участках
  for (std::size_t i = 0; i < other.blocks_count_; ++i) {
    data_[i] |= other.data_[i];
  }
}

void BitSet::invert() {
  for (std::size_t i = 0; i < blocks_count_; ++i) {
    data_[i] = ~data_[i];
  }

  auto extra_bits = usize_ % BLOCK_SIZE;
  if (extra_bits != 0) {
    auto mask = (block_type{1} << extra_bits) - 1;
    data_[blocks_count_ - 1] &= mask;
  }
}

BitSet& BitSet::operator&=(const BitSet& other) {
  intersect_with(other);
  return *this;
}

BitSet& BitSet::operator|=(const BitSet& other) {
  union_with(other);
  return *this;
}

BitSet BitSet::operator&(const BitSet& other) const {
  BitSet result = *this;
  result &= other;
  return result;
}

BitSet BitSet::operator|(const BitSet& other) const {
  BitSet result = *this;
  result |= other;
  return result;
}

BitSet BitSet::operator~() const {
  BitSet result = *this;
  result.invert();
  return result;
}

std::ostream& operator<<(std::ostream& os, const BitSet& set) {
  os << "{";
  bool first = true;
  for (std::size_t i = 0; i < set.blocks_count_; ++i) {
    auto block = set.data_[i];
    if (block == 0)
      continue; // Пропускаем пустые блоки

    for (std::size_t bit = 0; bit < BitSet::BLOCK_SIZE; ++bit) {
      std::size_t idx = i * BitSet::BLOCK_SIZE + bit;
      if (idx >= set.usize_)
        break;

      if ((block & (BitSet::block_type{1} << bit)) != 0) {
        if (!first)
          os << ", ";
        os << idx;
        first = false;
      }
    }
  }
  os << "}";
  return os;
}
} // namespace sets
