#include "Record.h"

std::vector<std::string>& Record::getValores()
{
    return this->valores;
}

void Record::setValores(std::vector<std::string> val)
{
    this->valores = val;
}
void Record::setProx(Record* r)
{
    this->prox = r;
}


Record* Record::getProx()
{
    return this->prox;
}
