#include "Table.h"
#include <functional>
#include <stdio.h>
#define TSIZE 1000
#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>


///retorna o nome da tabela
std::string Table::getName(){
    return this->name;
}
///retorna as colunas da tabela
std::vector<std::string>& Table::getColumns()
{
    return this->columns;
}

///retorna um ponteiro para o próximo nó da tabela na lista de tabelas do banco de dados
Table* Table::getNext()
{
    return this->next;
}

///retorna a tabela de hash da tabela
Record** Table::getRecords()
{
    return this->records;
}

///atribui um nome para a tabela
void Table::setName(std::string name)
{
    this->name = name;
}

///atribui as colunas da tabela
void Table::setColumns(std::vector<std::string> columns)
{
    this->columns = columns;
}

///atribui o próximo nó à tabela na lista de tabelas do banco de dados
void Table::setNext(Table* next)
{
    this->next = next;
}
///calcula o hash de divisão para o valor gerado pela funç
int Table::hashFunction(long long int k)
{
        return k%TSIZE;
}

///algoritmo de hash que mapeia strings em inteiros
long long int Table::djb2(std::string id)
{

    unsigned long hash = 5381;
    int c;
    char* str = (char*)id.c_str();
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    ///para caso ocorra overflow na variavel
    if(hash < 0)
        hash*= -1;

    return hash;
}


/// insere um registro na tabela
void Table::insertRecord(Record* reg)
{

    int k;
    std::string id = "";
    std::string aux ="";
    unsigned long hash = 0;

    std::vector<std::string> values = reg->getValues();

    ///tem pelo menos uma chave primária definida?
    if(primaryKeysIndex.size() > 0){
        for(int i = 0; i < primaryKeysIndex.size(); ++ i){

            aux = values[primaryKeysIndex[i]];
            ///elimina algum espaço que pode vir do arquivo
            aux.erase(std::remove(aux.begin(), aux.end(), ' '), aux.end());
            reg->getValues().at(primaryKeysIndex[i]) = aux;
            ///acumula o valor de hash para cada uma das chaves que constitui a chave primária
            hash+= djb2(aux);

        }
    }else{
        ///se não tem definição de chave primária, como o caso da tabela foot_note, define que o primeiro campo será chave
        primaryKeysIndex.push_back(0);
        hash = djb2(reg->getValues().at(0));
    }


    ///o número de valores passados no registro é igual ao número de colunas da tabela?
    if(values.size() == this->n_columns){
        ///calcula o hash de divisão para o valor de hash obtido pelo hash djb2
        k = hashFunction(hash);
        ///posição está vazia?
        if(this->records[k] == NULL){
            ///insere o registro na posição da tabela e atribui seu próximo como nulo
            reg->setNext(NULL);
            this->records[k] = reg;
        }else{
            ///chama a função de inserção auxiliar que irá inserir o registro na lista encadeada de colisões
            this->colisions+=1;
            insertAux(records[k], reg);
        }

    }else{
        std::cout<<"tabela: "<<name<<" numero de colunas: "<<columns.size()<<" numero de valores: "<<values.size()<<std::endl;
        perror ("Número errado de colunas");
    }
    this->records_number+=1;


}

/***
    Description:
        procura um registro na tabela
    Params:
        id - chave para busca do registro, se for chave composta, as chaves são passadas separadas por espaço
***/
Record* Table::searchRecord(std::string id)
{

    std::string aux;
    std::stringstream ss;
    ss.str(id);
    unsigned long hash = 0;
    ///as chaves compostas são passadas com espaço separando, esse loop quebra essas chaves e calcula o valor de djb2 para
    ///cada uma delas e acumula em uma variável
    while(getline(ss, aux, ' ')){
        hash+= djb2(aux);
    }
    ///retira o espaço separador das chaves de busca, o que acaba concatenado elas
    id.erase(std::remove(id.begin(), id.end(), ' '), id.end());

    ///calcula o hash de divisão para o valor de djb2 resultado
    int k = hashFunction(hash);
    ///a posição está vazia? se estiver o registro não existe
    if(records[k] == NULL) return NULL;
    ///concatena as chaves primárias do registro da posição k, se ela for igual a chave de busca concatenada achou o registro
    if(concatComposeKey(records[k]) == id){
        return records[k];
    }else{
        ///caso contrário é preciso percorrer a lista de colisões da posição k até que se ache o registro ou chegue ao final
        ///da lista
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
void Table::removeRecord(std::string id)
{
    Record* r = searchRecord(id);
    if(r == NULL){
        std::cout<<"Registro inexistente"<<std::endl;
        return;
    }
    std::string aux;
    std::stringstream ss;
    ss.str(id);
    unsigned long hash = 0;
    ///as chaves compostas são passadas com espaço separando, esse loop quebra essas chaves e calcula o valor de djb2 para
    ///cada uma delas e acumula em uma variável
    while(getline(ss, aux, ' ')){
        hash+= djb2(aux);
    }
    ///retira o espaço separador das chaves de busca, o que acaba concatenado elas
    id.erase(std::remove(id.begin(), id.end(), ' '), id.end());

    ///calcula o hash de divisão para o valor de djb2 resultado
    int k = hashFunction(hash);
    ///é o primeiro da lista
    if(r == this->records[k]){
        this->records[k] = r->getNext();
        delete r;
    ///caso contrario
    }else if(r->getNext() == NULL){
        Record* p = records[k];
        while(p->getNext() != r){
            p = p->getNext();
        }
        p->setNext(r->getNext());
        delete r;
    }
}



/**
    Params:
    p: primeiro elemento da lista encadeada da posição da tabela onde ocorreu colisão
    n: registro que sera inserido
**/
void Table::insertAux(Record* p, Record* n)
{
    ///vai ate o fim da tabela
    while(p->getNext() != NULL)
        p = p->getNext();
    ///atribui o proximo do novo registro como nulo
    n->setNext(NULL);
    ///atribui o proximo do ultimo no da lista como o novo registro
    p->setNext(n);
}

///construtor, inicializa os campos e aloca o que é necessário
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

///destrutor: percorre a lista encadeada de todas as entradas da tabela desalocando seus nós, no final desaloca o vetor
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

///recebe as definições de chave primária a cria um vetor de indices que indexa as colunas que sao chave primarias
void Table::setPrimaryKeys(std::vector<std::string> columns_primary)
{


    for(int i = 0; i < columns_primary.size(); ++i)
        for(int j = 0; j< columns.size(); ++j){
            if(columns_primary[i].compare(columns[j]) == 0)
                primaryKeysIndex.push_back(j);

        }


}
///concatena as chaves primárias de uma tabela
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

///função utilizada somente pra auxiliar nos experimentos
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

void Table::selectCount()
{
    int count = 0;
    for(int i = 0; i < TSIZE; ++i){
        ///entrada i da tabela de hash não está vazia
        if(records[i] != NULL){
            count++;
            ///percorre a lista de colisoes da posicao i
            Record* p = records[i]->getNext();
            while(p != NULL){
                count++;
                p = p->getNext();
            }
        }
    }
    std::cout<<count<<" registros encontrados para esta consulta."<<std::endl;

}

void Table::selectCountByField(std::string field, std::string value)
{
    int count = 0;

    int columnIndex = this->getFieldIndex(field);

    if(columnIndex == -1){
        std::cout<<"Campo inexistente na tabela " <<this->name<<std::endl;
        return;
    }
    std::vector<std::string> values;
    for(int i = 0; i < TSIZE; ++i){

        if(records[i] != NULL){
            values = records[i]->getValues();
            if(values[columnIndex] == value){
                count++;
            }
            Record* p = records[i]->getNext();
            while(p != NULL){
                values = p->getValues();
                if(values[columnIndex] == value){
                    count++;
                }
                p = p->getNext();
            }

        }
    }

    std::cout<<count<<" registros encontrados para esta consulta."<<std::endl;
}


std::vector<Record*> Table::moveRecToVector()
{
    std::vector<Record*> auxVector;
    int count = 0;
    for(int i = 0; i < TSIZE; ++i){
        ///entrada i da tabela de hash não está vazia
        if(records[i] != NULL){
            auxVector.push_back(records[i]);
            ///percorre a lista de colisoes da posicao i
            Record* p = records[i]->getNext();
            while(p != NULL){
                auxVector.push_back(p);
                p = p->getNext();
            }
        }
    }
    return auxVector;
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

int Table::getFieldIndex(std::string field)
{
    int columnIndex = -1;
    for(int i = 0; i < columns.size(); ++i){
        if(field == columns[i]){
            columnIndex = i;
            break;
        }
    }
    return columnIndex;
}
