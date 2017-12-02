#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

enum type_tag {
	IDENTIFIER, QUALIFIER, TYPE
};

struct token {
	char type;
	string str;
};

token current;
stack<token> tokens;
string decl;
string::size_type i = 0;

enum type_tag classify_string();
void gettoken();
void read_to_first_identifer();
void deal_with_declarator();
void deal_with_arrays();
void deal_with_functions_args();
void deal_with_pointers();

int main()
{
	while (getline(cin, decl))
	{
		read_to_first_identifer();
		deal_with_declarator();
		cout << endl;
		i = 0;
	}
	return 0;
}


void gettoken()
{
	char buf;
	current.str.clear();
	if (i == decl.size())
		return;
	/*skip the space character*/
	while (isspace(buf = decl[i++]))
		;
	/*identifer consists of A-z and 0-9*/
	if (isalnum(buf))
	{
		current.str.push_back(buf);
		while (isalnum(buf = decl[i]))
		{
			current.str.push_back(buf);
			i++;
		}
		current.type = classify_string();
		return;
	}
	/*token is a pointer*/
	if (buf == '*')
	{
		current.str = "pointer to";
		current.type = buf;
		return;
	}
	/* token is '(',')','[' or ']'*/
	current.str= buf;
	current.type = buf;
}

enum type_tag classify_string()
{
	if (current.str == "const")
	{
		current.str = "read-only";
		return QUALIFIER;
	}
	if (current.str == "volatile")
		return QUALIFIER;
	if (current.str == "char" ||current.str == "int" || current.str == "long" || current.str == "signed" || current.str == "unsigned" || current.str == "void" 
		|| current.str == "double" || current.str == "float" || current.str == "struct" || current.str == "union" || current.str == "enum")
		return TYPE;
	return IDENTIFIER;
}

void read_to_first_identifer()
{
	gettoken();
	while (current.type != IDENTIFIER)
	{
		tokens.push(current);
		gettoken();
	}
	cout << "Declared " << current.str << " as ";
	gettoken();
}

void deal_with_declarator()
{
	switch(current.type)
	{
	case '(':
		deal_with_functions_args();
		break;
	case '[':
		deal_with_arrays();
		break;
	}

	deal_with_pointers();

	while (!tokens.empty())
	{
		if (tokens.top().type == '(')
		{
			tokens.pop();
			gettoken();
			deal_with_declarator();
		}
		else
		{
			cout << tokens.top().str << " ";
			tokens.pop();
		}
	}
}

void deal_with_pointers()
{
	while (tokens.top().type == '*')
	{
		cout << "pointer to ";
		tokens.pop();
	}
}

void deal_with_arrays()
{
	while (current.type == '[')
	{
		cout << "array ";
		gettoken();
		if (isdigit(current.str[0]))
		{
			cout << current.str << " ";
			gettoken();
		}
		gettoken();
		cout << "of ";
	}
}

void deal_with_functions_args()
{
	cout << "function ";
	gettoken();
	bool has_args = false;
	while (current.type != ')')
	{
		if (!has_args)
		{
			cout << '(';
			has_args = true;
		}
		if (current.type == '*')
			cout << "*";
		else
			cout << current.str;
		gettoken();
	}
	if (has_args)
		cout << ") ";
	gettoken();
	cout << "returning ";
}