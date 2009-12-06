#ifndef SQDB_SQDB_H
#define SQDB_SQDB_H

#include <string>

#include "sqlite3.h"

namespace sqdb
{

class Exception
{
public:
  Exception(sqlite3* db);

  Exception(sqlite3* db, int errorCode);

  Exception(const char* errorMsg);

  ~Exception();

  int GetErrorCode() const;

  const char* GetErrorMsg() const;
private:
  int m_errorCode;
  char* m_errorMsg;
};

#define CHECK(db, returnCode) \
  if ( (returnCode) != SQLITE_OK ) throw Exception(db, returnCode) 

class RefCount
{
protected:
  RefCount();

  RefCount(const RefCount& x);
  RefCount& operator=(const RefCount& x);

  void IncRef();
  unsigned DecRef();

private:
  unsigned* m_refCount;
};

class Blob : public RefCount
{
public:
  Blob(const void* data, int size);

  Blob(const Blob& x);
  Blob& operator=(const Blob& x);

  int GetSize() const;
  const char* GetData() const;

  ~Blob();

private:
  char* m_data;
  int m_size;
};

class Convertor
{
public:
  Convertor(sqlite3* db, sqlite3_stmt* stmt, int field);

  operator int() const;
  operator long long() const;
  operator double() const;
  operator std::string() const;
  operator const char*() const;
  operator Blob() const;

  int GetInt() const;
  long long GetLongLong() const;
  double GetDouble() const;
  std::string GetString() const;
  const char* GetText() const;
  Blob GetBlob() const;

private:
  sqlite3* m_db;
  sqlite3_stmt* m_stmt;
  int m_field;
};

class Statement : public RefCount
{
public:
  Statement(sqlite3* db, sqlite3_stmt* stmt);

  Statement(const Statement& x);
  Statement& operator=(const Statement& x);

  bool Next();
  Convertor GetField(int field) const;

  template<class T>
  void Bind(int i, const T& value)
  {
    if ( m_needReset ) 
      Reset();
    DoBind(i, value);
  }

  void BindBlob(int i, const void* value, int n);
  void BindNull(int i);

  ~Statement();

private:
  void DoBind(int i, int value); 
  void DoBind(int i, long long value); 
  void DoBind(int i, double value);
  void DoBind(int i, const std::string& value);
  void DoBind(int i, const char* value);

  // Bind blob.
  void DoBind(int i, const void* value, int n);

  // Bind null.
  void DoBind(int i);

  // Reset binders so that new values can be bound.
  void Reset();

  sqlite3* m_db;
  sqlite3_stmt* m_stmt;
  bool m_needReset;
};

class QueryStr
{
public:
  QueryStr();

  const char* Format(const char* fmt, ...);

  const char* Get() const;

  ~QueryStr();

private:
  char* m_buf;
};

class Db : public RefCount
{
public:
  Db(const char* fileName);

  void BeginTransaction();
  void CommitTransaction();
  void RollbackTransaction();

  bool TableExists(const char* tableName);
  Statement Query(const char* queryStr);
  long long LastId();

  Db(const Db& x);
  Db& operator=(const Db& x);

  ~Db();

private:
  sqlite3* m_db;
};

}

#endif

