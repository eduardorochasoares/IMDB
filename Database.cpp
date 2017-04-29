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
    std::cout<<"table found"<<std::endl;
    Record* r = t->searchRecord(id);

     for(int i = 0; i < t->getColumns().size(); ++i)
        std::cout<<t->getColumns().at(i)<<"\t";
    std::cout<<std::endl;

    for(int i = 0; i < r->getValues().size(); ++i )
        std::cout<<r->getValues().at(i)<<"\t";
}

