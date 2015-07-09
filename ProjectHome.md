This is a simple to use c++ wrapper for sqlite3.
It is composed of two files **sqdb.h** and **sqdb.cpp**.

Features
  * no external dependencies
  * tested with win32 and linux
  * supports unicode/UTF-16
### Create a table if it doesn't exist ###
```
#include "sqdb.h"
int main()
{
  sqdb::Db db("db.sdb");
  if ( !db.TableExists("t") )
  {
    db.Query("create table t(i int);").Next();
  }
}
```
### Query database ###
```
#include "sqdb.h"
int main()
{
  sqdb::Db db("db.sdb");
  // Select all tuples from table t with two columns integer and float.
  Statement s = db.Query("select * from t;");
  while ( s.Next() )
  {
    int i = s.GetField(0);    // Get the first column.
    double d = s.GetField(1); // Get second column.
  }
}
```