#include "Table.h"
#include <functional>
#include <stdio.h>
#define TSIZE 1000
#include <iostream>
#include <math.h>
#include <sstream>

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
    for(int i = 0; i < primaryKeysIndex.size(); ++ i){
        aux = values[primaryKeysIndex[i]];

        aux.erase(std::remove(aux.begin(), aux.end(), ' '), aux.end());
        reg->getValues().at(primaryKeysIndex[i]) = aux;

       hash+= djb2(aux);

    }



    if(values.size() == this->n_columns){
        k = hashFunction(hash);

        if(this->records[k] == NULL){

            reg->setNext(NULL);
            this->records[k] = reg;
        }else{
            insertAux(records[k], reg);
        }

    }else{
        std::cout<<"tabela: "<<name<<" numero de colunas: "<<columns.size()<<" numero de valores: "<<values.size()<<std::endl;
        perror ("Número errado de colunas");
    }

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

    if(concatComposeKey(records[k]) == id){
        return records[k];
    }else{
        Record* r = records[k]->getNext();
        while(r != NULL){
            if(concatComposeKey(r)== id)
                return r;
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
}


Table::~Table()
{

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

