#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <sstream>
#include <iostream>

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
	FUNC,
	IF,
	ELSE,
	FOR,
	WHILE,
	RETURN,
	BREAK,
	CONTINUE,
	DEFINE,
	PRINT,
	READ,
	IDENTIFIER,
	STRING,
	FUNCTION,
	VARIABLE,
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
#define Vector std::vector

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
Node_Type get_token_type(String str)
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

	if (str == "return") return RETURN;
	if (str == "if") return IF;
	if (str == "else") return ELSE;
	if (str == "for") return FOR;
	if (str == "while") return WHILE;
	if (str == "def") return DEFINE;
	if (str == "break") return BREAK;
	if (str == "continue") return CONTINUE;
	if (str == "func") return FUNC;
	if (str == "read") return READ;
	if (str == "print") return PRINT;

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
	case FUNC: return "func";
	case IF: return "if";
	case ELSE: return "else";
	case FOR: return "for";
	case WHILE: return "while";
	case RETURN: return "return";
	case BREAK: return "break";
	case CONTINUE: return "continue";
	case DEFINE: return "define";
	default:
		return "";
	}
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

Vector<Token> get_processed_sequence(const String& str)
{
	size_t i = 0;
	while (i < str.length() && get_char_type(str[i]) == EMPTY)	i++;
	String temp;
	Vector<Token> list;
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


class Scope
{
	
};

class AbstractSyntaxTree
{
public:

};


void interactive_interface()
{
	String temp;
	std::getline(std::cin, temp);
	Vector<Token> vec = get_processed_sequence(temp);
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
