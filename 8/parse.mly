%{
	open Abs_syn
%}

/* Define the tokens of the language: */
%token <int> INT
%token <string> IDENT
%token INC DEC LT GT LEQ GEQ EQ NEQ IF EOF LPAREN RPAREN


%start main
%type <Abs_syn.program> main

%%

main:   entries EOF                     { $1 }

entries:
	instruction			{ [$1] }
|	entries instruction	{ ($1) @ [$2] }


instruction:
	IDENT reg_op INT IF expression		{ BasicInst(get_inst_count(),BasicRegVar $1,$2,ConstExp(IntConst $3),$5) }

expression:
	relational_expression								{ $1 }
|	expression EQ relational_expression	{ BinOpAppExp (EqOp,$1,$3) }
|	expression NEQ relational_expression	{ BinOpAppExp (NEqOp,$1,$3) }

relational_expression:
	primary_expression								{ $1 }
|	relational_expression LT primary_expression		{ BinOpAppExp (LessOp,$1,$3) }
|	relational_expression GT primary_expression		{ BinOpAppExp (GreaterOp,$1,$3) }
|	relational_expression LEQ primary_expression	{ BinOpAppExp (LEqOp,$1,$3) }
|	relational_expression GEQ primary_expression	{ BinOpAppExp (GEqOp,$1,$3) }

primary_expression:
	IDENT						{ RegExp (BasicRegVar $1) }
|	INT 						{ ConstExp (IntConst $1) }
|	LPAREN expression RPAREN	{ $2 }

reg_op:
	INC 	{ IncOp }
|	DEC 	{ DecOp }