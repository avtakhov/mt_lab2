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
  };

  Type type;
  std::string_view value;
  std::size_t pos;
};

struct Tokenizer;
struct TokenizerIterator;

struct Tokenizer {
  Tokenizer(std::string target);

  Token Read(std::size_t pos) const;

  TokenizerIterator begin() const;

  TokenizerIterator end() const;
 private:
  std::string target_;
};

struct TokenizerIterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = Token const;
  using reference = value_type;
  using pointer = value_type*;
  using difference_type = std::ptrdiff_t;

  TokenizerIterator() = default;
  TokenizerIterator(TokenizerIterator const &other) = default;
  explicit TokenizerIterator(Tokenizer const *tokenizer, std::size_t position);

  TokenizerIterator &operator=(TokenizerIterator const &other) = default;
  TokenizerIterator &operator++();

  TokenizerIterator operator++(int);
  bool operator==(TokenizerIterator const &other) const;

  bool operator!=(TokenizerIterator const &other) const;
  reference operator*() const;
  pointer operator->() const;
 private:
  Tokenizer const *tokenizer_;
  difference_type current_pos_;
  Token last_;
};

}