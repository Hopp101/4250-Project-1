#include <iostream>
#include <string>
#include <cctype>

using namespace std;

//Creating a Lexicalanalyzer Class
class Lexicalanalyzer{
    //All C Global and Function declarations in a private access specifier.
private:
    int charClass;
    string lexeme; // replacing the C (char) with string
    char nextChar; //storage; don't care what it is.
    int LexLen;
    int token;
    int nextToken;

    //C Functions declared in private access specifier
    void addChar();
    void getChar();
    void getNonBlank();
    int lex();


public:
    enum tokenCodeTypes{
        INT_LIT = 10,
        IDENT = 11,
        ASSIGN_OP = 20,
        ADD_OP = 21,
        SUB_OP = 22,
        MULT_OP = 23,
        DIV_OP = 24,
        LEFT_PAREN = 25,
        RIGHT_PAREN = 26
    };


    int lookup(char ch);

    static int main();
};

enum CharacterClasses{
    LETTER = 0,
    DIGIT = 1,
    UNKNOWN = 99

};

void Lexicalanalyzer::addChar() {
    if(LexLen <= 98){
        lexeme[LexLen++] = nextChar;
        lexeme[LexLen] = 0;
    }else{
        printf("Error - lexeme is to long \n");
    }

}

void Lexicalanalyzer::getChar() {
    if((nextChar = getc(in_fp)) !=EOF){
        if (isalpha(nextChar)) {
            charClass = LETTER;
        }
        else if(isdigit(nextChar)){
            charClass = DIGIT;
        }
        else{
            charClass = UNKNOWN;
        }

    }else{
        charClass = EOF;
    }
}

void Lexicalanalyzer::getNonBlank() {
    getchar();

    while(isspace(nextChar)){
        getChar();
    }


}

int Lexicalanalyzer::lex() {
    LexLen = 0;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT){
              addChar();
              getChar();
            }
            nextToken = IDENT;
            break;

        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT){
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '0';
            break;
    }
    cout << "Next token is "<< nextToken << ", Next lexeme is "<< lexeme << endl;
    return nextToken;
}

int Lexicalanalyzer::lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case'+':
            addChar();
            nextToken = ADD_OP;
            break;
        case'-':
            addChar();
            nextToken = SUB_OP;
            break;
        case'*':
            addChar();
            nextToken = MULT_OP;
            break;
        case'/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

int Lexicalanalyzer::main(){
    if((in_fp = fopen("front.in", "r")) == NULL){
        printf("ERROR - Cannot open front.in \n");
    }else{
        getchar();
        do {
            lex();

        } while(nextToken != EOF);
    }
}


