#include <main.hpp>

using tk = math::Token;
using enum math::OperatorType;
using enum math::ParenthesisType;

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    const math::Equation eq{tk('x'), tk(MUL),
    tk(OPEN),
    tk(5), tk(SUB), tk(7),
    tk(CLOSED)};

    std::cout << eq.infix_str() << std::endl;
    std::cout << eq.postfix_str() << std::endl;
    std::cout << eq.evaluate({{'x', 3}}).value << std::endl;

    Window window;
    window.show();

    return QApplication::exec();
}