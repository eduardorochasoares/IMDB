#include "Database.h"
#include <stdio.h>
#include <iostream>
#include <stack>
#include <fstream>
struct Local{
    Local(std::vector<int> param1){
        this->param1 = param1;

    }
    bool operator() (Record* r1, Record* r2){

        return Database::concatFieldsValue(r1, param1) < Database::concatFieldsValue(r2, param1);
    }
    std::vector<int> param1;
};
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


void Database::outerJoin(Table* t1, Table* t2, std::vector<std::string> field, char type)
{

    std::vector<int> fieldIndexT1;
    std::vector<int> fieldIndexT2;
    std::vector<std::string> rows;
    int current = 0;
    int previousInital = 0;
    std::vector<Record*> vecT1;
    std::vector<Record*> vecT2;

    std::vector<std::string> valuesT1;
    std::vector<std::string> valuesT2;

    if(type == 'R'){
        vecT1 = t2->moveRecToVector();
        vecT2 = t1->moveRecToVector();

        fieldIndexT1 = t2->getFieldIndex(field);
        fieldIndexT2 = t1->getFieldIndex(field);
    }else{
        vecT1 = t1->moveRecToVector();
        vecT2 = t2->moveRecToVector();


        fieldIndexT1 = t1->getFieldIndex(field);
        fieldIndexT2 = t2->getFieldIndex(field);
    }
    if(fieldIndexT1.empty() || fieldIndexT2.empty()){
        std::cout<<"Alguns dos campos informados não existem nas tabelas, tente novamente"<<std::endl;
        return;
    }

    std::sort(vecT1.begin(), vecT1.end(), Local(fieldIndexT1));
    std::sort(vecT2.begin(), vecT2.end(), Local(fieldIndexT2));


    int i;
    for(i = 0; i < vecT1.size(); ++i){

        if(i > 0){
            std::string values1 = concatFieldsValue(vecT1[i], fieldIndexT1);
            std::string values2 = concatFieldsValue(vecT1[i - 1], fieldIndexT1);

            if(values1 == values2){
                current = previousInital;
            }else{
                previousInital = current;

            }
        }
        std::string left ="";
        std::string right = "";
        std::string row = "";

        valuesT1 = vecT1[i]->getValues();
        for(int j = 0; j < valuesT1.size(); ++j){
            left+=valuesT1[j] + '\t';
        }

        std::string t1FieldsValue = concatFieldsValue(vecT1[i], fieldIndexT1);
        if(current < vecT2.size()){

            valuesT2 = vecT2[current]->getValues();
            std::string t2FieldsValue = concatFieldsValue(vecT2[current], fieldIndexT2);

            if(t1FieldsValue != t2FieldsValue){

                    while(t1FieldsValue > t2FieldsValue){
                        right ="";

                        for(int j = 0; j < valuesT2.size(); ++j){
                            right+=valuesT2[j] + '\t';
                        }

                        if(type == 'F'){
                            rows.push_back("NULL | " + right);
                        }

                        ++current;

                        if(current >= vecT2.size())
                            break;

                        t2FieldsValue = concatFieldsValue(vecT2[current], fieldIndexT2);
                        valuesT2 = vecT2[current]->getValues();

                    }

                    previousInital = current;

            }

            if(t1FieldsValue < t2FieldsValue){
                if(type == 'R'){
                   rows.push_back("NULL | " + left);

                }else{
                    rows.push_back  (left + " | NULL");
                }

            }else{
                while(t1FieldsValue == t2FieldsValue){
                    right = "";
                    for(int j = 0; j < valuesT2.size(); ++j){
                        right+=valuesT2[j] + '\t';
                    }

                     rows.push_back(left + " | " + right);

                    ++current;

                    if(current >= vecT2.size())
                        break;

                    t2FieldsValue = concatFieldsValue(vecT2[current], fieldIndexT2);
                    valuesT2 = vecT2[current]->getValues();

                }
            }

        }else{
            if(type == 'R'){
                rows.push_back("NULL | "  + left);
            }else{
                rows.push_back(left + " | NULL");
            }
        }






    }
    if(type == 'F'){
        while(current < vecT2.size()){


            if(current >= vecT2.size()) break;

            valuesT2 = vecT2[current]->getValues();
            std::string right = "";
            for(int j = 0; j < valuesT2.size(); ++j){
                right+=valuesT2[j] + '\t';
            }

            rows.push_back("NULL | "  + right);
            ++current;
        }
    }
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<"LINHAS RETORNADAS: "<<rows.size()<<std::endl;
        std::cout<<"Deseja Imprimir os resultados? Y ou N"<<std::endl;

        char choice;
        std::cin >> choice;

        if(choice == 'Y')
            for(int i = 0; i < rows.size(); ++i){
                std::cout<<rows[i]<<std::endl;
                std::cout<<std::endl;
        }else if(choice != 'N'){
            std::cout<<"Opção inválida"<<std::endl;
        }

}

void Database::innerJoin(Table* t1, Table* t2, std::vector<std::string> field)
{
    std::vector<int> fieldIndexT1 = t1->getFieldIndex(field);
    std::vector<int> fieldIndexT2 = t2->getFieldIndex(field);
    if(fieldIndexT1.empty() || fieldIndexT2.empty()){
        std::cout<<"Alguns dos campos informados não existem nas tabelas, tente novamente"<<std::endl;
        return;
    }

    std::vector<std::string> rows;
    int current = 0;
    int previousInital = 0;

    std::vector<Record*> vecT1 = t1->moveRecToVector();
    std::sort(vecT1.begin(), vecT1.end(), Local(fieldIndexT1));

    std::vector<Record*> vecT2 = t2->moveRecToVector();

    std::sort(vecT2.begin(), vecT2.end(), Local(fieldIndexT2));



    int i;

    for(i = 0; i < vecT1.size(); ++i){
        if(i > 0){
            std::string values1 = concatFieldsValue(vecT1[i], fieldIndexT1);
            std::string values2 = concatFieldsValue(vecT1[i - 1], fieldIndexT1);

            if(values1 == values2){
                current = previousInital;
            }else{
                previousInital = current;

            }
        }

        std::string left ="";
        std::string right = "";
        std::string row = "";
        std::vector<std::string> valuesT1 = vecT1[i]->getValues();
        std::string t1FieldsValues = concatFieldsValue(vecT1[i], fieldIndexT1);

        for(int j = 0; j < valuesT1.size(); ++j){
            left+=valuesT1[j] + '\t';
        }
        if(current >= vecT2.size())
            break;

        std::vector<std::string> valuesT2 = vecT2[current]->getValues();
        std::string t2FieldsValues = concatFieldsValue(vecT2[current], fieldIndexT2);

        if(t1FieldsValues != t2FieldsValues){

              while(t1FieldsValues > t2FieldsValues){
                    ++current;

                    if(current >= vecT2.size())
                            break;

                    t2FieldsValues = concatFieldsValue(vecT2[current], fieldIndexT2);
                    valuesT2 = vecT2[current]->getValues();


                }
                previousInital = current;
        }

        while(t1FieldsValues == t2FieldsValues){
            right ="";

            for(int j = 0; j < valuesT2.size(); ++j){
                right+=valuesT2[j] + '\t';
            }

            rows.push_back(left + " | " + right);

            ++current;

            if(current >= vecT2.size())
                break;

            t2FieldsValues = concatFieldsValue(vecT2[current], fieldIndexT2);
            valuesT2 = vecT2[current]->getValues();

        }





    }
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<"LINHAS RETORNADAS: "<<rows.size()<<std::endl;
        std::cout<<"Deseja Imprimir os resultados? Y ou N"<<std::endl;

        char choice;
        std::cin >> choice;

        if(choice == 'Y')
            for(int i = 0; i < rows.size(); ++i){
                std::cout<<rows[i]<<std::endl;
                std::cout<<std::endl;

        }else if(choice != 'N'){

            std::cout<<"Opção inválida"<<std::endl;
        }
}
void Database::innerJoinNxM(Table* t1, Table* t2, std::vector<std::string>field)
{
    Record** hashTable1 = t1->getRecords();
    Record** hashTable2 = t2->getRecords();
    std::vector<int> fieldIndexT1 = t1->getFieldIndex(field);
    std::vector<int> fieldIndexT2 = t2->getFieldIndex(field);
    if(fieldIndexT1.empty() || fieldIndexT2.empty()){
        std::cout<<"Alguns dos campos informados não existem nas tabelas, tente novamente"<<std::endl;
        return;
    }
    std::vector<std::string> rows;
    std::string left ="";
    std::string right = "";
    std::vector<std::string> valuesT1;
    std::vector<std::string> valuesT2;
    ///percorre as posições da tabela de hash de T1
    for(int i = 0; i < t1->getTsize(); ++i){

            if(hashTable1[i] != NULL){
                Record* p1 = hashTable1[i];
                ///percorre a lista de colisão da posição i de T1
                while(p1 != NULL){
                    left = "";
                    valuesT1 = p1->getValues();


                    for(int j = 0; j < valuesT1.size(); ++j){
                            left+=valuesT1[j] + '\t';
                    }

                    ///percorre as posições da tabela de hash de T2
                    for(int j = 0; j < t2->getTsize(); ++j){
                        if(hashTable2[j] != NULL){

                            Record* p2 = hashTable2[j];
                            ///percorre a lista de colisão da posição j de T2

                            while(p2 != NULL){
                                if(concatFieldsValue(p1, fieldIndexT1 ) == concatFieldsValue(p2, fieldIndexT2)){
                                        valuesT2 = p2->getValues();

                                       right = "";
                                       for(int k = 0; k < valuesT2.size(); ++k){
                                            right+=valuesT2[k] + '\t';
                                        }
                                        rows.push_back(left + " | " + right);

                                }
                                p2 = p2->getNext();
                            }

                        }
                    }
                    p1 = p1->getNext();
                }

            }
        }

        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<"LINHAS RETORNADAS: "<<rows.size()<<std::endl;

        char choice;
        std::cin >> choice;

        if(choice == 'Y')
            for(int i = 0; i < rows.size(); ++i){
                std::cout<<rows[i]<<std::endl;
                std::cout<<std::endl;
        }else if(choice != 'N'){
            std::cout<<"Opção inválida"<<std::endl;
        }
}

void Database::outerJoinNxM(Table* t1, Table* t2, std::vector<std::string>field, char type)
{
    Record** hashTable1 = t1->getRecords();
    Record** hashTable2 = t2->getRecords();
    std::vector<int> fieldIndexT1 = t1->getFieldIndex(field);
    std::vector<int> fieldIndexT2 = t2->getFieldIndex(field);

    if(fieldIndexT1.empty() || fieldIndexT2.empty()){
        std::cout<<"ALGUNS DOS CAMPOS INFORMADOS NÃO EXISTEM NAS TABELAS, TENTE NOVAMENTE"<<std::endl;
        return;
    }
    std::vector<std::string> rows;
    std::string left ="";
    std::string right = "";
    std::vector<std::string> valuesT1;
    std::vector<std::string> valuesT2;

    ///percorre as posições da tabela de hash de T1
    for(int i = 0; i < t1->getTsize(); ++i){

            if(hashTable1[i] != NULL){
                Record* p1 = hashTable1[i];
                ///percorre a lista de colisão da posição i de T1
                while(p1 != NULL){
                    left = "";
                    valuesT1 = p1->getValues();


                    for(int j = 0; j < valuesT1.size(); ++j){
                            left+=valuesT1[j] + '\t';
                    }

                    ///percorre as posições da tabela de hash de T2
                    for(int j = 0; j < t2->getTsize(); ++j){
                        if(hashTable2[j] != NULL){

                            Record* p2 = hashTable2[j];
                            ///percorre a lista de colisão da posição j de T2

                            while(p2 != NULL){
                                if(concatFieldsValue(p1, fieldIndexT1 ) == concatFieldsValue(p2, fieldIndexT2)){
                                        valuesT2 = p2->getValues();

                                       right = "";
                                       for(int k = 0; k < valuesT2.size(); ++k){
                                            right+=valuesT2[k] + '\t';
                                        }
                                        rows.push_back(left + " | " + right);
                                        p1->setMatch(true);
                                        p2->setMatch(true);

                                }
                                p2 = p2->getNext();
                            }

                        }
                    }
                    p1 = p1->getNext();
                }

            }
        }

        if(type != 'R'){
            for(int i = 0; i < t1->getTsize(); ++i ){
                Record* p;
                if(hashTable1[i] != NULL){
                    p = hashTable1[i];

                    while(p != NULL){
                        if(!p->hasMatch()){
                            left = "";
                            for(int k = 0; k < valuesT1.size(); ++k)
                                left+= valuesT1[k] + '\t';

                            rows.push_back(left + " | " + "NULL");

                        }else{
                            p->setMatch(false);
                        }
                        p = p->getNext();

                    }
                }
            }
        }

        if(type != 'L'){
            for(int i = 0; i < t2->getTsize(); ++i ){
                Record* p;
                if(hashTable2[i] != NULL){
                    p = hashTable2[i];

                    while(p != NULL){
                        if(!p->hasMatch()){
                            right = "";
                            for(int k = 0; k < valuesT2.size(); ++k)
                                right+= valuesT2[k] + '\t';

                            rows.push_back("NULL | " + right);



                        }else{
                            p->setMatch(false);
                        }
                        p = p->getNext();

                    }
                }
            }
        }

        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<"LINHAS RETORNADAS: "<<rows.size()<<std::endl;
        std::cout<<"Deseja Imprimir os resultados? Y ou N"<<std::endl;
        char choice;
        std::cin >> choice;

        if(choice == 'Y')
            for(int i = 0; i < rows.size(); ++i){
                std::cout<<rows[i]<<std::endl;
                std::cout<<std::endl;
        }else if(choice != 'N'){
            std::cout<<"Opção inválida"<<std::endl;
        }
}




std::string Database::concatFieldsValue(Record* r,std::vector<int> fieldsIndex)
{
    std::string concat = "";
    for(int i = 0; i < fieldsIndex.size(); ++i)
        concat += r->getValues().at(fieldsIndex[i]);

    return concat;
}

void Database::selectCount(std::string tableName)
{
    Table* t = searchTable(tableName);
    if(t == NULL){
        std::cout<<"Tabela não existe"<<std::endl;
    }else{
        t->selectCount();
    }
}

void Database::selectCountId(std::string tableName, std::vector<std::string> fields, std::vector<std::string> values)
{
    Table* t = searchTable(tableName);
    if(t == NULL){
        std::cout<<"Tabela não existe"<<std::endl;
    }else{
        t->selectCountByFields(fields, values);
    }
}
void Database::removeRecord(std::string tableName, std::string id)
{
    Table* t = searchTable(tableName);
    if(t == NULL){
        std::cout<<"Tabela não existe"<<std::endl;
    }else{
        t->removeRecord(id);
    }
}

