#include "gtest/gtest.h"

#include "tokenizer.h"
#include "tree.h"
#include "grammar.h"

#include <algorithm>
#include <ranges>

using type = parser::Token::Type;

template <typename Range>
void Error(Range const& tokenizer, std::size_t pos, parser::Token::Type type) {
  try {
    parser::grammar::S(tokenizer.begin(), tokenizer.end());
    ASSERT_TRUE(false);
  } catch (parser::grammar::Error const &error) {
    ASSERT_EQ(error.token.pos, pos);
    ASSERT_EQ(error.token.type, type);
  }
}

void ErrorValue(std::string value, std::size_t pos, parser::Token::Type type) {
  Error(parser::Tokenizer(std::move(value)), pos, type);
}

TEST(tokenizer, simple) {
  parser::Tokenizer t("int a;");
  static_assert(std::forward_iterator<parser::TokenizerIterator>);
  auto view = t | std::views::transform(&parser::Token::type);
  auto expected = {
      type::kWord,
      type::kWord,
      type::kSemicolon
  };
  ASSERT_TRUE(std::equal(expected.begin(), expected.end(), view.begin()));
}

TEST(grammar, nothrow) {
  std::vector<parser::Token> tokens = {
      {type::kWord,},
      {type::kStar},
      {type::kWord},
      {type::kSemicolon}
  };
  ASSERT_NO_THROW(parser::grammar::S(tokens.begin(), tokens.end()));
}

TEST(grammar, error) {
  std::vector<parser::Token> tokens = {
      {type::kWord},
      {type::kSemicolon, "1-231023", 100},
      {type::kStar},
      {type::kWord},
      {type::kSemicolon}
  };
  Error(tokens, 100, type::kSemicolon);
}

TEST(tokenizer_then_grammar, nothrow) {
  parser::Tokenizer t("int a, *b, **c;");
  ASSERT_NO_THROW(parser::grammar::S(t.begin(), t.end()));
}

TEST(tokenizer_then_grammar, no_semicolon) {
  ErrorValue("int a, *b, **c", 14, type::kUnknown);
}

TEST(tokenizer_then_grammar, comma_first) {
  ErrorValue(",asd", 0, type::kComma);
}

TEST(tokenizer_then_grammar, tilda) {
  ErrorValue("int~ a, b, c;", 3, type::kUnknown);
}

TEST(tokenizer_then_grammar, only_names) {
  ErrorValue("int a b c", 6, type::kWord);
}

TEST(tokenizer_then_grammar, no_vars) {
  ErrorValue("int ;", 4, type::kSemicolon);
}

TEST(tokenizer_then_grammar, star_after_name) {
  ErrorValue("long a*;", 6, type::kStar);
}

TEST(tokenizer_then_grammar, missing_comma) {
  ErrorValue("long *a b;", 8, type::kWord);
}

TEST(tokenizer_then_grammar, many_commas) {
  ErrorValue("long **a,, b;", 9, type::kComma);
}

TEST(tokenizer_then_grammar, cpp_reference) {
  ErrorValue("long &a;", 5, type::kUnknown);
}