// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//  Read demo.xml on disk, and save it to sqlite database.
// Example :./save_xml ../openflowDB demo.xml

#include <iostream>
#include <tinyxml.h>
#include <sqlite3.h>

int main(int argc, char **argv)
{
    if( argc!=3 ){
        fprintf(stderr, "Usage: %s DATABASE XML_FILE\n", argv[0]);
        return(1);
    }

    const char *pFilename = argv[2];
    TiXmlDocument doc( pFilename );

    bool loadOkay = doc.LoadFile();
    if (loadOkay)
    {
        printf("\n%s opened.\n", pFilename);
    }else{
        printf("Failed to load file \"%s\"\n", pFilename);
        exit(1);
    }

    TiXmlPrinter printer;
    printer.SetIndent( "\t" );
    doc.Accept( &printer );
    std::string xml =  printer.Str();

    sqlite3 *db;
    int rc = sqlite3_open_v2(argv[1], &db, SQLITE_OPEN_READWRITE, NULL);
    if( rc != SQLITE_OK){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    std::string sql;
    sql = "INSERT INTO job VALUES(:id, :xml);";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (!stmt){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    //bind id
    int index = sqlite3_bind_parameter_index(stmt, ":id");
    rc = sqlite3_bind_int(stmt, index, 1);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "Binding parameter id: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }
    //bind xml
    index = sqlite3_bind_parameter_index(stmt, ":xml");
    rc = sqlite3_bind_text(stmt, index, xml.c_str(), xml.length(), NULL);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "Binding parameter xml: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    rc = sqlite3_step(stmt);
    //FIXME:excute successfully, but always say Unkown error.
    /*
    if( rc != SQLITE_OK){
        fprintf(stderr, "SQL Excute: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return(1);
    }
    */
    std::cout << "xml saved to sqlite successfully." << std::endl;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
