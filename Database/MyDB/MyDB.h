#pragma once

#include "my_common.h"

#ifdef MYDB_EXPORTS
	#define MyDB_API __declspec(dllexport)
#else
	#define MyDB_API __declspec(dllimport)
	#pragma comment(lib, "MyDB.lib")
#endif

namespace MyDB {

enum class DataType {
	Unknown,
	Int8,
	Int16,
	Int32,
	Int64,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Float,
	Double,
	c_str,
	string,
};

class Stmt : public MyNonCopyable {
public:
	class ExecParam {
	public:
		ExecParam(const int8_t  & v)	{ type = DataType::Int8;   value = &v; }
		ExecParam(const int16_t & v)	{ type = DataType::Int16;  value = &v; }
		ExecParam(const int32_t & v)	{ type = DataType::Int32;  value = &v; }
		ExecParam(const int64_t & v)	{ type = DataType::Int64;  value = &v; }
		ExecParam(const uint8_t & v)	{ type = DataType::UInt8;  value = &v; }
		ExecParam(const uint16_t& v)	{ type = DataType::UInt16; value = &v; }
		ExecParam(const uint32_t& v)	{ type = DataType::UInt32; value = &v; }
		ExecParam(const uint64_t& v)	{ type = DataType::UInt64; value = &v; }
		ExecParam(const float   & v)	{ type = DataType::Float;  value = &v; }
		ExecParam(const double  & v)	{ type = DataType::Double; value = &v; }
		ExecParam(const std::string& v) { type = DataType::string; value = &v; }
		ExecParam(const char*     v)	{ type = DataType::c_str;  value = v; }

		DataType type;
		const void* value = nullptr;
	};

	template<typename... ARGS>
	void exec(ARGS&&... args) {
		reset();
		ExecParam p[] = {args...};
		onExec(p, sizeof...(args)); 
	}

	void exec() {
		reset();
		onExec(nullptr, 0);
	}

	virtual int resultFieldCount() = 0;
	virtual const char* resultFieldName(int i) = 0;

	virtual bool isFieldNull(int i) = 0;

	class ResultField {
	public:
		ResultField(int8_t  & v)	{ type = DataType::Int8;   value = &v; }
		ResultField(int16_t & v)	{ type = DataType::Int16;  value = &v; }
		ResultField(int32_t & v)	{ type = DataType::Int32;  value = &v; }
		ResultField(int64_t & v)	{ type = DataType::Int64;  value = &v; }
		ResultField(uint8_t & v)	{ type = DataType::UInt8;  value = &v; }
		ResultField(uint16_t& v)	{ type = DataType::UInt16; value = &v; }
		ResultField(uint32_t& v)	{ type = DataType::UInt32; value = &v; }
		ResultField(uint64_t& v)	{ type = DataType::UInt64; value = &v; }
		ResultField(float   & v)	{ type = DataType::Float;  value = &v; }
		ResultField(double  & v)	{ type = DataType::Double; value = &v; }
		ResultField(std::string& v) { type = DataType::string; value = &v; }

		DataType type;
		void* value = nullptr;
	};

	template<typename... ARGS>
	bool fetch(ARGS&&... args) {
		ResultField p[] = {args...};
		return onFetch(p, sizeof...(args)); 
	}

	bool fetch() {
		return onFetch(nullptr, 0);
	}

	void reset() { onReset(); }

protected:
	virtual void onExec(ExecParam* p, int n) = 0;
	virtual bool onFetch(ResultField* p, int n) = 0;
	virtual void onReset() = 0;
};

// connection interface
class Conn : public MyNonCopyable {
public:
	virtual ~Conn() {}
	virtual void directExec(const char* sql) = 0;
	
	std::unique_ptr<Stmt> createStmt(const char* sql) { return onCreateStmt(sql); }

protected:
	virtual std::unique_ptr<Stmt> onCreateStmt(const char* sql) = 0;
};

MyDB_API std::unique_ptr<Conn> connectMySQL(const char* host, const char* db, const char* user, const char* password);
MyDB_API std::unique_ptr<Conn> connectSQLite3(const char* filename);

}