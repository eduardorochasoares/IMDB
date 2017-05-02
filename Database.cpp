#include "Database.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
///retorna um ponteiro para o primeiro elemento da lista de tabelas do banco de dados
Table* Database::getDescriptor()
{
    return this->descriptor;
}

///insere uma tabela no bando de dados
void Database::insertNode(Table* tableNode)
{
    Table* p = this->descriptor;
    ///BD está vazio
    if(p == NULL){
        ///faz o descritor apontar para a nova tabela
        this->descriptor = tableNode;
        ///aponta o proximo da tabela como nulo
        tableNode->setNext(NULL);

    }else{
        ///insere a tabela no final e aproveita para verificar se ja existe uma tabela com o mesmo nome
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
        ///caso não exista tabela com o mesmo nome atribui o próximo do último nó como a tabela a ser inserida
        ///e o próximo da nova tabela como nulo
        p->setNext(tableNode);
        tableNode->setNext(NULL);


    }

}

///inicializa o descritor da lista de tabelas como nulo
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

/***
    Description:
        Faz a busca por um registro em uma tabela  e retorna esse registro
    Params:
        tableName: nome da tabela onde a pesquisa será feita
        id: chave de busca de um registro

***/
Record* Database::searchRecord(std::string tableName, std::string id)
{
    ///busca a tabela
    Table* t = searchTable(tableName);

    ///a tabela não existe?
    if(t == NULL){
        std::cout<<"Tabela não encontrada"<<std::endl;
        return NULL;
    }
    ///chama o método de pesquisa da classe Table passando o id
    Record* r = t->searchRecord(id);

    if(r == NULL){
        std::cout<<"Registro não encontrado"<<std::endl;
        return NULL;
    }
    ///imprime as colunas da tabela
    std::cout<<std::endl;
    for(int i = 0; i < t->getColumns().size(); ++i){
        std::cout<<t->getColumns().at(i)<<"\t";
    }
    std::cout<<std::endl;

    ///imprime os dados do registro
    for(int i = 0; i < r->getValues().size(); ++i )
        std::cout<<r->getValues().at(i)<<"\t";


}

/***
    Description:
        Altera uma tabela definindo alguns de seus campos como chave primária
    Params:
        tableName: nome da tabela que será alterada
        keys: colunas da tabela que serão chaves primárias
***/
void Database::alterTable(std::string tableName, std::vector<std::string>keys)
{
    ///procura a tabela no banco de dados
    Table* t = searchTable(tableName);

    if(t == NULL){
        std::cout<<"Tabela não encontrada"<<std::endl;
        return;
    }
    ///chama a função que indexa seus campos de chave primária com um vetor auxiliar
    t->setPrimaryKeys(keys);


}

///insere um registro em uma tabela específica
void Database::insertRecord(std::string tableName, Record* rec)
{
    ///procura a tabela na base através do nome
    Table* t = searchTable(tableName);
    if(t == NULL){
        std::cout<<"Tabela não encontrada"<<std::endl;
        return;
    }
    ///chama o método de inserção na classe Table passando o registro
    t->insertRecord(rec);

}

///Função que imprime a taxa de colisão para cada tabela do banco de dados
///função usada apenas para gerar as estatíticas para o relatório
void Database::getDatabaseColisions()
{



    Table* p = descriptor;

    while(p != NULL){
        std::cout<<"Tabela: "<<p->getName()<<" taxa de colisao: " <<(float)p->getColisionsNumber()/p->getRecordsNumber()<<std::endl;
        p = p->getNext();
    }

}

///função que le um arquivo contendo as chaves primárias dos registros de cada tabela e chama a busca passando essas chaves
///função utilizada somente para os testes do relatório
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

///destrutor, desaloca a lista encadeada de tabelas
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
