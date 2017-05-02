#include "Record.h"

///retorna os valores de campo do registro
std::vector<std::string>& Record::getValues()
{
    return this->values;
}

///atribui um vetor com os  valores do campo
void Record::setValues(std::vector<std::string> val)
{
    this->values = val;
}
///atribui o seu próximo nó na fila de colisão
void Record::setNext(Record* r)
{
    this->next = r;
}

///retorna o seu próximo nó na fila de colisão
Record* Record::getNext()
{
    return this->next;
}
