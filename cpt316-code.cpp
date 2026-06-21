
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;



bool isWhitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool isDigitChar(char c) {
    return (c >= '0' && c <= '9');
}

bool isLowercase(char c) {
    return (c >= 'a' && c <= 'z');
}

// ============================================================================
// TOKEN CLASS
// ============================================================================

enum TokenType {
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    LPAREN,
    RPAREN,
    SEMICOLON,
    INVALID,
    END_OF_INPUT
};

class Token {
private:
    TokenType type;
    string value;
    int position;

public:
    Token() : type(END_OF_INPUT), value(""), position(0) {}
    Token(TokenType t, string val, int pos) : type(t), value(val), position(pos) {}
    
    TokenType getType() const { return type; }
    string getValue() const { return value; }
    int getPosition() const { return position; }
    
    string typeToString() const {
        switch(type) {
            case IDENTIFIER:    return "IDENTIFIER";
            case NUMBER:        return "NUMBER";
            case PLUS:          return "PLUS";
            case MINUS:         return "MINUS";
            case MULTIPLY:      return "MULTIPLY";
            case DIVIDE:        return "DIVIDE";
            case ASSIGN:        return "ASSIGN";
            case LPAREN:        return "LPAREN";
            case RPAREN:        return "RPAREN";
            case SEMICOLON:     return "SEMICOLON";
            case INVALID:       return "INVALID";
            case END_OF_INPUT:  return "END_OF_INPUT";
            default:            return "UNKNOWN";
        }
    }
};

// ============================================================================
// SYNTAX TREE NODE CLASS
// ============================================================================

class SyntaxTreeNode {
private:
    string label;
    SyntaxTreeNode** children;
    int childCount;
    int childCapacity;

public:
    SyntaxTreeNode(string nodeLabel) : label(nodeLabel), children(NULL), childCount(0), childCapacity(0) {}
    
    ~SyntaxTreeNode() {
        for (int i = 0; i < childCount; i++) {
            delete children[i];
        }
        delete[] children;
    }
    
    void addChild(SyntaxTreeNode* child) {
        if (childCount >= childCapacity) {
            int newCapacity = (childCapacity == 0) ? 4 : childCapacity * 2;
            SyntaxTreeNode** newChildren = new SyntaxTreeNode*[newCapacity];
            
            for (int i = 0; i < childCount; i++) {
                newChildren[i] = children[i];
            }
            
            delete[] children;
            children = newChildren;
            childCapacity = newCapacity;
        }
        
        children[childCount++] = child;
    }
    
    string getLabel() const { return label; }
    
    void displayTreeDiagram(string prefix = "", bool isLast = true) const {
        cout << prefix;
        cout << (isLast ? "+-- " : "|-- ");
        cout << label << endl;
        
        for (int i = 0; i < childCount; i++) {
            bool last = (i == childCount - 1);
            children[i]->displayTreeDiagram(prefix + (isLast ? "    " : "|   "), last);
        }
    }
};

// ============================================================================
// LEXER CLASS
// ============================================================================

class Lexer {
private:
    string input;
    int position;
    Token* tokens;
    int tokenCount;
    int tokenCapacity;
    Token* invalidTokens;
    int invalidCount;
    int invalidCapacity;
    
    int identifierCount;
    int numberCount;
    int operatorCount;
    int otherCount;
    
    void addToken(Token t) {
        if (tokenCount >= tokenCapacity) {
            int newCapacity = (tokenCapacity == 0) ? 10 : tokenCapacity * 2;
            Token* newTokens = new Token[newCapacity];
            
            for (int i = 0; i < tokenCount; i++) {
                newTokens[i] = tokens[i];
            }
            
            delete[] tokens;
            tokens = newTokens;
            tokenCapacity = newCapacity;
        }
        
        tokens[tokenCount++] = t;
    }
    
    void addInvalidToken(Token t) {
        if (invalidCount >= invalidCapacity) {
            int newCapacity = (invalidCapacity == 0) ? 5 : invalidCapacity * 2;
            Token* newInvalid = new Token[newCapacity];
            
            for (int i = 0; i < invalidCount; i++) {
                newInvalid[i] = invalidTokens[i];
            }
            
            delete[] invalidTokens;
            invalidTokens = newInvalid;
            invalidCapacity = newCapacity;
        }
        
        invalidTokens[invalidCount++] = t;
    }
    
    char currentChar() {
        if (isEnd()) return '\0';
        return input[position];
    }
    
    void advance() {
        position++;
    }
    
    bool isEnd() {
        return position >= (int)input.length();
    }
    
    void skipWhitespace() {
        while (!isEnd() && isWhitespace(currentChar())) {
            advance();
        }
    }
    
    Token scanNumber() {
        int startPos = position;
        string num = "";
        
        while (!isEnd() && isDigitChar(currentChar())) {
            num += currentChar();
            advance();
        }
        
        numberCount++;
        return Token(NUMBER, num, startPos);
    }
    
    Token scanIdentifier() {
        int startPos = position;
        char id = currentChar();
        advance();
        
        identifierCount++;
        return Token(IDENTIFIER, string(1, id), startPos);
    }

public:
    Lexer(string inputString) 
        : input(inputString), position(0), 
          tokens(NULL), tokenCount(0), tokenCapacity(0),
          invalidTokens(NULL), invalidCount(0), invalidCapacity(0),
          identifierCount(0), numberCount(0), operatorCount(0), otherCount(0) {}
    
    ~Lexer() {
        delete[] tokens;
        delete[] invalidTokens;
    }
    
    void tokenize() {
        while (!isEnd()) {
            skipWhitespace();
            
            if (isEnd()) break;
            
            char current = currentChar();
            int startPos = position;
            
            if (isLowercase(current)) {
                addToken(scanIdentifier());
            }
            else if (isDigitChar(current)) {
                addToken(scanNumber());
            }
            else if (current == '+') {
                addToken(Token(PLUS, "+", startPos));
                operatorCount++;
                advance();
            }
            else if (current == '-') {
                addToken(Token(MINUS, "-", startPos));
                operatorCount++;
                advance();
            }
            else if (current == '*') {
                addToken(Token(MULTIPLY, "*", startPos));
                operatorCount++;
                advance();
            }
            else if (current == '/') {
                addToken(Token(DIVIDE, "/", startPos));
                operatorCount++;
                advance();
            }
            else if (current == '=') {
                addToken(Token(ASSIGN, "=", startPos));
                otherCount++;
                advance();
            }
            else if (current == '(') {
                addToken(Token(LPAREN, "(", startPos));
                otherCount++;
                advance();
            }
            else if (current == ')') {
                addToken(Token(RPAREN, ")", startPos));
                otherCount++;
                advance();
            }
            else if (current == ';') {
                addToken(Token(SEMICOLON, ";", startPos));
                otherCount++;
                advance();
            }
            else {
                Token invalidToken(INVALID, string(1, current), startPos);
                addToken(invalidToken);
                addInvalidToken(invalidToken);
                advance();
            }
        }
        
        addToken(Token(END_OF_INPUT, "", position));
    }
    
    Token* getTokens() const { return tokens; }
    int getTokenCount() const { return tokenCount; }
    Token* getInvalidTokens() const { return invalidTokens; }
    int getInvalidCount() const { return invalidCount; }
    
    int getTotalTokenCount() const { return tokenCount - 1; }
    int getIdentifierCount() const { return identifierCount; }
    int getNumberCount() const { return numberCount; }
    int getOperatorCount() const { return operatorCount; }
    int getOtherCount() const { return otherCount; }
    
    void displayTokens() const {
        cout << "\n========== TOKEN STREAM ==========\n";
        cout << left << setw(5) << "No." 
             << setw(15) << "Type" 
             << setw(10) << "Value" 
             << setw(10) << "Position" << endl;
        cout << "----------------------------------------" << endl;
        
        int count = 1;
        for (int i = 0; i < tokenCount; i++) {
            if (tokens[i].getType() == END_OF_INPUT) continue;
            
            cout << left << setw(5) << count++
                 << setw(15) << tokens[i].typeToString()
                 << setw(10) << (tokens[i].getValue().empty() ? tokens[i].typeToString() : tokens[i].getValue())
                 << setw(10) << tokens[i].getPosition() << endl;
        }
        
        cout << "=================================\n";
    }
    
    void displayInvalidTokens() const {
        if (invalidCount > 0) {
            cout << "\n========== LEXICAL ERRORS ==========\n";
            for (int i = 0; i < invalidCount; i++) {
                cout << "LexicalError at position " << invalidTokens[i].getPosition() 
                     << ": Invalid character '" << invalidTokens[i].getValue() << "'" << endl;
            }
            cout << "====================================\n";
        }
    }
    
    void displayTokenCounts() const {
        cout << "\n========== TOKEN STATISTICS ==========\n";
        cout << "Total tokens: " << getTotalTokenCount() << endl;
        cout << "Identifiers: " << identifierCount << endl;
        cout << "Numbers: " << numberCount << endl;
        cout << "Operators: " << operatorCount << endl;
        cout << "Other tokens: " << otherCount << endl;
        cout << "======================================\n";
    }
};

// ============================================================================
// PARSER CLASS
// ============================================================================

class Parser {
private:
    Token* tokens;
    int tokenCount;
    int currentPosition;
    bool hasError;
    string errorMessage;
    
    string intToString(int num) {
        stringstream ss;
        ss << num;
        return ss.str();
    }
    
    Token currentToken() {
        if (currentPosition < tokenCount) {
            return tokens[currentPosition];
        }
        return Token(END_OF_INPUT, "", 0);
    }
    
    void advance() {
        if (currentPosition < tokenCount) {
            currentPosition++;
        }
    }
    
    bool check(TokenType type) {
        return currentToken().getType() == type;
    }
    
    void reportError(string msg) {
        hasError = true;
        errorMessage = msg;
    }
    
    SyntaxTreeNode* parseStmt() {
        SyntaxTreeNode* stmtNode = new SyntaxTreeNode("<stmt>");
        
        if (!check(IDENTIFIER)) {
            if (check(END_OF_INPUT)) {
                reportError("SyntaxError: Expected identifier but found end of input");
            } else {
                reportError("SyntaxError at position " + intToString(currentToken().getPosition()) + 
                           ": Expected identifier but found '" + currentToken().getValue() + "'");
            }
            delete stmtNode;
            return NULL;
        }
        
        SyntaxTreeNode* idNode = new SyntaxTreeNode(currentToken().getValue());
        stmtNode->addChild(idNode);
        advance();
        
        if (!check(ASSIGN)) {
            reportError("SyntaxError at position " + intToString(currentToken().getPosition()) + 
                       ": Expected '=' but found '" + currentToken().getValue() + "'");
            delete stmtNode;
            return NULL;
        }
        
        SyntaxTreeNode* assignNode = new SyntaxTreeNode("=");
        stmtNode->addChild(assignNode);
        advance();
        
        SyntaxTreeNode* exprNode = parseExpr();
        if (exprNode == NULL) {
            delete stmtNode;
            return NULL;
        }
        stmtNode->addChild(exprNode);
        
        if (!check(SEMICOLON)) {
            if (check(END_OF_INPUT)) {
                reportError("SyntaxError at end of input: Missing ';'");
            } else {
                reportError("SyntaxError at position " + intToString(currentToken().getPosition()) + 
                           ": Expected ';' but found '" + currentToken().getValue() + "'");
            }
            delete stmtNode;
            return NULL;
        }
        
        SyntaxTreeNode* semiNode = new SyntaxTreeNode(";");
        stmtNode->addChild(semiNode);
        advance();
        
        return stmtNode;
    }
    
    SyntaxTreeNode* parseExpr() {
        SyntaxTreeNode* exprNode = new SyntaxTreeNode("<expr>");
        
        SyntaxTreeNode* termNode = parseTerm();
        if (termNode == NULL) {
            delete exprNode;
            return NULL;
        }
        exprNode->addChild(termNode);
        
        while (check(PLUS) || check(MINUS)) {
            SyntaxTreeNode* opNode = new SyntaxTreeNode(currentToken().getValue());
            exprNode->addChild(opNode);
            advance();
            
            termNode = parseTerm();
            if (termNode == NULL) {
                delete exprNode;
                return NULL;
            }
            exprNode->addChild(termNode);
        }
        
        return exprNode;
    }
    
    SyntaxTreeNode* parseTerm() {
        SyntaxTreeNode* termNode = new SyntaxTreeNode("<term>");
        
        SyntaxTreeNode* factorNode = parseFactor();
        if (factorNode == NULL) {
            delete termNode;
            return NULL;
        }
        termNode->addChild(factorNode);
        
        while (check(MULTIPLY) || check(DIVIDE)) {
            SyntaxTreeNode* opNode = new SyntaxTreeNode(currentToken().getValue());
            termNode->addChild(opNode);
            advance();
            
            factorNode = parseFactor();
            if (factorNode == NULL) {
                delete termNode;
                return NULL;
            }
            termNode->addChild(factorNode);
        }
        
        return termNode;
    }
    
    SyntaxTreeNode* parseFactor() {
        SyntaxTreeNode* factorNode = new SyntaxTreeNode("<factor>");
        
        if (check(IDENTIFIER)) {
            SyntaxTreeNode* idNode = new SyntaxTreeNode(currentToken().getValue());
            factorNode->addChild(idNode);
            advance();
            return factorNode;
        }
        
        if (check(NUMBER)) {
            SyntaxTreeNode* numNode = new SyntaxTreeNode(currentToken().getValue());
            factorNode->addChild(numNode);
            advance();
            return factorNode;
        }
        
        if (check(LPAREN)) {
            SyntaxTreeNode* lparenNode = new SyntaxTreeNode("(");
            factorNode->addChild(lparenNode);
            advance();
            
            SyntaxTreeNode* exprNode = parseExpr();
            if (exprNode == NULL) {
                delete factorNode;
                return NULL;
            }
            factorNode->addChild(exprNode);
            
            if (!check(RPAREN)) {
                reportError("SyntaxError at position " + intToString(currentToken().getPosition()) + 
                           ": Expected ')' but found '" + currentToken().getValue() + "'");
                delete factorNode;
                return NULL;
            }
            
            SyntaxTreeNode* rparenNode = new SyntaxTreeNode(")");
            factorNode->addChild(rparenNode);
            advance();
            
            return factorNode;
        }
        
        if (check(END_OF_INPUT)) {
            reportError("SyntaxError: Unexpected end of input in expression");
        } else {
            reportError("SyntaxError at position " + intToString(currentToken().getPosition()) + 
                       ": Expected identifier, number, or '(' but found '" + currentToken().getValue() + "'");
        }
        delete factorNode;
        return NULL;
    }

public:
    Parser(Token* tokenList, int count) 
        : tokens(tokenList), tokenCount(count), currentPosition(0), hasError(false) {}
    
    SyntaxTreeNode* parse() {
        return parseStmt();
    }
    
    bool hasErrors() const { return hasError; }
    string getErrorMessage() const { return errorMessage; }
};

// ============================================================================
// MAIN PROGRAM
// ============================================================================

void processInput(string input) {
    cout << "\n============================================================" << endl;
    cout << "INPUT: " << input << endl;
    cout << "============================================================" << endl;
    
    // Lexical Analysis
    cout << "\n--- LEXICAL ANALYSIS ---\n";
    Lexer lexer(input);
    lexer.tokenize();
    
    lexer.displayTokens();
    lexer.displayInvalidTokens();
    lexer.displayTokenCounts();
    
    // Syntax Analysis
    cout << "\n--- SYNTAX ANALYSIS ---\n";
    
    if (lexer.getInvalidCount() > 0) {
        cout << "\nParsing skipped due to lexical errors.\n";
        return;
    }
    
    Parser parser(lexer.getTokens(), lexer.getTokenCount());
    SyntaxTreeNode* syntaxTree = parser.parse();
    
    if (parser.hasErrors()) {
        cout << "\n" << parser.getErrorMessage() << endl;
    } else {
        cout << "\nParsing successful! Syntax tree generated.\n";
        cout << "\n========== SYNTAX TREE ==========\n";
        syntaxTree->displayTreeDiagram();
        cout << "=================================\n";
        delete syntaxTree;
    }
}

int main() {
    cout << "***********************************************\n";
    cout << "*     COMPY LANGUAGE MINI-COMPILER           *\n";
    cout << "*     CPT316 Assignment 1                    *\n";
    cout << "***********************************************\n";
    
    string choice;
    
    while (true) {
        cout << "\n\nChoose an option:\n";
        cout << "1. Enter custom input\n";
        cout << "2. Exit\n";
        cout << "Enter choice: ";
        getline(cin, choice);
        
        if (choice == "1") {
            cout << "\nEnter COMPY statement: ";
            string input;
            getline(cin, input);
            processInput(input);
        }
        else if (choice == "2") {
            cout << "\nExiting program. Goodbye!\n";
            break;
        }
        else {
            cout << "\nInvalid choice. Please try again.\n";
        }
    }
    
    return 0;
}
