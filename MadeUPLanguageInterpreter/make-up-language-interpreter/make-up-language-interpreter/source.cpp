#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <sstream>
#include <iostream>
#include <functional>
#include <stack>

enum Node_Type
{
	NUMBER,
	OPER_ADD,
	OPER_SUB,
	OPER_MUL,
	OPER_DIV,
	OPER_MOD,
	OPER_ASSIGN,
	OPER_EQUAL,
	OPER_GREATER,
	OPER_LESS,
	OPER_GREATEREQ,
	OPER_LESSEQ,
	OPER_LOGICAL_OR,
	OPER_LOGICAL_AND,
	OPER_LOGICAL_NOT,
	OPER_BITWISE_OR,
	OPER_BITWISE_AND,
	OPER_BITWISE_INV,
	OPER_BITWISE_XOR,
	LEFT_PAR,
	RIGHT_PAR,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_BRACE,
	RIGHT_BRACE,
	COMMA,
	SEMICOLON,
	DOT,
	COLON,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	KEYWORD_FUNC,
	KEYWORD_IF,
	KEYWORD_ELSE,
	KEYWORD_FOR,
	KEYWORD_WHILE,
	KEYWORD_RETURN,
	KEYWORD_BREAK,
	KEYWORD_CONTINUE,
	KEYWORD_DEFINE,
	KEYWORD_PRINT,
	KEYWORD_READ,
	KEYWORD_VAR,
	IDENTIFIER,
	STRING,
	FUNCTION,
	VARIABLE,
	LIST,
	BLOCK,
	STATEMENT,
	EXPRESSION,
	UNDEFINED,
};

enum Char_Type
{
	LETTER,
	OPERATOR,
	BRACKET,
	NUM,
	SPACE,
	EMPTY
};

typedef std::string String;
typedef long long data_type;
#define STD_Vector std::vector
#define STD_Map std::map
#define STD_Stack std::stack

Char_Type get_char_type(char charactor)
{
	if (charactor >='A' && charactor <= 'Z' || charactor >= 'a' && charactor <= 'z')
	{
		return LETTER;
	}
	if (charactor >= '0' && charactor <='9')
	{
		return NUM;
	}
	if (charactor == '(' || charactor == ')' || charactor == '[' || charactor == ']' || charactor == '{' || charactor == '}')
	{
		return BRACKET;
	}
	if (charactor == ' ')
	{
		return SPACE;
	}
	switch (charactor)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '=':
	case '!':
	case '~':
	case '>':
	case '<':
	case '|':
	case '^':
	case '%':
	case '.':
	case ',':
	case '\'':
	case '"':
	case ';':
	case ':':
		return OPERATOR;
	default:
		return EMPTY;
	}
}
bool is_identifier(char ch)
{
	return get_char_type(ch) == NUM || get_char_type(ch) == LETTER;
}
Node_Type get_token_type(const String& str)
{
	if (str == "+") return OPER_ADD;
	if (str == "-") return OPER_SUB;
	if (str == "*") return OPER_MUL;
	if (str == "/") return OPER_DIV;
	if (str == "%") return OPER_MOD;

	if (str == "=") return OPER_ASSIGN;
	if (str == "==") return OPER_EQUAL;

	if (str == "(") return LEFT_PAR;
	if (str == ")") return RIGHT_PAR;
	if (str == "[") return LEFT_BRACKET;
	if (str == "]") return RIGHT_BRACKET;
	if (str == "{") return LEFT_BRACE;
	if (str == "}") return RIGHT_BRACE;


	if (str == "||") return OPER_LOGICAL_OR;
	if (str == "&&") return OPER_LOGICAL_AND;
	if (str == "!") return OPER_LOGICAL_NOT;

	if (str == "|") return OPER_BITWISE_OR;
	if (str == "&") return OPER_BITWISE_AND;
	if (str == "~") return OPER_BITWISE_INV;
	if (str == "^") return OPER_BITWISE_XOR;
	if (str == ":") return COLON;
	if (str == ".") return DOT;
	if (str == ";") return SEMICOLON;
	if (str == ",") return COMMA;

	if (str == "return") return KEYWORD_RETURN;
	if (str == "if") return KEYWORD_IF;
	if (str == "else") return KEYWORD_ELSE;
	if (str == "for") return KEYWORD_FOR;
	if (str == "while") return KEYWORD_WHILE;
	if (str == "def") return KEYWORD_DEFINE;
	if (str == "break") return KEYWORD_BREAK;
	if (str == "continue") return KEYWORD_CONTINUE;
	if (str == "func") return KEYWORD_FUNC;
	if (str == "read") return KEYWORD_READ;
	if (str == "print") return KEYWORD_PRINT;
	if (str == "var") return KEYWORD_VAR;

	bool flag = true;
	for (int i = 0; i < str.length(); ++i)
	{
		if (get_char_type(str[i]) != NUM)
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{
		return NUMBER;
	}
	

	return IDENTIFIER;
		
}

String get_token_string(Node_Type type)
{
	switch (type)
	{
	case OPER_ADD: return "+";
	case OPER_SUB: return "-";
	case OPER_MUL: return "*";
	case OPER_DIV: return "/";
	case OPER_MOD: return "%";

	case OPER_LOGICAL_OR: return "||";
	case OPER_LOGICAL_AND: return "&&";
	case OPER_LOGICAL_NOT: return "!";

	case OPER_BITWISE_OR: return "|";
	case OPER_BITWISE_AND: return "&";
	case OPER_BITWISE_INV: return "~";
	case OPER_BITWISE_XOR: return "^";

	case OPER_ASSIGN: return "=";

	case OPER_GREATER: return ">";
	case OPER_LESS: return "<";
	case OPER_EQUAL: return "==";
	case OPER_GREATEREQ: return ">=";
	case OPER_LESSEQ: return "<=";

	case LEFT_BRACE: return "{";
	case RIGHT_BRACE: return "}";
	case LEFT_PAR: return "(";
	case RIGHT_PAR: return ")";
	case LEFT_BRACKET: return "[";
	case RIGHT_BRACKET: return "]";

	case COMMA: return ",";
	case DOT: return ".";
	case SEMICOLON: return ";";
	case COLON: return ":";
	case SINGLE_QUOTE: return "\'";
	case DOUBLE_QUOTE: return "\"";
	case KEYWORD_FUNC: return "func";
	case KEYWORD_IF: return "if";
	case KEYWORD_ELSE: return "else";
	case KEYWORD_FOR: return "for";
	case KEYWORD_WHILE: return "while";
	case KEYWORD_RETURN: return "return";
	case KEYWORD_BREAK: return "break";
	case KEYWORD_CONTINUE: return "continue";
	case KEYWORD_DEFINE: return "define";
	case KEYWORD_PRINT: return "print";
	case KEYWORD_READ: return "read";
	case KEYWORD_VAR: return "var";
	default:
		return "";
	}
}

int operator_priority(Node_Type oper)
{
	switch (oper)
	{
	case OPER_LOGICAL_NOT:
		return 2;
	case OPER_MUL:
	case OPER_DIV:
	case OPER_MOD:
		return 3;
	case OPER_ADD:
	case OPER_SUB:
		return 4;
	case OPER_GREATER:
	case OPER_GREATEREQ:
	case OPER_LESS:
	case OPER_LESSEQ:
		return 5;
	case OPER_EQUAL:
		return 6;
	case OPER_BITWISE_AND:
		return 7;
	case OPER_BITWISE_XOR:
		return 8;
	case OPER_BITWISE_OR:
		return 9;
	case OPER_LOGICAL_AND:
		return 10;
	case OPER_LOGICAL_OR:
		return 11;
	case OPER_ASSIGN:
		return 12;
	default:
		return -1;
	}
}

bool is_left_associative(Node_Type oper)
{
	if (oper == OPER_ASSIGN)
	{
		return false;
	}
	return true;
}

class Token
{
public:

	Token()
	{
		data = 0ll;
		type = UNDEFINED;
	}
	Token(const Token& token): name(token.name), data(token.data), type(token.type)
	{
	}

	Token(const String& str)
	{
		type = get_token_type(str);
		data = 0;
		if (type == NUMBER)
		{
			std::stringstream ss(str);
			data_type temp;
			ss >> temp;
			data = temp;
		}
		else if (type == IDENTIFIER)
		{
			name = str;
		}
	}

	String to_string() const
	{
		if (type == NUMBER)
		{
			std::stringstream ss;
			ss << data;
			return String("< num: ") + ss.str() + " >";
		}
		if (type == IDENTIFIER)
		{
			return String("< id: ") + name + " >";
		}
		return String("< ") + get_token_string(type) + " >";
	}

	String name;
	data_type data;
	Node_Type type;
};

STD_Vector<Token> get_processed_sequence(const String& str)
{
	size_t i = 0;
	while (i < str.length() && get_char_type(str[i]) == EMPTY)	i++;
	String temp;
	STD_Vector<Token> list;
	int flag = -1;
	int start = 0;
	for (; i < str.length(); ++i)
	{
		if (get_char_type(str[i]) == SPACE)
		{
			if (i > start)
			{
				list.push_back(Token(str.substr(start, i - start)));
				start = i + 1;
				flag = -1;
			}
		}
		else
		{
			Char_Type ct = get_char_type(str[i]);
			if (flag == -1)
			{
				start = i;
				if (ct == LETTER)
				{
					flag = 0;
				}
				else if (ct == NUM)
				{
					flag = 1;
				}
				else if (ct == OPERATOR)
				{
					flag = 2;
				}
				else if (ct == BRACKET)
				{
					start = i + 1;
					list.push_back(Token(str.substr(i, 1)));
				}
			}
			else
			{
				if (flag == 0)
				{
					if (ct == OPERATOR || ct == BRACKET)
					{
						list.push_back(Token(str.substr(start, i - start)));
						flag = -1;
						start = i;
						i--;
						continue;
					}
				}
				else if (flag == 1)
				{
					if (ct != NUM)
					{
						list.push_back(Token(str.substr(start, i - start)));
						flag = -1;
						start = i;
						i--;
						continue;
					}
				}
				else if (flag == 2)
				{
					if (ct == OPERATOR)
					{
					}
					else
					{
						list.push_back(Token(str.substr(start, i - start)));
						flag = -1;
						start = i;
						i--;
					}
				}
			}
		}
	}
	if (start < i)
	{
		list.push_back(Token(str.substr(start, i - start)));
	}
	return list;
}

class AbstractSyntaxTreeNode;
class Object
{
public:
	Object(): function_body(nullptr), type(UNDEFINED), data_value(0)
	{
	}

	Object(const data_type& _data): function_body(nullptr)
	{
		data_value = _data;
		type = NUMBER;
	}

	Object(const STD_Vector<Object>& vec)
	{
		type = LIST;
		data_value = 0;
		function_body = nullptr;
		list = vec;
	}

	AbstractSyntaxTreeNode* function_body;
	Node_Type type;
	data_type data_value;
	String name;
	STD_Vector<Object> list;
};

Object undefined_object()
{
	Object temp;
	temp.type = UNDEFINED;
	return temp;
}

typedef STD_Vector<Object> List;


class Scope
{
public:
	Scope(): parent(nullptr)
	{
	}

	Object find_in_scope(const String& identifier_name)
	{
		if (identifiers.find(identifier_name) != identifiers.end())
		{
			return identifiers[identifier_name];
		}
		if (parent != nullptr)
		{
			return parent->find_in_scope(identifier_name);
		}
		return Object();
	}

	void add_in_scope(const String& identifier_name, const Object& value)
	{
		identifiers[identifier_name] = value;
	}

	STD_Map<String, Object> identifiers;
	Scope* parent;
};
  
typedef Scope* scope_pointer;

class AbstractSyntaxTreeNode
{
public:

	AbstractSyntaxTreeNode(): scope(nullptr), parent(nullptr), type(UNDEFINED), data_value(0)
	{
	}

	Object evaluate(Scope* eval_scope) const 
	{
		if (children.empty())
		{
			if (type == NUMBER)
			{
				return data_value;
			}
			if (type == VARIABLE)
			{
				return eval_scope->find_in_scope(name);
			}
			std::cerr << "i guess there is something wrong with your code" << std::endl;
			return 0;
		}
		else // to do
		{
			if (type == LIST)
			{
				STD_Vector<Object> list;
				for (auto i = 0; i < children.size(); ++i)
				{
					Object temp = children[i]->evaluate(scope);
					if (temp.type != NUMBER && temp.type == VARIABLE && temp.type != FUNCTION)
					{
						std::cerr << String("I guess there is something wrong with your list member") << std::endl;
					}
					else
					{
						list.push_back(temp);
					}
				}
			}
		}
	}

	scope_pointer scope;

	STD_Vector<AbstractSyntaxTreeNode*> children;
	AbstractSyntaxTreeNode* parent;
	Node_Type type;
	data_type data_value;
	
	String name;
};

void SyntaxAnalysis(AbstractSyntaxTreeNode*& current, 
	const STD_Vector<Token>& token_list, 
	int start,
	int finish,
	Node_Type type)
{

	//Token head = token_list[0];

	if (type == BLOCK)
	{
		current->type = BLOCK;
		int last = start;
		for (int i = start + 1; i < finish; ++i)
		{
			if (token_list[i].type == SEMICOLON)
			{
				current->children.push_back(new AbstractSyntaxTreeNode);
				SyntaxAnalysis(current->children[current->children.size() - 1], token_list, last, i - 1, STATEMENT);
				last = i + 1;
			}
			else if (token_list[i].type == LEFT_BRACE)
			{
				int brace_counter = 1;
				int j = i + 1;
				while (j < finish && brace_counter != 0)
				{
					if (token_list[j].type == LEFT_BRACE)
					{
						brace_counter++;
					}
					else if (token_list[j].type == RIGHT_BRACE)
					{
						brace_counter--;
					}
					j++;
				}
				if (j == finish)
				{
					std::cerr << "I guess you miss some '{' or '}' here" << std::endl;
					return;
				}
				i = j - 1;
			}
		}
	}
	else if (type == STATEMENT)
	{
		if (token_list[start].type == KEYWORD_DEFINE)
		{
			current->type = KEYWORD_DEFINE;
		}
		else if (token_list[start].type == KEYWORD_FOR)
		{
			current->type = KEYWORD_FOR;
		}
		else if (token_list[start].type == KEYWORD_WHILE)
		{
			current->type = KEYWORD_WHILE;
		}
		else if (token_list[start].type == KEYWORD_IF)
		{
			current->type = KEYWORD_IF;
		}
		else if (token_list[start].type == KEYWORD_VAR)
		{
			current->type = KEYWORD_VAR;
		}
		else if (token_list[start].type == KEYWORD_BREAK)
		{
			current->type = KEYWORD_BREAK;
		}
		else if (token_list[start].type == KEYWORD_CONTINUE)
		{
			current->type = KEYWORD_CONTINUE;
		}
		else
		{
			finish -= 1;
			int pos = start;
			STD_Vector<AbstractSyntaxTreeNode*> vec;
			STD_Stack<AbstractSyntaxTreeNode*> stk;
			STD_Stack<Token> oper_stk;

			for (int i = start; i < finish; ++i)
			{
				if (token_list[i].type == NUMBER || token_list[i].type == IDENTIFIER)
				{
					AbstractSyntaxTreeNode* temp_node = new AbstractSyntaxTreeNode;
					temp_node->type = token_list[i].type;
					temp_node->data_value = token_list[i].data;
					temp_node->name = token_list[i].name;
					vec.push_back(temp_node);
				}
				else if (token_list[i].type == LEFT_PAR)
				{
					oper_stk.push(token_list[i]);
				}
				else if (token_list[i].type == RIGHT_PAR)
				{
					while (oper_stk.top().type != LEFT_PAR)
					{
						AbstractSyntaxTreeNode* temp_node = new AbstractSyntaxTreeNode;
						temp_node->type = oper_stk.top().type;
						vec.push_back(temp_node);
						oper_stk.pop();
					}
				}
				else
				{
					while (!oper_stk.empty() && 
						(is_left_associative(oper_stk.top().type) 
							&& (operator_priority(oper_stk.top().type) <= operator_priority(token_list[i].type))) &&
						(!is_left_associative(oper_stk.top().type)
							&& (operator_priority(oper_stk.top().type) < operator_priority(token_list[i].type))))
					{
						AbstractSyntaxTreeNode* temp_node = new AbstractSyntaxTreeNode;
						temp_node->type = oper_stk.top().type;
						vec.push_back(temp_node);
						oper_stk.pop();
					}
					oper_stk.push(token_list[i]);
				}
			}
			while (!oper_stk.empty())
			{
				AbstractSyntaxTreeNode* temp_node = new AbstractSyntaxTreeNode;
				temp_node->type = oper_stk.top().type;
				vec.push_back(temp_node);
				oper_stk.pop();
			}
			for (int i = 0; i < vec.size(); ++i)
			{
				if (vec[i]->type == NUMBER || vec[i]->type == IDENTIFIER)
				{
					stk.push(vec[i]);
				}
				else
				{
					// to do
				}
			}
		}
	}
	else if (type == EXPRESSION)
	{
		
	}
	else if (type == KEYWORD_FUNC)
	{
		
	}
	else if (type == KEYWORD_DEFINE)
	{
		
	}
	else if (type == KEYWORD_VAR)
	{
		
	}
	else if (type == KEYWORD_PRINT)
	{
		
	}
	else if (type == KEYWORD_READ)
	{
		
	}
}

void interactive_interface()
{
	String temp;
	getline(std::cin, temp);  
	STD_Vector<Token> vec = get_processed_sequence(temp);
	for (int i = 0; i < vec.size(); ++i)
	{
		std::cout << vec[i].to_string() << " ";
		if (vec[i].type == SEMICOLON)
		{
			std::cout << std::endl;
		}
	}
	if (vec[vec.size() - 1].type != SEMICOLON)
	{
		std::cout << std::endl;
	}
	
}

int main(int argc, char* argv[])
{
	while (true)
	{
		interactive_interface();
	}
}
