/*
ICS 142A Homework 9
Name: Xiao Yang
ID: 90282658
*/

#include <string>
#include <string.h>
#include <iostream>
#include <cstdlib>
#define    TRUE 		   4396
#define    FALSE   		   8964
typedef int token;
typedef int integer;

enum type_kind {undefined_type, integer_type, boolean_type};
int count = 0;

typedef struct symbol_block* symbol;

struct symbol_block {
    char * name;
    type_kind type;
    integer value;

    symbol_block(char * n, type_kind t, integer v) :
        name(n), type(t), value(v)
    {
    }
};

typedef struct table_block* table;
struct table_block {
    symbol head;
    table tail;

    table_block(symbol h, table t) :
        head(h), tail(t)
    {
    }
    table_block(table t)
    {
    	head = 0;
    	tail = t;
    }

};

class my_symtab{
	table head;
	table scope_table;
	int count;
public:
	void enter_symbol(char* name, type_kind type, int i)
	{
		string n(name);
		symbol t = new symbol_block(name, type, i); //initialize as -1
		head = new table_block(t, head);
		if(scope_table != 0 )
		{
			for(table p = scope_table; p && p->head != 0; p = p->tail)
			{
				string s(p->head->name);
//				cout << "'compare " <<n <<" and "<<s<<endl;
				if(s == n)
				{
//					cout <<"override "<<endl;
					p->head->type = type;
					break;
				}
				else
				{

					if(p->tail == 0 || p->tail->head == 0)
					{					
//						cout <<"new"<<endl;
						scope_table = new table_block(t, scope_table);
						break;
					}
				}
			}
		}
	}
	type_kind find_type(char* name)
	{
//		cout <<"in find_type "<<name<<endl;
		string n(name); 
		for(table p = head; p; p = p->tail)
		{
			if(p->head != 0)
			{
				string s(p->head->name);
//				cout <<s<<endl;
				if(n == s)
					return p->head->type;
			}
		}
//		cout <<n <<" is not find, exit"<<endl;
		exit(1);
	}
	int find_value(char* name)
	{
		string n(name); 
		for(table p = head; p; p = p->tail)
		{
			if(p->head != 0)
			{
				string s = p->head->name;
				if(n == s)
					return p->head->value;
			}
		}
		return -1;
	}
	void enter_scope()
	{
		table temp = head;
		head = new table_block(head);
		scope_table = new table_block(scope_table);
		for(; temp && temp->head != 0; temp = temp->tail)
		{
			scope_table = new table_block(temp->head, scope_table);
		}
	}
	
	void exit_scope()
	{
		table temp, temp2;
		temp = head;
		temp2 = scope_table;
		if(head != 0)
		{
			for(; head->head != 0; temp = head)
			{
				head = head->tail;
				delete temp;
			}
			temp = head;
			head = head->tail;
			delete temp;
		}

		if (scope_table != 0)
		{
			for(; scope_table->head !=0; temp2 = scope_table)
			{
				scope_table = scope_table->tail;
				delete temp2;
			}
			temp2 = scope_table;
			scope_table = scope_table->tail;
			delete temp2;
		}
	}
	bool is_repeat_declare(char* name)
	{
		string n = name;
		for(table p = head; p; p=p->tail)
		{
			if(p->head != 0)
			{
				string s = p->head->name;
				if(n == s)
					return true;
			}
			else
				return false;
		}
		return false;
	}

	my_symtab(){
		head = 0; 
		scope_table = 0;

		enter_symbol(strdup("integer"), integer_type, -1);
        enter_symbol(strdup("boolean"), boolean_type, -1);
        enter_symbol(strdup("true"), boolean_type, -1);
        enter_symbol(strdup("false"), boolean_type, -1);
	}
	
};



int compute(int op, int left, int right)
{
	switch(op)
	{
		case '+':
			return left + right;
		case '-':
			return left - right;
		case '*':
			return left * right;
		case '/':
			return left / right;
		default:
			return 10086;
	}
}

const char* convert(type_kind a)
{
	switch(a)
	{
		case integer_type:
			return "integer";
		case boolean_type:
			return "boolean";
		default:
			return "undefined";
	}
}

type_kind type_check(int op, type_kind left, type_kind right)
{
	switch(op){
		case '+':case '-':case '*':case '/':

			if(left == integer_type && right == integer_type)
				return integer_type;
			else
				exit(1);
		
		case '<':
			if(left == integer_type && right == integer_type)
				return boolean_type;
			else
				exit(1);

		case 263:
			if(left == right)
			{
				if(left == integer_type)
					return integer_type;
				if(left == boolean_type)
					return boolean_type;
				else
					exit(1);
			}
			else
				exit(1);

		case 258: case 259:
			if(left == boolean_type && right == boolean_type)
				return boolean_type;
			else
				exit(1);
		default:
			exit(1);
	}
}

const char* to_string(token op)
{
	switch(op)
	{
		case '+':
			return "+";
		case '-':
			return "-";
		case '*':
			return "*";
		case '/':
			return "/";
		case '<':
			return "<";
		default:
			return "suprise";
	}
}
void print_indent(int indent)
{
	string n;
	for(int i = 0; i < indent-1; i++)
		n += "    ";	
	if(indent > 1)
		cout<<n;
}

// ***<<< LEFT FOR YOU TO DEFINE >>>***
// ***<<< type symtab is linked_stack of table >>>***

my_symtab mytable;


typedef struct eval_result_block* eval_result;
struct eval_result_block {
    type_kind type;
    integer value;

    eval_result_block(type_kind t, integer v) :
        type(t), value(v)
    {
    }

};

typedef struct node_block* node;

struct node_block {

    virtual eval_result eval() {
        cerr << "Error: You forgot to implement eval for this node type\n";
    }
    virtual void pprint(int indent = 0) {
        cerr << "Error: You forgot to implement pprint for this node type\n";
    }

};

typedef struct node_pair* node_list;

struct node_pair {

    node head;
    node_list tail;
    
    node_pair(node h, node_list t) :
        head(h), tail(t)
    {
    }

    eval_result eval()
    {
    }

};

struct binary_node : node_block {

    token bin_op;
    node left, right;

    binary_node(token b_op, node l, node r) :
        bin_op(b_op), left(l), right(r)
    {
    }

    virtual eval_result eval() 
    {
 //   	cout <<"start evaluate binary "<<bin_op<<endl;
    	int result;
    	eval_result left_result = left->eval();
    	eval_result right_result = right->eval();
    	type_kind t = type_check(bin_op, left_result->type, right_result->type);
    	if(bin_op == 258)
    	{
    		if(t == boolean_type)
    		{
    			if(left_result->value == TRUE && right_result->value == TRUE)
    				result = TRUE;
    			else
    				result = FALSE;
    		}
    		else
    			exit(1);
    	}
    	if(bin_op == '<')
    	{
    		if(t == boolean_type)
    		{
    			if(left_result->value < right_result->value)
    			{
//    				cout <<"true"<<endl;
    				return new eval_result_block(boolean_type, TRUE);
    			}
    			else
    			{
//    				cout <<"false"<<endl;
    				return new eval_result_block(boolean_type, FALSE);
    			}
    		}
    		else
    			exit(1);
    	}
    	else
    	{	
    		if(t == integer_type)
    			result = compute(bin_op, left_result->value, right_result->value);
    		else
    			exit(1);
    	}
    	return new eval_result_block(t, result);

    }
    virtual void pprint(int indent) 
    {
    	left->pprint(indent);
    	cout <<" "<< to_string(bin_op) <<" ";
    	right->pprint(indent);
    }
};

struct unary_node : node_block {

    token un_op;
    node operand;

    unary_node(token u_op, node op) :
        un_op(u_op), operand(op)
    {
    }

    virtual eval_result eval() 
    {
    	eval_result result = operand->eval();
    	if(result->type == integer_type)
    		exit(1);
    	else
    	{
    		if(result->value == TRUE)
    			return new eval_result_block(boolean_type, FALSE);
    		else
    			return new eval_result_block(boolean_type, TRUE);
    	}
    }
    virtual void pprint(int indent) 
    {
    	cout <<"Not ";
    	operand->pprint(indent);
    	cout <<";";
    }

};

struct integer_node : node_block {
    
    int value;

    integer_node(int v) :
        value(v)
    {
    }

    virtual eval_result eval() 
    {
 //   	cout <<"evaluate integer "<<value <<endl;
    	return new eval_result_block(integer_type,value);
    }
    virtual void pprint(int indent) 
    {
    	cout << value ;
    }

};

struct ident_node : node_block {

    char * id_name;

    ident_node(char * n) :
        id_name(n)
    {
    }

    virtual eval_result eval() 
    {
 //   	cout <<"start evaluate identifier "<<id_name<<endl;
	    string name(id_name);
	    if(name == "true")
	    	return new eval_result_block(boolean_type, TRUE);
	    if(name == "false")
	    {
//	    	cout <<"false"<<endl;
	    	return new eval_result_block(boolean_type, FALSE);
	    }
	    else
	    {
		    type_kind t = mytable.find_type(id_name); //if id is not in table, exit(1)
//		    cout <<"find "<<id_name <<" is type "<<t<<endl;
		    if( t != undefined_type)
		    {
		    	int v = mytable.find_value(id_name);
		    	return new eval_result_block(t, v);
		    }	    	
	    }


    }
    virtual void pprint(int indent) 
    {
    	cout <<id_name;
    }

};

struct assign_node : node_block {

    char * dest;
    node src;

    assign_node(char * d, node s) :
        dest(d), src(s)
    {
    }

    virtual eval_result eval() 
    {
//    	cout << "start evaluate "<<dest <<endl;
	    string name(dest);
    	eval_result temp = src->eval();
    	type_kind t = mytable.find_type(dest);
//    	cout <<"pass find type"<<endl;
    	if(t != temp->type)
    	{
//    		cout <<" unmatched type "<<t <<" := "<<temp->type<<endl;
    		exit(1);
    	}
    	else
    	{
    		mytable.enter_symbol(dest, temp->type, temp->value);
//    		cout <<"update "<<dest <<"to "<<temp->value<<endl;
    		return temp;
    	}

    }
    virtual void pprint(int indent) 
    {
    	cout << "    "<<dest << " := ";
    	src->pprint();
    	cout <<";" << endl;
    }

};

struct var_decl_node : node_block {
    char * name;
    char * type;

    var_decl_node(char * n, char * t) :
        name(n), type(t)
    {

    }

    virtual eval_result eval() 
    {
 //   	cout <<"evaluate declaration "<<name <<" : "<<type<<endl;
    	if(*type == 'i')
    	{
    		mytable.enter_symbol(name, integer_type, -1);
    		return new eval_result_block(integer_type, -1);
    	}
    	if(*type == 'b')
    	{
    		mytable.enter_symbol(name, boolean_type, -1);
    		return new eval_result_block(boolean_type, -1);
    	}
    	
    }
    virtual void pprint(int indent) 
    {
    	cout <<"    "<<name<<" : "<<type <<";\n";
    }
};

struct declare_node : node_block {
    node_list decls;
    node_list stmts;

    declare_node(node_list d, node_list s) :
        decls(d), stmts(s)
    {
    }

    virtual eval_result eval() 
    {
    	node_list d = decls;
    	node_list s = stmts;
		mytable.enter_scope();
//		cout <<"start evaluation in scope "<<count <<endl;
		count ++;
    	for(; d->tail !=0; d= d->tail)
    	{
    		d->head->eval();
    	}
    	d->head->eval();

		for(; s->tail != 0; s = s->tail)
		{		
			s->head->eval();
		}
		s->head->eval();
		mytable.exit_scope();
		count--;		
//		cout <<"exit scope "<<count <<endl;



    }
    virtual void pprint(int indent) 
    {
  		count = indent;
		count++;
	   	node_list d = decls;
    	node_list s = stmts;
    	print_indent(count);
		cout<<"Declare"<<endl;	

    	for(; d->tail !=0; d= d->tail)
    	{
 			print_indent(count);			
			d->head->pprint(count);
    	}
		print_indent(count);		   	
    	d->head->pprint(count);

  		print_indent(count);
		cout<<"Begin"<<endl;
	
		for(; s->tail != 0; s = s->tail)
		{

 			print_indent(count);		   		
			s->head->pprint(count);

		}
		print_indent(count);
		s->head->pprint(count);
  		print_indent(count);
		cout <<"End;"<<endl;
		count--;
    }
};

struct if_node : node_block {
    node if_cond;
    node_list if_stmts;
    node if_tail;

    if_node(node cond, node_list s, node n):
        if_cond(cond), if_stmts(s), if_tail(n)
    {
    }

    virtual eval_result eval() 
    {
    	node_list d = if_stmts;

    	eval_result condition = if_cond->eval();
    	if(condition->type == integer_type)
    		exit(1);
    	else
    	{
    		if(condition->value == TRUE)
    		{	
		    	for(; d->tail !=0; d= d->tail)
		    	{
		    		d->head->eval();
		    	}
		    	d->head->eval();
    		}
    		else
    			return if_tail->eval();
    	}
    }
    virtual void pprint(int indent) 
    {
    	node_list d = if_stmts;
    	cout <<"    if ";
    	if_cond->pprint(indent);
    	cout<<" then"<<endl;
    	for(; d->tail !=0; d= d->tail)
    	{
	    	print_indent(indent); 		
	    	cout <<"    ";
	    	d->head->pprint(indent);
    	}
    	cout <<"    ";
    	print_indent(indent);
    	d->head->pprint(indent);




    	node_list f = ((if_node*)if_tail)->if_stmts;
    	if(!((if_node*)if_tail)->if_cond)
    	{
	    	print_indent(indent);
    		cout<<"    else"<<endl;
    		
	    	for(; f->tail !=0; f= f->tail)
	    	{
		    	print_indent(indent); 		
		    	cout <<"    ";
		    	d->head->pprint(indent);
	    	}
	    	cout <<"    ";
	    	f->head->pprint(indent);  
	    	print_indent(indent); 
	    	cout <<"    end if"<<endl;  	
		}    	
		else{
			if_tail->pprint(indent);
	    }
	}
};

struct elif_node : node_block {
    node if_cond;
    node_list if_stmts;
    node if_tail;

    elif_node(node cond, node_list s, node n):	
    	if_cond(cond), if_stmts(s), if_tail(n)
    {
    }
    virtual eval_result eval()
    {
   		node_list d = if_stmts;

    	eval_result condition = if_cond->eval();
    	if(condition->type == integer_type)
    		exit(1);
    	else
    	{
    		if(condition->value == TRUE)
    		{
		    	for(; d->tail !=0; d= d->tail)
		    	{
		    		d->head->eval();
		    	}
		    	d->head->eval();
    		}
    		else
    			return if_tail->eval();
    	}
    }
    virtual void pprint(int indent)
    {
    	node_list d = if_stmts;
     	print_indent(indent);
    	cout <<"    elsif ";
    	if_cond->pprint(indent);
    	cout<<" then"<<endl;
    	for(; d->tail !=0; d= d->tail)
    	{
	    	print_indent(indent); 		
	    	cout <<"    ";
	    	d->head->pprint(indent);
    	}
    	cout <<"    ";
    	print_indent(indent);
    	d->head->pprint(indent);

    	node_list f = ((if_node*)if_tail)->if_stmts;
    	if(!((if_node*)if_tail)->if_cond)
    	{
	    	print_indent(indent);
    		cout<<"    else"<<endl;
    	}	
    	for(; f->tail !=0; f= f->tail)
    	{
	    	print_indent(indent); 		
	    	cout <<"    ";
	    	d->head->pprint(indent);
    	}
    	cout <<"    ";
    	print_indent(indent);
    	f->head->pprint(indent);  
    	print_indent(indent); 
    	cout <<"    end if"<<endl;  	

    }    	

};

struct while_node : node_block {
    node while_cond;
    node_list while_stmts;

    while_node(node cond, node_list s) :
        while_cond(cond), while_stmts(s)
    {
    }

    virtual eval_result eval() 
    {
 
    	eval_result condition = while_cond->eval();
    	if(condition->type == integer_type)
    		exit(1);
    	else
    	{
    		if(condition->value == TRUE)
    		{
			   	node_list d = while_stmts;
//    			cout <<"in while :"<<endl;
		    	for(; d->tail !=0; d= d->tail)
		    	{
		    		d->head->eval();
		    	}
		    	d->head->eval();
    			this->eval();
    		}
    	}
    }
    virtual void pprint(int indent) 
    {
    	node_list d = while_stmts;
    	print_indent(indent);
    	cout <<"    while ";
    	while_cond->pprint(indent);
    	cout <<" loop"<<endl;
    	for(; d->tail !=0; d= d->tail)
    	{
	    	print_indent(indent); 		
	    	cout <<"    ";
	    	d->head->pprint(indent);
    	}
    	cout <<"    ";
    	d->head->pprint(indent);
      	print_indent(indent);
    	cout <<"    end loop;" <<endl;
    	

    }

};

struct output_node : node_block {
    char * function;
    node argument;

    output_node(char * f, node arg) :
        function(f), argument(arg)
    {
    }

    virtual eval_result eval() 
    {
    	eval_result arg = argument->eval();
    	if(*function == 'g')
    	{
			int in;
    		if(arg->type == integer_type)
    		{

    			cin >> in;
    			mytable.enter_symbol(((ident_node*)argument)->id_name, integer_type, in);
    		}
    		else
    		{
    			bool b;
    			cin >> b;
    			if(b)
    				in = TRUE;
    			else
    				in = FALSE;
    			mytable.enter_symbol(((ident_node*)argument)->id_name, boolean_type, in);
    		}   		
    	}
    	else
    	{
    		cout <<"put " <<arg->value<<endl;
    	}
    }
    virtual void pprint(int indent) 
    {
    	cout <<"    " <<function<<"(";
    	argument->pprint(indent);
    	cout <<")"<<endl;
    }

};

