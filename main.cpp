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


public:
    size_t index = 0;
    int nextToken;
    string inputString;

    //C Functions declared in private access specifier
    void addChar();
    void getChar();
    void getNonBlank();
    int lex();



    enum tokenCodeTypes{
        INT_LIT = 10,       // Integer literals (e.g., 25, 100)
        IDENT = 11,         // Identifiers (e.g., variable names)

        ASSIGN_OP = 20,     // Assignment operator (=)
        ADD_OP = 21,        // Addition (+)
        SUB_OP = 22,        // Subtraction (-)
        MULT_OP = 23,       // Multiplication (*)
        DIV_OP = 24,        // Division (/)

        LEFT_PAREN = 25,    // Left parenthesis (
        RIGHT_PAREN = 26,   // Right parenthesis )

        SEMICOLON = 27      // **NEW: Semicolon (;)**

    };


    int lookup(char ch);
    bool isInteger();
    bool isIdentifier();
    bool isOperator();


};

enum CharacterClasses{
    LETTER = 0,
    DIGIT = 1,
    UNKNOWN = 99

};

void Lexicalanalyzer::addChar() {

    if(lexeme.length() <= 98){ // Needed to replace the LexLen <= 98 due to how fast the program grows. lexeme.length() will set the growth as big as the User inputs.
        lexeme += nextChar; // Replacing lexeme[LexLen++] = nextChar; With C++ style implementation
    }else{
        printf("Error - lexeme is to long \n");
    }

}

void Lexicalanalyzer::getChar() { // Function looks through the input char by char and sorts it into three buckets.
    if((index < inputString.length())){ // Bug #1, Found that != EOF or -1 will not compile in C++ like it does in C.
        nextChar = inputString[index++];

        // This function will set up the back book for checking the input in the lex() method.
        if (isalpha(nextChar)) { //Checks to see if char is a Letter
            charClass = LETTER;
        }
        else if(isdigit(nextChar)){ // Checks to see if char is a Number
            charClass = DIGIT;
        }
        else
            charClass = UNKNOWN;// If Charater is not a Number of a Letter call set the char to Unknown to look up if it is an operator.

    }else{
        charClass = -1;// If char is not recognized error out.
    }
}

void Lexicalanalyzer::getNonBlank() {// if white space is found. go to the next char in the input.
    getChar();

    while(isspace(nextChar)){
        getChar();
    }


}
// the lex() Method will take in each Char for the getChar function and check to see what type of Lexeme it is; Number, Letter or Operator.
int Lexicalanalyzer::lex() {
    lexeme.clear(); //Need to clear the lexeme out, so we can move on the  next Char in line of the Input.

    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT){
              addChar();
              getChar();
            }
            if(isIdentifier()){
            nextToken = IDENT;
            }

            break;

        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT){
                addChar();
                getChar();
            }
            if (isInteger()){
                nextToken = INT_LIT;
            }
            break;

        case UNKNOWN:// Unknown is used for non Letter or Number Chars. I am using it to call the lookup() function to find out if it's an Operator.
            addChar();
            getChar();

            if(isOperator()){
                nextToken = lookup(lexeme[0]);// call lookup function to find out if char is an Operator and check one it is.
            }
            else{
                cout << "Error -- Charater is Unknown." << endl;
            }
            break;

        case -1:
            nextToken = -1;
            lexeme = "EOF"; // replaced this with the C version. If we were to keep the C version the program can potentially crash do to the dynamical growth.
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

        case'=':
            addChar();
            nextToken = ASSIGN_OP;
            break;

        case';':
            addChar();
            nextToken = SEMICOLON;
            break;

        default:
            addChar();
            nextToken = -1;
            break;
    }
    return nextToken;
}


bool Lexicalanalyzer::isInteger() {
    if(lexeme.empty()) return false; //Make sure the input is not empty.

    for(size_t i = 0; i < lexeme.length(); i++){ // setting up for loop to look through the rest of the numbers in the input.
        if(!isdigit(lexeme[i])){// If no numbers found then return false.
            return false;
        }
    }
    return true; // If numbers found return true.

}

bool Lexicalanalyzer::isIdentifier() {
    if (lexeme.empty() || !isalpha(lexeme[0])) return false; // First Charater must be a Letter


    for(size_t i = 1; i < lexeme.length(); i++){ // Check the rest of the numbers/letters in the input.
        if(!isalnum(lexeme[i])){ // If no Numbers or Letters found return False.
            return false;
        }
    }
return true; // if a number or letter found return true.

}

bool Lexicalanalyzer::isOperator() {
    if(lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "=" || lexeme == ";"){ // check the user input for any operator.
        return true; //If one of the operators above are found is will return true to the lex().
    }else{
        return false;// If no operators found then it will return false.
    }

}


int main(){
    Lexicalanalyzer lexer;
    string usersInputString; // used to store desired string to analyze.

    cout << "Please input your data to be analyzed: ";
    getline(cin, usersInputString);

    lexer.inputString = usersInputString;
    lexer.index = 0;

    lexer.getChar();

    do{
        lexer.lex();
    }while(lexer.nextToken != -1);

    return 0;
}


