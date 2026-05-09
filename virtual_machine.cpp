#include "virtual_machine.h"
#include <cstdint>
#include <fstream>
#include <filesystem>

Machine::Machine(int data, int inst){
    this->data_mem = new uint8_t[data];
    this->inst_mem = new uint8_t[inst];
}


//ARGUMENTS GETTERS
int Machine::get_next_arg(int addr=1){
    uint8_t bytes[4] = {this->inst_mem[pc + addr], this->inst_mem[pc + addr + 1], this->inst_mem[pc + addr + 2], this->inst_mem[pc + addr + 3]};

    char pivot = 24;
    int result = 0;

    for(int i = 0; i < 4; i++){
        result += (int)bytes[i] << pivot;
        pivot -= 8;
    }
    return result;

}
int Machine::get_arg(int addr){
    uint8_t bytes[4] = {this->data_mem[addr], this->data_mem[addr + 1], this->data_mem[addr + 2], this->data_mem[addr + 3]};

    char pivot = 24;
    int result = 0;

    for(int i = 0; i < 4; i++){
        result += (int)bytes[i] << pivot;
        pivot -= 8;
    }
    return result;

}

uint8_t Machine::get_next_arg_byte(int addr = 1){
    return this->inst_mem[pc + addr];
}


//INTEGER OPERATIONS
void Machine::add(int a, int b){
    this->ir = a + b;
    this->pc += 9;
}


void Machine::mul(int a, int b){
    this->ir = a * b;
    this->pc += 9;
}

void Machine::div(int a, int b){
    this->ir = a / b;
    this->pc += 9;
}

//Boolean operations
//comparing two numbers
void Machine::com(uint8_t type, int a, int b){
    bool expression = false;

    switch(type){
        case 1:
            expression = a == b;
            break;
        case 2:
            expression = a > b;
            break;
        case 3:
            expression = a < b;
            break;
        case 4:
            expression = a >= b;
            break;
        case 5:
            expression = a <= b;
            break;
        case 6:
            expression = a != b;
            break;
    }

    this->br = expression;
    this->pc += 10;
}

//Memory operations
void Machine::load(uint8_t type, int addr, int num){
    switch(type){
        //Byte mode
        case 1:
            this->data_mem[addr] = num;
            break;
        //Integer (32-bit) mode
        case 2:
            this->data_mem[addr] = num >> 24;
            this->data_mem[addr + 1] = num >> 16;
            this->data_mem[addr + 2] = num >> 8;
            this->data_mem[addr + 3] = num;

            break;
    }
    this->pc += 10;
}

void Machine::rem(uint8_t type, int addr){
    switch(type){
        //Byte mode
        case 1:
            this->data_mem[addr] = 0;
            break;
            //Integer (32-bit) mode
        case 2:
            this->data_mem[addr] = 0;
            this->data_mem[addr + 1] = 0;
            this->data_mem[addr + 2] = 0;
            this->data_mem[addr + 3] = 0;
            break;
    }
    this->pc += 6;
}


//Program operations
void Machine::go(int addr){
    this->pc = this->inst_mem[addr];
}


//Other operations
void Machine::ptr(int start){
    std::cout << this->data_mem + start;
    this->pc += 5;
}


void Machine::execute(){
    uint16_t opcode = this->inst_mem[this->pc];
    opcode = (opcode << 8) + this->inst_mem[this->pc + 1];
    this->pc += 1;


    switch(opcode){
        case 0:
            this->run = false;
            break;
        case 0x1001:
            this->add(this->get_next_arg(), this->get_next_arg(5));
            break;
        case 0x2001:
            this->add(this->get_next_arg(), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x8001:
            this->add(this->get_arg(this->get_next_arg()), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x1002:
            this->mul(this->get_next_arg(), this->get_next_arg(5));
            break;
        case 0x2002:
            this->mul(this->get_next_arg(), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x8002:
            this->mul(this->get_arg(this->get_next_arg()), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x1003:
            this->div(this->get_next_arg(), this->get_next_arg(5));
            break;
        case 0x2003:
            this->div(this->get_next_arg(), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x8003:
            this->div(this->get_arg(this->get_next_arg()), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x3003:
            this->div( this->get_arg(this->get_next_arg()), this->get_next_arg(5));
            break;
        case 0x9004:
            this->com(this->get_next_arg_byte(), this->get_arg(this->get_next_arg(2)), this->get_arg(this->get_next_arg(6)));
        case 0x6005:
            this->ptr(this->get_next_arg());
            break;
        case 0x5006:
            this->load(this->get_next_arg_byte(), this->get_next_arg(2), this->get_next_arg(6));
            break;
        case 0x2007:
            this->rem(this->get_next_arg_byte(), this->get_next_arg(2));
            break;
        case 0x6008:
            this->go(this->get_next_arg());
            break;

        default:
            std::cout << "Failed to execute instruction : unknown opcode :" <<opcode<< "\n";
            this->run = false;
    }
}


int main(int argc, char* argv[]){
    Machine machine(128, 128);



    std::ifstream file(argv[1]);

    for(int i = 0;i < std::filesystem::file_size(argv[1]); i++){
        machine.inst_mem[i] = (uint8_t)file.get();
    }


    while(machine.run){
        machine.execute();
    }

    //std::cout << machine.ir;
    return 0;
}
