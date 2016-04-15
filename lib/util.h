#ifndef UTIL_H_
#define UTIL_H_

namespace std {

template <typename T, std::size_t N>
std::istream& operator>>(std::istream& is, std::array<T, N>& p) {
  std::string line;
  if (!std::getline(is, line)) throw std::runtime_error("Empty stream");
  std::istringstream iss(line);
  for (auto& e : p) iss >> e;
  return is;
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& p) {
  os << "( ";
  copy(p.cbegin(), p.cend(), ostream_iterator<T>(os, " "));
  os << ')';
  return os;
}

template <typename T,
          template <typename Elem, typename Alloc = std::allocator<Elem>>
          class Container>
std::istream& operator>>(std::istream& is, Container<T>& p) {
  std::string line;
  if (!std::getline(is, line)) throw std::runtime_error("Empty stream");
  std::istringstream iss(line);
  while (!iss.eof()) {
    if (iss.peek() != EOF) {
      T value;
      iss >> value;
      p.push_back(value);
    }
  }
  return is;
}

template <typename T,
          template <typename Elem, typename Alloc = std::allocator<Elem>>
          class Container>
std::ostream& operator<<(std::ostream& os, const Container<T>& p) {
  os << "( ";
  copy(p.cbegin(), p.cend(), ostream_iterator<T>(os, " "));
  os << ')';
  return os;
}

}  // namespace std

namespace util {

template <typename T, std::size_t N>
auto read_points(std::istream& is) {
  std::vector<std::array<T, N>> result;

  while (!is.eof()) {
    if (is.peek() != EOF) {
      typename decltype(result)::value_type pt;
      is >> pt;
      result.push_back(pt);
    }
  }
  return result;
}

template <typename T>
auto read_points(std::istream& is) {
  std::vector<std::vector<T>> result;

  while (!is.eof()) {
    if (is.peek() != EOF) {
      typename decltype(result)::value_type pt;
      is >> pt;
      result.push_back(pt);
    }
  }
  return result;
}

}  // namespace util

#endif  //  UTIL_H_
