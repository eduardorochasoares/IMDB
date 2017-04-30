#include "Database.h"
#include <stdio.h>
#include <iostream>

Table* Database::getDescriptor()
{
    return this->descriptor;
}

void Database::insertNode(Table* tableNode)
{

    tableNode->setNext(this->descriptor);
    this->descriptor = tableNode;
}


Database::Database()
{
    this->descriptor = NULL;
}


Table* Database::searchTable(std::string name)
{
    Table* p = this->descriptor;
    if(p == NULL){

        perror("BD vazio");
        return NULL;

    }else{

        while(p != NULL){
            if(p->getName().compare(name) == 0)
                return p;
            else
                p = p->getNext();
        }
    }
}

Record* Database::searchRecord(std::string tableName, std::string id)
{
    Table* t = searchTable(tableName);

    if(t == NULL){
        std::cout<<"Tabela não encontrada"<<std::endl;
        return NULL;
    }

    Record* r = t->searchRecord(id);

    if(r == NULL){
        std::cout<<"Registro não encontrado"<<std::endl;
        return NULL;
    }
    for(int i = 0; i < t->getColumns().size(); ++i){
        std::cout<<t->getColumns().at(i)<<"\t";
    }
    std::cout<<std::endl;

    for(int i = 0; i < r->getValues().size(); ++i )
        std::cout<<r->getValues().at(i)<<"\t";
}

void Database::alterTable(std::string tableName, std::vector<std::string>keys)
{

    Table* t = searchTable(tableName);

    if(t == NULL){
        std::cout<<"Tabela não encontrada"<<std::endl;
        return;
    }

    t->setPrimaryKeys(keys);


}

void Database::insertRecord(std::string tableName, Record* rec)
{
    Table* t = searchTable(tableName);
    if(t == NULL){
        std::cout<<"Tabela não encontrada"<<std::endl;
        return;
    }
    t->insertRecord(rec);

}
