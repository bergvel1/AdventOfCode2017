(* abstract syntax Advent of Code 2017 Day 8 *)

type reg_var = 
	BasicRegVar of string

let string_of_reg_var = function
	BasicRegVar rv -> rv

type const = 
	IntConst of int

let string_of_const = function
    IntConst i -> string_of_int i

type reg_op =
	IncOp
|  	DecOp

let string_of_reg_op = function
	IncOp -> "inc"
| 	DecOp -> "dec" 

type bin_op =
 	EqOp 		(* _ == _ *)
| 	NEqOp		(* _ != _ *) 
|  	GreaterOp	(* _ > _ *) 
|  	LessOp		(* _ < _ *)
|  	GEqOp		(* _ >= _ *)
|  	LEqOp		(* _ <= _ *)

let string_of_bin_op = function
 	EqOp -> "=="
| 	NEqOp -> "!="
| 	GreaterOp -> ">"
| 	LessOp -> "<"
| 	GEqOp -> ">="
| 	LEqOp -> "<="

type exp =
	RegExp of reg_var 						(* variables *)
|  	ConstExp of const 					(* constants *)
|  	BinOpAppExp of bin_op * exp * exp 	(* exp1 % exp2, where % is a bin_op *)

let rec string_of_exp = function
	RegExp rv -> string_of_reg_var rv
| 	ConstExp c -> string_of_const c
|   BinOpAppExp (b,e1,e2) -> (string_of_exp e1) ^ " " ^ (string_of_bin_op b) ^ " " ^ (string_of_exp e2)

type inst =
	BasicInst of int * reg_var * reg_op * exp * exp (* e.g. "b inc 5 if a > 1" *)

let string_of_inst = function
	BasicInst (n,r,r_op,c,e) -> (string_of_int n) ^ ": " ^ (string_of_reg_var r) ^ " " ^ (string_of_reg_op r_op) ^ " " ^ (string_of_exp c) ^ " if " ^ (string_of_exp e)

type program = inst list

let (get_inst_count, see_inst_count, reset_inst_count) =
  let count = ref 1 in
  ((fun () -> (let c = !count in (count := c + 1; c))),
   (fun () -> (!count)),
   (fun () -> count := 1))
