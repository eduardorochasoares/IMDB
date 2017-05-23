#ifndef TABELA_H_INCLUDED
#define TABELA_H_INCLUDED
#include <string>
#include "Record.h"
#include <algorithm>
#include <iterator>

class Table{
    private:
        int t_size;
        int n_columns;
        std::string name;
        ///colunas da tabela
        std::vector<std::string> columns;
        int colisions;
        int records_number;
        Table* next;
        Record** records;
        ///indice os campos que são chaves primárias em columns
        std::vector<int> primaryKeysIndex;

        std::string concatComposeKey(Record* r);
        int hashFunction(long long int k);
        long long int  djb2(std::string id);
        void insertAux(Record* p, Record* n);
        void writePrimaryKeyFile(Record* reg);



    public:
        std::vector<int> getFieldIndex(std::vector<std::string> field);
        std::vector<Record*> moveRecToVector();
        Table(std::string name, std::vector<std::string> columns);
        std::string getName();
        std::vector<std::string>& getColumns();
        Table* getNext();
        Record** getRecords();
        Record* searchRecord(std::string id);
        int getColisionsNumber();
        int getRecordsNumber();
        std::vector<int> getPrimaryKeyIndex();
        int getTsize();

        void setPrimaryKeyIndex(std::vector<int> index);
        void setPrimaryKeys(std::vector<std::string> columns);
        void setName(std::string name);
        void setColumns(std::vector<std::string> columns);

        void setNext(Table* next);
        void insertRecord(Record* reg);
        void printResult(Record* rec);
        void removeRecord(std::string id);

        void selectCount();
        void selectCountByFields(std::vector<std::string >field, std::vector<std::string> value);
        ~Table();
};

#endif // TABELA_H_INCLUDED
