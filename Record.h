#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED
#include <string>
#include <vector>
class Record{
    private:
        std::vector<std::string> values;
        Record* next;

    public:
        std::vector<std::string>& getValues();
        void setValues(std::vector<std::string> val);
        void setNext(Record* r);
        Record* getNext();


};


#endif // REGISTRO_H_INCLUDED
