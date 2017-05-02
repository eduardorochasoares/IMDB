#include "Database.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
Table* Database::getDescriptor()
{
    return this->descriptor;
}

void Database::insertNode(Table* tableNode)
{
    Table* p = this->descriptor;
    //BD está vazio
    if(p == NULL){
        this->descriptor = tableNode;
        tableNode->setNext(NULL);

    }else{
        while(p->getNext() != NULL){
            if(p->getName() == tableNode->getName()){
                std::cout<<"Erro: ja existe uma tabela com esse nome"<<std::endl;
                return;
            }
            p = p->getNext();
        }
        if(p->getName() == tableNode->getName()){
            std::cout<<"Erro: ja existe uma tabela com esse nome"<<std::endl;
            return;
        }
        p->setNext(tableNode);
        tableNode->setNext(NULL);


    }

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
   /* std::cout<<std::endl;
    for(int i = 0; i < t->getColumns().size(); ++i){
        std::cout<<t->getColumns().at(i)<<"\t";
    }
    std::cout<<std::endl;

    for(int i = 0; i < r->getValues().size(); ++i )
        std::cout<<r->getValues().at(i)<<"\t";

    */
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

void Database::getDatabaseColisions()
{



    Table* p = descriptor;

    while(p != NULL){
        std::cout<<"Tabela: "<<p->getName()<<" taxa de colisao: " <<(float)p->getColisionsNumber()/p->getRecordsNumber()<<std::endl;
        p = p->getNext();
    }

}


void Database::getAllData()
{
    Table* p = descriptor;
    std::ifstream file;
    std::string path;
    std::string key = "";
    std::string aux;
    while(p != NULL){
        path = p->getName() + ".txt";
        file.open(path.c_str(),  std::ifstream::in);
        int n = p->getPrimaryKeyIndex().size();
        int  i;
        while(file >> aux){
            i = 0;
            while(i < n -1){
                key += aux + " ";
                file >> aux;
                ++i;
            }
            key += aux;

            searchRecord(p->getName(), key);
            key ="";
        }

        file.close();
        p = p->getNext();
    }
}

Database::~Database()
{
    Table* aux;
    Table* p = this->descriptor;

    while(p != NULL){
        aux = p->getNext();
        delete p;
        p = aux;
    }
}
