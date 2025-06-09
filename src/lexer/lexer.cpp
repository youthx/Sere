
#include "lexer/core/lexer.hpp"

namespace sere::lexer
{

    LexerCore::LexerCore(const char *input_buffer)
        : buffer(input_buffer ? input_buffer : ""),
          current(0), start(0),
          line(1), column(1),
          at_line_start(true),
          current_indent(0),
          paren_level(0)
    {
        SCANNER_DEBUG_LOG("Lexer Initialized");
    }

    TokenStream LexerCore::compile_token_stream()
    {
        reset_position();
        while (!is_at_end())
        {
            start = current;
            scan_next();
        }

        // Emit remaining DEDENTs at EOF
        while (!indent_stack.empty())
        {
            add_token(TokenType::Dedent);
            indent_stack.pop_back();
        }

        // Ensure last token is NEWLINE if not already
        if (!token_stream.get_tokens().empty() && token_stream.get_tokens().back().get()->type != TokenType::Newline)
        {
            add_token(TokenType::Newline);
        }

        // Add EOF
        TokenLiteral ZERO;
        
        token_stream.add_token(Token(TokenType::Eof, "", ZERO, line, column));
        return token_stream;
    }

    void LexerCore::reset_position()
    {
        line = 1;
        column = 1;
        current = 0;
        start = 0;

        current_indent = 0;
        indent_stack.clear();
        token_stream.clear();
        at_line_start = true;
        paren_level = 0;
        SCANNER_DEBUG_LOG("Position reset");
    }

    bool LexerCore::is_at_end() const noexcept
    {
        return SCANNER_UNLIKELY(current >= static_cast<int>(buffer.length()));
    }

    char LexerCore::advance()
    {
        if (is_at_end())
            return '\0';

        char c = buffer[current++];
        if (c == '\n')
        {
            line++;
            column = 1;
            at_line_start = true;
        }
        else
            column++;

        SCANNER_DEBUG_LOG("Advanced");
        return c;
    }

    char LexerCore::peek() const noexcept
    {
        if (is_at_end())
            return '\0';
        return buffer[current];
    }

    char LexerCore::peek_next() const noexcept
    {
        if (current + 1 >= static_cast<int>(buffer.length()))
            return '\0';
        return buffer[current + 1];
    }

    bool LexerCore::match(char expected_char)
    {
        if (is_at_end() || buffer[current] != expected_char)
            return false;
        current++;
        column++;
        return true;
    }

    void LexerCore::add_token(TokenType type)
    {
        TokenLiteral lit;

        std::string text = buffer.substr(start, current - start);
        lit = text;
        auto t = Token(type, text, lit, line, column);
        token_stream.add_token(t);
        SCANNER_DEBUG_LOG("Token added: " << text << " type: " << type);
    }

    template <typename T>
    void LexerCore::add_token(TokenType type, T literal)
    {
        TokenLiteral lit;
        lit = literal;

        std::string text = buffer.substr(start, current - start);
        token_stream.add_token(Token(type, text, lit, line, column));
        SCANNER_DEBUG_LOG("Token added: " << text << " type: " << type << " literal: " << literal);
    }


    void LexerCore::scan_next()
    {
        // Scan indent only & skip blanks.
        if (at_line_start && paren_level == 0)
        {
            int nl_count = 0;
            // Eat all blank lines (emit NEWLINE for each)
            while (peek() == '\n')
            {
                start = current;
                advance();
                add_token(TokenType::Newline);
                nl_count++;
                at_line_start = true;
            }
            // After blank lines, scan indentation (if not EOF or comment)
            if (!is_at_end() && peek() != '#' && peek() != '\n')
            {
                scan_indent();
                at_line_start = false;
            }
            // If it was just blank lines or EOF, return (don't double-read)
            if (peek() == '\n' || is_at_end())
                return;
        }

        char c = advance();
        switch (c)
        {
        case '\n':
            if (paren_level == 0)
            {
                add_token(TokenType::Newline);
                at_line_start = true;
            }
            break;
        case '\0':
            break;
        case ' ':
        case '\t':
            // Ignore whitespace except for indentation (handled at line start)
            break;
        case '#':
            while (peek() != '\n' && !is_at_end())
                advance();
            break;
        case '(':
            paren_level++;
            add_token(TokenType::LeftParen);
            break;
        case ')':
            if (paren_level > 0)
                paren_level--;
            add_token(TokenType::RightParen);
            break;
        case '{':
            add_token(TokenType::LeftBrace);
            break;
        case '}':
            add_token(TokenType::RightBrace);
            break;
        case '[':
            paren_level++;
            add_token(TokenType::LeftBracket);
            break;
        case ']':
            if (paren_level > 0)
                paren_level--;
            add_token(TokenType::RightBracket);
            break;
        case ',':
            add_token(TokenType::Comma);
            break;
        case '.':
            if (std::isdigit(static_cast<unsigned char>(peek())))
            {
                scan_number(true, (char)0); // float starting with . (e.g. .5)
            }
            else
            {
                add_token(TokenType::Dot);
            }
            break;
        case '+':
            add_token(match('=') ? TokenType::PlusEqual : TokenType::Plus);
            break;
        case '-':
            add_token(match('=') ? TokenType::MinusEqual : (match('>') ? TokenType::Arrow : TokenType::Minus));
            break;
        case '*':
            add_token(match('=') ? TokenType::StarEqual : (match('*') ? (match('=') ? TokenType::DoubleStarEqual : TokenType::DoubleStar) : TokenType::Star));
            break;
        case '/':
            add_token(match('=') ? TokenType::SlashEqual : (match('/') ? (match('=') ? TokenType::DoubleSlashEqual : TokenType::DoubleSlash) : TokenType::Slash));
            break;
        case '%':
            add_token(match('=') ? TokenType::PercentEqual : TokenType::Percent);
            break;
        case ':':
            add_token(match('=') ? TokenType::ColonEqual : TokenType::Colon);
            break;
        case ';':
            add_token(TokenType::Semicolon);
            break;
        case '|':
            add_token(match('=') ? TokenType::PipeEqual : (match('|') ? TokenType::DoublePipe : TokenType::Pipe));
            break;
        case '&':
            add_token(match('=') ? TokenType::AmpersandEqual : (match('&') ? TokenType::DoubleAmpersand : TokenType::Ampersand));
            break;
        case '^':
            add_token(match('=') ? TokenType::CaretEqual : TokenType::Caret);
            break;
        case '~':
            add_token(TokenType::Tilde);
            break;
        case '!':
            add_token(match('=') ? TokenType::BangEqual : TokenType::Bang);
            break;
        case '=':
            add_token(match('=') ? TokenType::EqualEqual : TokenType::Equal);
            break;
        case '<':
            if (match('<'))
            {
                add_token(match('=') ? TokenType::LeftShiftEqual : TokenType::LeftShift);
            }
            else
            {
                add_token(match('=') ? TokenType::LessEqual : TokenType::Less);
            }
            break;
        case '>':
            if (match('>'))
            {
                add_token(match('=') ? TokenType::RightShiftEqual : TokenType::RightShift);
            }
            else
            {
                add_token(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
            }
            break;
        case '\'':
        case '"':
            scan_string(c);
            break;
        case '\\':
            // Line continuation
            if (peek() == '\n')
            {
                advance();
                at_line_start = true;
            }
            break;
        default:
            if (std::isdigit(static_cast<unsigned char>(c)))
            {
                scan_number(false, c);
            }
            else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
            {
                scan_identifier(c);
            }
            else
            {
                throw std::runtime_error("Unexpected character: '" + std::string(1, c) + "'");
            }
            break;
        }
    }

    // INDENT/DEDENT LOGIC (Python-style)
    void LexerCore::scan_indent()
    {
        int spaces = 0;
        int orig_current = current;
        int orig_column = column;
        int count = 0;
        // Only count spaces/tabs at the very start of the line
        while (!is_at_end() && (buffer[current] == ' ' || buffer[current] == '\t'))
        {
            spaces += (buffer[current] == ' ') ? 1 : 8; // treat tab as 8 spaces (Python default)
            ++current;
            ++count;
        }
        column += count; // reflects the actual horizontal position

        if (spaces > current_indent)
        {
            // New indent
            indent_stack.push_back(current_indent);
            current_indent = spaces;
            add_token(TokenType::Indent);
        }
        else if (spaces < current_indent)
        {
            // DEDENT(s)
            while (!indent_stack.empty() && spaces < current_indent)
            {
                add_token(TokenType::Dedent);
                current_indent = indent_stack.back();
                indent_stack.pop_back();
            }
            if (spaces != current_indent)
            {
                throw std::runtime_error("Indentation error.");
            }
        }
        // If spaces matches current_indent, do nothing
        SCANNER_DEBUG_LOG("Indentation scanned: " << spaces << " (current_indent: " << current_indent << ")");
    }

    // Accepts float_start_dot for .5, -.5, etc.
    void LexerCore::scan_number(bool float_start_dot, char first_char)
    {
        int number_start = float_start_dot ? current - 1 : start;
        bool is_float = float_start_dot;
        std::string num_str;
        if (float_start_dot)
        {
            num_str = "0.";
        }
        else if (first_char)
        {
            num_str = first_char;
        }

        // Integer part
        while (std::isdigit(static_cast<unsigned char>(peek())))
            num_str += advance();

        // Fractional part
        if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next())))
        {
            is_float = true;
            num_str += advance(); // '.'
            while (std::isdigit(static_cast<unsigned char>(peek())))
                num_str += advance();
        }

        // Exponent part
        if (peek() == 'e' || peek() == 'E')
        {
            is_float = true;
            num_str += advance();
            if (peek() == '+' || peek() == '-')
                num_str += advance();
            if (!std::isdigit(static_cast<unsigned char>(peek())))
            {
                throw std::runtime_error("Invalid numeric literal: missing exponent digits");
            }
            while (std::isdigit(static_cast<unsigned char>(peek())))
                num_str += advance();
        }

        // Integer literal bases (0x, 0o, 0b)
        if (!is_float && num_str == "0" && (peek() == 'x' || peek() == 'X' || peek() == 'o' || peek() == 'O' || peek() == 'b' || peek() == 'B'))
        {
            char base_char = advance();
            num_str += base_char;
            int base = 10;
            if (base_char == 'x' || base_char == 'X')
                base = 16;
            else if (base_char == 'o' || base_char == 'O')
                base = 8;
            else if (base_char == 'b' || base_char == 'B')
                base = 2;
            while (std::isxdigit(static_cast<unsigned char>(peek())) || (base == 2 && (peek() == '0' || peek() == '1')))
            {
                num_str += advance();
            }
            try
            {
                errno = 0;
                long long val = std::stoll(num_str, nullptr, base);
                if (errno == ERANGE || val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
                    throw std::out_of_range("Integer literal out of range");
                add_token<int>(TokenType::Integer, static_cast<int>(val));
            }
            catch (const std::exception &e)
            {
                throw e;
            }
            return;
        }

        try
        {
            if (is_float)
            {
                errno = 0;
                double val = std::stod(num_str);
                if (errno == ERANGE)
                    throw std::out_of_range("Float literal out of range");
                add_token<float>(TokenType::Float, static_cast<float>(val));
            }
            else
            {
                errno = 0;
                long long val = std::stoll(num_str);
                if (errno == ERANGE || val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
                    throw std::out_of_range("Integer literal out of range");
                add_token<int>(TokenType::Integer, static_cast<int>(val));
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Invalid numeric literal");
        }
    }

    // Handles single/double/triple quoted strings, escapes, and multi-line
    void LexerCore::scan_string(char quote_type)
    {
        bool triple = false;
        int string_line = line;
        int string_column = column;
        std::string value;
        // Triple quote? Only check if enough characters remain
        if (peek() == quote_type && peek_next() == quote_type)
        {
            if (current + 2 < static_cast<int>(buffer.size()))
            {
                triple = true;
                advance();
                advance();
            }
        }
        bool closed = false;
        while (!is_at_end())
        {
            char c = peek();
            if (!triple && c == '\n')
            {
                // Single-line string cannot contain newlines
                break;
            }
            if (c == quote_type)
            {
                if (triple)
                {
                    // Only check for triple quote if enough characters remain
                    if (current + 2 < static_cast<int>(buffer.size()) &&
                        peek_next() == quote_type && buffer[current + 2] == quote_type)
                    {
                        advance();
                        advance();
                        advance();
                        closed = true;
                        break;
                    }
                    else
                    {
                        value += advance();
                    }
                }
                else
                {
                    advance();
                    closed = true;
                    break;
                }
            }
            else if (c == '\\')
            {
                advance();
                char next = peek();
                switch (next)
                {
                case 'n':
                    value += '\n';
                    break;
                case 't':
                    value += '\t';
                    break;
                case 'r':
                    value += '\r';
                    break;
                case '\\':
                    value += '\\';
                    break;
                case '\'':
                    value += '\'';
                    break;
                case '"':
                    value += '"';
                    break;
                case 'a':
                    value += '\a';
                    break;
                case 'b':
                    value += '\b';
                    break;
                case 'f':
                    value += '\f';
                    break;
                case 'v':
                    value += '\v';
                    break;
                case '0':
                    value += '\0';
                    break;
                case '\n':
                    break; // line continuation
                default:
                    value += next;
                    break;
                }
                advance();
            }
            else
            {
                value += advance();
            }
        }
        if (!closed)
        {
            throw std::runtime_error("Unterminated string literal");
        }
        add_token<std::string>(TokenType::String, value);
    }

    // Accepts the first character (already read) for correct identifier start
    void LexerCore::scan_identifier(char first_char)
    {
        std::string text(1, first_char);
        while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')
            text += advance();
        // Check for keyword
        auto it = reserved_toks.find(text);
        if (it != reserved_toks.end())
        {
            add_token(it->second);
        }
        else
        {
            add_token(TokenType::Identifier);
        }
    }
}
