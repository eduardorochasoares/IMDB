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

 /*  Database* db = new Database();
   std::vector<std::string> columns;
   columns.push_back("id");
   columns.push_back("nome");
   columns.push_back("idade");

   Table* table = new Table("Pessoa",columns);
   db->insertNode(table);
   Record* r = new Record();
   std::vector<std::string> values;
   values.push_back("99");
   values.push_back("Eduardo");
   values.push_back("22");
   r->setValues(values);
   table->insertRecord(r);

   Record* r2 = new Record();
   std::vector<std::string> values2;
   values2.push_back("1");
   values2.push_back("Eduardo2");
   values2.push_back("23");
   r2->setValues(values2);
   table->insertRecord(r2);
   std::cout<<db->searchRecord("Pessoa", "99")->getValues().at(1)<<std::endl;*/


}

void readSql(std::string path){
    std::ifstream file;
    std::string line;
    std::string data = "";
    std::string col;
    std::string tableName;
    std::vector<std::string> columns;
    Database* db = new Database();

    file.open(path.c_str(), std::ifstream::in);

    while(std::getline(file,line)){
        while(line.find("COPY") != 0){
            std::getline(file, line);
        }
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
        Table* table = new Table(tableName, columns);
        db->insertNode(table);

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
                //std::cout<<col<<std::endl;

                tokens.push_back(col);


            }
            int i = table->getColumns().size() - tokens.size();
            if(i > 0){
                while(i > 0){
                    tokens.push_back("");
                    --i;
                    }
            }

            Record* r = new Record();
            r->setValues(tokens);
            table->insertRecord(r);

            getline(file, line);
        }
        columns.erase(columns.begin(), columns.end());

    }
    std::cout<<"xablau"<<std::endl;
    db->searchRecord("nut_data", "01001");


}
