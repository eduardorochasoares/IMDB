#include <iostream>
#include <fstream>
#include "Database.h"
#include <string.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <climits>
#include <stdlib.h>
#include <time.h>



void readSqlAndIndexing(std::string path, Database* db);


/**
    Menu que permite ao usuário escolher uma ação a ser tomada
**/
int main()

{
    srand(time(NULL));
    Database* db = new Database();
    int opt;
    std::string path = "";
    do{
        std::cout<<"Escolha uma opção"<<std::endl;
        std::cout<<"1 - Indexar base de dados em um arquivo"<<std::endl;
        std::cout<<"2 - Criar uma tabela manualmente"<<std::endl;
        std::cout<<"3 - Inserir registros em uma tabela manualmente"<<std::endl;
        std::cout<<"4 - Buscar um registro"<<std::endl;
        std::cout<<"5 - Remover um registro"<<std::endl;
        std::cout<<"6 - SELECT COUNT(*) FROM TABLE;"<<std::endl;
        std::cout<<"7 - SELECT COUNT(*) FROM TABLE WHERE ID = X;"<<std::endl;
        std::cout<<"9 - Sair"<<std::endl;
        std::cin >> opt;

        switch(opt){
            case 1:{
                std::cout<<"Informe o caminho completo do arquivo sql"<<std::endl;
                std::cin >> path;
                clock_t begin = clock();
                readSqlAndIndexing(path, db);
                clock_t end = clock();

                std::cout<<"Indexacao concluida"<<std::endl;
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                std::cout<<"Time elapsed: " <<time_spent<<std::endl;
                break;

            }
            case 2:{

                std::string name;
                std::string aux;
                Table* table;
                std::vector<std::string> columns;
                std::cout<<"Informe o nome da tabela"<<std::endl;
                std::cin >> name;
                std::cin.ignore();

                std::cout<<"Informe o nome das colunas da tabela em uma única linha e separados por espaço. Para definir um ou mais campos como chave primaria, coloque * no final de seu nome sem espaco"<<std::endl;
                std::cout<<"Exemplo: id* nome idade"<<std::endl;
                std::getline (std::cin, aux);
                std::stringstream ss;
                std::vector<int> primaryKeysIndex;

                ss.str(aux);
                int i = 0;
                while(getline(ss, aux, ' ')){
                    if(aux.find("*") != std::string::npos){
                        primaryKeysIndex.push_back(i);
                        aux.erase(std::remove(aux.begin(), aux.end(), '*'), aux.end());
                    }
                    columns.push_back(aux);
                    ++i;
                }

                table = new Table(name, columns);
                table->setPrimaryKeyIndex(primaryKeysIndex);

                clock_t begin = clock();
                db->insertNode(table);
                clock_t end = clock();
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                std::cout<<"Time elapsed: " <<time_spent<<std::endl;
                break;
            }

            case 3:{
                std::string tableName;
                std::string aux;
                std::stringstream ss;
                std::vector<std::string> values;
                Record* record;
                std::cout<<"Informe o nome da tabela onde o registro sera inserido: "<<std::endl;
                std::cin >> tableName;
                Table* t = db->searchTable(tableName);
                if(t == NULL){
                    std::cout<<"A tabela nao existe"<<std::endl;
                    break;
                }
                std::cout<<"Digite em uma única linha separado por espaco o valor dos seguintes campos (em ordem):"<<std::endl;
                std::vector<std::string> fields = t->getColumns();

                for(int i = 0; i < fields.size(); ++i)
                    std::cout<<fields[i]<<" ";

                std::cout<<std::endl;
                std::cin.ignore();
                std::getline (std::cin, aux);
                ss.str(aux);

                while(getline(ss, aux, ' ')){
                    values.push_back(aux);

                }
                record = new Record();
                record->setValues(values);
                t->insertRecord(record);
                break;
            }

            case 4:{
                std::string tableName;
                std::cout<<"Digite o nome da tabela onde a busca sera feita"<<std::endl;
                std::cin >> tableName;
                Table* t = db->searchTable(tableName);

                if(t == NULL){
                    std::cout<<"Tabela inexistente"<<std::endl;
                    std::cout<<std::endl;
                }else{
                    std::vector<std::string> columns;
                    std::string aux;
                    std::string values;
                    int count = 0;
                    std::vector<int> indexPrimaryKeys = t->getPrimaryKeyIndex();
                    columns = t->getColumns();
                    std::cout<<"A tabela selecionada tem os ćampos abaixo como chave primaria, caso seja mais de uma digite os valores em uma única linha separados por espaco."<<std::endl;
                    std::cout<<"Campos de chave primaria: ";
                    for(int i = 0; i < indexPrimaryKeys.size(); ++i){
                        std::cout<<columns[indexPrimaryKeys[i]]<<" ";
                        ++count;
                    }
                    std::cout<<std::endl;
                    std::cin.ignore();
                    std::getline (std::cin, aux);
                    std::cout<<std::endl;
                    std::cout<<"Registro Retornado"<<std::endl;
                    std::cout<<std::endl;

                    t->printResult(t->searchRecord(aux));

                    std::cout<<std::endl;
                }
                break;
            }

            case 5:{
                std::string tableName;

                std::cout<<"Digite o nome da tabela onde ocorrerá a remoção"<<std::endl;
                std::cin >> tableName;

                Table* t = db->searchTable(tableName);

                if(t == NULL){
                    std::cout<<"Tabela inexistente no banco de dados"<<std::endl;
                }else{
                    int count = 0;
                    std::string aux;
                    std::string values;
                    std::vector<std::string> columns;
                    std::vector<int> indexPrimaryKeys = t->getPrimaryKeyIndex();
                    columns = t->getColumns();
                    std::cout<<"A tabela selecionada tem os ćampos abaixo como chave primaria, caso seja mais de uma digite os valores em uma única linha separados por espaco."<<std::endl;
                    std::cout<<"Campos de chave primaria: ";
                    for(int i = 0; i < indexPrimaryKeys.size(); ++i){
                        std::cout<<columns[indexPrimaryKeys[i]]<<" ";
                        ++count;
                    }
                    std::cout<<std::endl;
                    std::cin.ignore();
                    std::getline (std::cin, aux);




                    t->removeRecord(aux);


                }
                break;
            }

            case 6:{
                std::string tableName;

                std::cout<<"Digite o nome da tabela onde a consulta será realizada"<<std::endl;
                std::cin >> tableName;

                Table* t = db->searchTable(tableName);

                if(t == NULL){
                    std::cout<<"Tabela inexistente no banco de dados"<<std::endl;
                }else{
                    t->selectCount();
                }
                break;
            }

            case 7:{
                std::string tableName;
                std::string aux;
                std::stringstream ss;
                std::vector<std::string> fields;
                std::vector<std::string> values;
                std::cout<<"Digite o nome da tabela onde a consulta será realizada"<<std::endl;
                std::cin >> tableName;
                std::vector<std::string> columns;

                Table* t = db->searchTable(tableName);

                if(t == NULL){
                    std::cout<<"Tabela inexistente no banco de dados"<<std::endl;
                }else{
                    columns = t->getColumns();
                    std::cout<<"Os campos da tabela são: "<<std::endl;
                    for(int i = 0; i < columns.size(); ++i)
                        std::cout<<columns[i]<<"\t"<<std::endl;

                    std::cout<<"Entre com o nome de um ou mais desses campos para realizar a contagem"<<std::endl;
                    std::cin.ignore();

                    std::getline (std::cin, aux);
                    ss.str(aux);

                    while(getline(ss, aux, ' ')){
                        fields.push_back(aux);

                    }

                    std::cout<<"Entre com os valores dos campos de busca definidos acima, separados por espaço, e na ordem que os campos foram definidos para busca"<<std::endl;

                    std::string a;
                    std::getline (std::cin, a);


                    ss.str("");
                    ss.clear();
                    ss.str(a);
                    while(getline(ss, a, ' ')){
                        values.push_back(a);

                    }
                    std::cout<<"values size: "<<values.size();


                    t->selectCountByFields(fields, values);
                }
                break;
            }

            case 8:{
                std::string table1 = "nut_data";
                std::string table2 = "datsrcln";
                std::string field = "ndb_no";
                std::vector<std::string> fields;

                fields.push_back(field);
                fields.push_back("nutr_no");

                clock_t begin = clock();

                db->innerJoin(db->searchTable(table1), db->searchTable(table2), fields);

                clock_t end = clock();

                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                std::cout<<"Time elapsed: " <<time_spent<<std::endl;


                break;
            }

        }

    }while(opt != 9);

    delete db;


    return 0;


}

/**
    Description: Função que lê o arquivo sql da base de dados, e chama as funções de inserção
    de tabelas e registros

    Params:
        path - caminho para o arquivo sql
        db - Objeto que representa o banco de dados onde serão criadas as tabelas e inseridos
        os registros
**/
void readSqlAndIndexing(std::string path, Database* db){

    std::ifstream file;
    std::string line;
    std::string data = "";
    std::string col;
    std::string tableName;
    std::stringstream ss;
    std::vector<std::string> columns;
    std::vector<std::string> primaryKeys;
    std::vector<std::string> tokens;

    Table* table = NULL;
    file.open(path.c_str(), std::ifstream::in);
    if(!file.good()){
            std::cout<<"Arquivo nao existe"<<std::endl;
            return;
    }
    while(std::getline(file,line)){
        if(line.find("ALTER TABLE ONLY") != std::string::npos){
            primaryKeys.erase(primaryKeys.begin(), primaryKeys.end());

            ss.str("");
            ss.clear();
            ss.str(line);


            while(getline(ss, line, ' '));

            tableName = line;


            std::getline(file,line);
            ss.str("");
            ss.clear();
            ss.str(line);
            ///achou a definicao de chaves primárias
            if(line.find("PRIMARY KEY") != std::string::npos){

                while(getline(ss,line,'('));

                ss.str("");
                ss.clear();
                ss.str(line);
                while(getline(ss, line, ',')){
                    line.erase(std::remove(line.begin(), line.end(), ')'), line.end());
                    line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
                    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
                    primaryKeys.push_back(line);

                }

                db->alterTable(tableName, primaryKeys);
            }

        }

        if(line.find("COPY") == 0){
            data = line;

            while(line.find(")") == std::string::npos){
                data+=line;
                std::getline(file, line);
            }

            ss.clear();
            ss.str("");
            ss.str(data);


            while(getline(ss, tableName, '(')){
                tokens.push_back(tableName);
            }

            ///pega o nome da tabela///
            std::string t = tokens[0];
            ss.str("");
            ss.clear();
            ss.str(t);
            getline(ss,tableName , ' ');
            getline(ss, tableName, ' ');

            t = tokens[1];

            ///pega as colunas da tabela///
            t = tokens[1];
            ss.str("");
            ss.clear();
            ss.str(t);
            getline(ss,col , ')');


            ss.str("");
            ss.clear();
            ss.str(col);

            while(getline(ss, col, ',')){

                col.erase(std::remove(col.begin(), col.end(), ' '), col.end());
                columns.push_back(col);

            }



            ///cria a tabela e a insere no banco de dados///
            table = new Table(tableName, columns);
            db->insertNode(table);
            tokens.erase(tokens.begin(), tokens.begin());

        }
        tokens.erase(tokens.begin(), tokens.end());
        columns.erase(columns.begin(), columns.end());
    }

    file.clear();

    ///volta para o início do arquivo
    file.seekg(0, std::ios::beg);
    tokens.erase(tokens.begin(), tokens.end());

     while(std::getline(file,line)){
        ///achou onde estão os registros
        if(line.find("COPY") == 0){

            data = line;

            while(line.find(")") == std::string::npos){
                data+=line;
                std::getline(file, line);
            }

            std::stringstream ss(data);
            std::vector<std::string> tokens;

            while(getline(ss, tableName, '(')){
                tokens.push_back(tableName);
            }

            ///pega o nome da tabela///
            std::string t = tokens[0];
            ss.str("");
            ss.clear();
            ss.str(t);
            getline(ss,tableName , ' ');
            getline(ss, tableName, ' ');

            t = tokens[1];
            table = db->searchTable(tableName);
            tokens.erase(tokens.begin(), tokens.end());
            getline(file, line);

            while(line.find("\\.") == std::string::npos){

                ss.str("");
                ss.clear();
                ss.str(line);
                tokens.erase(tokens.begin(), tokens.end());
                while(getline(ss,col, '\t' )){

                    if(col == "")
                        col = "null";


                    tokens.push_back(col);


                }

                int i = table->getColumns().size() - tokens.size();

                if(i > 0){
                    while(i > 0){
                        tokens.push_back("null");
                        --i;
                    }
                }
                ///insere o registro no banco de dados
                Record* r = new Record();
                r->setValues(tokens);
                db->insertRecord(tableName, r);

                getline(file, line);
            }
        }

        if(line.find("ALTER") != std::string::npos){
            break;
        }


    }


}
