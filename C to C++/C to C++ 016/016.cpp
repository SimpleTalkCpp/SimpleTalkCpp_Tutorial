#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>
#include <memory>
#include <iostream>

template<class T>
class SharedPtr {
public:
	SharedPtr(T* p = nullptr) {
		reset(p);
	}

	SharedPtr(const SharedPtr& rhs) {
		auto& r = const_cast<SharedPtr&>(rhs);
		reset(r.ptr());
	}

	~SharedPtr() {
		reset(nullptr);
	}

	void operator=(const SharedPtr& rhs) {
		auto& r = const_cast<SharedPtr&>(rhs);
		reset(r.ptr());
	}

	void reset(T* p) {
		if (m_ptr) {
			assert(m_ptr->refCount > 0);
			m_ptr->refCount--;
			std::cout << "refCount = " << m_ptr->refCount << "\n";

			if (m_ptr->refCount == 0) {
				delete m_ptr;
			}
		}
		m_ptr = p;

		if (m_ptr) {
			m_ptr->refCount++;
			std::cout << "refCount = " << m_ptr->refCount << "\n";
		}
	}

	T* ptr()       { return m_ptr; }

	T* operator->() { return m_ptr; }
	T& operator*() { return *m_ptr; }

	explicit operator bool() { return m_ptr != nullptr; }

private:
	T* m_ptr = nullptr;
};

template<class T>
class WeakPtr {
public:

	SharedPtr<T> toSharedPtr() {
		if (m_ptr && m_ptr->refCount > 0) {
			return SharedPtr<T>(m_ptr);
		}
		return SharedPtr<T>(nullptr);
	}

private:
	T* m_ptr = nullptr;
};


class MyImage {
public:
	~MyImage() {
		std::cout << "~MyImage\n";
	}

	void draw() {
	}

	int refCount = 0;
};

class MyButton {
public:
	~MyButton() {
		std::cout << "~MyButton\n";
	}

	void func() {
		if (auto sp = anotherButton.toSharedPtr()) {
			sp->doSometing();
		}
	}

	void doSometing() {
	}

	SharedPtr<MyImage> image;
	WeakPtr<MyButton> anotherButton;

	int refCount = 0;
};

class MyWindow {
public:

	SharedPtr<MyButton> buttonA, buttonB;
};

void test_example() {
	MyWindow win;
	win.buttonA.reset(new MyButton());
	win.buttonB.reset(new MyButton());

 	SharedPtr<MyImage> img(new MyImage());

	win.buttonA->image = img;
	win.buttonB->image = img;
}

void func(const std::shared_ptr<MyButton> & p) {
	//-----------
}

int main() {
	test_example();

	std::shared_ptr<MyButton> button(new MyButton());

	func(button);


	std::weak_ptr<MyButton> wp;

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}