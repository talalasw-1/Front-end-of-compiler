#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ------------------------
// Token Type Definitions
// ------------------------
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
    MUL,
    DIV,
    MOD,
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
int indent = 0;

void printNode(const string &label)
{
    for (int i = 0; i < indent; ++i)
        cout << "  ";
    cout << label << endl;
}

Token currentToken()
{
    return (current < tokens.size()) ? tokens[current] : Token{END, ""};
}

void match(TokenType expected)
{
    if (currentToken().type == expected)
    {
        printNode("Matched: " + currentToken().value);
        current++;
    }
    else
    {
        cout << "Syntax Error: Expected token type " << expected
             << ", but got " << currentToken().type << " (" << currentToken().value << ")\n";
        exit(1);
    }
}

bool accept(TokenType expected)
{
    if (currentToken().type == expected)
    {
        printNode("Accepted: " + currentToken().value);
        current++;
        return true;
    }
    return false;
}

void Program();
void Functions();
void Func();
void MainFunc();
void DType();
void ParamSet();
void ParamList();
void Parameter();
void Block();
void StmtSet();
void Stmt();
void DeclStmt();
void AssignStmt();
void ReturnStmt();
void FuncCallStmt();
void DisplayStmt();
void IfStmt();
void Expr();
void VarList();
void Var();
void ArgList();
void Cond();
void Sign();

void Program()
{
    printNode("<Program>");
    indent++;
    Functions();
    MainFunc();
    if (currentToken().type != END)
    {
        cout << "Syntax Error: Extra token at end: " << currentToken().value << endl;
        exit(1);
    }
    else
    {
        cout << "Parsing completed successfully!\n";
    }
    indent--;
}

void Functions()
{
    while (currentToken().type == INT || currentToken().type == FLOAT ||
           currentToken().type == CHAR || currentToken().type == STRING)
    {
        if (tokens[current + 1].value == "main")
            break;
        Func();
    }
}

void DType()
{
    printNode("<DType>");
    indent++;
    if (!accept(INT) && !accept(FLOAT) && !accept(CHAR) && !accept(STRING))
    {
        cout << "Syntax Error: Expected data type.\n";
        exit(1);
    }
    indent--;
}

void Func()
{
    printNode("<Func>");
    indent++;
    DType();
    match(ID);
    match(LPAREN);
    ParamSet();
    match(RPAREN);
    Block();
    indent--;
}

void ParamSet()
{
    printNode("<ParamSet>");
    indent++;
    if (currentToken().type == INT || currentToken().type == FLOAT ||
        currentToken().type == CHAR || currentToken().type == STRING)
    {
        ParamList();
    }
    indent--;
}

void ParamList()
{
    printNode("<ParamList>");
    indent++;
    Parameter();
    while (accept(COMMA))
    {
        Parameter();
    }
    indent--;
}

void Parameter()
{
    printNode("<Parameter>");
    indent++;
    DType();
    match(ID);
    indent--;
}

void MainFunc()
{
    printNode("<MainFunc>");
    indent++;
    match(INT);
    match(ID);
    match(LPAREN);
    match(RPAREN);
    Block();
    indent--;
}

void Block()
{
    printNode("<Block>");
    indent++;
    match(LBRACE);
    StmtSet();
    match(RBRACE);
    indent--;
}

void StmtSet()
{
    printNode("<StmtSet>");
    indent++;
    while (currentToken().type != RBRACE && currentToken().type != END)
    {
        Stmt();
    }
    indent--;
}

void Stmt()
{
    printNode("<Stmt>");
    indent++;
    if (currentToken().type == INT || currentToken().type == FLOAT ||
        currentToken().type == CHAR || currentToken().type == STRING)
    {
        DeclStmt();
    }
    else if (currentToken().type == ID)
    {
        if (tokens[current + 1].type == ASSIGN)
        {
            AssignStmt();
        }
        else if (tokens[current + 1].type == LPAREN)
        {
            FuncCallStmt();
        }
        else
        {
            cout << "Syntax Error: Unexpected token after ID: " << tokens[current + 1].value << endl;
            exit(1);
        }
    }
    else if (currentToken().type == RETURN)
    {
        ReturnStmt();
    }
    else if (currentToken().type == COUT)
    {
        DisplayStmt();
    }
    else if (currentToken().type == IF)
    {
        IfStmt();
    }
    else
    {
        cout << "Syntax Error: Unknown statement start: " << currentToken().value << endl;
        exit(1);
    }
    indent--;
}

void DeclStmt()
{
    printNode("<DeclStmt>");
    indent++;
    DType();
    VarList();
    match(SEMI);
    indent--;
}

void VarList()
{
    printNode("<VarList>");
    indent++;
    Var();
    while (accept(COMMA))
    {
        Var();
    }
    indent--;
}

void Var()
{
    printNode("<Var>");
    indent++;
    match(ID);
    if (accept(ASSIGN))
    {
        Expr();
    }
    indent--;
}

void AssignStmt()
{
    printNode("<AssignStmt>");
    indent++;
    match(ID);
    match(ASSIGN);
    Expr();
    match(SEMI);
    indent--;
}

void ReturnStmt()
{
    printNode("<ReturnStmt>");
    indent++;
    match(RETURN);
    Expr();
    match(SEMI);
    indent--;
}

void FuncCallStmt()
{
    printNode("<FuncCallStmt>");
    indent++;
    match(ID);
    match(LPAREN);
    ArgList();
    match(RPAREN);
    match(SEMI);
    indent--;
}

void ArgList()
{
    printNode("<ArgList>");
    indent++;
    if (currentToken().type == ID || currentToken().type == NUM)
    {
        if (currentToken().type == ID)
            match(ID);
        else
            match(NUM);
        while (accept(COMMA))
        {
            if (currentToken().type == ID)
                match(ID);
            else
                match(NUM);
        }
    }
    indent--;
}

void DisplayStmt()
{
    printNode("<DisplayStmt>");
    indent++;
    match(COUT);
    match(LS);
    match(ID);
    match(SEMI);
    indent--;
}

void IfStmt()
{
    printNode("<IfStmt>");
    indent++;
    match(IF);
    match(LPAREN);
    Cond();
    match(RPAREN);
    Block();
    match(ELSE);
    Block();
    indent--;
}

void Cond()
{
    printNode("<Cond>");
    indent++;
    if (currentToken().type == ID || currentToken().type == NUM)
    {
        match(currentToken().type);
        if (currentToken().type >= GT && currentToken().type <= GEQ)
        {
            Sign();
            Cond();
        }
    }
    else
    {
        cout << "Syntax Error: Invalid condition expression.\n";
        exit(1);
    }
    indent--;
}

void Sign()
{
    printNode("<Sign>");
    indent++;
    if (!accept(GT) && !accept(LESS) && !accept(EQ) && !accept(NEQ) && !accept(LEQ) && !accept(GEQ))
    {
        cout << "Syntax Error: Invalid comparison operator.\n";
        exit(1);
    }
    indent--;
}

void Expr()
{
    printNode("<Expr>");
    indent++;
    if (currentToken().type == ID)
    {
        match(ID);
        if (accept(LPAREN))
        {
            ArgList();
            match(RPAREN);
        }
        else if (accept(PLUS) || accept(MINUS))
        {
            match(ID);
        }
    }
    else if (currentToken().type == NUM)
    {
        match(NUM);
    }
    else
    {
        cout << "Syntax Error: Invalid expression.\n";
        exit(1);
    }
    indent--;
}

void loadTokens()
{
    tokens = {
        {INT, "int"},
        {ID, "max"},
        {LPAREN, "("},
        {INT, "int"},
        {ID, "a"},
        {COMMA, ","},
        {INT, "int"},
        {ID, "b"},
        {RPAREN, ")"},
        {LBRACE, "{"},
        {IF, "if"},
        {LPAREN, "("},
        {ID, "a"},
        {GT, ">"},
        {ID, "b"},
        {RPAREN, ")"},
        {LBRACE, "{"},
        {RETURN, "return"},
        {ID, "a"},
        {PLUS, "+"},
        {ID, "b"},
        {SEMI, ";"},
        {RBRACE, "}"},
        {ELSE, "else"},
        {LBRACE, "{"},
        {RETURN, "return"},
        {ID, "a"},
        {MINUS, "-"},
        {ID, "b"},
        {SEMI, ";"},
        {RBRACE, "}"},
        {RBRACE, "}"},
        {INT, "int"},
        {ID, "main"},
        {LPAREN, "("},
        {RPAREN, ")"},
        {LBRACE, "{"},
        {INT, "int"},
        {ID, "x"},
        {ASSIGN, "="},
        {NUM, "10"},
        {SEMI, ";"},
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
        {COUT, "cout"},
        {LS, "<<"},
        {ID, "z"},
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
    cout << "Tokens loaded. Starting parsing...\n";
    for (auto &t : tokens)
        cout << t.type << " -> " << t.value << endl;
    Program();
    return 0;
}
