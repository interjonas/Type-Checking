#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "parser.h"

using namespace std;

LexicalAnalyzer lexer;
bool status = true;
int counter = 0;
int symboltablecount = 0;
int newTypeCount = 4;
symbolTable table[30];
var_decl globalList[5];
void body_parse();

void case_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == CASE)
	{
		token1 = lexer.GetToken();
		if (token1.token_type == NUM)
		{
			token1 = lexer.GetToken();
			if (token1.token_type == COLON)
			{
				body_parse();
			}
			else
			{

			}

		}
		else
		{

		}

	}
	else
	{

	}
}
void case_list_parse()
{
	case_parse();
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == CASE)
	{
		lexer.UngetToken(token1);
		case_parse();
	}
	else
	{

	}
}
void unary_parse()
{

}
void binary_parse()
{

}
int primary_parse()
{
	int value;
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == NUM)
	{
		value = 1;
	}
	else if (token1.token_type == REALNUM)
	{
		value = 2;
	}
	else if (token1.token_type == TRUE || token1.token_type == FALSE)
	{
		value = 3;
	}
	else if (token1.token_type == ID)
	{
		for (int i = 0; i < symboltablecount; i++)
		{
			if (token1.lexeme == table[i].symbol)
			{
				return table[i].value;
			}
		}
		table[symboltablecount].symbol = token1.lexeme;
		table[symboltablecount].value = newTypeCount;
		newTypeCount++;
		symboltablecount++;
		return 	table[symboltablecount - 1].value;
	}
	return value;
}
int unify(int left, int right)
{
	if (left == right)
	{
		return right;
	}
	else if (left != right && left <= 3 && right <= 3)
	{
		return -1;
	}
	else if (left > 3 && right <= 3)
	{
		for (int i = 0; i < symboltablecount; i++)
		{
			if (table[i].value == left)
			{
				table[i].value = right;
			}
		}
		return right;
	}
	else if (right > 3 && left <= 3)
	{
		for (int i = 0; i < symboltablecount; i++)
		{
			if (table[i].value == right)
			{
				table[i].value = left;
			}
		}
		return left;
	}
	if (right>3 && left > 3)
	{
		for (int i = 0; i < symboltablecount; i++)
		{
			if (table[i].value == left)
			{
				table[i].value = right;
			}
		}
		return right;
	}
	return 0;
}
int expression_parse()
{
	int value = 0;
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == ID || token1.token_type == NUM || token1.token_type == REALNUM || token1.token_type == TRUE || token1.token_type == FALSE)
	{
		lexer.UngetToken(token1);
		return primary_parse();
	}
	else if (token1.token_type == PLUS || token1.token_type == MINUS || token1.token_type == MULT || token1.token_type == DIV || token1.token_type == GREATER || token1.token_type == LESS || token1.token_type == GTEQ || token1.token_type == LTEQ || token1.token_type == EQUAL || token1.token_type == NOTEQUAL)
	{
		Token binary_token = token1;
		value = unify(expression_parse(), expression_parse());
		if (binary_token.token_type == PLUS || binary_token.token_type == MINUS || binary_token.token_type == MULT || binary_token.token_type == DIV)
		{
			if (value == -1)
			{
				cout << "TYPE MISMATCH " << token1.line_no << " C2" << endl;
				status = false;
				return -1;
			}
			else
			{
				return value;
			}
		}
		else if (binary_token.token_type == GREATER || binary_token.token_type == LESS || binary_token.token_type == GTEQ || binary_token.token_type == LTEQ || binary_token.token_type == EQUAL || binary_token.token_type == NOTEQUAL)
		{
			if (value == -1)
			{
				cout << "TYPE MISMATCH " << token1.line_no << " C2" << endl;
				status = false;
				return -1;
			}
			else
			{
				return 3;
			}
		}
	}
	else if (token1.token_type == NOT)
	{
		value = expression_parse();
		if (value == 3 || value > 3)
		{
			if (value > 3)
			{
				unify(3, value);
			}
			return 3;
		}
		else
		{
			cout << "TYPE MISMATCH " << token1.line_no << " C3" << endl;
			status = false;
			return -1;
		}

	}
	return value;
}
void switch_stmt_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == LPAREN)
	{
		int value = expression_parse();
		if (value == 1 || value>3)
		{
			if (value > 3)
			{
				unify(1, value);
			}
			token1 = lexer.GetToken();
			if (token1.token_type == RPAREN)
			{
				token1 = lexer.GetToken();
				if (token1.token_type == LBRACE)
				{
					case_list_parse();
					token1 = lexer.GetToken();
					if (token1.token_type == RBRACE)
					{

					}
					else
					{

					}

				}
				else
				{

				}
			}
			else
			{

			}
		}
		else
		{
			cout << "TYPE MISMATCH " << token1.line_no << " C5" << endl;
			status = false;
		}
	}
	else
	{

	}
}
void while_stmt_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == LPAREN)
	{
		int val = expression_parse();
		if (val == 3 || val>3)
		{
			if (val > 3)
			{
				unify(3, val);
			}
			token1 = lexer.GetToken();
			if (token1.token_type == RPAREN)
			{
				body_parse();
			}
			else
			{

			}
		}
		else
		{
			cout << "TYPE MISMATCH " << token1.line_no << " C4" << endl;
			status = false;
		}
	}
	else
	{

	}
}
void if_stmt_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == LPAREN)
	{
		int val = expression_parse();
		if (val == 3 || val>3)
		{
			if (val > 3)
			{
				unify(3, val);
			}
			token1 = lexer.GetToken();
			if (token1.token_type == RPAREN)
			{
				body_parse();
			}
			else
			{

			}
		}
		else
		{
			cout << "TYPE MISMATCH " << token1.line_no << " C4" << endl;
			status = false;
		}
	}
	else
	{

	}
}
void assignment_stmt_parse()
{
	Token token1;
	int left;
	int right;
	bool found = false;
	token1 = lexer.GetToken();
	for (int i = 0; i < symboltablecount; i++)
	{
		if (token1.lexeme == table[i].symbol)
		{
			left = table[i].value;
			found = true;
		}
	}
	if (!found)
	{
		table[symboltablecount].symbol = token1.lexeme;
		table[symboltablecount].value = newTypeCount;
		left = newTypeCount;
		newTypeCount++;
		symboltablecount++;
	}
	token1 = lexer.GetToken();
	if (token1.token_type == EQUAL)
	{
		right = expression_parse();
		if (left <= 3 && right <= 3)
		{
			if (left == right)
			{

			}
			else if (left!=right)
			{
				if (right != -1)
				{
					cout << "TYPE MISMATCH " << token1.line_no << " C1" << endl;
					status = false;
				}
			}
		}
		else if (left > 3 || right > 3)
		{
			unify(left, right);
		}
		token1 = lexer.GetToken();
		if (token1.token_type == SEMICOLON)
		{

		}
	}
}
void stmt_parse()
{
	Token token1;
	token1 = lexer.GetToken();

	if (token1.token_type == ID)
	{
		lexer.UngetToken(token1);
		assignment_stmt_parse();
	}
	else if (token1.token_type == IF)
	{
		if_stmt_parse();
	}
	else if (token1.token_type == SWITCH)
	{
		switch_stmt_parse();
	}
	else if (token1.token_type == WHILE)
	{
		while_stmt_parse();
	}
	else
	{

	}
}
void stmt_list_parse()
{
	stmt_parse();
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == RBRACE)
	{
		lexer.UngetToken(token1);
	}
	else if (token1.token_type == ID || token1.token_type == IF || token1.token_type == WHILE || token1.token_type == SWITCH)
	{
		lexer.UngetToken(token1);
		stmt_list_parse();
	}
	else
	{

	}
}
void type_name_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == INT)
	{
		globalList[counter].variable = 1;
	}
	else if (token1.token_type == REAL)
	{
		globalList[counter].variable = 2;
	}
	else if (token1.token_type == BOOL)
	{
		globalList[counter].variable = 3;
	}
	else
	{
		//cout << "TYPE NAME ERROR" << endl;
	}
	//cout << "EXITING TYPE NAME" << endl;
}
void var_list_parse()
{
	Token token1;
	Token token2;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	if (token2.token_type == COMMA)
	{
		globalList[counter].list[globalList[counter].count] = token1.lexeme;
		globalList[counter].count++;
		var_list_parse();
	}
	else if (token2.token_type == COLON)
	{
		globalList[counter].list[globalList[counter].count] = token1.lexeme;
		lexer.UngetToken(token2);
	}
	else
	{

	}
}
void var_decl_parse()
{
	var_list_parse();
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == COLON)
	{
		type_name_parse();
		token1 = lexer.GetToken();
		if (token1.token_type == SEMICOLON)
		{

		}
		else
		{

		}
	}
	else
	{

	}
}
void var_decl_list_parse()
{
	var_decl_parse();
	counter++;
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == ID)
	{
		lexer.UngetToken(token1);
		var_decl_list_parse();
	}
	else if (token1.token_type == LBRACE)
	{
		lexer.UngetToken(token1);
	}
	else
	{

	}
	//cout << "EXITING VARIABLE DECLARATION LIST" << endl;
}
void body_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == LBRACE)
	{
		stmt_list_parse();
		token1 = lexer.GetToken();
		if (token1.token_type == RBRACE)
		{

		}
		else
		{

		}
	}
}
void global_parse()
{
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == ID)
	{
		lexer.UngetToken(token1);
		var_decl_list_parse();
		for (int i = 0; i < counter; i++)
		{
			for (int j = 0; j <= globalList[i].count; j++)
			{
				table[symboltablecount].symbol = globalList[i].list[j];
				table[symboltablecount].value = globalList[i].variable;
				symboltablecount++;
			}
		}
	}
	else if (token1.token_type == LBRACE)
	{
		lexer.UngetToken(token1);
	}
	else
	{
		//cout << "GLOBAL FIRST TOKEN NOT VALID" << endl;
	}
}
void program_parse()
{
	global_parse();
	body_parse();
}


int main()
{
	program_parse();
	if (status)
	{
		int val=50;
		string undefined = "";
		for (int i = 0; i < symboltablecount; i++)
		{
			if (table[i].value <= 3)
			{
				cout << table[i].symbol << ": ";
				if (table[i].value == 1)
				{
					cout << "int #" << endl;
				}
				else if (table[i].value == 2)
				{
					cout << "real #" << endl;
				}
				else if (table[i].value == 3)
				{
					cout << "bool #" << endl;
				}
			}
			else if (table[i].value > 3)
			{
				if (!table[i].marked)
				{
					val = table[i].value;
					for (int j = 0; j < symboltablecount; j++)
					{
						if (val == table[j].value)
						{
							undefined = undefined + table[j].symbol + ", ";
							table[j].marked = true;
						}
					}
					undefined = undefined.substr(0, undefined.size() - 2);
					undefined = undefined + ": ? #";
					cout << undefined << endl;
					undefined = "";
				}
			}
		}
	}

	int x;
	cin >> x;
}


