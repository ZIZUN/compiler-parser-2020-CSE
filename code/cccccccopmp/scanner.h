//#include<stdio.h>
//#include<string.h>
//#include<stdlib.h>
#define TRUE 1
#define FALSE 0

queue< pair<string, string> > q;
char Next(FILE* fp);
int Word(char c);
int Num(char c);
int Symbol(char c);
int print_reject_output();
int print_accept_output(FILE* infp);
char symbol_type[10];
int check_keyword = FALSE;
int line_count = 1;
char whatis_now;
int word_ing = FALSE;
int num_ing = FALSE;
char check_word[20] = "";
char check_num[20] = "";
char int_keyword[] = "int";
char if_keyword[] = "if";
char then_keyword[] = "then";
char else_keyword[] = "else";
char while_keyword[] = "while";
char return_keyword[] = "return";
char eq_keyword[] = "==";
char buff[1];

int scanner(char* file) {
	int count = 0;
	FILE* input;
	char filename[50] = "";

	strcpy(filename, file);

	if ((input = fopen(filename, "r")) == NULL) {
		perror("input file open error!!\n");
	}

	while (Next(input) != TRUE) {
		if (whatis_now == 10 || whatis_now == 9) {
			line_count++;
			continue;
		}

		if (Word(whatis_now) == TRUE) {
			if (num_ing == TRUE)
				print_reject_output();

			check_word[count++] = whatis_now;
			word_ing = TRUE;
			continue;
		}
		else if (Num(whatis_now) == TRUE) {
			if (word_ing == TRUE)
				print_reject_output();

			check_num[count++] = whatis_now;
			num_ing = TRUE;
			continue;
		}

		else if (Symbol(whatis_now) == TRUE || whatis_now == 32) {
			if (word_ing == TRUE) {
				check_word[count] = '\0';
				print_accept_output(input);
			}
			else if (num_ing == TRUE) {
				check_num[count] = '\0';
				print_accept_output(input);
			}

			else
				print_accept_output(input);

			word_ing = num_ing = FALSE;
			count = 0;
			memset(check_num, 0, sizeof(check_num));
			memset(symbol_type, 0, sizeof(symbol_type));
			memset(check_word, 0, sizeof(check_word));
		}

		else {
			print_reject_output();
			exit(1);
		}
	}

	fclose(input);
	return 0;
}
char Next(FILE * fp) {
	whatis_now = fgetc(fp);
	if (!feof(fp))
		return whatis_now;
	else
		return TRUE;
}

int Word(char c) {
	if (c >= 'a' && c <= 'z')
		return TRUE;
	else if (c >= 'A' && c <= 'Z')
		return TRUE;
	else
		return FALSE;
}

int Num(char c) {
	if (c >= '0' && c <= '9')
		return TRUE;
	else
		return FALSE;
}

int Symbol(char c) {
	switch (c) {
	case '{':
		strcpy(symbol_type, "BLOCK");
		return TRUE;
		break;
	case '}':
		strcpy(symbol_type, "BLOCK");
		return TRUE;
		break;
	case '(':
		strcpy(symbol_type, "PAREN");
		return TRUE;
		break;
	case ')':
		strcpy(symbol_type, "PAREN");
		return TRUE;
		break;
	case '>':
		strcpy(symbol_type, "GREATER_THAN");
		return TRUE;
		break;
	case '=':
		strcpy(symbol_type, "EQUAL");
		return TRUE;
		break;
	case '+':
		strcpy(symbol_type, "PLUS");
		return TRUE;
		break;
	case '*':
		strcpy(symbol_type, "MUL");
		return TRUE;
		break;
	case '-':
		strcpy(symbol_type, "SUB");
		return TRUE;
		break;
	case '/':
		strcpy(symbol_type, "DIV");
		return TRUE;
		break;
	case ';':
		strcpy(symbol_type, "SEMI");
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}


int print_reject_output() {
	printf("Reject!! Line number : %d\n", line_count);
	exit(1);
}

int print_accept_output(FILE * infp) {
	if (word_ing == TRUE) {
		if ((strcmp(check_word, int_keyword)) == 0) {
			q.push({ int_keyword,"keyword" });
		}
		else if ((strcmp(check_word, if_keyword)) == 0) {
			q.push({ if_keyword,"keyword" });
		}
		else if ((strcmp(check_word, then_keyword)) == 0) {
			q.push({ then_keyword,"keyword" });
		}
		else if ((strcmp(check_word, else_keyword)) == 0) {
			q.push({ else_keyword,"keyword" });
		}
		else if ((strcmp(check_word, while_keyword)) == 0) {
			q.push({ while_keyword,"keyword" });
		}
		else if ((strcmp(check_word, return_keyword)) == 0) {
			q.push({ return_keyword,"keyword" });
		}
		else if ((strcmp(check_word, eq_keyword)) == 0) {
			q.push({ eq_keyword,"keyword" });
		}
		else {
			q.push({ check_word,"WORD" });
		}
	}

	else if (num_ing == TRUE) {
		q.push({ check_num,"NUM" });
	}

	if (Symbol(whatis_now) == TRUE) {
		if (whatis_now == '=') {
			Next(infp);
			if (whatis_now == '=') {
				strcpy(symbol_type, "EQ");
				q.push({ "==","keyword" });
			}
			else {
				whatis_now = '=';
				strcpy(symbol_type, "EQUAL");
				q.push({ "=","keyword" });
			}
		}
		if (whatis_now != '=') {
			buff[0] = whatis_now;
			q.push({ buff,"symbol_type" });
		}
	}

	strcpy(symbol_type, "");
	strcpy(check_num, "");
	strcpy(check_word, "");
	num_ing = FALSE;
	word_ing = FALSE;

	if (whatis_now == 32)	return 0;
}