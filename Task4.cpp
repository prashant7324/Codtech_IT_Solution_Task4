#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

class Parser {
    std::string input;
    size_t pos;

public:
    Parser(const std::string& expr) : input(expr), pos(0) {}

    double parse() {
        double result = parseExpression();
        if (pos < input.size()) {
            throw std::runtime_error("Unexpected character at end: " + std::string(1, input[pos]));
        }
        return result;
    }

private:
    void skipWhitespace() {
        while (pos < input.size() && std::isspace(input[pos])) ++pos;
    }

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            skipWhitespace();
            if (pos < input.size() && (input[pos] == '+' || input[pos] == '-')) {
                char op = input[pos++];
                double term = parseTerm();
                if (op == '+') result += term;
                else result -= term;
            } else {
                break;
            }
        }
        return result;
    }

    double parseTerm() {
        double result = parseFactor();
        while (true) {
            skipWhitespace();
            if (pos < input.size() && (input[pos] == '*' || input[pos] == '/')) {
                char op = input[pos++];
                double factor = parseFactor();
                if (op == '*') result *= factor;
                else {
                    if (factor == 0) throw std::runtime_error("Division by zero");
                    result /= factor;
                }
            } else {
                break;
            }
        }
        return result;
    }

    double parseFactor() {
        skipWhitespace();
        if (pos >= input.size()) throw std::runtime_error("Unexpected end of input");

        if (input[pos] == '(') {
            ++pos; // skip '('
            double value = parseExpression();
            skipWhitespace();
            if (pos >= input.size() || input[pos] != ')') throw std::runtime_error("Expected ')'");
            ++pos; // skip ')'
            return value;
        } else if (std::isdigit(input[pos]) || input[pos] == '.') {
            return parseNumber();
        } else {
            throw std::runtime_error("Unexpected character: " + std::string(1, input[pos]));
        }
    }

    double parseNumber() {
        skipWhitespace();
        size_t start = pos;
        while (pos < input.size() && (std::isdigit(input[pos]) || input[pos] == '.')) ++pos;
        return std::stod(input.substr(start, pos - start));
    }
};

int main() {
    std::string expr;
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, expr);

    try {
        Parser parser(expr);
        double result = parser.parse();
        std::cout << "Result: " << result << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    return 0;
}
