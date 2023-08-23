grammar SysY;

compilationUnit
    :   translationUnit? EOF
    ;

translationUnit
    :   externalDeclaration+
    ;

externalDeclaration
    :   decl
    |   funcDef
    |   ';'
    ;

decl
    :   constDecl 
    |   varDecl
    ;

constDecl
    :   'const' bType constDef (',' constDef)* ';' 
    ;

bType
    :   'int' 
    |   'float'
    ;

constDef
    :   Identifier ('[' constExp ']')* '=' constInitVal
    ;

constInitVal
    :   constExp # scalarConstInitVal
    |   '{' (constInitVal (',' constInitVal)* )? '}' # listConstInitVal
    ;

varDecl 
    :   bType varDef (',' varDef)* ';'
    ;

varDef
    :   Identifier ('[' constExp ']')* # uninitVarDef
    |   Identifier ('[' constExp ']')* '=' initVal # initVarDef
    ;

initVal
    :   exp # scalarInitVal
    |   '{' (initVal (',' initVal)* )? '}' # listInitval
    ;

funcDef
    :   funcType Identifier '(' (funcFParams)? ')' block
    ;

funcType
    :   'void' 
    |   'int' 
    |   'float'
    ;

funcFParams
    :   funcFParam (',' funcFParam)*
    ;

funcFParam
    :   bType Identifier ('[' ']' ('[' constExp ']')* )?
    ;

block
    :   '{' blockItemList? '}'
    ;

blockItemList
    :   blockItem+
    ;

blockItem
    :   decl
    |   stmt
    ;

stmt
    :   assignStmt
    |   expStmt
    |   blockStmt
    |   seleStmt
    |   loopStmt
    |   jumpStmt
    ;

assignStmt
    :   lVal assignOp exp ';'
    ;


assignOp
    :   '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
    ;

expStmt
    :   (exp)? ';'
    ;

blockStmt
    :   block
    ;

seleStmt
    :   If '(' condExp ')' stmt (Else stmt)? # ifStmt
    ;

loopStmt
    :   While '(' condExp ')' stmt # whileLoop
    ;

jumpStmt    
    :   Continue ';' # continueStmt
    |   Break ';' # breakStmt
    |   Return (exp)? ';' #returnStmt
    ;

exp
    :   addExp
    ;

constExp
    :   addExp 
    ;

lVal
    :   Identifier ('[' exp ']')*
    ;

primaryExp
    :   '(' exp ')' # primaryExp1
    |   lVal # primaryExp2
    |   number # primaryExp3
    ;

number
    :   IntLiteral # number1
    |   FloatLiteral # number2
    ; 

funcRParams
    :   funcRParam (',' funcRParam)*
    ;

funcRParam
    :   exp
    ;

unaryExp
    :   primaryExp # unary1
    |   Identifier '(' (funcRParams)? ')' # unary2
    |   unaryOp unaryExp # unary3
    ;

unaryOp 
    :   '&' | '*' | '+' | '-' | '~' | '!'
    ;

mulExp
    :   unaryExp # mul1
    |   mulExp mulOp unaryExp # mul2
    ;

mulOp
    :   '*' | '/' | '%'
    ;

addExp
    :   mulExp # add1
    |   addExp addOp mulExp # add2
    ;

addOp
    :   '+' | '-'
    ;

relExp
    :   addExp # rel1
    |   relExp relOp addExp # rel2
    ;

relOp
    :   '<' | '>' | '<=' | '>='
    ;

eqExp
    :   relExp # eq1
    |   eqExp eqOp relExp # eq2
    ;

eqOp
    :   '==' | '!='
    ;

lAndExp
    : eqExp # lAnd1
    | lAndExp '&&' eqExp # lAnd2
    ;

lOrExp
    : lAndExp # lOr1
    | lOrExp '||' lAndExp # lOr2
    ;

condExp
    :   lOrExp
    ;

// keyword
Int : 'int';
Void: 'void';
Const: 'const';
Return : 'return';
If : 'if';
Else : 'else';
While : 'while';
For : 'for';
Do : 'do';
Break : 'break';
Continue : 'continue';


// operator
Lparen : '(' ;
Rparen : ')' ;
Lbrkt : '[' ;
Rbrkt : ']' ;
Lbrace : '{' ;
Rbrace : '}' ;
Comma : ',' ;
Semicolon : ';';

Minus : '-';
Addition : '+';
Exclamation : '!';
Multiplication : '*';
Division : '/';
Modulo : '%';
LAND : '&&';
LOR : '||';
EQ : '==';
NEQ : '!=';
LT : '<';
LE : '<=';
GT : '>';
GE : '>=';

// integer, identifier
IntLiteral
    :   [0-9]+
    |   ('0x'|'0X')[0-9a-fA-F]+
    ;

FloatLiteral
    :   ([0-9]+|[0-9]*'.'[0-9]*)([eE]('+'|'-')?[0-9]+)?[fFlL]?
    |   ('0x'|'0X')([0-9a-fA-F]*'.'[0-9a-fA-F]*)([pP]('+'|'-')?[0-9]+)?[fFlL]?
    ;


Identifier
    :   [a-zA-Z_][a-zA-Z_0-9]*
    ;

Whitespace
    :   [ \t]+
        -> skip
    ;

Newline
    :   (   '\r' '\n'?
        |   '\n'
        )
        -> skip
    ;

BlockComment
    :   '/*' .*? '*/'
        -> skip
    ;

LineComment
    :   '//' ~[\r\n]*
        -> skip
    ;
