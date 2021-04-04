#include "Shape.h"

const TypeInfo* Shape::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
			static FieldInfo fi[] = {
				FieldInfo("x", &This::x),
				FieldInfo("y", &This::y),
			};
			setFields(fi);
		}
	};
	static TI ti;
	return &ti;
}

const TypeInfo* Circle::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
			static FieldInfo fi[] = {
				FieldInfo("radius", &This::radius),
			};
			setFields(fi);
		}
	};
	static TI ti;
	return &ti;
}

const TypeInfo* Rect::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
			static FieldInfo fi[] = {
				FieldInfo("width", &This::width),
				FieldInfo("height", &This::height),
			};
			setFields(fi);
		}
	};
	static TI ti;
	return &ti;
}
