#include "asm.h"



Token::Token(std::string name, std::string type){
    this->name = name;
    this->type = type;
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
    LinkedList* current_token = tokens;

    for(int i = 0; i < code.length(); i++){
        if(code[i] == '\n'){
            continue;
        }

        if(code[i] == ' ' || code[i] == ';'){
            tokens_len++;
            current_token->value = (void*)new Token(pivot_token, token_type);
            current_token->next = new LinkedList;
            current_token = current_token->next;
            pivot_token = "";
            if(token_type == "inst"){
                token_type = "operand";
            }
            if(code[i] == ';'){
                token_type = "inst";
            }
            continue;
        }

        pivot_token += code[i];

    }
}



std::string code;
LinkedList tokens;
size_t tokens_len = 0;

int main(int argc, char* argv[]){
    code = read_file(argv[1]);
    read_tokens_from_file(code, &tokens, tokens_len);


    LinkedList* pivot = &tokens;
    for(int i = 0; i < tokens_len; i++){
        std::cout<<"Name : " << ((Token*)(pivot->value))->name << "\n";
        std::cout<<"Type : " << ((Token*)(pivot->value))->type << "\n\n\n";
        pivot = pivot->next;
    }

    return 0;
}
