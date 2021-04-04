#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>
#include <stdlib.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>

template<class FIRST, class SECOND>
class Pair {
public:
	FIRST first;
	SECOND second;
};

int main() {
	// map - B-Tree (Red-Black Tree)
	//       Key -> Value
	std::map<int, std::string> id_to_name_map;
	id_to_name_map[10] = "John";
	id_to_name_map[44] = "Mary";

	id_to_name_map[10] = "Tom";

	std::cout << id_to_name_map[11] << "\n\n";

	for (auto it : id_to_name_map) {
		std::cout << it.first << " : " << it.second << "\n";
	}

	auto q = id_to_name_map.find(10);
	if (q == id_to_name_map.end()) {
		std::cout << "Not found\n";
	}
	
	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}