#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED
#include <string>
#include <vector>
class Record{
    private:
        std::vector<std::string> valores;
        Record* prox;

    public:
        std::vector<std::string>& getValores();
        void setValores(std::vector<std::string> val);
        void setProx(Record* r);
        Record* getProx();


};


#endif // REGISTRO_H_INCLUDED
