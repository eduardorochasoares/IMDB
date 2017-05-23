#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include "Table.h"

class Database{
    private:
        Table* descriptor;
        void quickSort(std::vector<Record*> arr, std::vector<int> fieldsIndex);
        int partition(std::vector<Record*> arr, int start, int end, std::vector<int> fieldsIndex);
        void swap(std::vector<Record*> arr, int i, int j);

    public:
        std::string static concatFieldsValue(Record* r, std::vector<int> fieldsIndex);
        Database();
        Table* getDescriptor();
        Record* searchRecord(std::string tableName, std::string id);
        Table* searchTable(std::string name);
        void getDatabaseColisions();

        void insertRecord(std::string tableName, Record* rec);
        void alterTable(std::string tableName, std::vector<std::string> keys);
        void setDescriptor(Table* descriptor);
        void insertNode(Table* tableNode);

        void innerJoin(Table* t1, Table* t2, std::vector<std::string> field);
        void outerJoin(Table* t1, Table* t2, std::vector<std::string> field, char type);

        ///inner e outer join O(NxM) usando apenas a estrutura de busca da primeira parte do trabalho
        void innerJoinNxM(Table* t1, Table* t2, std::vector<std::string> field);
        void outerJoinNxM(Table* t1, Table* t2, std::vector<std::string> field, char type);

        void getAllData();
        ~Database();

};


#endif // DATABASE_H_INCLUDED
