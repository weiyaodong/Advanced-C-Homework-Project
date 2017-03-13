#pragma once

#include <cmath>
#include <vector>
#include <sstream>

const double EPS = 1e-11;

enum Node_Type
{
	NUMBER,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	POWER,
	SIN,
	COS,
	TAN,
	IF,
	IF_2,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	DOT,
	LETTER,
	NONE,
};

const std::string PREFIX_BRANCH_START = "+";//'©Ð';
const std::string PREFIX_BRANCH_MID	= "+";//'©À';
const std::string PREFIX_BRANCH_FILL = "|";//'©¦';
const std::string PREFIX_BRANCH_END	= "+";//'©¸';
const std::string PREFIX_LEAF = "-";//'©¤';
const std::string PREFIX_BLANK = " ";

const char temp = '©Ð';

inline std::string blank_str(size_t len)
{
	std::string temp;
	for (size_t i = 0; i < len; ++i)
	{
		temp += " ";
	}
	return temp;
}

inline Node_Type type_mapping(char c)
{
	switch (c)
	{
	case '+':return PLUS;
	case '-':return MINUS;
	case '*':return MULTIPLY;
	case '/':return DIVIDE;
	case '^':return POWER;
	case '(':return LEFT_BRACKET;
	case ')':return RIGHT_BRACKET;
	case '?':return IF;
	case ':':return IF_2;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':return NUMBER;
	case '.':return DOT;
	default:
		if (c >= 'a' && c <= 'z' || c>='A' && c<='Z')
		{
			return LETTER;
		}
		return NONE;
	}
}

inline int priority(Node_Type type)
{
	switch (type)
	{
	case IF:
	case IF_2:
		return 1;
	case PLUS:
	case MINUS:
		return 2;
	case MULTIPLY:
	case DIVIDE:
		return 3;
	case POWER:
		return 4;
	case SIN:
	case COS:
	case TAN:
		return 5;
	default:
		return 10000;
	}
}

template <typename data_type>
class SyntaxElement
{
public:

	SyntaxElement()
	{
		type = NUMBER; 
		data = 0;
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	SyntaxElement(char ch)
	{
		type = type_mapping(ch);
		data = 0;
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	SyntaxElement(std::string str)
	{
		if (str == "sin")
		{
			type = SIN;
			data = 0;
		}
		else if (str == "cos")
		{
			type = COS;
			data = 0;
		}
		else if (str == "tan")
		{
			type = TAN;
			data = 0;
		}
		else
		{
			std::istringstream ss(str);
			ss >> data;
			type = NUMBER;
		}
	}

	bool operator == (char ch) const
	{
		return type == type_mapping(ch);
	}

	bool operator == (Node_Type ntype) const
	{
		return type == ntype;
	}

	Node_Type type;
	data_type data;
};

inline void syntax_analysis_pretreatment(std::string& str)
{
	std::string temp_string = "(";
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (type_mapping(str[i]) != NONE)
		{
			temp_string += str[i];
		}
	}
	temp_string += ")";
	str = temp_string;
}

template<typename data_type>
std::vector<SyntaxElement<data_type>> convert_to_syntax_element_sequence(const std::string& str)
{
	std::vector<SyntaxElement<data_type>> result;

	for (int i = 0; i < str.length(); ++i)
	{
		if (str.length() - i > 3)
		{
			if (str.substr(i, 3) == "sin")
			{
				i += 2;
				result.push_back(SyntaxElement<data_type>("sin"));
				continue;
			}
			if (str.substr(i, 3) == "cos")
			{
				i += 2;
				result.push_back(SyntaxElement<data_type>("cos"));
				continue;
			}
			if (str.substr(i, 3) == "tan")
			{
				i += 2;
				result.push_back(SyntaxElement<data_type>("tan"));
				continue;
			}
		}
		if (type_mapping(str[i]) == NUMBER || type_mapping(str[i]) == DOT)
		{
			int dot_counter = 1;
			int flag = i;
			while (flag < str.length() && (type_mapping(str[flag]) == NUMBER || type_mapping(str[flag]) == DOT))
			{
				if (type_mapping(str[flag]) == DOT)
				{
					if (dot_counter)
					{
						dot_counter--;
					}
					else
					{
						std::cerr << "above one dots in a number" << std::endl;
					}
				}
				flag++;
			}
			result.push_back(SyntaxElement<data_type>(str.substr(i, flag - i)));
			i = flag - 1;
		}
		else if (type_mapping(str[i]) == MINUS && result[result.size() - 1].type == LEFT_BRACKET)
		{
			result.push_back(SyntaxElement<data_type>("-1"));
			result.push_back(SyntaxElement<data_type>('*'));
		}
		else
		{
			result.push_back(SyntaxElement<data_type>(str[i]));
		}
	}
	return result;
}

template <typename data_type>
class AbstractSyntaxTreeNode
{
public:

	AbstractSyntaxTreeNode()
	{
		type = NUMBER;
		child1 = child2 = child3 = nullptr;
		data = 0;
	}

	AbstractSyntaxTreeNode(const AbstractSyntaxTreeNode& ast_node):
		type(ast_node.type), child1(ast_node.child1), child2(ast_node.child2), child3(ast_node.child3), data(ast_node.data)
	{
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	AbstractSyntaxTreeNode(const data_type& data_)
	{
		type = NUMBER;
		child1 = child2 = child3 = nullptr;
		data = data_;
	}

	typedef AbstractSyntaxTreeNode* ast_pointer;

	data_type get_value() const
	{
		switch (type)
		{
		case NUMBER:
			return data;
		case PLUS:
			return child1->get_value() + child2->get_value();
		case MINUS:
			return child1->get_value() - child2->get_value();
		case MULTIPLY:
			return child1->get_value() * child2->get_value();
		case DIVIDE:
			return child1->get_value() / child2->get_value();
		case POWER:
			return pow(static_cast<double>(child1->get_value()), 
				static_cast<double>(child2->get_value()));
		case SIN:
			return sin(child1->get_value());
		case COS:
			return cos(child1->get_value());
		case TAN:
			return tan(child1->get_value());
		case IF:
			return (child1->get_value() < EPS && child1->get_value() > -EPS) ? child3->get_value() : child2->get_value();
		default:
			return 0;
		}
	}

	size_t degree() const
	{
		switch (type)
		{
		case NUMBER:
			return 0;
		case PLUS:
		case MINUS:
		case MULTIPLY:
		case DIVIDE:
		case POWER:
			return 2;
		case SIN:
		case COS:
		case TAN:
			return 1;
		case IF:
			return 3;
		default:
			return 0;
		}
	}

	std::string transfer_to_string() const
	{
		std::stringstream ss;
		switch (type)
		{
		case NUMBER:
			ss << data;
			return ss.str();
		case PLUS:
			return "add";
		case MINUS:
			return "sub";
		case MULTIPLY:
			return "mul";
		case DIVIDE:
			return "div";
		case POWER:
			return "pow";
		case SIN:
			return "sin";
		case COS:
			return "cos";
		case TAN:
			return "tan";
		default:
			return "";
		}
	}
	
	Node_Type type;

	ast_pointer child1;

	ast_pointer child2;

	ast_pointer child3;

	data_type data;
};

template <typename data_type>
void clear(AbstractSyntaxTreeNode<data_type>* ast_node)
{
	if (ast_node->child1 != nullptr)
	{
		clear(ast_node->child1);
		delete ast_node->child1;
	}
	if (ast_node->child2 != nullptr)
	{
		clear(ast_node->child2);
		delete ast_node->child1;
	}
	if (ast_node->child3 != nullptr)
	{
		clear(ast_node->child4);
		delete ast_node->child1;
	}
}


template<typename data_type>
void build(AbstractSyntaxTreeNode<data_type>*& current_node, const std::vector<SyntaxElement<data_type>>& vec, int begin, int end)
{
	current_node = new AbstractSyntaxTreeNode<data_type>;
	
	int min_pri = 10000;
	int pos = -1;

	for (int i = begin; i < end; ++i)
	{
		if (vec[i] == LEFT_BRACKET)
		{
			int temp = 1;
			int j;
			for (j = i + 1; j < end && temp; ++j)
			{
				if (vec[j] == LEFT_BRACKET)
				{
					temp++;
				}
				if (vec[j] == RIGHT_BRACKET)
				{
					temp--;
				}
			}
			if (j == end && i == begin)
			{
				build(current_node, vec, begin + 1, end - 1);
				return;
			}
			if (j == end)
			{
				break;
			}
			i = j;
		}
		if (vec[i].type == NUMBER)
		{
			continue;
		}
		Node_Type type = vec[i].type;
		if (priority(type) <= min_pri)
		{
			min_pri = priority(type);
			pos = i;
		}
	}

	if (pos != -1)
	{
		if (vec[pos].type == IF)
		{
			int pos2 = -1;
			for (int i = pos + 1; i < end; ++i)
			{
				if (vec[i].type == IF_2)
				{
					pos2 = i;
					break;
				}
			}
			if (pos2 == -1)
			{
				std::cerr << "i think you need a ':' " << std::endl;
				return;
			}
			current_node->type = vec[pos].type;
			build(current_node->child1, vec, begin, pos);
			build(current_node->child2, vec, pos + 1, pos2);
			build(current_node->child3, vec, pos2 + 1, end);
			
		}
		else if (vec[pos].type == SIN || vec[pos].type == COS || vec[pos].type == TAN)
		{
			current_node->type = vec[pos].type;
			if (vec[pos + 1].type == LEFT_BRACKET)
			{
				int counter = 1;
				int j;
				for (j = pos + 2; j < vec.size() && counter; ++j)
				{
					if (vec[j] == LEFT_BRACKET)
					{
						counter++;
					}
					if (vec[j] == RIGHT_BRACKET)
					{
						counter--;
					}
				}
				build(current_node->child1, vec, pos + 2, end - 1);
			}
			else
			{
				build(current_node->child1, vec, begin + 1, end);
			}
		}
		else
		{
			current_node->type = vec[pos].type;
			build(current_node->child1, vec, begin, pos);
			build(current_node->child2, vec, pos + 1, end);
		}
	}

	else
	{
		if (vec[begin] == LEFT_BRACKET && vec[end - 1] == RIGHT_BRACKET)
		{
			begin++;
		}
		current_node->type = NUMBER;
		current_node->data = vec[begin].data;
	}
}


inline void print_prefix(std::string prefix)
{
	if (!prefix.empty())
	{
		for (size_t i = 0; i < prefix.length(); ++i)
		{
			if (prefix[i] == PREFIX_BRANCH_START[0] 
				|| prefix[i] == PREFIX_BRANCH_MID[0] 
				|| prefix[i] == PREFIX_BRANCH_FILL[0])
			{
				prefix[i] = PREFIX_BRANCH_FILL[0];
			}
			else
			{
				prefix[i] = ' ';
			}
		}
	}
	std::cout << prefix.substr(0, prefix.length() - 1);
}

template<typename data_type>
void print_structure(AbstractSyntaxTreeNode<data_type>* current_node, std::string prefix)
{
	std::string temp_prefix;
	switch (current_node->degree())
	{
	case 0:
		std::cout << PREFIX_LEAF << "(" << current_node->transfer_to_string() << ")" << std::endl;
		return;
	case 1:
		std::cout << PREFIX_LEAF << current_node->transfer_to_string() << PREFIX_LEAF;

		prefix += (PREFIX_LEAF);
		prefix += current_node->transfer_to_string() + PREFIX_LEAF;

		print_structure(current_node->child1, prefix);
		return;
	case 2:
		std::cout << PREFIX_LEAF << current_node->transfer_to_string() << PREFIX_BRANCH_START;

		temp_prefix = prefix + PREFIX_LEAF + current_node->transfer_to_string() + PREFIX_BRANCH_START;

		print_structure(current_node->child1, temp_prefix);

		print_prefix(temp_prefix);

		std::cout << PREFIX_BRANCH_END;
		print_structure(current_node->child2, prefix + blank_str(5));
		return;
	case 3:
		std::cout << PREFIX_LEAF << current_node->transfer_to_string() << PREFIX_BRANCH_START;

		temp_prefix = prefix + PREFIX_LEAF + current_node->transfer_to_string() + PREFIX_BRANCH_START;
				
		print_structure(current_node->child1, temp_prefix);

		print_prefix(temp_prefix);
		std::cout << PREFIX_BRANCH_MID;

		print_structure(current_node->child2, temp_prefix);

		print_prefix(temp_prefix);
		std::cout << PREFIX_BRANCH_END;

		print_structure(current_node->child3, prefix + blank_str(5));
		return;
	default:
		std::cerr << "what the fuck ?!!" << std::endl;
	}
}

template<typename data_type>
data_type get_value(const std::string& str)
{
	std::string temp_str = std::string("(") + str + ")";
	auto temp = convert_to_syntax_element_sequence<data_type>(temp_str);
	AbstractSyntaxTreeNode<data_type>* temp2;
	build<data_type>(temp2, temp, 0, temp.size());
	return temp2->get_value();
}

template<typename data_type>
void print_structure(const std::string& str)
{
	std::string temp_str = std::string("(") + str + ")";
	auto temp = convert_to_syntax_element_sequence<data_type>(temp_str);
	AbstractSyntaxTreeNode<data_type>* temp2;
	build<data_type>(temp2, temp, 0, temp.size());
	print_structure(temp2, "");
}
