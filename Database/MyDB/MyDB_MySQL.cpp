#include "MyDB.h"

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

namespace MyDB {

class MySQL_Conn : public Conn {
public:
	MySQL_Conn(const char* host, const char* db, const char* user, const char* password);
	~MySQL_Conn();

	virtual void directExec(const char* sql) override;
	virtual std::unique_ptr<Stmt> onCreateStmt(const char* sql) override;

	MYSQL* conn() { return _conn; }

private:
	MYSQL* _conn = nullptr;
};

class MySQL_Stmt : public Stmt {
public:
	MySQL_Stmt(MySQL_Conn* conn, const char* sql) {
		_stmt = mysql_stmt_init(conn->conn());
		if (!_stmt)
			throw MyError("MySQL_Stmt");

		if (0 != mysql_stmt_prepare(_stmt, sql, (unsigned long)strlen(sql))) {
			throw MyError(mysql_stmt_error(_stmt));
		}
	}

	~MySQL_Stmt() {
		if (_stmt) {
			mysql_stmt_close(_stmt);
			_stmt = nullptr;
		}
	}

	void onExec(ExecParam* params, int n) {
		if (!_stmt)
			throw MyError("_stmt is null");

		if (n != mysql_stmt_param_count(_stmt))
			throw MyError("incorrect parameter count");

		_bind.resize(n);

		for (int i = 0; i < n; i++) {
			const auto& p = params[i];
			auto& b = _bind[i];
			my_bzero(b);
			switch (p.type)
			{
				case DataType::Int8:	b.buffer_type = MYSQL_TYPE_TINY;		b.buffer = (void*)p.value; break;
				case DataType::Int16:	b.buffer_type = MYSQL_TYPE_SHORT;		b.buffer = (void*)p.value; break;
				case DataType::Int32:	b.buffer_type = MYSQL_TYPE_LONG;		b.buffer = (void*)p.value; break;
				case DataType::Int64:	b.buffer_type = MYSQL_TYPE_LONGLONG;	b.buffer = (void*)p.value; break;

				case DataType::UInt8:	b.buffer_type = MYSQL_TYPE_TINY;		b.buffer = (void*)p.value; b.is_unsigned = true; break;
				case DataType::UInt16:	b.buffer_type = MYSQL_TYPE_SHORT;		b.buffer = (void*)p.value; b.is_unsigned = true; break;
				case DataType::UInt32:	b.buffer_type = MYSQL_TYPE_LONG;		b.buffer = (void*)p.value; b.is_unsigned = true; break;
				case DataType::UInt64:	b.buffer_type = MYSQL_TYPE_LONGLONG;	b.buffer = (void*)p.value; b.is_unsigned = true; break;

				case DataType::Float:	b.buffer_type = MYSQL_TYPE_FLOAT;		b.buffer = (void*)p.value; break;
				case DataType::Double:	b.buffer_type = MYSQL_TYPE_DOUBLE;		b.buffer = (void*)p.value; break;

				case DataType::c_str: {
					auto* s = (const char*)p.value;
					b.buffer_type = MYSQL_TYPE_STRING;
					b.buffer = (void*)s;
					b.buffer_length = (unsigned long)strlen(s);
				}break;

				case DataType::string: {
					auto* s = (std::string*)p.value;
					b.buffer_type = MYSQL_TYPE_STRING;
					b.buffer = (void*)s->c_str();
					b.buffer_length = (unsigned long)s->size();
				}break;


				default: throw MyError("Unknown param type");
			}
		}

		if (0 != mysql_stmt_bind_param(_stmt, _bind.data())) {
			throw MyError("mysql_stmt_bind_param");
		}

		if (0 != mysql_stmt_execute(_stmt)) {
			throw MyError(mysql_stmt_error(_stmt));
		}
	}

	virtual int resultFieldCount() override {
		if (!_stmt)
			throw MyError("_stmt is null");

		auto meta = mysql_stmt_result_metadata(_stmt);
		return mysql_num_fields(meta);
	}

	virtual const char* resultFieldName(int i) override {
		if (!_stmt)
			throw MyError("_stmt is null");

		auto meta = mysql_stmt_result_metadata(_stmt);
		mysql_field_seek(meta, i);
		auto f = mysql_fetch_field(meta);
		return f->name;
	}

	virtual bool isFieldNull(int i) {
		if (i < 0 || i >= _isNull.size())
			throw MyError("out of range");
		return _isNull[i];
	}

	template<typename T>
	void _bind_number_result(MYSQL_BIND& b, enum enum_field_types type, ResultField& f) {
		b.buffer_type = type;
		b.buffer = (void*)f.value; 
		b.is_unsigned = std::is_unsigned<T>::value;

		auto* dst = (T*)f.value;
		*dst = 0;
	}

	virtual bool onFetch(ResultField* fields, int n) override {

		if (n != (int)mysql_stmt_field_count(_stmt)) {
			throw MyError("incorrect field count");
		}

		_bind.resize(n);
		_resultLen.resize(n);
		_isNull.resize(n);

		for (int i = 0; i < n; i++) {
			auto& p = fields[i];
			auto& b = _bind[i];
			my_bzero(b);
			b.is_null = &_isNull[i];

			switch (p.type)
			{
				case DataType::Int8:	_bind_number_result<int8_t  >(b, MYSQL_TYPE_TINY,     p); break;
				case DataType::Int16:	_bind_number_result<int16_t >(b, MYSQL_TYPE_SHORT,    p); break;
				case DataType::Int32:	_bind_number_result<int32_t >(b, MYSQL_TYPE_LONG,     p); break;
				case DataType::Int64:	_bind_number_result<int64_t >(b, MYSQL_TYPE_LONGLONG, p); break;

				case DataType::UInt8:	_bind_number_result<uint8_t >(b, MYSQL_TYPE_TINY,     p); break;
				case DataType::UInt16:	_bind_number_result<uint16_t>(b, MYSQL_TYPE_SHORT,    p); break;
				case DataType::UInt32:	_bind_number_result<uint32_t>(b, MYSQL_TYPE_LONG,     p); break;
				case DataType::UInt64:	_bind_number_result<uint64_t>(b, MYSQL_TYPE_LONGLONG, p); break;

				case DataType::Float:	_bind_number_result<float   >(b, MYSQL_TYPE_FLOAT,    p); break;
				case DataType::Double:	_bind_number_result<double  >(b, MYSQL_TYPE_DOUBLE,   p); break;

				case DataType::string:	{
					auto& dst = *(std::string*)p.value;
					dst.clear();
					b.length = &_resultLen[i]; // mysql_stmt_fetch_column later
				}break;
			}			
		}

		mysql_stmt_bind_result(_stmt, _bind.data());
		int ret = mysql_stmt_fetch(_stmt);
		if (ret == 0 || ret == MYSQL_DATA_TRUNCATED) {

			for (int i = 0; i < n; i++) {
				const auto& p = fields[i];
				auto& b = _bind[i];

				switch (p.type) {
					case DataType::string: {
						auto len = _resultLen[i];
						_tmp.resize(len);
						b.buffer_type = MYSQL_TYPE_STRING;
						b.buffer = _tmp.data();
						b.buffer_length = (unsigned long)_tmp.size();
						mysql_stmt_fetch_column(_stmt, &b, i, 0);

						auto& dst = *(std::string*)p.value;
						dst.assign(_tmp.data(), len);
					}break;
				}
			}

			return true;
		}

		if (ret == MYSQL_NO_DATA) 
			return false;
		
		throw MyError("fetch error");
	}

	virtual void onReset() override {
		mysql_stmt_free_result(_stmt);
	}

private:
	MYSQL_STMT* _stmt = nullptr;
	std::vector<MYSQL_BIND> _bind;
	std::vector<unsigned long> _resultLen;
	std::vector<my_bool> _isNull;
	std::vector<char> _tmp;
};

std::unique_ptr<Conn> connectMySQL(const char* host, const char* db, const char* user, const char* password) {
	return std::unique_ptr<Conn>(new MySQL_Conn(host, db, user, password));
}

std::unique_ptr<MyDB::Stmt> MySQL_Conn::onCreateStmt(const char* sql) {
	return std::unique_ptr<Stmt>(new MySQL_Stmt(this, sql));
}

MySQL_Conn::MySQL_Conn(const char* host, const char* db, const char* user, const char* password) {
	_conn = mysql_init(nullptr);
	if (!_conn)
		throw MyError("mysql_init");

	if (nullptr == mysql_real_connect(_conn, host, user, password, db, 0, nullptr, 0)) {
		throw MyError(mysql_error(_conn));
	}
}

MySQL_Conn::~MySQL_Conn() {
	if (_conn) {
		mysql_close(_conn);
	}
}

void MySQL_Conn::directExec(const char* sql) {
	if (!_conn) 
		throw MyError("directExec _conn is null");

	if (mysql_query(_conn, sql)) {
		throw MyError("mysql_query");
	}
}


}