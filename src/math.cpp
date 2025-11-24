#include <math.hpp>

math::Equation::Equation(const std::initializer_list<Token> eq, const bool isinfix) {
    if (isinfix) {
        infix = eq;
        dijkstra();
    } else equation = eq;
}

int math::Equation::precedence(const OperatorType op) {
    if (op == OperatorType::EXP) return 3;
    if (op == OperatorType::MUL || op == OperatorType::DIV) return 2;
    return 1;
}

void math::Equation::dijkstra() {
    std::stack<Token> tmp;

    for (const auto&[type, token] : infix) {
        if (type == TokenType::VAR || type == TokenType::NUM) equation.emplace_back(Token(type, token));
        else if (type == TokenType::PRTH && token.paren == ParenthesisType::OPEN) tmp.push(Token(type, token));
        else if (type == TokenType::PRTH && token.paren == ParenthesisType::CLOSED) {
            Token tk = tmp.top();
            tmp.pop();
            while (!(tk.type == TokenType::PRTH && tk.token.paren == ParenthesisType::OPEN)) {
                equation.emplace_back(tk);

                tk = tmp.top();
                tmp.pop();
            }
        } else if (type == TokenType::OP) {
            while (!tmp.empty() && tmp.top().type == TokenType::OP) {
                if (precedence(tmp.top().token.op) >= precedence(token.op)) {
                    equation.emplace_back(tmp.top());
                    tmp.pop();
                } else break;
            }
            tmp.push(Token(type, token));
        }
    }

    while (!tmp.empty()) {
        equation.emplace_back(tmp.top());
        tmp.pop();
    }
}

std::string math::to_string(const Token& t) {
    switch (t.type) {

    case TokenType::VAR: {
        return std::string(1, t.token.var);
    }

    case TokenType::PRTH: {
        return t.token.paren == ParenthesisType::OPEN ? "(" : ")";
    }

    case TokenType::NUM: {
        return std::to_string(t.token.num);
    }

    case TokenType::OP: {
        std::string op;

        switch (t.token.op) {
        case OperatorType::ADD: {
            op = "+";
            break;
        }
        case OperatorType::SUB: {
            op = "-";
            break;
        }
        case OperatorType::MUL: {
            op = "*";
            break;
        }
        case OperatorType::DIV: {
            op = "/";
            break;
        }
        case OperatorType::EXP: {
            op = "^";
            break;
        }

        default: {
            std::cerr << "impossible enum again...";
            throw std::invalid_argument("non-existent operator rtpe");
        }
        }

        return op;
    }

    default: {
        std::cerr << "impossible enum again...";
        throw std::invalid_argument("non-existent operator rtpe");
    }
    }
}


std::string math::Equation::infix_str() const {
    std::stringstream ss;
    for (auto t : infix) {
        ss << to_string(t);
    }

    return std::move(ss).str();
}

std::string math::Equation::postfix_str() const {
    std::stringstream ss;
    for (auto t : equation) {
        ss << to_string(t);
    }

    return std::move(ss).str();
}