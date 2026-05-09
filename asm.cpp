#include "asm.h"
#include <cstdint>
#include <cstdio>

std::map<std::string, std::string> hash;
std::map<std::string, std::string> inst_prefixes;
const int instructions_quan = 9;
const int instruction_prefixes_len = 11;

void load_hash(std::map<std::string, std::string>& h){
    std::string instructions[instructions_quan] = {"brk","add", "mul", "div", "com", "ptr", "load", "rem", "go"};
    std::string codes[instructions_quan] = {"000", "001", "002", "003", "004", "005", "006", "007", "008"};
    std::string types[instruction_prefixes_len] = {"", "numbernumber", "numberadress", "adressnumber", "numbernumberadress", "numberadressnumber", "adress", "number", "adressadress", "numberadressadress", "numbernumbernumber"};
    std::string prefixes[instruction_prefixes_len] = {"0","1", "2", "3", "4", "5", "6", "7", "8", "9", "A"};

    for(int i = 0; i < instructions_quan; i++){
        h.insert(std::pair<std::string, std::string>(instructions[i], codes[i]));
    }
    for(int i = 0; i <instruction_prefixes_len; i++){
        inst_prefixes.insert(std::pair<std::string, std::string>(types[i], prefixes[i]));
    }
}

unsigned to_power(int num, int power){
    unsigned int result = 1;
    for(int i = 0; i < power; i++){
        result *= num;
    }
    return result;
}

uint16_t inst_to_16bit(std::string instruction){
    uint16_t result = 0;

    for(int i = instruction.length() - 1; i >= 0; i--){
        int pivot;
        if(instruction[i] > 57){
            pivot = instruction[i] - 55;
        }
        else{
            pivot = instruction[i] - 48;
        }
        result += pivot *to_power(16, instruction.length() - 1 - i);
    }

    return result;
}



Token::Token(std::string name, std::string type, std::string subtype, std::string subsubtype){
    this->name = name;
    this->type = type;
    this->subtype = subtype;
    this->subsubtype = subsubtype;
}


//reading functions
std::string read_file(const char* file){
    std::ifstream code(file);
    std::stringstream buffer;
    buffer << code.rdbuf();
    std::string content = buffer.str();
    return content;
}

void read_tokens_from_file(std::string code, LinkedList* tokens, size_t& tokens_len){
    std::string pivot_token;
    std::string token_type = "inst";
    std::string token_subtype = "";
    std::string token_subsubtype = "";
    LinkedList* current_token = tokens;

    for(int i = 0; i < code.length(); i++){
        //switching types

        if(code[i] == '#'){
            token_subtype = "number";
            token_subsubtype = "integer";
        }
        else if(code[i] == '$'){
            token_subtype = "adress";
            token_subsubtype = "integer";
        }
        else if (code[i] == '@') {
            token_subtype = "number";
            token_subsubtype = "byte";
        }

        std::cout << code[i] << "\n";

        if(code[i] == '\n'){
            continue;
        }

        if(code[i] == ' ' || code[i] == ';'){
            tokens_len++;
            current_token->value = (void*)new Token(pivot_token, token_type, token_subtype, token_subsubtype);
            if(token_type == "inst"){
                token_type = "operand";
                ((Token*)(current_token->value))->instruction_code = hash[pivot_token];
            }
            current_token->next = new LinkedList;
            current_token = current_token->next;
            pivot_token = "";
            token_subtype = "";
            if(code[i] == ';'){
                token_type = "inst";
            }
            continue;
        }

        pivot_token += code[i];

    }
}

std::string analyze_operands(Token** tokens, size_t tokens_len){
    std::string types = "";

    for(int i = 0; i < tokens_len; i++){
        types += tokens[i]->subtype;
    }

    return inst_prefixes[types];
}

template<class X> byte_operand<X> get_byte_operand(Token* operand){
    X value = 0;
    if(operand->subsubtype == "integer" || operand ->subsubtype == "byte"){
        for(int i = 1; i < operand->name.length(); i++){
            value *= 10;
            value += operand->name[i] - 48;
        }
    }
    return {sizeof(X), value};
}

void write_integer(std::ofstream* file, int value){
    int byte_offset = 24;
    for(int i = 0; i < 4; i++){
        file->put((char) (value >> byte_offset));
        byte_offset -= 8;
    }
}



void compile(LinkedList* tokens, size_t tokens_len, std::string output){
    std::ofstream of(output.c_str(), std::ios::binary);
    LinkedList* current_node = tokens;
    std::string current_instruction_code;
    std::string instruction_prefix;
    size_t operands_len = 0;
    Token** operands = new Token* [3];

    for(int i = 0; i < tokens_len; i++){
        Token* current_token = ((Token*)current_node->value);
        Token* next_token = i == tokens_len - 1 ? current_token : ((Token*)current_node->next->value);
        if(current_token->type == "inst"){
            current_instruction_code = current_token->instruction_code;
            operands_len = 0;
        }
        else{
            operands[operands_len] = ((Token*)current_node->value);
            operands_len++;
        }
        if(next_token->type == "inst" || i == tokens_len - 1){
            instruction_prefix = analyze_operands(operands, operands_len);
            uint16_t instruction = inst_to_16bit(instruction_prefix + current_instruction_code);
            of.write((const char*)(&instruction) + 1, 1);
            of.write((const char*)(&instruction), 1);
            for(int i = 0; i < operands_len; i++){
                byte_operand<uint8_t> byte;
                byte_operand<int> integer;
                if(operands[i]->subsubtype == "byte"){
                    byte = get_byte_operand<uint8_t>(operands[i]);
                    of.write((const char*)&byte.val, 1);
                }
                else{
                    integer = get_byte_operand<int>(operands[i]);
                    write_integer(&of, integer.val);
                }

            }

        }
        current_node = current_node->next;
    }
    of.close();

}



std::string code;
LinkedList tokens;
size_t tokens_len = 0;

int main(int argc, char* argv[]){
    load_hash(hash);
    code = read_file(argv[1]);
    read_tokens_from_file(code, &tokens, tokens_len);
    std::cout <<tokens_len << "\n";

    LinkedList* pivot = &tokens;
    for(int i = 0; i < tokens_len; i++){
        std::cout<<"Name : " << ((Token*)(pivot->value))->name << "\n";
        std::cout<<"Type : " << ((Token*)(pivot->value))->type << "\n";
        std::cout<<"Subtype : " << ((Token*)(pivot->value))->subtype << "\n";
        std::cout<<"Instruction code : " << ((Token*)(pivot->value))->instruction_code<< "\n\n\n";
        pivot = pivot->next;
    }
    compile(&tokens, tokens_len, argv[2]);



    return 0;
}
