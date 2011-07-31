
/*
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: rdb.c 386 2011-01-04 17:16:43Z hal9000 $
 *
 */
#include "denora.h"

char *rdb_errmsg;

/*************************************************************************/

int rdb_init()
{
    int res = 0;
#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        res = db_mysql_init();
    }
    SET_SEGV_LOCATION();
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        res = db_postgre_init();
    }
    SET_SEGV_LOCATION();
#endif
    if (res) {
        db_connect();
    }
    return res;
}

/*************************************************************************/

int rdb_close()
{
    denora->do_sql = 0;
#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        if (mysql) {
            mysql_close(mysql);
        }
        mysql = NULL;
        return 1;
    }
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        db_postgre_close();
        return 1;
    }
#endif
    return 0;
}

/*************************************************************************/

int rdb_clear_table(char *table)
{
#ifdef USE_MYSQL
    static char buf[1024];
#endif

    SET_SEGV_LOCATION();

#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        if (KeepUserTable && !stricmp(table, UserTable)) {
            ircsnprintf(buf, sizeof(buf), "UPDATE %s SET online='N'",
                        table);
        } else if (KeepServerTable && !stricmp(table, ServerTable)) {
            ircsnprintf(buf, sizeof(buf), "UPDATE %s SET online='N'",
                        table);
        } else {
            ircsnprintf(buf, sizeof(buf), "TRUNCATE TABLE %s", table);
        }
        return db_mysql_query(buf);

    }
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        if (KeepUserTable && !stricmp(table, UserTable)) {
            ircsnprintf(buf, sizeof(buf), "UPDATE %s SET online='N'",
                        table);
        } else if (KeepServerTable && !stricmp(table, ServerTable)) {
            ircsnprintf(buf, sizeof(buf), "UPDATE %s SET online='N'",
                        table);
        } else {
            ircsnprintf(buf, sizeof(buf), "TRUNCATE TABLE %s", table);
        }
        return db_postgre_query(buf);
    }
#endif
    return 0;
}

/*************************************************************************/

int rdb_direct_query(char *query)
{
    SET_SEGV_LOCATION();

    if (!denora->do_sql) {
        return -1;
    }
#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        return db_mysql_query(query);
    }
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        return db_postgre_query(query);
    }
#endif
    return 0;
}

/*************************************************************************/

/* send an SQL query to the database */
int rdb_query(int i, const char *fmt, ...)
{
    va_list args;
    char buf[NET_BUFSIZE];      /* should be enough for long queries (don't underestimate :o)) */
    int res;

    if (!denora->do_sql) {
        return 0;
    }

    if (BadPtr(fmt)) {
        return 0;
    }

    va_start(args, fmt);
    ircvsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

#ifdef USE_THREADS
    if (UseThreading) {
        if (i == QUERY_HIGH) {
            res = rdb_direct_query(buf);
        } else {
            qp = AddQueueEntry(qp, buf);
            res = 1;
        }
    } else {
        res = rdb_direct_query(buf);
    }
#else
    res = rdb_direct_query(buf);
#endif
    return res;
}

/*************************************************************************/

/* escape the string */
char *rdb_escape(char *ch)
{
#ifdef USE_MYSQL
    char *result;
#endif
    char *ret = NULL;

#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        result = db_mysql_quote(ch);
        ret = sstrdup(result);
        free(result);
    }
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        result = db_postgre_quote(ch);
        ret = sstrdup(result);
        free(result);
    }
#endif
    return ret;
}

/*************************************************************************/

int rdb_insertid()
{
    SET_SEGV_LOCATION();

#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        return mysql_insert_id(mysql);
    }
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        return 0;
    }
#endif
    return 0;
}

/*************************************************************************/

char *rdb_error_msg()
{
    free(rdb_errmsg);
    rdb_errmsg = NULL;
#ifdef USE_MYSQL
    if (sqltype == SQL_MYSQL) {
        rdb_errmsg = sstrdup(mysql_error(mysql));
        return rdb_errmsg;
    }
#endif
#ifdef USE_POSTGRE
    if (sqltype == SQL_POSTGRE) {
        rdb_errmsg = sstrdup(PQerrorMessage(postgre));
    }
#endif
    return rdb_errmsg;
}

/*************************************************************************/

int rdb_check_table(char *table)
{
#ifdef USE_MYSQL
    int res = 1;
    MYSQL_RES *my_res;

    rdb_query(QUERY_HIGH, "SHOW TABLES LIKE '%s';", table);
    my_res = mysql_store_result(mysql);

    if (my_res) {
        if (mysql_num_rows(my_res) == 0) {
            res = 0;
        }
        mysql_free_result(my_res);
    } else {
        res = 0;
    }
    if (!res) {
        alog(LOG_DEBUG, "Table %s was not found", table);
    }
    return res;
#endif
    return 0;
}

/*************************************************************************/
