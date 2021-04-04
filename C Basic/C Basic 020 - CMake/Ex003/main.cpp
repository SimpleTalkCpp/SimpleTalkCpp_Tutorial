#include <iostream>

#include <fmt/format.h>

int main() {
	printf("Hello World\n");
	std::string s = fmt::format("I'd rather be {1} than {0}.", "right", "happy");
	std::cout << s;
}