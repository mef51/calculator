/**
*
* A Calculator REPL based on an example from Stroustrup's C++ book
*
*/

#include <iostream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

const char NEWLINE = '\n';

enum TokenValue {
    NAME,       NUMBER,         END,
    PLUS ='+',  MINUS ='-',     MUL ='*',   DIV ='/',
    PRINT =';', ASSIGN ='=',    LP ='(',    RP = ')'
};

TokenValue currentToken = PRINT;
int numErrors;
double numberValue;
string stringValue;
map<string, double> table;

double error(const string& s){
    numErrors++;
    cerr << "error: " << s << NEWLINE;
    return 1;
}

TokenValue getToken() {
    char ch;
    do {
        if(!cin.get(ch))
            return currentToken = END;
    } while(ch != NEWLINE && isspace(ch));

    switch(ch){
    case ';':
    case NEWLINE:
        return currentToken = PRINT;
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case '=':
        return currentToken = TokenValue(ch);
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        cin.putback(ch);
        cin >> numberValue;
        return currentToken = NUMBER;
    default:
        if(isalpha(ch)) {
            stringValue = ch;
            while(cin.get(ch) && isalnum(ch)){
                stringValue.push_back(ch);
            }
            cin.putback(ch);
            return currentToken = NAME;
        }
        error("Bad token.");
        return currentToken = PRINT;
    }
}

double valExpression(bool get);

// get value of a 'primary'
double valPrimary(bool get){
    if(get){
        getToken();
    }

    switch(currentToken){
    case NUMBER:{
        double v = numberValue;
        getToken();
        return v;
    }
    case NAME: {
        double& v = table[stringValue];
        if(getToken() == ASSIGN)
            v = valExpression(true);
        return v;
    }
    case MINUS:
        return - valPrimary(true);
    case LP: {
        double e = valExpression(true);
        if(currentToken != RP)
            return error("Expected ).");
        getToken();
        return e;
    }
    default:
        return error("Primary expected.");
    }
}

// get value of a term
double valTerm(bool get){
    double left = valPrimary(get);
    for(;;){
        switch(currentToken){
        case MUL:
            left *= valPrimary(true);
            break;
        case DIV:
            if(double d = valPrimary(true)){
                left /= d;
                break;
            }
            return error("Divide by 0! KABOOM");
        default:
            return left;
        }
    }
}

double valExpression(bool get){
    double left = valTerm(get);
    for(;;){
        switch(currentToken){
        case PLUS:
            left += valTerm(true);
            break;
        case MINUS:
            left -= valTerm(true);
            break;
        default:
            return left;
        }
    }
}

const string prompt = "calc> ";
const string out = ">> ";

int driveCalculator() {
    table["pi"] = 3.1415926535897932385;
    table["e"] = 2.7182818284590452354;
    cout << "Calculator v1.0:" << NEWLINE;

    while(cin) {
        cout << prompt;
        getToken();
        if(currentToken == END){
            break;
        }
        if(currentToken == PRINT){
            continue;
        }
        cout << out << valExpression(false) << NEWLINE;
    }
    return numErrors;
}

int main()
{
    return driveCalculator();
}

