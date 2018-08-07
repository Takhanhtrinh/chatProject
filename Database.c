#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
sqlite3* db;
uint32_t userId = 1;
const char* CREATE_USER_STMT = "INSERT INTO USER (ID,NAME) VALUES(?1, ?2);";
const char* SEARCH_USER_STMT = "SELECT ID, NAME FROM USER WHERE NAME = ?;";
const char* GET_USER_STMT = "SELECT NAME FROM USER WHERE ID = ?;";
const char* CREATE_ROOM_STMT = "INSERT INTO ROOM (ID,NAME) VALUES(?1, ?2);";
const char* SEARCH_ROOM_STMT = "SELECT ID,NAME FROM ROOM WHERE NAME = ?;";
const char* GET_ROOM_NAME_STMT = "SELECT NAME FROM ROOM WHERE ID = ?;";
const char* CREATE_MESSAGE_STMT = "INSERT INTO MESSAGE(ID,ROOM_ID, USER_ID, MESSAGE) VALUES(?1,?2,?3,?4);";
const char* GET_MESSAGE_STMT = "SELECT MESSAGE FROM MESSAGE WHERE "
const int SEARCH_USER_NAME_FOUND = 1;
const int SEARCH_USER_NAME_NOT_FOUND = -1;
const int ERROR = -2;
const int CREATE_USERNAME_EXIST = -1;
const int CREATE_USERNAME_SUCCESS = 1;
const int GET_USERNAME_SUCCESS = 1;
const int GET_USERNAME_CANT_FIND = -1;
const int SEARCH_ROOM_NAME_EXIST = 1;
const int SEARCH_ROOM_NAME_NOT_EXIST = -1;
const int CREATE_ROOM_EXIST = -1;
const int CREATE_ROOM_SUCCESS = 1;
const int GET_ROOM_DOEST_EXIST = -1;
const int GET_ROOM_NAME_EXIST = 1;
int createDatabase(const char* databaseName) {
  int rt;
  rt = sqlite3_open(databaseName, &db);
  if (rt) {
    printf("CANT OPEN DATABASE: %s\n", sqlite3_errmsg(db));
    return 0;
  } else {
    printf("CREATE DATABASE SUCCESS\n");
    return 1;
  }
}

int closeDatabase() {
  assert(db != NULL);
  int rt = sqlite3_close(db);
  if (rt == SQLITE_OK) {
    printf("close database success\n");
    return 1;
  } else {
    printf("close database failed\n");
    return 0;
  }
}
static int createTableCb(void* data, int numCol, char** colName,
                         char** colVal) {
  printf("%s\n", (const char*)data);
  for (int i = 0; i < numCol; i++) {
    printf("%s = %s\n", colName[i], colVal[i]);
  }
  return 0;
}
static int SearchTableCb(void* data, int numCol, char** colName,
                         char** colVal) {
  for (int i = 0; i < numCol; i++) {
    printf("%s = %s\n", colName[i], colVal[i]);
  }
  return 0;
}
int searchUsername(const char* name, void* data) {
  assert(db != NULL);
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, SEARCH_USER_STMT, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    printf("Search user name prepare error: %s\n", sqlite3_errmsg(db));
    assert(1 == 0 && "Search user name prepare error");
    return ERROR;
  }
  rc = sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    printf("Search user bind error: %s\n", sqlite3_errmsg(db));
    assert(1 == 0 && "Search user name bind error");
    return ERROR;
  }
  int found = -1;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    found = 1;
    printf("ID: %d NAME: %s\n", sqlite3_column_int(stmt, 0),
           sqlite3_column_text(stmt, 1));
  }
  if (found == -1) {
    return SEARCH_USER_NAME_NOT_FOUND;
  }
  sqlite3_finalize(stmt);
  return SEARCH_USER_NAME_FOUND;
}
int getUserName(const int& id, std::string& str) {
  assert(db != NULL);
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, GET_USER_STMT, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    printf("SQLITE getUserName Prepare failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_int(stmt, 1, id);
  if (rc != SQLITE_OK) {
    printf("SQLITE getUsername bind Int failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    str = (const char*)sqlite3_column_text(stmt, 0);
    sqlite3_finalize(stmt);
    return GET_USERNAME_SUCCESS;
  }
  return GET_USERNAME_CANT_FIND;
}
int createMessage(const char* msg, const int& msgId, const int& fromId,
                  const int& roomId) {}
int searchRoom(const char* roomName) {
  assert(db != NULL);
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, SEARCH_ROOM_STMT, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    printf("SQLITE SearchRoom Prepare Room Error: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_text(stmt, 1, roomName, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    printf("SQLITE SearchRoom Bind Room Error: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    printf("search room result id: %d, name: %s\n", sqlite3_column_int(stmt, 0),
           sqlite3_column_text(stmt, 1));
    sqlite3_finalize(stmt);
    return SEARCH_ROOM_NAME_EXIST;
  }
  sqlite3_finalize(stmt);
  return SEARCH_ROOM_NAME_NOT_EXIST;
}
int createRoom(const char* roomName, const int& roomId) {
  assert(db != NULL);
  if (searchRoom(roomName) == SEARCH_ROOM_NAME_EXIST) {
    return CREATE_ROOM_EXIST;
  }
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, CREATE_ROOM_STMT, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    printf("SQLITE CREATE ROOM prepare ERROR: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_int(stmt, 1, roomId);
  if (rc != SQLITE_OK) {
    printf("SQLITE CREATE ROOM bind int Error: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_text(stmt, 2, roomName, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    printf("SQLITE CREATE ROOM bind text  Error: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    printf("SQLITE ERROR step Error: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  sqlite3_finalize(stmt);
  return CREATE_ROOM_SUCCESS;
}
int getRoomName(const int& id, std::string& str) {
  assert(db != NULL);
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, GET_ROOM_NAME_STMT, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    printf("sqlite get room name prepare failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_int(stmt, 1, id);
  if (rc != SQLITE_OK) {
    printf("sqlite get room name bind int failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    // printf("sqlite get room name success: %s\n", sqlite3_column_text(stmt,
    // 0));
    str = (const char*)sqlite3_column_text(stmt, 0);
    sqlite3_finalize(stmt);
    return GET_ROOM_NAME_EXIST;
  }
  sqlite3_finalize(stmt);
  return GET_ROOM_DOEST_EXIST;
}
int createUser(const char* name) {
  if (searchUsername(name, NULL) == SEARCH_USER_NAME_FOUND)
    return CREATE_USERNAME_EXIST;
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, CREATE_USER_STMT, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    printf("SQLITE Create user prepare failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_int(stmt, 1, userId++);
  if (rc != SQLITE_OK) {
    printf("SQLITE Create user bind int failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    printf("SQLITE Create user bind text failed: %s\n", sqlite3_errmsg(db));
    return ERROR;
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    printf("SQLITE ERROR INSERTING VALUE:%s", sqlite3_errmsg(db));
    return ERROR;
  }
  sqlite3_finalize(stmt);
  printf("create user: %s success\n", name);
  return CREATE_USERNAME_SUCCESS;
}
int createTables() {
  const char* USER =
      "CREATE TABLE USER(ID INT PRIMARY KEY NOT NULL, NAME CHAR(255) NOT "
      "NULL);";
  char* errMsg = 0;
  const char* data = "CREATED USER TABLES";
  int rc = sqlite3_exec(db, USER, createTableCb, (void*)data, &errMsg);
  if (rc != SQLITE_OK) {
    printf("SQL ERROR: %s\n", errMsg);
    sqlite3_free(errMsg);
    closeDatabase();
    return -1;
  } else
    printf("CREATED TABLE USER SUCCESSFUL\n");
  const char* ROOM =
      "CREATE TABLE ROOM(ID INT PRIMARY KEY NOT NULL, NAME CHAR(255) NOT "
      "NULL);";
  const char* data1 = "CREATED ROOM  TABLES";
  rc = sqlite3_exec(db, ROOM, createTableCb, (void*)data1, &errMsg);
  if (rc != SQLITE_OK) {
    printf("SQL ERROR: %s\n", errMsg);
    sqlite3_free(errMsg);
    closeDatabase();
    return -1;
  } else
    printf("CREATE ROOM SUCCESS\n");
  const char* MESSAGE =
      "CREATE TABLE MESSAGE(ID INT PRIMARY KEY NOT NULL, ROOM_ID INT NOT NULL, "
      "USER_ID INT NOT NULL, TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP, "
      "MESSAGE TEXT NOT NULL);";
  const char* data2 = "CREATED MESSAGE TABLES";
  rc = sqlite3_exec(db, MESSAGE, createTableCb, (void*)data2, &errMsg);
  if (rc != SQLITE_OK) {
    printf("SQL ERROR: %s\n", errMsg);
    sqlite3_free(errMsg);
    closeDatabase();
    return -1;
  } else
    printf("CREATE MESSAGE SUCCESS\n");
  const char* PIXEL_MESSAGE =
      "CREATE TABLE PIXEL_MESSAGE(ID INT PRIMARY KEY NOT NULL, ROOM_ID INT NOT "
      "NULL, USER_ID INT NOT NULL, TIMESTAMP DATETIME DEFAULT "
      "CURRENT_TIMESTAMP, PIXEL BLOB NOT NULL);";
  const char* data3 = "CREATED PIXEL TABLES";
  rc = sqlite3_exec(db, PIXEL_MESSAGE, createTableCb, (void*)data3, &errMsg);
  if (rc != SQLITE_OK) {
    printf("SQL ERROR: %s\n", errMsg);
    sqlite3_free(errMsg);
    closeDatabase();
    return -1;
  } else
    printf("CREATE PIXEL_MESSAGE SUCCESS\n");
  return 1;
}
int main() {
  createDatabase((const char*)"ChatProject.db");
  if (createTables()) {
    char buffer[512];
    while (true) {
      printf("Please Enter room name: ");
      scanf("%s", buffer);
      if (buffer[0] == '.' && buffer[1] == 'e' && buffer[2] == 'x' &&
          buffer[3] == 'i' && buffer[4] == 't')
        break;
      int rt = createRoom(buffer, userId++);
      if (rt == CREATE_ROOM_EXIST) {
        printf("Room Already exist, please choose another name\n");
      }
      int id;
      printf("Please Enter Id To query room Name: ");
      scanf("%d", &id);
      std::string name;
      int getRt = getRoomName(id, name);
      if (getRt == GET_ROOM_DOEST_EXIST) {
        printf("cant find the room with id: %d\n", id);
      } else if (getRt == GET_ROOM_NAME_EXIST)
        printf("get room success: %s\n", name.c_str());
      fflush(stdin);
    }
    closeDatabase();
  }
  return 0;
}
