#pragma once

#include "tree.h"

namespace parser::grammar {

struct Error : std::runtime_error {
  Token token;

  Error(Token token);
};

template<typename Iterator>
Tree ExpectTerm(Iterator &it, Token::Type expected_type) {
  Token result = *(it++);
  if (result.type != expected_type) {
    throw Error(result);
  }
  return Tree(result.value);
}

template <typename Iterator>
Tree N(Iterator &it) {
  return ExpectTerm(it, Token::Type::kWord);
}

template <typename Iterator>
Tree V(Iterator &it) {
  switch (it->type) {
    case Token::Type::kWord: {
      return ExpectTerm(it, Token::Type::kWord);
    }
    case Token::Type::kStar: {
      Tree result("nonterm V -> * V");
      result.Add(ExpectTerm(it, Token::Type::kStar));
      result.Add(V(it));
      return result;
    }
    default: {
      throw Error(*it);
    }
  }
}

template <typename Iterator>
Tree Vars(Iterator &it) {
  switch (it->type) {
    case Token::Type::kComma: {
      Tree result("nonterm Vars -> , V Vars");
      result.Add(ExpectTerm(it, Token::Type::kComma));
      result.Add(V(it));
      result.Add(Vars(it));
      return result;
    }
    case Token::Type::kSemicolon: {
      return Tree("nonterm Vars -> eps");
    }
    default: {
      throw Error(*it);
    }
  }
}

template <typename Iterator>
Tree S(Iterator begin, Iterator end) {
  Tree result("nonterm S -> N V Vars ;");
  result.Add(N(begin));
  result.Add(V(begin));
  result.Add(Vars(begin));
  result.Add(ExpectTerm(begin, Token::Type::kSemicolon));
  if (begin != end) {
    throw Error(*begin);
  }
  return result;
}

}