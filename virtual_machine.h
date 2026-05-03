#include <cstdint>
#include <iostream>


class Machine{
public:
    uint8_t* data_mem;
    uint8_t* inst_mem;
    int pc = 0;
    int ir = 0;
    bool sr = false;
    bool run = true;

    int get_next_arg(int);
    int get_arg(int);

    void add(int, int);
    void min(int, int);
    void mul(int, int);
    void div(int, int);

    void execute();

    Machine(int, int);
};
