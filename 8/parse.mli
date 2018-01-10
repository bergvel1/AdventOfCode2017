type token =
  | INT of (int)
  | IDENT of (string)
  | INC
  | DEC
  | LT
  | GT
  | LEQ
  | GEQ
  | EQ
  | NEQ
  | IF
  | EOF
  | LPAREN
  | RPAREN

val main :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Abs_syn.program
