#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <fstream>
#include "scanner.h"

stack<int> s;
extern queue< pair<string, string> > q;
int ParsingTable[100][100];
char gram[100][100];
int stair = 0;


int tok_to_num(string token, string attribute)
{
	if (!attribute.compare("WORD"))return 0;
	if (!token.compare("=")) {	return 1;}
	if (!token.compare("++"))return 1;
	if (!token.compare("+"))return 1;
	if (!token.compare("**"))return 2;
	if (!token.compare("*"))return 2;
	if (!token.compare("(("))return 3;
	if (!token.compare("("))return 3;
	if (!token.compare("))"))return 4;
	if (!token.compare(")"))return 4;
	if (!token.compare("&"))return 5; //���� 1 = 5, ���� 2 = 3


}

typedef struct Handle { 
	unsigned int reduced;
	unsigned int length;
}Handle;
//
Handle handleTable[7] = { // ���� 1 �����
	{ 20001, 2  },

	{ 20006, 3  },
	{ 20006, 1  },
	{ 20007, 3  },
	{ 20007, 1  },
	{ 20008, 3  },
	{ 20008, 1  },
};
//Handle handleTable[7] = {  // ���� 2 �����
//	{ 20004, 1  },
//
//	{ 20004, 3  },
//	{ 20004, 1  },
//	{ 20006, 1  },
//	{ 20005, 2  },	
//	{ 20005, 1  },
//
//};

//Handle handleTable[7] = {  // ���� 2 - CLR, LALR �����
//	{ 20004, 1  },
//
//	{ 20004, 3  },
//	{ 20004, 1  },
//	{ 20005, 2  },
//	{ 20005, 1  },
//	{ 20006, 1  },
//
//};




void Read_Table(const char* table) {
	ifstream ifile;
	int row = -1;
	char line[200]; // �� �پ� �о �ӽ÷� ������ ����
	ifile.open(table);  // ���� ����

	if (ifile.is_open()) {
		while (ifile.getline(line, sizeof(line))) // �� �پ� �о� ó���� �����Ѵ�.
		{
			if (row != -1)
				cout << "[" << row << "] ";

			cout << line << endl; // ���� ���

			if (row == -1) {
				row++;
				continue;
			}

			char* ptr = strtok(line, " "); // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ


			int tem = 0;
			while (ptr != NULL)   // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
			{
				ParsingTable[row][tem++] = atoi(ptr);
				ptr = strtok(NULL, " ");  // ���� ���ڿ��� �߶� �����͸� ��ȯ
			}
			row++;
		}
	}
	ifile.close(); // ���� �ݱ�
}

int terminal(string str) {
	if (!str.compare("+")) return 1;
	if (!str.compare("*")) return 1;
	if (!str.compare("(")) return 1;
	if (!str.compare(")")) return 1;
	if (!str.compare("id")) return 1;
	if (!str.compare("=")) return 1;
	return 0;
}

char non_terminal[100] = { 0, 1, 2, 3 };
int N_NONTERMINAL;
char terminal_[100] = { 60, 61, 62, 63, 64, 65, 66, 67 };
int N_TERMINAL;
#define N_RHS 100
#define MAX_CFG_STRING 100

char cfg[100][100] = {};

map<string, int> tb;
map<int, string> tb2;

void Read_Grammer(char* grammer) {
	ifstream ifile;
	int row = -1;
	char line[800]; // �� �پ� �о �ӽ÷� ������ ����
	ifile.open(grammer);  // ���� ����

	char temp[800];
	
	cout << endl<<endl<< "���͹̳� //// �͹̳�" << endl;
	if (ifile.is_open()) {
		while (ifile.getline(line, sizeof(line))) // �� �پ� �о� ó���� �����Ѵ�.
		{
			if (row != -1)
				cout << "[" << row << "] ";

			cout << line << endl; // ���� ���

			if (row == -1) {
				row++;

				char* ptr = strtok(line, " "); // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ
				int temp4 = 0;
				while (ptr != NULL)   // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
				{
					//cout << ptr;

					tb.insert(make_pair(ptr, temp4));
					tb2.insert(make_pair(temp4, ptr));

					ptr = strtok(NULL, " ");  // ���� ���ڿ��� �߶� �����͸� ��ȯ

					if (ptr != NULL)
						if (strcmp(ptr, "////") == 0) {
							N_NONTERMINAL = tb.size();
							ptr = strtok(NULL, " ");
							temp4 = 60;
							continue;
						}


					temp4++;
				}
				//cout << tb.size()- N_NONTERMINAL+1 << endl;
				N_TERMINAL = tb.size() - N_NONTERMINAL + 1;
				continue;
			}

			char* ptr = strtok(line, " "); // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ

			int tem = 0;
			while (ptr != NULL)   // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
			{
				if (tem != 0)
					strcat(gram[row], " ");

				cfg[row][tem]= tb[ptr];

				strcat(gram[row], ptr);

				ptr = strtok(NULL, " ");  // ���� ���ڿ��� �߶� �����͸� ��ȯ

				if (ptr != NULL)
					if (strcmp(ptr, "==>") == 0)
						ptr = strtok(NULL, " ");

				tem++;
			}
			row++;
		}
	}
	ifile.close(); // ���� �ݱ�

	cout << "\n\nFinish Read_Grammer ...\n\n";
}

void Shift() {

	int i = s.top();
	auto temp = q.front();
	q.pop();

	s.push(tok_to_num(temp.first, temp.second));

	s.push(ParsingTable[i][tok_to_num(temp.first, temp.second)] - 100);


	cout << "\n<" << stair++ << ">\nSHIFT "<< s.top() <<"\n";

	
	queue< pair<string, string> > q_tem;
	stack<int> s_tem;
	s_tem = s;
	q_tem = q;

	cout << "stack����: ";
	for (int i = 0; i < s.size(); i++) {
		cout << s_tem.top() << " ";
		s_tem.pop();
	}

	cout << "\n�Է� Token: ";
	for (int i = 0; i < q.size(); i++) {
		cout << q_tem.front().first << " ";
		q_tem.pop();
	}



}


void Reduce() {
	int i, j, go_to, cnt;

	int temp_node = s.top();

	s.pop();

	auto temp = q.front();

	i = ParsingTable[temp_node][tok_to_num(temp.first, temp.second)];

	cnt = handleTable[i - 1000].length * 2 - 1;

	cout << cnt<<endl;
	for (j = 0; j < cnt; j++) {
		s.pop();  // ���⼭ ������
	}

	go_to = ParsingTable[s.top()][handleTable[i - 1000].reduced - 20000];

	cout << "\n<"<< stair++ << ">\nREDUCE "<< i - 1000 << "\n";
	
	queue< pair<string, string> > q_tem;
	stack<int> s_tem;
	s_tem = s;
	q_tem = q;

	cout << "stack����: ";
	for (int i = 0; i < s.size(); i++) {
		cout << s_tem.top() << " ";
		s_tem.pop();
	}

	cout << "\n�Է� Token: ";
	for (int i = 0; i < q.size(); i++) {
		cout << q_tem.front().first << " ";
		q_tem.pop();
	}


	s.push(handleTable[i - 1000].reduced % 20000);
	s.push(go_to);

	cout << "\n\n<" << stair++ << ">\nGOTO " << s.top() << "\n";

	queue< pair<string, string> > q_tem2;
	stack<int> s_tem2;
	s_tem2 = s;
	q_tem2 = q;

	cout << "stack����: ";
	for (int i = 0; i < s.size(); i++) {
		cout << s_tem2.top() << " ";
		s_tem2.pop();
	}

	cout << "\n�Է� Token: ";
	for (int i = 0; i < q.size(); i++) {
		cout << q_tem2.front().first << " ";
		q_tem2.pop();
	}

}

int Act() {
	stack<int> s_tem;
	s_tem = s;

	cout << endl;	

	int i = s.top();
	auto temp = q.front();
	int j = ParsingTable[i][tok_to_num(temp.first, temp.second)];

	if (j == 0) {
		cout << "\n<" << stair++ << ">\nREJECT\n";
		return 2;
	}
	else if (j > 99 && j < 1000) {
		Shift();
		return 1;
	}
	else if (j < 10000) {
		Reduce();
		return 1;
	}
	else if (j == 10000) {
		cout << "\n<" << stair++ << ">\nACCEPT\n";
		return 3;
	}
	else if (j < 0) {
		cout << "\n<" << stair++ << ">\nERROR\n";
		return 0;
	}
}


int record_first[100][100] = { 0 };
int record_follow[100][100] = { 0 };


int get_nonterminal_num(char ch)
{
	int i;
	for (i = 0; i < N_NONTERMINAL; i++) {
		if (ch == non_terminal[i]) return i;
	}
	return -1;
}



int get_terminal_num(char ch)
{
	int i;
	for (i = 0; i < N_TERMINAL; i++) {
		if (ch == terminal_[i]) return i;
	}
	return -1;
}

int is_terminal(char ch)
{
	int t;

	if (ch <= 50) return 0;
	else return 1;
}


void first(char* cur_cfg_RHS, int* cur_record_first) //ù���ڴ� �۽�Ʈ���� ���͹̳� ��ȣ, �ι�°���ڴ� �۽�Ʈ������ �ּ�
{
	int i, record_num;
	char* cur_cfg = NULL;
	char cur_nonterminal = *cur_cfg_RHS;

	if (*cur_cfg_RHS == *(cur_cfg_RHS - 1))return;



	if (cur_cfg_RHS == NULL) return;
	if (is_terminal(*cur_cfg_RHS)) { //terminal
		record_num = get_terminal_num(*cur_cfg_RHS);
		cur_record_first[record_num] = 1; //terminal record.
	}
	else { //nonterminal
		for (i = 0; i < N_RHS; i++) {
			cur_cfg = cfg[i];
			if (cur_nonterminal == cur_cfg[0]) {
				//LHS�� nonterminal�� ���� ���ؾ��� nonterminal�� ������.
				first(&cur_cfg[1], cur_record_first);
			}
		}
	}

	if (cur_record_first[N_TERMINAL - 1]) { //�� �������� epsilon �߰ߵ� ���
		/* FIRST(Y1Y2Y3..Yk) = FIRST(Y1) - {epsilon}) U FIRST(Y2Y3...Yk) */
		if (*(cur_cfg_RHS + 1) != 0) { //�� ������ Yk�� �ƴ϶��. 
			cur_record_first[N_TERMINAL - 1] = 0; //epsilon ����
			first(cur_cfg_RHS + 1, cur_record_first);
		}
	}
}

void get_firsts()
{
	int i, j;
	char cur_nonterminal;
	char* cur_cfg = NULL;

	/* �� nonterminal�鿡 ���Ͽ� first��� */
	for (i = 0; i < N_NONTERMINAL; i++) {
		cur_nonterminal = non_terminal[i]; //S���� ���

		for (j = 0; j < N_RHS; j++) {
			cur_cfg = cfg[j]; // cfg 8���� �����߿� j��°�� ���ٴ°���
			if (cur_nonterminal == cur_cfg[0]) {
				//LHS�� nonterminal�� ���� ���ؾ��� nonterminal�� ������.
				first(&cur_cfg[1], record_first[i]);  //curcfg1 �� ������ ù��° ��ȣ ����(���͹̳�)  recordfirst�� 1��° �۽�Ʈ�� ���ϴ°��� �� ���͹̳��� ����
			}
		}
	}
}


void put_firsts()
{
	int i, j;

	cout << endl << "--------------------FIRST--------------------" << endl;
	for (i = 0; i < N_NONTERMINAL; i++) {
		cout  << tb2[non_terminal[i]] << " = ";
		

		for (j = 0; j < N_TERMINAL; j++) {
			if (record_first[i][j]) cout << tb2[terminal_[j]]<<" ";//printf("%d ", terminal_[j]);
		}
		cout << endl;
	}
}


void follow(char cur_nonterminal, int* cur_record_follow)
{
	int i, j;
	char* cur_cfg_ch = NULL;
	char cur_cfg_nonterminal;

	
	

	if (get_nonterminal_num(cur_nonterminal) == 0) { //���۱�ȣ�̸�, $���.
		cur_record_follow[N_TERMINAL] = 1;
		
	}
	int k = 0;
	for (i = 0; i < N_RHS; i++) {
		cur_cfg_nonterminal = cfg[i][0];
		cur_cfg_ch = cfg[i] + 1;



		if (*cur_cfg_ch == *(cur_cfg_ch - 1)-1) {
			//cur_record_follow[*(cur_cfg_ch + 1)] = 1;
			return;
		}
		
		while (*cur_cfg_ch) { //RHS char �ϳ��� ���캸�鼭
			// follow���� nonterminal�� ���� nonterminal�� RHS���� �߰��ϸ�.


			if (*cur_cfg_ch == cur_nonterminal) {
				char next_ch = *(cur_cfg_ch + 1);

				//printf("%d ", next_ch);

				if (next_ch == 0) { //next ch�� epsilon.
					if (cur_cfg_nonterminal != cur_nonterminal)
						//���� ����ϰ� �ִ� follow(A), ����� follow(A), ���� nonterminal A��
						//���ؼ��� �ݺ��ǹǷ� ��꿡�� ����. 
						follow(cur_cfg_nonterminal, cur_record_follow);
				}
				else if (is_terminal(next_ch)) { // next ch�� terminal: FIRST(a)=a
					int record_num = get_terminal_num(next_ch);
					cur_record_follow[record_num] = 1; //terminal record
				}
				else { //next ch�� nonterminal. 
				 //(FIRST(A) = ������ ���ߴ� FIRST(A) �̿��Ͽ� ���.
					int nonterminal_num = get_nonterminal_num(next_ch);
					for (j = 0; j < N_TERMINAL; j++) {
						if (record_first[nonterminal_num][j]) cur_record_follow[j] = 1;
					}
				}

				if (cur_record_follow[N_TERMINAL - 1]) { // epsilon�� �����ϸ�
					/* (FIRST(b)-{epsilon}) U FOLLOW(LHS) */
					cur_record_follow[N_TERMINAL - 1] = 0; //epsilon ����
					follow(cur_cfg_nonterminal, cur_record_follow);
				}
				break;
			}
			else cur_cfg_ch++;
		}
	}
}

void get_follows()
{
	int i;
	char cur_nonterminal;

	/* �� nonterminal�鿡 ���Ͽ� follow��� */
	for (i = 0; i < N_NONTERMINAL; i++) {
		cur_nonterminal = non_terminal[i];
		follow(cur_nonterminal, record_follow[i]);
	}
}


void put_follows()
{
	int i, j;

	printf("\n--------------------FOLLOW--------------------\n");
	for (i = 0; i < N_NONTERMINAL; i++) {
		cout  << tb2[non_terminal[i]] << " = ";

		for (j = 0; j < N_TERMINAL; j++) {
			if (record_follow[i][j]) {
				cout << tb2[terminal_[j]]<<" ";
				
			}
		}
		if (record_follow[i][N_TERMINAL]) printf("$");
		printf("\n");
	}
}


int main() {
	int stat;
	char parsing[50] = "sentence.txt"; // �Ľ��� ���� �ؽ�Ʈ����
	char grammer[50] = "Ex1_GRAMMER.txt"; // ���� �ؽ�Ʈ����
	char table[50] = "Ex1_SLR.txt"; // �Ľ����̺� �ؽ�Ʈ����


	for (int i = 0; i < 60; i++) {
		non_terminal[i] = i;
	}
	for (int i = 0; i < 60; i++) {
		terminal_[i] = 60+i;
	}
	Read_Table(table); //�Ľ����̺� �о����
	Read_Grammer(grammer); // ���� �о����
	
	get_firsts();
	put_firsts();
	get_follows();
	put_follows();

	cout << endl << "--------------------�����м� START--------------------";
	
	scanner(parsing);
	q.push({ "&","&" });
	stat = 1;
	s.push(0);

	while (stat == 1)
		stat = Act();
		
	cin >> grammer;
	
	return 0;
}