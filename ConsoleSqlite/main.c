//
//  main.cpp
//  sqlite_test
//
//  Created by Daniil Belov on 06.12.2021.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "third_paty/sqlite3/sqlite3.h"
#include <string.h>
#include <stdint.h>
#include "main.h"

#define true 1
#define false 0

typedef uint8_t bool;


bool TrySQLExec(sqlite3* database, char* request)
{
    char* errorMessage;
    if (sqlite3_exec(database, request, 0, 0, &errorMessage))
    {
        printf("Error occured: %s", errorMessage);
        return true;
    }
    sqlite3_free(errorMessage);
    return false;
}


void DrawTableLine(int columnsCount)
{
    for (int i = 0; i < columnsCount; i++)
    {
        printf("+");
        for (int j = 0; j < 22; j++) printf("-");
    }
    printf("+\n");
}


void DrawTableByStmt(sqlite3_stmt* stmt)
{
    int columnsCount = sqlite3_column_count(stmt);
    DrawTableLine(columnsCount);
    for (int i = 0; i < columnsCount; i++)
    {
        char* name;
        name = sqlite3_column_name(stmt, i);
        printf("| %20s ", name);
    }
    printf("| \n");
    DrawTableLine(columnsCount);
    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        for (int i = 0; i < columnsCount; i++)
        {
            int value = sqlite3_column_int(stmt, i);
            printf("| %20d ", value);
        }
        printf("|\n");
        DrawTableLine(columnsCount);
    }
}


sqlite3_stmt* TrySQLPrepareV3(sqlite3* database, char* request)
{
    sqlite3_stmt* result;
    sqlite3_prepare_v3(database, "SELECT * FROM test2", 1000, 0, &result, 0);
    return result;
}


int main(int argc, const char* argv[])
{
    sqlite3* database;
    if (sqlite3_open("test.sql", &database))
    {
        printf("Can't open databese");
    }

    TrySQLExec(database, "CREATE TABLE IF NOT EXISTS test2(columnA INTEGER, columnB INTEGER)");

    printf("Insert values to add: ");
    int a, b;
    scanf("%d%d", &a, &b);
    char insertRequest[500];
    sprintf(insertRequest, "INSERT INTO test2 VALUES('%d', '%d')", a, b);
    TrySQLExec(database, insertRequest);

    sqlite3_stmt* stmt = TrySQLPrepareV3(database, "SELECT * FROM test2");
    DrawTableByStmt(stmt);

    sqlite3_free(database);
}
