#include "Record.h"

std::vector<std::string>& Record::getValues()
{
    return this->values;
}

void Record::setValues(std::vector<std::string> val)
{
    this->values = val;
}
void Record::setNext(Record* r)
{
    this->next = r;
}


Record* Record::getNext()
{
    return this->next;
}
