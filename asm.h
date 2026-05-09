#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <utility>

template <class X> struct byte_operand{
    size_t lenght;
    X val;
};


std::string read_file(const char*);
void load_hash(std::map<std::string, std::string>&);


//linked list realization
class LinkedList{
public:
    LinkedList* next = this;
    void* value;
};


class Token{
public:
    std::string name;
    std::string type;
    std::string subtype;
    std::string subsubtype;
    std::string instruction_code;
    //Subtypes for operands -- #integer $address @byte

    Token(std::string, std::string, std::string, std::string);
};

void compile(LinkedList, size_t);
