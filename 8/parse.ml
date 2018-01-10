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

open Parsing;;
let _ = parse_error;;
# 2 "parse.mly"
	open Abs_syn
# 22 "parse.ml"
let yytransl_const = [|
  259 (* INC *);
  260 (* DEC *);
  261 (* LT *);
  262 (* GT *);
  263 (* LEQ *);
  264 (* GEQ *);
  265 (* EQ *);
  266 (* NEQ *);
  267 (* IF *);
    0 (* EOF *);
  268 (* LPAREN *);
  269 (* RPAREN *);
    0|]

let yytransl_block = [|
  257 (* INT *);
  258 (* IDENT *);
    0|]

let yylhs = "\255\255\
\001\000\002\000\002\000\003\000\005\000\005\000\005\000\006\000\
\006\000\006\000\006\000\006\000\007\000\007\000\007\000\004\000\
\004\000\000\000"

let yylen = "\002\000\
\002\000\001\000\002\000\005\000\001\000\003\000\003\000\001\000\
\003\000\003\000\003\000\003\000\001\000\001\000\003\000\001\000\
\001\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\000\000\018\000\000\000\002\000\016\000\017\000\
\000\000\001\000\003\000\000\000\000\000\014\000\013\000\000\000\
\000\000\000\000\008\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\015\000\000\000\000\000\009\000\010\000\011\000\
\012\000"

let yydgoto = "\002\000\
\004\000\005\000\006\000\009\000\017\000\018\000\019\000"

let yysindex = "\255\255\
\010\255\000\000\006\255\000\000\002\000\000\000\000\000\000\000\
\024\255\000\000\000\000\017\255\004\255\000\000\000\000\004\255\
\005\255\012\255\000\000\254\254\004\255\004\255\004\255\004\255\
\004\255\004\255\000\000\012\255\012\255\000\000\000\000\000\000\
\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\004\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\003\000\013\000\000\000\000\000\000\000\
\000\000"

let yygindex = "\000\000\
\000\000\000\000\024\000\000\000\014\000\005\000\254\255"

let yytablesize = 282
let yytable = "\001\000\
\005\000\010\000\006\000\004\000\014\000\015\000\021\000\022\000\
\007\000\008\000\027\000\003\000\007\000\021\000\022\000\016\000\
\023\000\024\000\025\000\026\000\030\000\031\000\032\000\033\000\
\012\000\028\000\029\000\013\000\011\000\020\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\005\000\003\000\006\000\004\000\000\000\000\000\
\000\000\005\000\005\000\006\000\006\000\005\000\007\000\006\000\
\000\000\000\000\000\000\000\000\000\000\007\000\007\000\000\000\
\000\000\007\000"

let yycheck = "\001\000\
\000\000\000\000\000\000\000\000\001\001\002\001\009\001\010\001\
\003\001\004\001\013\001\002\001\000\000\009\001\010\001\012\001\
\005\001\006\001\007\001\008\001\023\000\024\000\025\000\026\000\
\001\001\021\000\022\000\011\001\005\000\016\000\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\002\001\002\001\002\001\002\001\255\255\255\255\
\255\255\009\001\010\001\009\001\010\001\013\001\002\001\013\001\
\255\255\255\255\255\255\255\255\255\255\009\001\010\001\255\255\
\255\255\013\001"

let yynames_const = "\
  INC\000\
  DEC\000\
  LT\000\
  GT\000\
  LEQ\000\
  GEQ\000\
  EQ\000\
  NEQ\000\
  IF\000\
  EOF\000\
  LPAREN\000\
  RPAREN\000\
  "

let yynames_block = "\
  INT\000\
  IDENT\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'entries) in
    Obj.repr(
# 16 "parse.mly"
                                        ( _1 )
# 184 "parse.ml"
               : Abs_syn.program))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'instruction) in
    Obj.repr(
# 19 "parse.mly"
               ( [_1] )
# 191 "parse.ml"
               : 'entries))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'entries) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'instruction) in
    Obj.repr(
# 20 "parse.mly"
                      ( (_1) @ [_2] )
# 199 "parse.ml"
               : 'entries))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 4 : string) in
    let _2 = (Parsing.peek_val __caml_parser_env 3 : 'reg_op) in
    let _3 = (Parsing.peek_val __caml_parser_env 2 : int) in
    let _5 = (Parsing.peek_val __caml_parser_env 0 : 'expression) in
    Obj.repr(
# 24 "parse.mly"
                                 ( BasicInst(get_inst_count(),BasicRegVar _1,_2,ConstExp(IntConst _3),_5) )
# 209 "parse.ml"
               : 'instruction))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'relational_expression) in
    Obj.repr(
# 27 "parse.mly"
                              ( _1 )
# 216 "parse.ml"
               : 'expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expression) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'relational_expression) in
    Obj.repr(
# 28 "parse.mly"
                                      ( BinOpAppExp (EqOp,_1,_3) )
# 224 "parse.ml"
               : 'expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expression) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'relational_expression) in
    Obj.repr(
# 29 "parse.mly"
                                       ( BinOpAppExp (NEqOp,_1,_3) )
# 232 "parse.ml"
               : 'expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'primary_expression) in
    Obj.repr(
# 32 "parse.mly"
                           ( _1 )
# 239 "parse.ml"
               : 'relational_expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'relational_expression) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'primary_expression) in
    Obj.repr(
# 33 "parse.mly"
                                               ( BinOpAppExp (LessOp,_1,_3) )
# 247 "parse.ml"
               : 'relational_expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'relational_expression) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'primary_expression) in
    Obj.repr(
# 34 "parse.mly"
                                               ( BinOpAppExp (GreaterOp,_1,_3) )
# 255 "parse.ml"
               : 'relational_expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'relational_expression) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'primary_expression) in
    Obj.repr(
# 35 "parse.mly"
                                               ( BinOpAppExp (LEqOp,_1,_3) )
# 263 "parse.ml"
               : 'relational_expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'relational_expression) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'primary_expression) in
    Obj.repr(
# 36 "parse.mly"
                                               ( BinOpAppExp (GEqOp,_1,_3) )
# 271 "parse.ml"
               : 'relational_expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 39 "parse.mly"
            ( RegExp (BasicRegVar _1) )
# 278 "parse.ml"
               : 'primary_expression))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : int) in
    Obj.repr(
# 40 "parse.mly"
            ( ConstExp (IntConst _1) )
# 285 "parse.ml"
               : 'primary_expression))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'expression) in
    Obj.repr(
# 41 "parse.mly"
                           ( _2 )
# 292 "parse.ml"
               : 'primary_expression))
; (fun __caml_parser_env ->
    Obj.repr(
# 44 "parse.mly"
      ( IncOp )
# 298 "parse.ml"
               : 'reg_op))
; (fun __caml_parser_env ->
    Obj.repr(
# 45 "parse.mly"
       ( DecOp )
# 304 "parse.ml"
               : 'reg_op))
(* Entry main *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
|]
let yytables =
  { Parsing.actions=yyact;
    Parsing.transl_const=yytransl_const;
    Parsing.transl_block=yytransl_block;
    Parsing.lhs=yylhs;
    Parsing.len=yylen;
    Parsing.defred=yydefred;
    Parsing.dgoto=yydgoto;
    Parsing.sindex=yysindex;
    Parsing.rindex=yyrindex;
    Parsing.gindex=yygindex;
    Parsing.tablesize=yytablesize;
    Parsing.table=yytable;
    Parsing.check=yycheck;
    Parsing.error_function=parse_error;
    Parsing.names_const=yynames_const;
    Parsing.names_block=yynames_block }
let main (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 1 lexfun lexbuf : Abs_syn.program)
