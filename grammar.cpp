#include "grammar.h"

namespace parser::grammar {

namespace {

std::string MakeErrorMessage(std::size_t pos, std::string_view value) {
  return "Unexpected token '" + std::string(value) + "' at pos " + std::to_string(pos);
}

}

Error::Error(Token token)
    : std::runtime_error(MakeErrorMessage(token.pos, token.value)),
      token(token) {}
}
