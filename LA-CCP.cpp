#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

// Helper function to check if character is a letter
bool isLetter(char ch)
{
    return isalpha(ch);
}

// Helper function to check if character is a digit
bool isDigit(char ch)
{
    return isdigit(ch);
}

// Check if lexeme is a keyword
bool isKeyword(char *lexeme)
{
    const char *keywords[] = {"int", "void", "get", "if", "while", "for", "return", "main"};
    int i = 0;
    while (i < 8 && strcmp(lexeme, keywords[i]))
        i++;
    if (i < 8)
        return true;
    else
        return false;
}

using namespace std;

bool isKeyword(char *lexeme);
bool isLetter(char ch);
bool isDigit(char ch);

int main()
{
    ifstream file;
    file.open("CCP.txt");

    int state = 0;
    char ch;
    char lexeme[50];
    int i = 0;

    while (file.get(ch) && ch != EOF)
    {
        switch (state)
        {
            // start state
        case 0:
            if (isalpha(ch))
                state = 1;
            else if (isdigit(ch))
                state = 3;
            else if (ch == '+')
                state = 5;
            else if (ch == '-')
                state = 6;
            else if (ch == '(')
                state = 8;
            else if (ch == '>')
                state = 27;
            else if (ch == '=')
                state = 11;
            else if (ch == ')')
                state = 7;
            else if (ch == '{')
                state = 30;
            else if (ch == '}')
                state = 31;
            else if (ch == ';')
                state = 9;
            else if (ch == ',')
                state = 33;
            break;

        case 1:
            if (isalnum(ch))
                state = 1;
            else
                state = 2;
            break;

        case 2:
            file.unget();
            lexeme[i] = '\0';
            if (isKeyword(lexeme))
            {
                cout << " keyword found: ";
                cout << lexeme << endl;
            }
            else
            {
                cout << " identifier found: ";
                cout << lexeme << endl;
            }
            state = 0;
            i = 0;
            continue;

        // DIGITS state
        case 3:
            if (isdigit(ch))
                state = 3;

            else
                state = 4;
            break;

        // Final NUMBER state
        case 4:
            file.unget();
            lexeme[i] = '\0';
            cout << " number found: ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            continue;

        // ADDOP state
        case 5:
            lexeme[i] = '\0';
            cout << " + : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 6:
            lexeme[i] = '\0';
            cout << " - : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        // Left/Right Brace state
        case 7:
            lexeme[i] = '\0';
            cout << " ) : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 8:
            lexeme[i] = '\0';
            cout << " ( : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 30:
            lexeme[i] = '\0';
            cout << " { : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 31:
            lexeme[i] = '\0';
            cout << " } : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 33:
            lexeme[i] = '\0';
            cout << " , : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 27:
            lexeme[i] = '\0';
            cout << " > : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 9:
            lexeme[i] = '\0';
            cout << " ; : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 11:
            lexeme[i] = '\0';
            cout << " = : ";
            cout << lexeme << endl;

            state = 0;
            i = 0;
            break;

        case 23:
            if (isdigit(ch))
                state = 23;
            else
                state = 4;
            break;
        }

        if (ch != ' ' && ch != '\t' && ch != '\n')
        {
            lexeme[i] = ch;
            i++;
        }
    }

    file.close();
    return 0;
}
