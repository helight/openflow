#ifndef DB_CONFIG_H
#define DB_CONFIG_H
#pragma once

#include <sqlite3.h>

namespace common {

/**
 * for database
 */

enum DB_TYPE {
    DB_SQLITE,
    DB_MYSQL
};

} // end namespace common

#endif // DB_CONFIG_H
