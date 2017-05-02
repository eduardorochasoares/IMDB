#include "Table.h"
#include <functional>
#include <stdio.h>
#define TSIZE 1000
#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>

std::string Table::getName(){
    return this->name;
}
std::vector<std::string>& Table::getColumns()
{
    return this->columns;
}

Table* Table::getNext()
{
    return this->next;
}

Record** Table::getRecords()
{
    return this->records;
}

void Table::setName(std::string name)
{
    this->name = name;
}

void Table::setColumns(std::vector<std::string> columns)
{
    this->columns = columns;
}

void Table::setRecords(Record** reg)
{
    this->records = reg;
}
void Table::setNext(Table* next)
{
    this->next = next;
}
int Table::hashFunction(long long int k)
{
        return k%TSIZE;
}

long long int Table::djb2(std::string id)
{

    unsigned long hash = 5381;
    int c;
    char* str = (char*)id.c_str();
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}



void Table::insertRecord(Record* reg)
{

    int k;
    std::string id = "";
    std::string aux ="";
    unsigned long hash = 0;

    std::vector<std::string> values = reg->getValues();
    if(primaryKeysIndex.size() > 0){
        for(int i = 0; i < primaryKeysIndex.size(); ++ i){

            aux = values[primaryKeysIndex[i]];
            aux.erase(std::remove(aux.begin(), aux.end(), ' '), aux.end());
            reg->getValues().at(primaryKeysIndex[i]) = aux;

            hash+= djb2(aux);

        }
    }else{
        primaryKeysIndex.push_back(0);
        hash = djb2(reg->getValues().at(0));
    }



    if(values.size() == this->n_columns){
        k = hashFunction(hash);

        if(this->records[k] == NULL){

            reg->setNext(NULL);
            this->records[k] = reg;
        }else{
            this->colisions+=1;
            insertAux(records[k], reg);
        }

    }else{
        std::cout<<"tabela: "<<name<<" numero de colunas: "<<columns.size()<<" numero de valores: "<<values.size()<<std::endl;
        perror ("Número errado de colunas");
    }
    this->records_number+=1;
    writePrimaryKeyFile(reg);

}
Record* Table::searchRecord(std::string id)
{

    std::string aux;
    std::stringstream ss;
    ss.str(id);
    unsigned long hash = 0;
    while(getline(ss, aux, ' ')){
        hash+= djb2(aux);
    }
    id.erase(std::remove(id.begin(), id.end(), ' '), id.end());


    int k = hashFunction(hash);
    if(records[k] == NULL) return NULL;

    if(concatComposeKey(records[k]) == id){
        return records[k];
    }else{
        Record* r = records[k]->getNext();
        while(r != NULL){
            if(concatComposeKey(r)== id){
                return r;
            }
            else
                r = r->getNext();
        }
        return NULL;
    }
}

void Table::insertAux(Record* p, Record* n)
{
    while(p->getNext() != NULL)
        p = p->getNext();

    n->setNext(NULL);
    p->setNext(n);
}


Table::Table(std::string name, std::vector<std::string> columns)
{
    this->records = new Record*[TSIZE];
    this->n_columns = columns.size();
    this->name = name;

    for(int i = 0; i < TSIZE; ++i)
        records[i] = NULL;

    this->columns = columns;
    this->colisions = 0;
    this->records_number = 0;
}


Table::~Table()
{
    Record* p;
    Record* aux;
    for(int i = 0; i < TSIZE; ++i){
        p = this->records[i];
        while(p != NULL){
            aux = p->getNext();
            delete p;
            p = aux;
        }
    }
    delete []records;
}

void Table::setPrimaryKeys(std::vector<std::string> columns_primary)
{


    for(int i = 0; i < columns_primary.size(); ++i)
        for(int j = 0; j< columns.size(); ++j){
            if(columns_primary[i].compare(columns[j]) == 0)
                primaryKeysIndex.push_back(j);

        }


}
std::string Table::concatComposeKey(Record* r)
{
    std::string result ="";
    for(int i = 0; i < primaryKeysIndex.size(); ++i)
        result += r->getValues().at(primaryKeysIndex[i]);
    return result;
}

int Table::getColisionsNumber()
{
    return this->colisions;
}

int Table::getRecordsNumber()
{
    return this->records_number;
}


void Table::writePrimaryKeyFile(Record* rec)
{
    std::ofstream f;
    std::vector<std::string> data = rec->getValues();
    std::string path = name + ".txt";
    f.open(path.c_str(), std::ios_base::app);

    for(int i = 0; i < primaryKeysIndex.size(); ++i)
        f << data[primaryKeysIndex[i]] << " ";

    f << std::endl;
}

std::vector<int> Table::getPrimaryKeyIndex()
{
    return this->primaryKeysIndex;
}

void Table::setPrimaryKeyIndex(std::vector<int>index)
{
    this->primaryKeysIndex = index;
}

void Table::printResult(Record* rec)
{
    if(rec == NULL){
        std::cout<<"Registro nao existente"<<std::endl;
        return;
    }
    for(int i = 0; i < columns.size(); ++i)
        std::cout<<columns[i]<< " ";
    std::cout<<std::endl;

    std::vector<std::string> values = rec->getValues();
    for(int i = 0; i < values.size(); ++i)
        std::cout<<values[i]<<" ";
    std::cout<<std::endl;
}
