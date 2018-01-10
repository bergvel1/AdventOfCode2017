open Lex
open Parse
open Abs_syn

type reg = (Abs_syn.reg_var * int)
	(*BasicReg of {name :Abs_syn.reg_var; value : int}*)

let get_reg_name r = string_of_reg_var (fst r)

let get_reg_value r = snd r

let string_of_reg r = (get_reg_name r) ^ " -> " ^ (string_of_int (get_reg_value r))

let rec string_of_env env = 
	(match env with
		  [] -> "\n"
		| r::rs -> (string_of_reg r) ^ "\n" ^ (string_of_env rs))

let inst_num inst =
  (match inst
   with BasicInst (n,r,ro,e1,e2) -> n)

(* returns register with highest value *)
let max_reg env = 
	List.fold_left (fun r1 r2 -> 
		(let v1 = (get_reg_value r1) in 
		let v2 = (get_reg_value r2) in 
			(if (v1 > v2) then r1 else r2))) (List.hd env) env

let eval_exp e env =
	(match e with
		  RegExp rv -> (match (List.find_opt (fun n -> String.equal (string_of_reg_var rv) (get_reg_name n)) env) with
		  	  Some reg -> ((get_reg_value reg),env)
		  	| None -> (0,((rv,0)::env)))
		| ConstExp (IntConst i) -> (i,env)
		| _ -> failwith "not yet supported") (* int<->bool casting? *)

let eval_bexp bexp env = 
	(match bexp with
		  BinOpAppExp (b,e1,e2) -> 
		  	(let (v1,env') = (eval_exp e1 env) in
		  	let (v2,env'') = (eval_exp e2 env') in
			let res = (match b with
			  	  EqOp -> (v1 = v2)
			  	| NEqOp -> (v1 <> v2)
			  	| GreaterOp -> (v1 > v2)
			  	| LessOp -> (v1 < v2)
			  	| GEqOp -> (v1 >= v2)
			  	| LEqOp -> (v1 <= v2)) in (res,env''))
		| _ -> failwith "not yet supported") (* int<->bool casting? *)

let one_step i env max_reg_opt = 
	let ret = (match i with
		  BasicInst (n,rv,ro,e1,e2) -> 
		  let (_,envRV) = (eval_exp (RegExp rv) env) in
		  (match (eval_bexp e2 envRV) with
		  	  (false,env') -> env'
		  	| (true,env') -> 
		  		(let (curr_val,env'') = (eval_exp (RegExp rv) env') in
		  		let (inc_val,env''') = (eval_exp e1 env'') in
		  		let final_val = (match ro with
		  						  IncOp -> curr_val + inc_val
		  						| DecOp -> curr_val - inc_val) in
		  		let env'''' = (List.remove_assoc rv env''') in
		  			(rv,final_val)::env''''))) in
	let _ = print_string ((string_of_inst i) ^ "\n" ^ (string_of_env ret)) in
		let new_max_reg_opt = 
			(let curr_max_reg = (max_reg ret) in
			(match max_reg_opt with
			  None -> Some curr_max_reg
			| Some old_max_reg -> (if ((get_reg_value curr_max_reg) > (get_reg_value old_max_reg)) then (Some curr_max_reg) else (Some old_max_reg)))) in
		(ret,new_max_reg_opt)

let rec execute insts env max_reg_opt = 
	(match insts with
		  [] -> (env,max_reg_opt)
		| x::xs -> (let step_res = (one_step x env max_reg_opt) in
			execute xs (fst step_res) (snd step_res)))



let rec mainloop () = 
reset_inst_count();
Printf.printf "Enter a filename: \n";
let filename = read_line() in
let c = open_in filename in
let lexbuf = Lexing.from_channel c in
let insts = Parse.main Lex.token lexbuf in
let (final_env,max_overall_reg_opt) = execute insts [] None in
let max_overall_reg = (match max_overall_reg_opt with
		  None -> failwith "No overall max register!"
		| Some r -> r) in
let max_final_reg = max_reg final_env in
	print_string ("Largest (overall) register: " ^ (string_of_reg max_overall_reg) ^ "\n" ^ "Largest (final) register: " ^ (string_of_reg max_final_reg) ^ "\n")
(*mainloop ()*)

let () = mainloop ()