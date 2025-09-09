Implementation Details
First of all, In Lexical Analyzer, I have done implementation in C++ manually from the DFA diagram which identifies the keywords,
identifiers, literals, symbols, and operators. First, I have read sample code for analyzing from a CCP.txt file and load it into
the code then use switch for further implementation of different states of DFA and output lexeme by lexeme whether its identifier,
number, symbol or operator. Then output all the possible tokens of this sample code, that is the input for next phase.Secondly, In
Syntax Analyzer, as we have stream of tokens from previous phase that used now as input in this phase.
First, I have stored the tokens and perform token handling and then perform the token matching and accepting, ensures
the current token is of the expected type. If not, it throws a syntax error. Then I write the CFG for this code and then
implement Recursive Decent Parser. This parser reads tokens from the lexical analyzer and verifies that the program follows
the grammar rules of MiniLang++. Then I make parsing function like Program(), Stmt(), or Expr() etc. It uses match() to 
enforce expected tokens and throws syntax errors if mismatches occur.

CFG for MiniLang++:
Program         → Functions MainFunc | MainFunc
Functions       → Func Functions | ε
Func            → DType ID ( ParamSet ) Block
MainFunc        → int main ( ) Block
DType           → int | float | char | string
ParamSet        → ParamList | ε
ParamList       → Parameter , ParamList | Parameter
Parameter       → DType ID
Block           → { StmtSet }
StmtSet         → Stmt StmtSet | stmt
Stmt            → DeclStmt | AssignStmt  | ReturnStmt  | FuncCallStmt  | DisplayStmt  | IfStmt
DeclStmt        → DType VarList ;
VarList         → Var , VarList | Var
Var             → ID | ID = Expr
AssignStmt      → ID = Expr ;
ReturnStmt      → return Expr ;
FuncCallStmt    → ID ( ArgList ) ;
DisplayStmt     → cout << ID ;

IfStmt          → if ( Cond ) Block else Block
Cond            → Operand Sign Operand
Operand         → ID | NUM
Sign            → < | > | == | != | <= | >=
Expr            → ID | NUM | ID + ID | ID - ID | ID ( ArgList )
ArgList         → Operand , ArgList | Operand | ε

The Semantic Analyzer is the third phase of our MiniLang++ compiler. It validates the correctness of
a program beyond syntax, ensuring things “make sense.” It checks types, return values, and declarations to catch logic-level errors before code generation.
In our implementation:
•	We built it as a separate pass after parsing.
•	It walks through the list of tokens and applies rules like:
•	Every variable must be declared before use.
•	Assigned types must match the declared type.
•	Functions must return the correct type.
•	Return values are compared with the function’s return type.
For example, assigning a float value to an int variable would trigger a type mismatch error.
