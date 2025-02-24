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
        INT_LIT = 10,       // Integer literals (exp: 25, 100)
        IDENT = 11,         // Identifiers (exp: variable names)

        ASSIGN_OP = 20,     // Assignment operator (=)
        ADD_OP = 21,        // Addition (+)
        SUB_OP = 22,        // Subtraction (-)
        MULT_OP = 23,       // Multiplication (*)
        DIV_OP = 24,        // Division (/)

        LEFT_PAREN = 25,    // Left parenthesis (
        RIGHT_PAREN = 26,   // Right parenthesis )

        SEMICOLON = 27      // Semicolon (;)

    };


    int lookup(char ch);//pass char to the lookup function.
    bool isInteger();//Added for Assignment
    bool isIdentifier();//Added for Assignment
    bool isOperator();//Added for Assignment


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
    // Debugging Output
   // cout << "[getChar] nextChar: '" << nextChar << "', charClass: " << charClass << endl;
}

void Lexicalanalyzer::getNonBlank() {// if white space is found. go to the next char in the input.

    while(isspace(nextChar)){
        getChar();
    }


}
// the lex() Method will take in each Char for the getChar function and check to see what type of Lexeme it is; Number, Letter or Operator.
int Lexicalanalyzer::lex() {
    lexeme.clear(); //Need to clear the lexeme out, so we can move on the  next Char in line of the Input.

    getNonBlank();

    // Debugging output
    //cout << "[lex] Processing charClass: " << charClass << " (nextChar: '" << nextChar << "')" << endl;
    switch (charClass) {
        case LETTER:
            addChar();

            while(true){
                getChar();
                if(charClass == LETTER || charClass == DIGIT){
                    addChar();
                    //cout << "[lex] Adding char: " << lexeme << endl; // Debugging line
                }else{
                    break;
                }
            }

            if(isIdentifier()){
                nextToken = IDENT;
                //cout << "[lex] Identifier detected: " << lexeme << endl; // Debugging line
            }else
            {
                nextToken = -1;
                cout << "Error -- Invalid Identifier" << endl;
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

            if(isOperator()){
                addChar();
                nextToken = lookup(lexeme[0]);// call lookup function to find out if char is an Operator and check one it is.
                getChar();
            }
            else{
                nextToken = -1;
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

            nextToken = LEFT_PAREN;
            break;
        case')':

            nextToken = RIGHT_PAREN;
            break;
        case'+':

            nextToken = ADD_OP;
            break;
        case'-':

            nextToken = SUB_OP;
            break;
        case'*':

            nextToken = MULT_OP;
            break;
        case'/':

            nextToken = DIV_OP;
            break;

        case'=':

            nextToken = ASSIGN_OP;
            break;

        case';':

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
    return(nextChar == '+' || nextChar == '-' || nextChar == '*' ||
            nextChar == '/' || nextChar == '='|| nextChar == '('|| nextChar == ')'|| nextChar == ';'); // check the user input for any operator.
}


int main(){
    Lexicalanalyzer lexer;// Call in the Constructor
    string usersInputString; // used to store desired string to analyze.

    cout << "Please input your data to be analyzed: ";// Output for the User to enter there input
    getline(cin, usersInputString);//Users Input

    lexer.inputString = usersInputString;// Assign the users input to inputString in the Lexer Constructor
    lexer.index = 0;// resetting the index to 0 for the next input

    lexer.getChar();//call get char to begin the analysis

    do{
        lexer.lex();// lex()
    }while(lexer.nextToken != -1);// Run while there is input left once EOF terminate functions

    return 0;
}