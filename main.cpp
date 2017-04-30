#include <iostream>
#include <fstream>
#include "Database.h"
#include <string.h>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;
void readSql(std::string path);
int main()
{
    readSql("/home/eduardo/Documentos/usda-r18-1.0/usda.sql");
    return 0;


}

void readSql(std::string path){

    std::ifstream file;
    std::string line;
    std::string data = "";
    std::string col;
    std::string tableName;
    std::stringstream ss;
    std::vector<std::string> columns;
    std::vector<std::string> primaryKeys;
    std::vector<string> tokens;
    Database* db = new Database();
    Table* table = NULL;
    file.open(path.c_str(), std::ifstream::in);

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
            if(line.find("PRIMARY KEY") != std::string::npos){

                while(getline(ss,line,'('));
                    //std::cout<<line<<std::endl;
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

            /*pega o nome da tabela*/
            std::string t = tokens[0];
            ss.str("");
            ss.clear();
            ss.str(t);
            getline(ss,tableName , ' ');
            getline(ss, tableName, ' ');

            t = tokens[1];

            /*pega as colunas da tabela*/
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



            /*cria a tabela e a insere no banco de dados*/
            table = new Table(tableName, columns);
            db->insertNode(table);
            tokens.erase(tokens.begin(), tokens.begin());

        }
        tokens.erase(tokens.begin(), tokens.end());
        columns.erase(columns.begin(), columns.end());
    }

    file.clear();
    file.seekg(0, ios::beg);
    tokens.erase(tokens.begin(), tokens.end());

     while(std::getline(file,line)){
        if(line.find("COPY") == 0){

            data = line;

            while(line.find(")") == std::string::npos){
                data+=line;
                std::getline(file, line);
            }

            std::stringstream ss(data);
            std::vector<string> tokens;

            while(getline(ss, tableName, '(')){
                tokens.push_back(tableName);
            }

            /*pega o nome da tabela*/
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




    db->searchRecord("weight", "83110 5");


}
