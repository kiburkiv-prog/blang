#include <cstdint>
#include <iostream>


class Machine{
public:
    uint8_t* data_mem;
    uint8_t* inst_mem;
    int pc = 0;
    int ir = 0;
    bool br = false;
    bool run = true;

    int get_next_arg(int);
    int get_arg(int);
    uint8_t get_next_arg_byte(int);

    void add(int, int);
    void mul(int, int);
    void div(int, int);
    void com(uint8_t, int, int);
    void ptr(int);
    void load(uint8_t, int, int);
    void rem(uint8_t, int);
    void go(int);

    void execute();

    Machine(int, int);
};
