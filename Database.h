#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include "Table.h"

class Database{
    private:
        Table* descriptor;
        Table* searchTable(std::string name);
    public:
        void setDescriptor(Table* descriptor);
        void insertNode(Table* tableNode);
        Table* getDescriptor();
        Record* searchRecord(std::string tableName, std::string id);
        Database();

};


#endif // DATABASE_H_INCLUDED
