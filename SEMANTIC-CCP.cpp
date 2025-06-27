// semantic.cpp

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;

// Token types
enum TokenType
{
    INT,
    FLOAT,
    CHAR,
    STRING,
    RETURN,
    IF,
    ELSE,
    ID,
    NUM,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    SEMI,
    ASSIGN,
    PLUS,
    MINUS,
    COUT,
    CIN,
    LS,
    RS,
    GT,
    LESS,
    EQ,
    NEQ,
    LEQ,
    GEQ,
    END
};

struct Token
{
    TokenType type;
    string value;
};

vector<Token> tokens;
int current = 0;

struct Symbol
{
    string name;
    string type;
};

stack<unordered_map<string, Symbol>> symbolStack;
unordered_map<string, string> functionReturnType;
unordered_map<string, vector<string>> functionParamTypes;
string currentFunctionReturn = "";

void enterScope()
{
    unordered_map<string, Symbol> temp;
    symbolStack.push(temp);
    cout << "Entered new scope.\n";
}

void exitScope()
{
    symbolStack.pop();
    cout << "Exited scope.\n";
}

string lookupVariable(string name)
{
    stack<unordered_map<string, Symbol>> temp = symbolStack;
    while (!temp.empty())
    {
        auto &table = temp.top();
        if (table.count(name))
            return table[name].type;
        temp.pop();
    }
    cout << "Semantic Error: Undeclared variable '" << name << "'\n";
    return "";
}

void addVariable(string name, string type)
{
    auto &table = symbolStack.top();
    if (table.count(name))
    {
        cout << "Semantic Error: Redeclared variable '" << name << "'\n";
        return;
    }
    table[name] = {name, type};
    cout << "Rule: " << name << ".type = " << type << "\n";
}

Token currentToken()
{
    return (current < tokens.size()) ? tokens[current] : Token{END, ""};
}

void match(TokenType expected)
{
    if (currentToken().type == expected)
        current++;
    else
    {
        cout << "Syntax Error during semantic phase. Expected token type " << expected
             << " but got " << currentToken().type << " (" << currentToken().value << ")\n";
        exit(1);
    }
}

void SemanticProgram();
void SemanticFunc();
void SemanticBlock();
void SemanticStmtSet();
void SemanticDeclStmt();
void SemanticAssignStmt();
void SemanticReturnStmt();
string SemanticExpr();
void SemanticFuncCall(string fname, vector<string> args);

void SemanticProgram()
{
    enterScope();
    while (currentToken().type != END)
    {
        if ((currentToken().type == INT || currentToken().type == FLOAT || currentToken().type == STRING || currentToken().type == CHAR) && tokens[current + 2].type == LPAREN)
        {
            SemanticFunc();
        }
        else
        {
            current++;
        }
    }
    exitScope();
    cout << "Semantic analysis completed with rules applied.\n";
}

void SemanticFunc()
{
    string returnType;
    if (currentToken().type == INT)
        returnType = "int";
    else if (currentToken().type == FLOAT)
        returnType = "float";
    else if (currentToken().type == STRING)
        returnType = "string";
    else if (currentToken().type == CHAR)
        returnType = "char";
    match(currentToken().type);

    string fname = currentToken().value;
    match(ID);
    match(LPAREN);

    vector<string> paramTypes;
    vector<string> paramNames;

    if (currentToken().type == INT || currentToken().type == FLOAT || currentToken().type == STRING || currentToken().type == CHAR)
    {
        string type;
        if (currentToken().type == INT)
            type = "int";
        else if (currentToken().type == FLOAT)
            type = "float";
        else if (currentToken().type == STRING)
            type = "string";
        else
            type = "char";
        match(currentToken().type);
        string name = currentToken().value;
        match(ID);
        paramTypes.push_back(type);
        paramNames.push_back(name);

        while (currentToken().type == COMMA)
        {
            match(COMMA);
            if (currentToken().type == INT)
                type = "int";
            else if (currentToken().type == FLOAT)
                type = "float";
            else if (currentToken().type == STRING)
                type = "string";
            else
                type = "char";
            match(currentToken().type);
            name = currentToken().value;
            match(ID);
            paramTypes.push_back(type);
            paramNames.push_back(name);
        }
    }

    match(RPAREN);
    functionReturnType[fname] = returnType;
    functionParamTypes[fname] = paramTypes;
    currentFunctionReturn = returnType;
    cout << "Rule: " << fname << ".returnType = " << returnType << "\n";

    enterScope();
    for (int i = 0; i < paramNames.size(); i++)
    {
        addVariable(paramNames[i], paramTypes[i]);
    }
    SemanticBlock();
    exitScope();
}

void SemanticBlock()
{
    match(LBRACE);
    SemanticStmtSet();
    match(RBRACE);
}

void SemanticStmtSet()
{
    while (currentToken().type != RBRACE && currentToken().type != END)
    {
        if (currentToken().type == INT || currentToken().type == FLOAT || currentToken().type == STRING || currentToken().type == CHAR)
        {
            SemanticDeclStmt();
        }
        else if (currentToken().type == ID && tokens[current + 1].type == ASSIGN)
        {
            SemanticAssignStmt();
        }
        else if (currentToken().type == RETURN)
        {
            SemanticReturnStmt();
        }
        else
        {
            current++;
        }
    }
}

void SemanticDeclStmt()
{
    string dtype;
    if (currentToken().type == INT)
        dtype = "int";
    else if (currentToken().type == FLOAT)
        dtype = "float";
    else if (currentToken().type == STRING)
        dtype = "string";
    else if (currentToken().type == CHAR)
        dtype = "char";
    else
    {
        cout << "Semantic Error: Unsupported data type.\n\n";
        exit(1);
    }
    match(currentToken().type);

    string name = currentToken().value;
    match(ID);

    if (currentToken().type == ASSIGN)
    {
        match(ASSIGN);
        string exprType = SemanticExpr();
        if (exprType != dtype)
        {
            cout << "Semantic Error: Type mismatch in assignment to '" << name << "'\n\n";
        }
    }

    addVariable(name, dtype);
    match(SEMI);
}

void SemanticAssignStmt()
{
    string name = currentToken().value;
    match(ID);
    match(ASSIGN);
    string exprType = SemanticExpr();
    string varType = lookupVariable(name);
    if (exprType != varType)
    {
        cout << "Semantic Error: Type mismatch in assignment to '" << name << "'\n\n";
    }
    match(SEMI);
}

void SemanticReturnStmt()
{
    match(RETURN);
    string retType = SemanticExpr();
    if (retType != currentFunctionReturn)
    {
        cout << "Semantic Error: Return type mismatch. Expected " << currentFunctionReturn << ", got " << retType << "\n\n";
    }
    match(SEMI);
}

string SemanticExpr()
{
    if (currentToken().type == NUM)
    {
        match(NUM);
        return "int";
    }
    else if (currentToken().type == ID)
    {
        string name = currentToken().value;
        match(ID);

        if (currentToken().type == LPAREN)
        {
            match(LPAREN);
            vector<string> args;
            if (currentToken().type != RPAREN)
            {
                args.push_back(SemanticExpr());
                while (currentToken().type == COMMA)
                {
                    match(COMMA);
                    args.push_back(SemanticExpr());
                }
            }
            match(RPAREN);
            SemanticFuncCall(name, args);
            return functionReturnType[name];
        }

        string type = lookupVariable(name);
        if (type == "")
            type = "undefined";
        return type;
    }
    else
    {
        cout << "Semantic Error: Invalid expression\n\n";
        return "";
    }
}

void SemanticFuncCall(string fname, vector<string> args)
{
    if (!functionParamTypes.count(fname))
    {
        cout << "Semantic Error: Function '" << fname << "' not declared\n";
        return;
    }
    auto expected = functionParamTypes[fname];
    if (expected.size() != args.size())
    {
        cout << "Semantic Error: Argument count mismatch in call to '" << fname << "'\n\n";
        return;
    }
    for (size_t i = 0; i < expected.size(); ++i)
    {
        if (expected[i] != args[i])
        {
            cout << "Semantic Error: Argument " << i + 1 << " of function '" << fname << "' expects " << expected[i] << " but got " << args[i] << "\n\n";
        }
        else
        {
            cout << "Rule: Argument " << i + 1 << " type matched: " << args[i] << "\n";
        }
    }
}

void loadTokens()
{
    tokens = {
        {INT, "int"}, {ID, "max"}, {LPAREN, "("}, {INT, "int"}, {ID, "a"}, {COMMA, ","}, {INT, "int"}, {ID, "b"}, {RPAREN, ")"}, {LBRACE, "{"}, {IF, "if"}, {LPAREN, "("}, {ID, "a"}, {GT, ">"}, {ID, "b"}, {RPAREN, ")"}, {LBRACE, "{"}, {RETURN, "return"}, {ID, "a"}, {SEMI, ";"}, {RBRACE, "}"}, {ELSE, "else"}, {LBRACE, "{"}, {RETURN, "return"}, {ID, "b"}, {SEMI, ";"}, {RBRACE, "}"}, {RBRACE, "}"}, {INT, "int"}, {ID, "main"}, {LPAREN, "("}, {RPAREN, ")"}, {LBRACE, "{"}, {INT, "int"}, {ID, "x"}, {ASSIGN, "="}, {NUM, "10"}, {SEMI, ";"}, ////////-> for int use this line instead of <- that one    {INT, "int"}, {ID, "x"}, {ASSIGN, "="}, {NUM, "10"}, {SEMI, ";"},

        {INT, "int"},
        {ID, "y"},
        {ASSIGN, "="},
        {NUM, "20"},
        {SEMI, ";"},
        {INT, "int"},
        {ID, "z"},
        {ASSIGN, "="},
        {ID, "max"},
        {LPAREN, "("},
        {ID, "x"},
        {COMMA, ","},
        {ID, "y"},
        {RPAREN, ")"},
        {SEMI, ";"},
        {RETURN, "return"},
        {NUM, "0"},
        {SEMI, ";"},
        {RBRACE, "}"},
        {END, ""}};
}

int main()
{
    loadTokens();
    unordered_map<string, Symbol> global;
    symbolStack.push(global);
    SemanticProgram();
    return 0;
}
