#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include "Table.h"

class Database{
    private:
        Table* descriptor;
    public:
        Database();
        Table* getDescriptor();
        Record* searchRecord(std::string tableName, std::string id);
        Table* searchTable(std::string name);
        void getDatabaseColisions();

        void insertRecord(std::string tableName, Record* rec);
        void alterTable(std::string tableName, std::vector<std::string> keys);
        void setDescriptor(Table* descriptor);
        void insertNode(Table* tableNode);

        void innerJoin(Table* t1, Table* t2, std::string field);
        void outerJoin(Table* t1, Table* t2, std::string field, char type);

        void getAllData();
        ~Database();

};


#endif // DATABASE_H_INCLUDED
