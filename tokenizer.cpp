#include "tokenizer.h"

#include <optional>

namespace parser {

namespace {

Token::Type CharType(char c) {
  switch (c) {
    case '*': return Token::Type::kStar;
    case ',': return Token::Type::kComma;
    case ' ': return Token::Type::kSpace;
    case '\n': return Token::Type::kSpace;
    case ';': return Token::Type::kSemicolon;
    default: {
      return std::isalpha(c) || std::isdigit(c) || c == '_' ? Token::Type::kWord
                                                            : Token::Type::kUnknown;
    };
  }
}

std::size_t SkipSpaces(const std::string &target, std::size_t pos) {
  Token::Type type;
  while (true) {
    type = CharType(target[pos]);
    if (type == Token::Type::kSpace)
      ++pos;
    else
      break;
  }
  return pos;
}

}

Token Tokenizer::Read(std::size_t pos) const {
  pos = SkipSpaces(target_, pos);
  if (pos == target_.length()) {
    return {Token::Type::kEnd, std::string_view(target_.data() + pos, 0)};
  }
  std::size_t length = 1;
  auto type = CharType(target_[pos]);
  if (type == Token::Type::kWord) {
    std::size_t i = pos + 1;
    while (i < target_.length() && CharType(target_[i]) == Token::Type::kWord) {
      ++i;
    }
    length = i - pos;
  }
  return {type, std::string_view(target_.data() + pos, length)};
}

Iterator Tokenizer::begin() const {
  return Iterator(this, 0);
}

Iterator Tokenizer::end() const {
  return Iterator(this, target_.size());
}

Tokenizer::Tokenizer(std::string target) : target_(std::move(target)) {}

Iterator &Iterator::operator++() {
  current_pos_ = Position() + last_.value.size();
  last_ = tokenizer_->Read(current_pos_);
  return *this;
}

Iterator Iterator::operator++(int) {
  auto result = *this;
  this->operator++();
  return result;
}

bool Iterator::operator==(const Iterator &other) const {
  return current_pos_ == other.current_pos_;
}
bool Iterator::operator!=(const Iterator &other) const {
  return !(*this == other);
}

Iterator::reference Iterator::operator*() const {
  return last_;
}

Iterator::pointer Iterator::operator->() const {
  return &last_;
}
Iterator::difference_type Iterator::Position() const {
  return last_.value.data() - tokenizer_->begin()->value.data();
}

Iterator::Iterator(const Tokenizer *tokenizer, std::size_t position)
    : tokenizer_(tokenizer), current_pos_(position), last_(tokenizer_->Read(current_pos_)) {}

}