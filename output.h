#pragma once
#include <iostream>

struct print {
  template<typename T>
  auto& operator<<(const T& a) const { return std::cout << a; }
  ~print() { std::cout << std::endl; }
};

struct error {
  template<typename T>
  auto& operator<<(const T& a) const { return std::cerr << a; }
  ~error() { std::cerr << std::endl; }
};
