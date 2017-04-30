#ifndef TABELA_H_INCLUDED
#define TABELA_H_INCLUDED
#include <string>
#include "Record.h"
#include <algorithm>
#include <iterator>

class Table{
    private:
        int n_columns;
        std::string name;
        std::vector<std::string> columns;
        Table* next;
        Record** records;
        std::vector<int> primaryKeysIndex;
        std::string concatComposeKey(Record* r);
        int hashFunction(long long int k);
        long long int  djb2(std::string id);
        void insertAux(Record* p, Record* n);

    public:
        Table(std::string name, std::vector<std::string> columns);
        std::string getName();
        std::vector<std::string>& getColumns();
        Table* getNext();
        Record** getRecords();
        void setPrimaryKeys(std::vector<std::string> columns);
        void setName(std::string name);
        void setColumns(std::vector<std::string> columns);
        void setRecords(Record** reg);
        void setNext(Table* next);

        void insertRecord(Record* reg);
        Record* searchRecord(std::string id);
        ~Table();
};

#endif // TABELA_H_INCLUDED
