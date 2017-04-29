#include "Table.h"
#include <functional>
#include <stdio.h>
#define TSIZE 100000
#include <iostream>
#include <math.h>

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

unsigned long Table::djb2(std::string id)
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
    std::vector<std::string> values = reg->getValues();

    std::string id = reg->getValues().at(0);
    id.erase(std::remove(id.begin(), id.end(), ' '), id.end());
    reg->getValues().at(0) = id;

    if(values.size() == this->n_columns){
        k = hashFunction(djb2(id));

        if(this->records[k] == NULL){

            reg->setNext(NULL);
            this->records[k] = reg;
        }else{
            insertAux(records[k], reg);
        }

    }else{
        perror ("Número errado de colunas");
    }

}
Record* Table::searchRecord(std::string id)
{
    int k = hashFunction(djb2(id));
    if(records[k]->getValues().at(0) == id){
        return records[k];
    }else{
        Record* r = records[k]->getNext();
        while(r != NULL){
            if(r->getValues().at(0) == id)
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
