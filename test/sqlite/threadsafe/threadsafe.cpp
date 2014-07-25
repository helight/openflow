/**
 * test if sqlite configure to threadsafe.
 */

#include <stdio.h>
#include <iostream>
#include <sqlite3.h>

int main(int argc, char **argv){
    sqlite3 *db;
    int rc;

    if( argc!=1 ){
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return(1);
    }

    rc = sqlite3_threadsafe();

    switch(rc){
        case 0:
            std::cout << "the SQLITE_THREADSAFE compile-time option being set to 0" << std::endl;
            break;
        case 1:
            std::cout << "the SQLITE_THREADSAFE compile-time option being set to 1" << std::endl;
            break;
        case 2:
            std::cout << "the SQLITE_THREADSAFE compile-time option being set to 2" << std::endl;
            break;
        default:
            std::cout << "unknown value." << std::endl;
    }

    return 0;
}
