#include <fstream>
#include <string>
#include <sstream>
#include <iostream>


std::string read_file(const char*);

//linked list realization
class LinkedList{
public:
    LinkedList* next;
    void* value;
};


class Token{
public:
    std::string name;
    std::string type;
    Token(std::string, std::string);
};
