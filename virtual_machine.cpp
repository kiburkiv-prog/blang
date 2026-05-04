#include "virtual_machine.h"
#include <cstdint>

Machine::Machine(int data, int inst){
    //this->data_mem = new uint8_t[data];
    //this->inst_mem = new uint8_t[inst];
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


void Machine::execute(){
    uint8_t opcode = this->inst_mem[this->pc];


    switch(opcode){
        case 0:
            this->run = false;
            break;
        case 0x01:
            this->add(this->get_next_arg(), this->get_next_arg(5));
            break;
        case 0x02:
            this->add(this->get_next_arg(), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x03:
            this->add(this->get_arg(this->get_next_arg()), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x04:
            this->mul(this->get_next_arg(), this->get_next_arg(5));
            break;
        case 0x05:
            this->mul(this->get_next_arg(), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x06:
            this->mul(this->get_arg(this->get_next_arg()), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x07:
            this->div(this->get_next_arg(), this->get_next_arg(5));
            break;
        case 0x08:
            this->div(this->get_next_arg(), this->get_arg(this->get_next_arg(5)));
            break;
        case 0x09:
            this->div(this->get_arg(this->get_next_arg()), this->get_arg(this->get_next_arg(5)));
            break;
        case 0xA:
            this->div( this->get_arg(this->get_next_arg()), this->get_next_arg(5));
            break;


        default:
            std::cout << "Failed to execute instruction : unknown opcode";
    }
}


int main(){
    Machine machine(128, 128);

    uint8_t instructions[] = {1, 0,0,1,1 ,0,0,0,4, 0x02, 0,0,0,1, 0,0,0,0, 0x04, 0,0,0,10, 0,0,0,4, 0};
    uint8_t data[] = {1,0,0,4,0,0,0,10};

    machine.data_mem = data;
    machine.inst_mem = instructions;


    while(machine.run){
        machine.execute();
    }

    std::cout << machine.ir;
    return 0;
}
