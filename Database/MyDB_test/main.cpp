#include "../MyDB/MyDB.h"
#include <conio.h>

struct Row {
	int id;
	std::string name;
	int count;
	double weight;

	void print() {
		printf("id=%d name=%s count=%d weight=%g\n", id, name.c_str(), count, weight);
	}
};

int main() {

	auto conn = MyDB::connectMySQL("localhost", "test_db", "test_user", "1234");
//	auto conn = MyDB::connectSQLite3("test.sqlite3");

//	conn->directExec("INSERT INTO test_table (`name`, `count`, `weight`) VALUES ('cccc', '300', '5.12');");

//	auto stmt = conn->createStmt("INSERT INTO test_table (`name`, `count`, `weight`) VALUES (?,?,?);");
//	stmt->exec("kkkk", 150, 4.44);

	auto stmt = conn->createStmt("select id, name, count, weight from test_table;");

	int n = stmt->resultFieldCount();
	for (int i = 0; i<n; i++) {
		printf("field[%d] %s\n", i, stmt->resultFieldName(i));
	}

	stmt->exec();
	Row r;
	while (stmt->fetch(r.id, r.name, r.count, r.weight)) {
		r.print();

		for (int i=0; i<4; i++) {
			if (stmt->isFieldNull(i)) {
				printf("field[%d] isNull\n", i);
			}
		}

	}
	stmt->reset();

	printf("\n==== Program Ended ======\n");
	_getch();

	return 0;
}