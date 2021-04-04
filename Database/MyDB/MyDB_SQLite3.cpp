#include "MyDB.h"
#include "sqlite/sqlite3.h"

namespace MyDB {

class MySQLite3_Conn : public Conn {
public:
	MySQLite3_Conn(const char* filename);
	~MySQLite3_Conn();

	virtual void directExec(const char* sql) override;
	virtual std::unique_ptr<Stmt> onCreateStmt(const char* sql) override;

	sqlite3* conn() { return _conn; }

private:
	sqlite3* _conn = nullptr;
};

class MySQLite3_Stmt : public Stmt {
public:

	MySQLite3_Stmt(MySQLite3_Conn* conn, const char* sql) {
		const char* unusedSql = nullptr;
		if (SQLITE_OK != sqlite3_prepare_v3(conn->conn(), sql, static_cast<int>(strlen(sql)), SQLITE_PREPARE_PERSISTENT, &_stmt, &unusedSql)) {
			throw MyError("MySQLite3_Stmt");
		}

		if (unusedSql) {
			const char* c = unusedSql;
			for ( ;*c; c++) {
				if (*c == '\t') continue;
				if (*c == '\r') continue;
				if (*c == '\n') continue;
				if (*c == ' ') continue;

				throw MyError("SQLite3 doesn't support multiple SQL statement");
			}
		}
	}

	~MySQLite3_Stmt() {
		if (_stmt) {
			sqlite3_finalize(_stmt);
			_stmt = nullptr;
		}
	}

	virtual void onExec(ExecParam* params, int n) override {
		if (!_stmt)
			throw MyError("_stmt is null");

		if (n != sqlite3_bind_parameter_count(_stmt))
			throw MyError("incorrect parameter count");

		for (int i = 0; i < n; i++) {
			const auto& p = params[i];
			int idx = i + 1;

			switch (p.type)
			{
				case DataType::Int8:	sqlite3_bind_int	(_stmt, idx, *(int8_t *)p.value); break;
				case DataType::Int16:	sqlite3_bind_int	(_stmt, idx, *(int16_t*)p.value); break;
				case DataType::Int32:	sqlite3_bind_int	(_stmt, idx, *(int32_t*)p.value); break;
				case DataType::Int64:	sqlite3_bind_int64	(_stmt, idx, *(int64_t*)p.value); break;

				case DataType::UInt8:	sqlite3_bind_int	(_stmt, idx, *(uint8_t *)p.value); break;
				case DataType::UInt16:	sqlite3_bind_int	(_stmt, idx, *(uint16_t*)p.value); break;
				case DataType::UInt32:	sqlite3_bind_int64	(_stmt, idx, *(uint32_t*)p.value); break; // int64 can handle uint32
				//case DataType::UInt64:	sqlite3_bind_int64	(_stmt, idx, *(uint64_t*)p.value); break; // int64 might not able to handle uint64

				case DataType::Float:	sqlite3_bind_double	(_stmt, idx, *(float  *)p.value); break;
				case DataType::Double:	sqlite3_bind_double	(_stmt, idx, *(double *)p.value); break;
				case DataType::c_str: {
					auto* s = (const char*)p.value;
					sqlite3_bind_text	(_stmt, idx, s, (int)strlen(s), nullptr);
				}break;
				case DataType::string: {
					auto* s = (std::string*)p.value;
					sqlite3_bind_text	(_stmt, idx, s->c_str(), (int)s->size(), nullptr);
				}break;

				default: throw MyError("Unknown param type");
			}
		}

		_step();
	}

	void _step() {
		_endOfRows = true;
		int ret = sqlite3_step(_stmt);
		if (ret == SQLITE_DONE) {
			return;
		} else if (ret == SQLITE_ROW) {
			_endOfRows = false;
		} else {
			throw MyError("fetch");
		}
	}

	virtual int resultFieldCount() override {
		if (!_stmt)
			throw MyError("_stmt is null");
		return sqlite3_column_count(_stmt);
	}

	virtual const char* resultFieldName(int i) override {
		if (!_stmt)
			throw MyError("_stmt is null");
		return sqlite3_column_name(_stmt, i);
	}

	virtual bool isFieldNull(int i) override {
		if (!_stmt)
			throw MyError("_stmt is null");
		return sqlite3_column_type(_stmt, i) == SQLITE_NULL;
	}

	template<typename T>
	void _column_int(ResultField& f, int col) {
		auto & dst = *(T*)f.value;
		auto t = sqlite3_column_type(_stmt, col);

		if (t == SQLITE_NULL) {
			dst = 0;
			return;
		}

		if (t == SQLITE_INTEGER) {
			auto v = sqlite3_column_int64(_stmt, col);
			dst = static_cast<T>(v);
			if (dst != v) {
				throw MyError("Field overflow");
			}
			return;
		}
		
		throw MyError("invalid field type");
	}

	template<typename T>
	void _column_double(ResultField& f, int col) {
		auto & dst = *(T*)f.value;
		auto t = sqlite3_column_type(_stmt, col);

		if (t == SQLITE_NULL) {
			dst = 0;
			return;
		}

		if (t == SQLITE_FLOAT) {
			auto v = sqlite3_column_double(_stmt, col);
			dst = static_cast<T>(v);
			if (dst != v) {
				throw MyError("Field overflow");
			}
			return;
		}
		
		throw MyError("invalid field type");
	}

	virtual bool onFetch(ResultField* fields, int n) override {
		if (_endOfRows)
			return false;

		if (n != sqlite3_column_count(_stmt))
			throw MyError("incorrect result fields");

		for (int i = 0; i < n; i++) {
			auto& f = fields[i];

			switch (f.type) {
				case DataType::Int8:	_column_int<int8_t >(f,i); break;
				case DataType::Int16:	_column_int<int16_t>(f,i); break;
				case DataType::Int32:	_column_int<int32_t>(f,i); break;
				case DataType::Int64:	_column_int<int64_t>(f,i); break;

				case DataType::UInt8:	_column_int<uint8_t >(f,i); break;
				case DataType::UInt16:	_column_int<uint16_t>(f,i); break;
				case DataType::UInt32:	_column_int<uint32_t>(f,i); break;
				case DataType::UInt64:	_column_int<uint64_t>(f,i); break;

				case DataType::Float:	_column_double<float >(f,i); break;
				case DataType::Double:	_column_double<double>(f,i); break;

				case DataType::string: {
					auto & dst = *(std::string*)f.value;
					auto t = sqlite3_column_type(_stmt, i);
					if (t == SQLITE_NULL) {
						dst.clear();
					} else if (t == SQLITE_TEXT) {
						dst = reinterpret_cast<const char*>(sqlite3_column_text(_stmt, i));
					} else {
						throw MyError("invalid field type");
					}
				}break;

				default: throw MyError("fetch unsupported data type");
			}
		}

		_step();
		return true;
	}

	virtual void onReset() override {
		sqlite3_reset(_stmt);
	}

private:
	sqlite3_stmt* _stmt = nullptr;
	bool _endOfRows = false;
};

std::unique_ptr<Stmt> MySQLite3_Conn::onCreateStmt(const char* sql) {
	return std::unique_ptr<Stmt>(new MySQLite3_Stmt(this, sql));
}

MySQLite3_Conn::MySQLite3_Conn(const char* filename) {
	if (SQLITE_OK != sqlite3_open_v2(filename, &_conn, SQLITE_OPEN_READWRITE, nullptr)) {
		throw MyError("MySQLite3_Conn");
	}
}

MySQLite3_Conn::~MySQLite3_Conn() {
	if (_conn) {
		 sqlite3_close(_conn);
		 _conn = nullptr;
	}
}

void MySQLite3_Conn::directExec(const char* sql) {
	if (!_conn)
		throw MyError("_conn is null");
		
	char* errmsg = nullptr;
	if (SQLITE_OK != sqlite3_exec(_conn, sql, nullptr, nullptr, &errmsg)) {
		throw MyError(errmsg);
	}
}

std::unique_ptr<Conn> connectSQLite3(const char* filename) {
	return std::unique_ptr<Conn>(new MySQLite3_Conn(filename));
}

}