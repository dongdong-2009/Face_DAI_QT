#include "database.h"


face_database_api::face_database_api(void)
{
    m_pFaceStmt = mpNULL;
    face_database_init();
}

face_database_api::~face_database_api()
{
    face_database_shutdown();
}

int face_database_api::face_database_init(void)
{
    PRT_INFO("\n");
    if(sqlite3_open(SQLITE_DB_NAME, &face_db) != SQLITE_OK)
    {
        // check whether we need to have it in persistent memory or not
        PRT_INFO("Can't open database face_db: %s\n", sqlite3_errmsg(face_db));
        sqlite3_close(face_db);
        return -1;
    }

    MAKE_DB_QUERY(face_db,
                "Error in Creating the key_name table(Err): %s\n", -1, NULL, NULL,
                "CREATE TABLE IF NOT EXISTS face_feature"
                "(key_name varchar, "
                "key_data BLOB);");
    return 0;
}


int face_database_api::face_database_insert_feature(const char *kname, float *kdata, int nSize)
{
    if(!face_db) {
      PRT_INFO("Database has not been initialized\n");
      return -1;
    }
    if(!kname || !kdata) {
      PRT_INFO("kname or kdata cannot be NULL\n");
      return -1;
    }
    mpINT iRes =0;
    sqlite3_stmt *stmt = mpNULL;
    char *query_statement = mpNULL;
    do{
            MAKE_DB_QUERY(face_db,
                      "Deletion ERROR: %s\n", 0, NULL, NULL,
                      "DELETE FROM face_feature WHERE key_name = %Q", kname);//delete record data if exist.
            query_statement = sqlite3_mprintf("insert into face_feature (key_name,key_data) values(%Q,?);",kname);
            if (query_statement == mpNULL)
            {
                 PRT_INFO("query_statement cannot be NULL\n");
                 iRes = -1;
                 break;
            }
            if (sqlite3_prepare(face_db, query_statement, strlen(query_statement), &stmt, NULL) != SQLITE_OK)
            {
                PRT_INFO("ERROR:query preparation failed - %s\n", sqlite3_errmsg(face_db));
                iRes = -1;
                break;
            }

            if (sqlite3_bind_blob(stmt, 1, (float*)kdata, nSize, NULL) != SQLITE_OK)
            {
                PRT_INFO("ERROR:query bind blod failed - %s\n", sqlite3_errmsg(face_db));
                iRes = -1;
                break;
            }
            sqlite3_step(stmt);//exec insert command
            PRT_INFO("insert sucess!\n");
    }while(mpFALSE);

    if(query_statement != mpNULL)
    {
        sqlite3_free(query_statement);
        query_statement = mpNULL;
    }

    if(stmt != mpNULL)
    {
        sqlite3_finalize(stmt);
        stmt = mpNULL;
    }
    return iRes;
}

bool face_database_api::face_database_inquire_prepare()
{
    bool bRet = false;
    if(!face_db) {
        PRT_INFO("face_db cannot be NULL\n");
        return bRet;
    }

    char *query_statement = NULL;
    query_statement = sqlite3_mprintf("SELECT * FROM face_feature;");
    if (query_statement == NULL) return -1;
    if (sqlite3_prepare(face_db, query_statement, strlen(query_statement), &m_pFaceStmt, NULL) != SQLITE_OK)
    {
        PRT_INFO("ERROR:query preparation failed - %s\n", sqlite3_errmsg(face_db));
        sqlite3_free(query_statement);
        query_statement = NULL;
        return false;
    }

    sqlite3_free(query_statement);
    query_statement = NULL;
    return true;
}

SQLITE_API int face_database_api::face_database_inquire_step()
{
    if(!face_db)
    {
        PRT_INFO("face_db cannot be NULL\n");
        return -1;
    }
    if(!m_pFaceStmt)
    {
        PRT_INFO("m_pFaceStmt cannot be NULL\n");
        return -1;
    }
    return sqlite3_step(m_pFaceStmt);
}

int face_database_api::face_database_inquire_info(face_database_feature_info_t *face_info)
{
    if(!face_db) {
        PRT_INFO("face_db cannot be NULL\n");
        return -1;
    }
    if(!m_pFaceStmt)
    {
         PRT_INFO("m_pFaceStmt cannot be NULL\n");
         return -1;
    }
    char* kname = (char *)sqlite3_column_text(m_pFaceStmt, 0);
    if(kname){
       strncpy0(face_info->key_name, kname, FACE_DATABASE_FEATURE_INFO_NAME_TEXT_LEN);
    }
    const float* pFileContent = (const float*)sqlite3_column_blob(m_pFaceStmt,1);
    if (pFileContent)
    {
        int bytes=sqlite3_column_bytes(m_pFaceStmt,1);
        memcpy(face_info->key_data,pFileContent,bytes);
   }
   return 0;
}

int face_database_api::face_database_inquire_shutdown(void)
{
    if(m_pFaceStmt == mpNULL)
    {
        PRT_INFO("m_pFaceStmt cannot be NULL\n");
        return -1;
    }
    sqlite3_finalize(m_pFaceStmt);
    m_pFaceStmt =mpNULL;
    return 0;
}

int face_database_api::face_database_shutdown(void)
{
  if(!face_db) {
     PRT_INFO("face_db cannot be NULL\n");
     return -1;
  }
  sqlite3_close(face_db);
  face_db = NULL;
  return 0;
}

