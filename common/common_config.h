#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H
#pragma once

namespace common {

/**
 * for database
 */
#define USING_SQLITE_DB

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

#endif // COMMON_CONFIG_H
