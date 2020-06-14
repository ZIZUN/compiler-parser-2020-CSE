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
	if (!token.compare("&"))return 5; //예제 1 = 5, 예제 2 = 3


}

typedef struct Handle { 
	unsigned int reduced;
	unsigned int length;
}Handle;
//
Handle handleTable[7] = { // 예제 1 실행시
	{ 20001, 2  },

	{ 20006, 3  },
	{ 20006, 1  },
	{ 20007, 3  },
	{ 20007, 1  },
	{ 20008, 3  },
	{ 20008, 1  },
};
//Handle handleTable[7] = {  // 예제 2 실행시
//	{ 20004, 1  },
//
//	{ 20004, 3  },
//	{ 20004, 1  },
//	{ 20006, 1  },
//	{ 20005, 2  },	
//	{ 20005, 1  },
//
//};

//Handle handleTable[7] = {  // 예제 2 - CLR, LALR 실행시
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
	char line[200]; // 한 줄씩 읽어서 임시로 저장할 공간
	ifile.open(table);  // 파일 열기

	if (ifile.is_open()) {
		while (ifile.getline(line, sizeof(line))) // 한 줄씩 읽어 처리를 시작한다.
		{
			if (row != -1)
				cout << "[" << row << "] ";

			cout << line << endl; // 내용 출력

			if (row == -1) {
				row++;
				continue;
			}

			char* ptr = strtok(line, " "); // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환


			int tem = 0;
			while (ptr != NULL)   // 자른 문자열이 나오지 않을 때까지 반복
			{
				ParsingTable[row][tem++] = atoi(ptr);
				ptr = strtok(NULL, " ");  // 다음 문자열을 잘라서 포인터를 반환
			}
			row++;
		}
	}
	ifile.close(); // 파일 닫기
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
	char line[800]; // 한 줄씩 읽어서 임시로 저장할 공간
	ifile.open(grammer);  // 파일 열기

	char temp[800];
	
	cout << endl<<endl<< "논터미널 //// 터미널" << endl;
	if (ifile.is_open()) {
		while (ifile.getline(line, sizeof(line))) // 한 줄씩 읽어 처리를 시작한다.
		{
			if (row != -1)
				cout << "[" << row << "] ";

			cout << line << endl; // 내용 출력

			if (row == -1) {
				row++;

				char* ptr = strtok(line, " "); // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
				int temp4 = 0;
				while (ptr != NULL)   // 자른 문자열이 나오지 않을 때까지 반복
				{
					//cout << ptr;

					tb.insert(make_pair(ptr, temp4));
					tb2.insert(make_pair(temp4, ptr));

					ptr = strtok(NULL, " ");  // 다음 문자열을 잘라서 포인터를 반환

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

			char* ptr = strtok(line, " "); // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환

			int tem = 0;
			while (ptr != NULL)   // 자른 문자열이 나오지 않을 때까지 반복
			{
				if (tem != 0)
					strcat(gram[row], " ");

				cfg[row][tem]= tb[ptr];

				strcat(gram[row], ptr);

				ptr = strtok(NULL, " ");  // 다음 문자열을 잘라서 포인터를 반환

				if (ptr != NULL)
					if (strcmp(ptr, "==>") == 0)
						ptr = strtok(NULL, " ");

				tem++;
			}
			row++;
		}
	}
	ifile.close(); // 파일 닫기

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

	cout << "stack정보: ";
	for (int i = 0; i < s.size(); i++) {
		cout << s_tem.top() << " ";
		s_tem.pop();
	}

	cout << "\n입력 Token: ";
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
		s.pop();  // 여기서 오류남
	}

	go_to = ParsingTable[s.top()][handleTable[i - 1000].reduced - 20000];

	cout << "\n<"<< stair++ << ">\nREDUCE "<< i - 1000 << "\n";
	
	queue< pair<string, string> > q_tem;
	stack<int> s_tem;
	s_tem = s;
	q_tem = q;

	cout << "stack정보: ";
	for (int i = 0; i < s.size(); i++) {
		cout << s_tem.top() << " ";
		s_tem.pop();
	}

	cout << "\n입력 Token: ";
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

	cout << "stack정보: ";
	for (int i = 0; i < s.size(); i++) {
		cout << s_tem2.top() << " ";
		s_tem2.pop();
	}

	cout << "\n입력 Token: ";
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


void first(char* cur_cfg_RHS, int* cur_record_first) //첫인자는 퍼스트구할 논터미널 기호, 두번째인자는 퍼스트저장할 주소
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
				//LHS의 nonterminal과 현재 구해야할 nonterminal이 같으면.
				first(&cur_cfg[1], cur_record_first);
			}
		}
	}

	if (cur_record_first[N_TERMINAL - 1]) { //위 과정에서 epsilon 발견된 경우
		/* FIRST(Y1Y2Y3..Yk) = FIRST(Y1) - {epsilon}) U FIRST(Y2Y3...Yk) */
		if (*(cur_cfg_RHS + 1) != 0) { //맨 마지막 Yk가 아니라면. 
			cur_record_first[N_TERMINAL - 1] = 0; //epsilon 제거
			first(cur_cfg_RHS + 1, cur_record_first);
		}
	}
}

void get_firsts()
{
	int i, j;
	char cur_nonterminal;
	char* cur_cfg = NULL;

	/* 각 nonterminal들에 대하여 first계산 */
	for (i = 0; i < N_NONTERMINAL; i++) {
		cur_nonterminal = non_terminal[i]; //S부터 계산

		for (j = 0; j < N_RHS; j++) {
			cur_cfg = cfg[j]; // cfg 8개의 문법중에 j번째꺼 쓴다는거임
			if (cur_nonterminal == cur_cfg[0]) {
				//LHS의 nonterminal과 현재 구해야할 nonterminal이 같으면.
				first(&cur_cfg[1], record_first[i]);  //curcfg1 은 문법중 첫번째 기호 말함(논터미널)  recordfirst는 1번째 퍼스트를 구하는거임 그 논터미널의 ㅇㅇ
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

	
	

	if (get_nonterminal_num(cur_nonterminal) == 0) { //시작기호이면, $기록.
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
		
		while (*cur_cfg_ch) { //RHS char 하나씩 살펴보면서
			// follow구할 nonterminal과 같은 nonterminal을 RHS에서 발견하면.


			if (*cur_cfg_ch == cur_nonterminal) {
				char next_ch = *(cur_cfg_ch + 1);

				//printf("%d ", next_ch);

				if (next_ch == 0) { //next ch가 epsilon.
					if (cur_cfg_nonterminal != cur_nonterminal)
						//현재 계산하고 있는 follow(A), 계산할 follow(A), 같은 nonterminal A에
						//대해서는 반복되므로 계산에서 제외. 
						follow(cur_cfg_nonterminal, cur_record_follow);
				}
				else if (is_terminal(next_ch)) { // next ch가 terminal: FIRST(a)=a
					int record_num = get_terminal_num(next_ch);
					cur_record_follow[record_num] = 1; //terminal record
				}
				else { //next ch가 nonterminal. 
				 //(FIRST(A) = 이전의 구했던 FIRST(A) 이용하여 기록.
					int nonterminal_num = get_nonterminal_num(next_ch);
					for (j = 0; j < N_TERMINAL; j++) {
						if (record_first[nonterminal_num][j]) cur_record_follow[j] = 1;
					}
				}

				if (cur_record_follow[N_TERMINAL - 1]) { // epsilon이 존재하면
					/* (FIRST(b)-{epsilon}) U FOLLOW(LHS) */
					cur_record_follow[N_TERMINAL - 1] = 0; //epsilon 제거
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

	/* 각 nonterminal들에 대하여 follow계산 */
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
	char parsing[50] = "sentence.txt"; // 파싱할 예제 텍스트파일
	char grammer[50] = "Ex1_GRAMMER.txt"; // 문법 텍스트파일
	char table[50] = "Ex1_SLR.txt"; // 파싱테이블 텍스트파일


	for (int i = 0; i < 60; i++) {
		non_terminal[i] = i;
	}
	for (int i = 0; i < 60; i++) {
		terminal_[i] = 60+i;
	}
	Read_Table(table); //파싱테이블 읽어오기
	Read_Grammer(grammer); // 문법 읽어오기
	
	get_firsts();
	put_firsts();
	get_follows();
	put_follows();

	cout << endl << "--------------------구문분석 START--------------------";
	
	scanner(parsing);
	q.push({ "&","&" });
	stat = 1;
	s.push(0);

	while (stat == 1)
		stat = Act();
		
	cin >> grammer;
	
	return 0;
}