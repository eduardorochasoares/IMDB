#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED
#include <string>
#include <vector>
class Record{
    private:
        std::vector<std::string> values;
        Record* next;

        bool match = false; ///flag usado para o outerJoinNxM para verificar se o registro
                            ///teve match com alguém

    public:
        std::vector<std::string>& getValues();
        void setValues(std::vector<std::string> val);
        void setNext(Record* r);
        Record* getNext();
        bool hasMatch();
        void setMatch(bool match);

};


#endif // REGISTRO_H_INCLUDED
