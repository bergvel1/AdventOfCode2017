{
   open Parse
   exception Eof
}

let numeric     = ['0' - '9']
let letter      = ['a' - 'z' 'A' - 'Z' '_']
let ident_char = letter

rule token = parse
    | [' ' '\t' '\n']   { token lexbuf }  (* skip over whitespace *)
    | eof               { EOF }
  	| "<"				{ LT }
  	| ">"				{ GT }
  	| "<="				{ LEQ }
  	| ">="				{ GEQ }
  	| "=="				{ EQ }
  	| "!="				{ NEQ }
    | "inc"      { INC }
    | "dec"      { DEC }
  	| "if"				{ IF }
    | "("         { LPAREN }
    | ")"         { RPAREN }
  	| (("-")?numeric+) as s { INT (int_of_string s) }
  	| (ident_char*) as s { IDENT s }


{

(* do not modify this function: *)

let lextest s = token (Lexing.from_string (s^"\n"))


let get_all_tokens s =
  let c = open_in s in
  let b = Lexing.from_channel c in
  let rec g () toks=
    match token b with
        EOF -> toks
      | t ->  g () (t :: toks)
  in
  let res = g () [] in
  let _ = close_in c in
  res

let get_all_token_options s =
    let c = open_in s in
    let b = Lexing.from_channel c in
    let rec g () toks = (*let _ = print_string "." in *)
        match (try (Some (token b),b.lex_curr_p) with _ -> (None,b.lex_curr_p)) with
            (Some EOF,p) -> (Some EOF,p)::toks
            | (None,p) -> (None,p)::toks
            | (Some t,p) -> g () ((Some t,p) :: toks)
        in
    let res = g () [] in
    let _ = close_in c in
    res

let get_n_token_options s n =
    let c = open_in s in
    let b = Lexing.from_channel c in
    let rec g toks n =
        if (n = 0) then (Some EOF,b.lex_curr_p)::toks else
        match (try (Some (token b),b.lex_curr_p) with _ -> (None,b.lex_curr_p)) with
            (Some EOF,p) -> (Some EOF,p)::toks
            | (None,p) -> [(None,p)]
            | t -> g (t :: toks) (n-1)
        in
    let res = g [] n in
    let _ = close_in c in
    res

let try_get_all_tokens s =
    try (Some (get_all_tokens s), true)
    with Failure "unmatched open comment" -> (None, true)
       | Failure "unmatched closed comment" -> (None, false)


}
