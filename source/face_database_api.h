#ifndef SQLITE3_DB_API
#define SQLITE3_DB_API
//add  head
#include "sqlite3.h"
#include "global.h"

/* MAKE_DB_QUERY is a sqlite3 query idiom that hides the clutter */
/* error_message_format: is the format to print errror message returned from salite3_exec so it must
   contain one and only one %s */
/* return_code_on_error: is the value to return in an event of error.  Normally provide -1 for
   this. If continued execution is desired give 0 instead. */
/* callback_func: is the callback function or NULL when not used */
/* callback_arg: is the pointer to the argument to the callback_func */
/* query_string_format: is the format to formulate a query string.  It is passed to sqlite3_mprintf
   so all printf format as well as sqlite specific ones are allowd.  All the following parameters
   after query_string_format are passed on to sqlite3_mprintf. */
#define MAKE_DB_QUERY(db, error_message_format, return_code_on_error, callback_func, callback_arg, query_string_format, ...) \
  {                                                                     \
    char *query_statement = NULL;                                       \
    char *error_message = NULL;                                         \
    query_statement = sqlite3_mprintf(query_string_format, ##__VA_ARGS__); \
    if(query_statement == NULL) return -1;                              \
    if(sqlite3_exec(db, query_statement, callback_func, callback_arg, &error_message)) { \
      if((return_code_on_error) != 0) {                                 \
        printf(error_message_format, error_message);               \
        sqlite3_free(error_message);                                    \
        sqlite3_free(query_statement);                                  \
        return (return_code_on_error);                                  \
      }                                                                 \
    }                                                                   \
    sqlite3_free(query_statement);                                      \
    sqlite3_free(error_message); /* just in case, passing NULL is no harm */ \
  }

#define strncpy0(dest, src, n) do {             \
    if(src != NULL) {                           \
      strncpy((dest), (src), (n));              \
      (dest)[(n) - 1] = '\0';                   \
    } else {                                    \
      printf("strncpy0: src is NULL\n"); \
      (dest)[0] = '\0';                         \
    }                                           \
  } while(0)

/* clear an object (array, structure, etc.) */
#define memset0(object) do {                                            \
    if(sizeof(object) == sizeof(void*)) {                               \
      printf("memset0: object may be a pointer instead of an struct/array\n"); \
    }                                                                   \
    memset(&(object), 0, sizeof(object));                               \
  } while(0)

/*
** CAPI3REF: Result Codes
** KEYWORDS: SQLITE_OK {error code} {error codes}
** KEYWORDS: {result code} {result codes}
**
** Many SQLite functions return an integer result code from the set shown
** here in order to indicates success or failure.
**
** New error codes may be added in future versions of SQLite.
**
** See also: [SQLITE_IOERR_READ | extended result codes]
*/
#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* NOT USED. Table or record not found */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* NOT USED. Database lock protocol error */
#define SQLITE_EMPTY       16   /* Database is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

#define SQLITE_DB_NAME      "face.db"
#define SQLITE_FACE_FEATURE_VALUE_LEN 2048


static sqlite3 *sqlite3_db = NULL;
static sqlite3 *face_db = NULL;

typedef struct {
  int found;
  char *str;
  int len;
} sqlite3_dat_get_cb_arg_t;

static struct {
  sqlite3 **db;
  const char *table_name;
  const char *key_name;
} tables_to_use[] = {
  {&face_db,"face_feature","key_name"}
};


#define FACE_DATABASE_FEATURE_INFO_NAME_TEXT_LEN            128  /* 128b for char type*/
#define FACE_DATABASE_FEATURE_INFO_DATA_TEXT_LEN            2048 /* 8k for for float type */

typedef struct {

  /* face feature name */
  char key_name[FACE_DATABASE_FEATURE_INFO_NAME_TEXT_LEN + 1];

  /* face feature data */
  float key_data[FACE_DATABASE_FEATURE_INFO_DATA_TEXT_LEN +1];

} face_database_feature_info_t;


/******************************
 * FACE RECOGNIZIE ENUM
 ******************************/
typedef enum {
    FACE_RECO_DATA_INVALID = 0,
    FACE_RECO_DATA_LOCAL_FIRST,
    FACE_RECO_DATA_LOCAL_SECOND,
    FACE_RECO_DATA_LOCAL_LAST,
    // add new type before this line
    FACE_RECO_DATA_LOCAL_MAX
}EFaceRecDataLocal;

class face_database_api
{
public:
    explicit face_database_api(void);
    ~face_database_api();

    /*  sqlite3 init */
    int face_database_init(void);

    /*  sqlite3 insert blob type*/
    int face_database_insert_feature(const char *kname, float *kdata, int nSize);

    /*  sqlite3 inquire prepare*/
    bool face_database_inquire_prepare();

    /*  sqlite3 inquire current row step*/
    SQLITE_API int face_database_inquire_step();

    /*  sqlite3 inquire current row info*/
    int face_database_inquire_info(face_database_feature_info_t *face_info);

    /*  sqlite3 inquire shut down*/
    int face_database_inquire_shutdown(void);

    /* sqlite3 shutdown */
    int face_database_shutdown(void);
private:
    sqlite3_stmt *m_pFaceStmt;
};





#endif // SQLITE3_DB_API

