#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {

  const char *start;
  const char *current;
  size_t line;

} Scanner;

Scanner scanner;

void init_scanner(const char *source) {

  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool is_at_end() { return *scanner.current == '\0'; }

static bool is_digit(char c) { return (c >= '0' && c <= '9'); }

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static char peek() { return *scanner.current; }

static char peek_next() {

  if (is_at_end()) {
    return '\0';
  }

  return *(scanner.current + 1);
}

static char advance() {

  scanner.current++;
  return *(scanner.current - 1);
}

static Token create_token(TokenType type) {

  Token token;

  token.type = type;
  token.start = scanner.start;
  token.length = (size_t)(scanner.current - scanner.start);
  token.line = scanner.line;

  // printf("[Size: %d] %s\n", (int)token.length, TokenTypeName[token.type]);

  return token;
}

static Token error_token(const char *message) {

  Token token;

  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (size_t)strlen(message);
  token.line = scanner.line;

  return token;
}

static bool match(char expected) {

  if (is_at_end() || *scanner.current != expected) {
    return false;
  }

  scanner.current++;
  return true;
}

static Token number() {

  while (is_digit(peek())) {
    advance();
  }

  if (peek() == '.' && is_digit(peek_next())) {

    advance();

    while (is_digit(peek())) {
      advance();
    }
  }

  return create_token(TOKEN_NUMBER);
}

static void we_hate_spaces() {

  while (true) {

    char c = peek();

    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;

    case '\n':
      scanner.line++;
      advance();
      break;

    case '#':

      while ((peek() != '\n') && (!is_at_end())) {
        advance();
      }

      break;

    default:
      return;
    }
  }
}

static Token string() {

  while (peek() != '"' && !is_at_end()) {

    if (peek() == '\n') {
      scanner.line++;
    }

    advance();
  }

  if (is_at_end()) {
    return error_token("Unterminated string.");
  }

  advance();

  return create_token(TOKEN_STRING);
}

static TokenType check_keyword(size_t start, size_t length, const char *rest,
                               TokenType type) {

  bool yes = (scanner.current - scanner.start == start + length) &&
             (memcmp(scanner.start + start, rest, length) == 0);

  if (yes) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType identifier_type() {

  switch (scanner.start[0]) {
  case 'a':
    return check_keyword(1, 2, "nd", TOKEN_AND);
  case 'i':
    return check_keyword(1, 1, "f", TOKEN_IF);
  case 'r':
    return check_keyword(1, 5, "eturn", TOKEN_RETURN);
  case 'w':
    return check_keyword(1, 4, "hile", TOKEN_WHILE);
  case 'l':
    return check_keyword(1, 2, "et", TOKEN_LET);

  case 'f':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'a':
        return check_keyword(2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return check_keyword(2, 1, "r", TOKEN_FOR);
      case 'n':
        return check_keyword(2, 0, "", TOKEN_FN);
      }
    }
    break;

  case 'n':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'i':
        return check_keyword(2, 1, "l", TOKEN_NIL);
      }
    }
    break;

  case 'd':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'i':
        return check_keyword(2, 5, "splay", TOKEN_DISPLAY);
      case 'o': {
        if (scanner.current - scanner.start > 2) {
        case 'n':
          return check_keyword(3, 1, "e", TOKEN_END);
        }

        else {
          return check_keyword(2, 0, "", TOKEN_DO);
        }
      }
      }
    }
    break;

  case 'o':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'r':
        return check_keyword(2, 0, "", TOKEN_OR);
      case 't':
        return check_keyword(2, 7, "herwise", TOKEN_OTHERWISE);
      }
    }
    break;

  case 'e':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'l':
        return check_keyword(2, 2, "se", TOKEN_ELSE);
        // case 'n':
        //   return check_keyword(2, 1, "d", TOKEN_END);
      }
    }
    break;

  case 't':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'h':
        return check_keyword(2, 2, "en", TOKEN_THEN);
      case 'r':
        return check_keyword(2, 2, "ue", TOKEN_TRUE);
      }
    }
    break;
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier() {

  while (is_alpha(peek()) || is_digit(peek())) {
    advance();
  }

  return create_token(identifier_type());
}

Token scan_token() {

  we_hate_spaces();
  scanner.start = scanner.current;

  if (is_at_end()) {
    return create_token(TOKEN_EOF);
  }

  char c = advance();

  if (is_digit(c)) {
    return number();
  }

  if (is_alpha(c)) {
    return identifier();
  }

  switch (c) {
  case '(':
    return create_token(TOKEN_LEFT_PAREN);
  case ')':
    return create_token(TOKEN_RIGHT_PAREN);
  case '{':
    return create_token(TOKEN_LEFT_BRACE);
  case '}':
    return create_token(TOKEN_RIGHT_BRACE);
  case ';':
    return create_token(TOKEN_SEMICOLON);
  case ',':
    return create_token(TOKEN_COMMA);
  case '.':
    return create_token(TOKEN_DOT);
  case '-':
    return create_token(TOKEN_MINUS);
  case '+':
    return create_token(TOKEN_PLUS);
  case '/':
    return create_token(TOKEN_SLASH);
  case '*':
    return create_token(TOKEN_STAR);
  case '#':
    return create_token(TOKEN_HASH);

  case ':':
    return create_token(TOKEN_COLON);
  case '!':
    return create_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return create_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return create_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '>':
    return create_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

  case '"':
    return string();
  }

  return error_token("Unexpected character.");
}
