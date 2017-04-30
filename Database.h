#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include "Table.h"

class Database{
    private:
        Table* descriptor;
    public:
        void setDescriptor(Table* descriptor);
        void insertNode(Table* tableNode);
        Table* getDescriptor();
        Record* searchRecord(std::string tableName, std::string id);
        void alterTable(std::string tableName, std::vector<std::string> keys);
        Database();
        Table* searchTable(std::string name);
        void insertRecord(std::string tableName, Record* rec);


};


#endif // DATABASE_H_INCLUDED
