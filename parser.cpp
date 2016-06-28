#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "parser.h"

using namespace std;

LexicalAnalyzer lexer;
scope *head = NULL;
scope *current = NULL;
string varlist[50];
string results[50];
bool status = true;
int counter = 0;
int resultcounter = 49;
void stmt_parse();

void clean()
{
	for (int i = 0; i < 50; i++)
	{
		varlist[i] = "";
	}
}
void var_list(int x)
{
	//cout << "ENTERING VAR LIST" << endl;
	Token token1;
	Token token2;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	if (token2.token_type == COMMA)
	{
		varlist[x] = token1.lexeme;
		x++;
		var_list(x);
	}
	else if (token2.token_type == SEMICOLON)
	{
		varlist[x] = token1.lexeme;
		counter = x;
	}
	else
	{
		status = false;
	}
	//cout << "EXITING VAR LIST" << endl;
}
void private_parse()
{
	//cout << "ENTERING PRIVATE" << endl;
	Token token1;
	Token token2;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	if (token1.token_type == PRIVATE && token2.token_type == COLON)
	{
		var_list(0);
	}
	else if (token1.token_type == ID && token2.token_type == EQUAL)
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}
	else if (token1.token_type == ID && token2.token_type == LBRACE)
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}
	else
	{
		status = false;
	}
	//cout << "EXITING PRIVATE" << endl;
}
void public_parse()
{
	//cout << "ENTERING PUBLIC" << endl;
	Token token1;
	Token token2;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	if (token1.token_type == PUBLIC && token2.token_type == COLON)
	{
		var_list(0);
	}
	else if (token1.token_type == PRIVATE && token2.token_type == COLON)
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}
	else if (token1.token_type == ID && token2.token_type == EQUAL)
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}
	else if (token1.token_type == ID && token2.token_type == LBRACE)
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}
	else
	{
		status = false;
	}
	//cout << "EXITING PUBLIC" << endl;
}
void stmt_list_parse()
{
	//cout << "ENTERING STMT LIST" << endl;
	stmt_parse();
	//cout << "YOU ARE RIGHT HERE" << endl;
	Token token1;
	Token token2;
	token1 = lexer.GetToken();
	if (token1.token_type == RBRACE)
	{
		//cout << "THE LAST RBRACE IN A SCOPE" << endl;
		//token1.Print();
		lexer.UngetToken(token1);
	}
	//token2 = lexer.GetToken();
	else if (token1.token_type == ID)
	{
		token2 = lexer.GetToken();
		if (token2.token_type == EQUAL || token2.token_type == LBRACE)
		{
			lexer.UngetToken(token2);
			lexer.UngetToken(token1);
			stmt_list_parse();
		}
		else
		{
			status = false;
		}
	}
	else
	{
		status = false;
	}
	//cout << "EXITING STMT LIST" << endl;
}
void scope_parse()
{
	//cout << "ENTERING SCOPE" << endl;
	Token token1;
	Token token2;
	Token token3;
	string input;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	if (token1.token_type == ID && token2.token_type == LBRACE)
	{
		input = token1.lexeme + ".";
		scope *newScope = new scope(input);
		clean();
		public_parse();
		for (int i = 0; i <= counter; i++)
		{
			newScope->publicArray[i] = varlist[i];
		}
		clean();
		private_parse();
		for (int i = 0; i <= counter; i++)
		{
			newScope->privateArray[i] = varlist[i];
		}
		newScope->parent = current;
		current = newScope;
		stmt_list_parse();
		//cout << "STMT LIST JUST ENDED" << endl;
		token3 = lexer.GetToken();
		if (token3.token_type == RBRACE)
		{
			//cout << "FOUND THE END OF THE SCOPE WITH RBRACE" << endl;
			current = current->parent;
		}
		if (token3.token_type != RBRACE)
		{
			status = false;
		}
	}
	//cout << "STMT LIST JUST ENDED WITH BRACKET" << endl;
	else
	{
		status = false;
		//cout << "Syntax Error" << endl;
	}
	//cout << "EXITING SCOPE" << endl;
}
void stmt_parse()
{
	//cout << "ENTERING STMT" << endl;
	Token token1;
	Token token2;
	Token token3;
	Token token4;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	token3 = lexer.GetToken();
	token4 = lexer.GetToken();
	string match1 = "";
	string match2 = "";
	bool left = false;
	bool right = false;
	if (token1.token_type == ID && token2.token_type == LBRACE)
	{
		lexer.UngetToken(token4);
		lexer.UngetToken(token3);
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
		scope_parse();
	}
	else if (token1.token_type == ID && token2.token_type == EQUAL && token3.token_type == ID && token4.token_type == SEMICOLON)
	{
		scope *tmp = current;
		for (int i = 0; i < 20; i++)
		{
			if (token1.lexeme == tmp->publicArray[i])
			{
				left = true;
				match1 = tmp->scopeID + token1.lexeme;
			}
			if (token1.lexeme == tmp->privateArray[i])
			{
				left = true;
				match1 = tmp->scopeID + token1.lexeme;
			}
			if (token3.lexeme == tmp->publicArray[i])
			{
				right = true;
				match2 = tmp->scopeID + token3.lexeme;
			}
			if (token3.lexeme == tmp->privateArray[i])
			{
				right = true;
				match2 = tmp->scopeID + token3.lexeme;
			}
		}
		if (!left || !right)
		{
			while (tmp->parent != NULL)
			{
				tmp = tmp->parent;
				for (int i = 0; i < 20; i++)
				{
					if (token1.lexeme == tmp->publicArray[i] && !left)
					{
						left = true;
						match1 = tmp->scopeID + token1.lexeme;
					}
					if (token3.lexeme == tmp->publicArray[i] && !right)
					{
						right = true;
						match2 = tmp->scopeID + token3.lexeme;
					}
				}
			}
		}
		if (!left || !right)
		{
			if (!left)
			{
				match1 = "?." + token1.lexeme;
			}
			if (!right)
			{
				match2 = "?." + token3.lexeme;
			}
		}
		results[resultcounter] = match1 + "=" + match2;
		resultcounter = resultcounter - 1;
		//cout << "\nSOLUTION: " << match1 + "=" + match2 << "\n" <<endl;
	}
	else
	{
		status = false;
	}
	//cout << "EXITING STMT" << endl;
}
void global_parse()
{
	//cout << "ENTERING GlOBAL" << endl;
	Token token1;
	Token token2;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();
	if (token1.token_type == ID && (token2.token_type == COMMA || token2.token_type == SEMICOLON))
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
		clean();
		var_list(0);
		scope *newScope = new scope("::");
		for (int i = 0; i <= counter; i++)
		{
			newScope->publicArray[i] = varlist[i];
		}
		head = newScope;
		head->parent = NULL;
		current = head;
	}
	if (token1.token_type == ID && token2.token_type == LBRACE)
	{
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}
	if (!(token2.token_type == COMMA || token2.token_type == SEMICOLON || token2.token_type == LBRACE))
	{
		status = false;
	}
	//cout << "EXITING GLOBAL" << endl;
}
void program_parse()
{
	//cout << "ENTERING PROGRAM" << endl;
	global_parse();
	scope_parse();
	//cout << "EXITING PROGRAM" << endl;
}

int main()
{
	LexicalAnalyzer lexer;
	Token token;
	program_parse();
	if (status)
	{
		for (int i = 49; i > resultcounter; i--)
		{
			cout << results[i] << endl;
		}
	}
	if (!status)
	{
		cout << "Syntax Error" << endl;
	}
	int x;
	cin >> x;
	//token = lexer.GetToken();
	//token.Print();
	/*
	while (token.token_type != END_OF_FILE)
	{
	token = lexer.GetToken();
	//token.Print();
	}
	*/
}
