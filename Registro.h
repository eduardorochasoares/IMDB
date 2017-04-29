#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED
#include <string>
#include <vector>
class Register{
    private:
        std::vector<std::string> valores;
        Register* prox;

    public:
        std::vector<std::string>& getValores();
        void setValores(std::vector<std::string> val);
        void setProx(Register* r);
        Register* getProx();


};


#endif // REGISTRO_H_INCLUDED
