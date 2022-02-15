#include "tokenizer.h"

#include <iostream>

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
    }
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
  std::size_t length = 1;
  auto type = CharType(target_[pos]);
  if (type == Token::Type::kWord) {
    std::size_t i = pos + 1;
    while (i < target_.length() && CharType(target_[i]) == Token::Type::kWord) {
      ++i;
    }
    length = i - pos;
  }
  return {type, std::string_view(target_.data() + pos, length), pos};
}

TokenizerIterator Tokenizer::begin() const {
  return TokenizerIterator(this, 0);
}

TokenizerIterator Tokenizer::end() const {
  return TokenizerIterator(this, target_.size());
}

Tokenizer::Tokenizer(std::string target) : target_(std::move(target)) {}

TokenizerIterator &TokenizerIterator::operator++() {
  current_pos_ = (last_.value.data() - tokenizer_->begin().last_.value.data()) + last_.value.size();
  last_ = tokenizer_->Read(current_pos_);
  return *this;
}

TokenizerIterator TokenizerIterator::operator++(int) {
  auto result = *this;
  this->operator++();
  return result;
}

bool TokenizerIterator::operator==(const TokenizerIterator &other) const {
  return current_pos_ == other.current_pos_;
}
bool TokenizerIterator::operator!=(const TokenizerIterator &other) const {
  return !(*this == other);
}

TokenizerIterator::reference TokenizerIterator::operator*() const {
  return last_;
}

TokenizerIterator::TokenizerIterator(const Tokenizer *tokenizer, std::size_t position)
    : tokenizer_(tokenizer), current_pos_(position), last_(tokenizer_->Read(current_pos_)) {}

TokenizerIterator::pointer TokenizerIterator::operator->() const {
  return &last_;
}

}