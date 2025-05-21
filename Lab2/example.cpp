/**************************************************
 * Title: DatabaseSystem - Project 2 Example
 * Summary: blah blah
 *  |Date              |Author             |Version
    |2024-12-31        |Gildong Hong       |1.0.0
**************************************************/
#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost"; // change if necessary
const char* user = "root";      // change if necessary
const char* pw = "mysql";       // change if necessary

#define MAX_LEN			13000

int main(void) {
    MYSQL* connection = NULL;
    MYSQL conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW sql_row;
    FILE* fp = fopen("CRUD.txt", "rt");    // open CRUD file.
    char line[MAX_LEN];

    if (mysql_init(&conn) == NULL)
        printf("mysql_init() error!");

    connection = mysql_real_connect(&conn, host, user, pw, NULL, 3306, (const char*)NULL, 0);   // the first NULL can be replaced to an existing db instance.
    if (connection == NULL) {
        printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }
    else {
        printf("Connection Succeed\n\n");

        while (fgets(line, sizeof(line), fp) != NULL) {
            if (!strcmp(line, "$$$\n"))      // read lines from CRUD file, '$$$' separates CREATE / DELETE parts.
                break;
            mysql_query(connection, line);
        }

        if (mysql_select_db(&conn, "project")) {
            printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        const char* query = "select * from customer";
        int state = 0;

        state = mysql_query(connection, query);
        if (state == 0) {
            sql_result = mysql_store_result(connection);

            printf("[ SELECT * FROM CUSTOMER ]\n");
            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                printf(" %s  %s  %s  %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);

            mysql_free_result(sql_result);
        }

        // comment out if you want to persist example db instance.
        while (fgets(line, sizeof(line), fp) != NULL)
            mysql_query(connection, line);			// these are DELETEs & DROPs.

        mysql_close(connection);
    }

    return 0;
}
