#ifndef DB_CONFIG_H
#define DB_CONFIG_H
#pragma once

#include <sqlite3.h>

namespace common {

/**
 * for database
 */
#define USING_SQLITE_DB 1

#if USING_SQLITE_DB
    typedef sqlite3 db_id_t;
#elif USING_MYSQL_DB
    //for mysql
#endif

enum DB_TYPE {
    DB_SQLITE,
    DB_MYSQL
};

} // end namespace common

#endif // DB_CONFIG_H
