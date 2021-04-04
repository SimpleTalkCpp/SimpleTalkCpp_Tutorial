#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>

#include <exception>

class MyError : public std::exception {
public:
	MyError(int errorCode = -1) {
		m_errorCode = errorCode;
	}

	int m_errorCode;
};

class MyErrorOpenFile : public MyError {
};

class NonCopyable {
public:
	NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	void operator=(const NonCopyable&);
};

#define NON_COPYABLE(T) \
	T(const T&); \
	void operator=(const T&); \
//-----------

class MyFile : public NonCopyable {
public:
//	NON_COPYABLE(MyFile);

	MyFile() 
		: m_file(NULL)
	{
	}

	~MyFile() {
		close();
	}

	void open(const char* filename, const char* mode) {
		close();

		m_file = fopen(filename, mode);
		if (!m_file) {
			throw MyErrorOpenFile();
		}
	}

	void close() {
		if (m_file) {
			fclose(m_file);
			m_file = NULL;
		}
	}

	FILE* getFILE() {
		return m_file;
	}

	void move(MyFile & rhs) {
		close();
		m_file = rhs.m_file;
		rhs.m_file = NULL;
	}

private:
	//MyFile(const MyFile& rhs);
	//void operator=(const MyFile& rhs);

	// C++ 11
	//MyFile(const MyFile& rhs) = delete;
	//void operator=(const MyFile& rhs) = delete;


//	FILE* m_file = nullptr; //require C++ 11
	FILE* m_file;
};

void writeData(MyFile& file) {
	if (fprintf(file.getFILE(), "Hello\n") < 0) {
		throw MyError(-1);
	}
}

void saveFile(const char* filename) {
	//FILE* file = fopen(filename, "wb");
	//if (!file) {
	//	throw MyError(-2);
	//}

	MyFile file;
	file.open(filename, "wb");

// copy constructor, not allow
//	MyFile file2(file);
//	MyFile file2 = file; //same as above

// operator=, not allow
//	file2 = file;

	writeData(file);
	writeData(file);
	writeData(file);
	writeData(file);

	printf("\nfile %s saved\n\n", filename);
//	fclose(file);
}

void printMenu() {
	printf("-----------------------\n");
	printf("1: save file\n");
	printf("0: exit\n");
	printf("Input: ");
}

void userInput() {
	for(;;) {
		printMenu();
		int s;
		scanf("%d", &s);
		switch (s) {
			case 0: {
				return;
			}
			case 1: {
				try {
					saveFile("test.txt");
				} catch(MyError e) {
					fprintf(stderr, "error save file\n");
				}
				break;
			}
		}
	}
}

class MyObject : public NonCopyable {
public:
	void func() {
		printf("Hi\n");
	}
};

class AnotherObject {

};

template<class T>
class AutoPtr : public NonCopyable {
public:
	AutoPtr(T* p) 
		: m_p(p)
	{

	}

	~AutoPtr() {
		//free(NULL) // !! not allow 
		delete m_p; // delete NULL, OK !!
	}

	T* ptr() { return m_p; }

	T* operator->() { return m_p; }

private:
	T* m_p;
};

int main() {
	AutoPtr<MyObject> p(new MyObject());
	p.ptr()->func();
	p->func();

//----------------
	MyObject* obj = new MyObject();
	obj->func();
	delete obj;

//----------------
	AutoPtr<AnotherObject> q(new AnotherObject());

	userInput();

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}