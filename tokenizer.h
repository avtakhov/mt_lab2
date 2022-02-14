#pragma once

#include <memory>
#include <string_view>
#include <ranges>
#include <string>

namespace parser {

struct Token {
  enum struct Type {
    kStar,
    kSpace,
    kComma,
    kWord,
    kSemicolon,
    kUnknown,
    kEnd
  };

  Type type;
  std::string_view value;
};

struct Iterator;
struct Tokenizer;

struct Tokenizer {
  Tokenizer(std::string target);

  Token Read(std::size_t pos) const;

  Iterator begin() const;

  Iterator end() const;
 private:
  std::string target_;
};

struct Iterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = const Token;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::ptrdiff_t;

  Iterator() = default;
  Iterator(Iterator const &other) = default;
  explicit Iterator(Tokenizer const *tokenizer, std::size_t position);

  Iterator &operator=(Iterator const &other) = default;
  Iterator &operator++();

  Iterator operator++(int);
  bool operator==(Iterator const &other) const;

  bool operator!=(Iterator const &other) const;
  reference operator*() const;

  pointer operator->() const;

  difference_type Position() const;

 private:
  Tokenizer const *tokenizer_;
  difference_type current_pos_;
  Token last_;
};

}