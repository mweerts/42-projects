#include "RPN.hpp"
#include <iostream>


RPN::RPN() {}

RPN::RPN(const RPN& other) {
	*this = other;
}

RPN& RPN::operator=(const RPN& other) {
	if (this != &other) {
	}
	return *this;
}

RPN::~RPN() {}

void RPN::processInput(const std::string& input) {
	std::stack<int> _stack;

	for (size_t i = 0; i < input.length(); ++i) {
		char token = input[i];

		if (std::isspace(token)) {
			continue;
		} else if (isdigit(token)) {
			_stack.push(token - '0');
		} else if (token == '+' || token == '-' || token == '*' || token == '/') {
			if (_stack.size() < 2) {
				std::cerr << "Error" << std::endl;
				return;
			}

			int b = _stack.top();
			_stack.pop();
			int a = _stack.top();
			_stack.pop();

			int result;
			switch (token) {
				case '+':
					result = a + b;
					break;
				case '-':
					result = a - b;
					break;
				case '*':
					result = a * b;
					break;
				case '/':
					if (b == 0) {
						std::cerr << "Error" << std::endl;
						return;
					}
					result = a / b;
					break;
			}
			_stack.push(result);
		} else {
			std::cerr << "Error" << std::endl;
			return;
		}
	}

	if (_stack.size() != 1) {
		std::cerr << "Error" << std::endl;
		return;
	}

	std::cout << _stack.top() << std::endl;
}