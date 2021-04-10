#ifndef __MAIN_H__
#define __MAIN_H__

/* 포함되는 파일 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

/* 정의되는 상수 */ 
#define TRUE	1
#define FALSE	0
#define ROW_SIZE 65536
#define COL_SIZE 16
#define MAX_STR 100	
#define MAX_MNEM 7
#define TABLE_SIZE 20

/* 변수 */ 
unsigned char memory[ROW_SIZE][COL_SIZE]; 	// 메모리 저장 (65536 * 16)

/* History 관련 구조체 */
/* 1. history node     */
typedef struct historyNode {
	char input[MAX_STR];		// 입력내용저장 
	struct historyNode* next;	// 다음 node pointer
} HistoryNode;

/* 2. history linked list */
typedef struct historyList {
	HistoryNode* head;			// 시작 node pointer
	HistoryNode* curr;			// 현재 node pointer
} HistoryList;


/* Opcode Hash Table 관련 구조체 */
/* 1. opcode를 저장하는 node */
typedef struct opcodeNode {
	int index;					// hash table의 index
	int opcode;					// opcode
	char mnemonic[7];			// mnemonic
	int form;					// format
	struct opcodeNode* next;	// 다음 node pointer
} OpcodeNode;

/* 2. opcode들을 저장하는 hashTable */
typedef struct opcodeHashTable {
	int cnt;					// table에 저장된 node 수 
	OpcodeNode* node;			// node pointer
} OpcodeTable;


/* =============  명령어 정제함수  ============= */ 

/* -----------------------------------------------------------------------
	함수 : refineCommand(char* input, char* command, char* arguments);
   	목적 : 입력된 input을 command 부분과 argument 부분을 나눈다.
	그 외에도 command 부분을 정제처리한다. ('\t', ' ' 와 같은 것을 없앤다)
   	리턴값 : 없음
	---------------------------------------------------------------------*/
void refineCommand(char* input, char* command, char* arguments);

/* ----------------------------------------------------------------------
	함수 : refineArgument(char* arguments, char* first, char* second, char* third);
	목적 : argument 부분들을 1,2,3을 나눠 저장한다.
	그 외에도 '\t', ' ' 을 없애는 정제 작업을 처리한다. 
	또한 제대로 된 수인가 입력되었는지 예를 들면,
	콤마 수와 인수 수를 확인한다. 
	리턴값 : 최종적으로 인수값을 출력한다 .(0~3 범위내) 
	만약 인수가 정상적으로 입력되지 않은경우, (예를 들면 콤마수와 인수가 맞지않을경우)
	는 -1를 출력한다.  
	---------------------------------------------------------------------*/
int refineArgument(char* arguments, char* first, char* second, char* third);


/* ============ shell 관련 함수 ============ */

/* -----------------------------------------------------------------------
	함수 : help(char* input, HistoryList* historyL);
	목적 : 실행 가능한 모든 명령어를 출력한다.
	리턴값 : 없음
   ----------------------------------------------------------------------	*/
void help(char* input, HistoryList* historyL);

/* ----------------------------------------------------------------------
	함수 : dir(char* input, HistoryList* historyL);
	목적 : 현재 디렉터리에 있는 파일들을 출력한다.
	실팽 파일에는 '*'을 붙이고, 디렉터리에는 '/'을 표시한다.
	리턴값 : 없음
	---------------------------------------------------------------------  */
void dir(char* input, HistoryList* historyL);

/* ----------------------------------------------------------------------
	함수 : quit(HistoryList* historyL, OpcodeTable** opcodeT);
	목적 : 프로그램을 종료한다.
	이때 historynode와 opcodetable을 free 한다. 
	반환값 : 없음
	--------------------------------------------------------------------- */
void quit(HistoryList* historyL, OpcodeTable** opcodeT);

/* ----------------------------------------------------------------------
	함수 : history(char* input, HistoryList* historyL);
	목적 : writeHistory 함수를 불러오고, printHistory함수를 불러온다. 
	반환값 : 없음
	---------------------------------------------------------------------- */
void history(char* input, HistoryList* historyL);

/* -----------------------------------------------------------------------
	함수 : void writeHistory(char* content, HistoryList** historyL);
	목적 : 정제된 사용한 명령어를 history node로 저장해서
	history list에 저장한다. (linked list로 저장)
	반환값 : 없음
	----------------------------------------------------------------------- */
void writeHistory(char* content, HistoryList** historyL);

/* ------------------------------------------------------------------------
	함수 : void printHistory(HistoryList* historyL);
	목적 : history list에 저장된 head node부터 끝까지 
	history node에 저장된 명령어를 출력한다. 
	반환값 : 없음
	----------------------------------------------------------------------- */
void printHistory(HistoryList* historyL);

/* ------------------------------------------------------------------------
	함수 : void deleteHistory(HistoryList** historyL);
	목적 : history list에 저장된 head node 부분부터 끝까지
	history node를 free한다.
	반환값 : 없음
	---------------------------------------------------------------------- */
void deleteHistory(HistoryList** historyL);


/* ============ memory 관련 함수 ============ */

/* -----------------------------------------------------------------------
	함수 : void reset(unsigned char(*memory)[COL_SIZE]);
	목적 : memory 배열의 모든 행과 열을 0으로 초기화 한다.
	반환값 : 없음. 
	---------------------------------------------------------------------- */ 
void reset(unsigned char(*memory)[COL_SIZE]);

/* -----------------------------------------------------------------------
	함수 : void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address);
    목적 : dump(인수가 없을 경우), address부분부터 기본적으로 10 line을 출력한다.    
		   dump (start) - 인수가 하나인 경우, start의 번지부터 10 line을 출력한다. 
		   dump (start, end  - 인수가 2개인 경우, start부터 end까지 출력한다.
	해당 인수값의 범위를 확인하고 예외처리를 한뒤 정상인 경우 printMemory를 호출하여
	실제적으로 start ~ end까지 출력한다. 
	반환값 : 없음
	----------------------------------------------------------------------- */	
void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address);	  

/* ------------------------------------------------------------------------
	함수 : void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end);
	목적 : 실제적으로 memory에 저장된 값을 출력한다.
	첫번째 부분은 주소를 출력하고 두번째 부분은 메모리 내용을 출력하고,
	세번째 부분은 메모리에 대응하는 ASCII코드를 출력한다.
	반환값 : 없음 
	------------------------------------------------------------------------ */
void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end);

/* ------------------------------------------------------------------------
	함수 : char asciiCode(int hex)
	목적 : memory 내용 중 세번째 메모리에 대응하는 ASCII 로 변환하는 작업을
	수행한다. 
	만약 16진수 20~7E범위를 초과할 경우는 '.'으로 출력한다.
	반환값 : 문자(정수에 대응하는 아스키코드로 반환된 문자)
	----------------------------------------------------------------------- */ 
char asciiCode(int hex);

/* ------------------------------------------------------------------------
	함수 : void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);
	목적 : 첫번째 인수(주소)에 두번째 인수(value)로 해당 메모리 값을 수정하는 작업을
	수행한다. 만약 범위를 초과할 경우 예외처리를 한다.
	반환 값 : 없음
	----------------------------------------------------------------------- */
void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);

/* -------------------------------------------------------------------------
	함수 : void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);
	목적 : fill 수행하기 전 메모리가 범위를 확인하고 예외를 처리한다.
	문제가 없다면 fill memory를 호출하여 메모리 값을 바꾼다. 
	반환값 : 없음.
	------------------------------------------------------------------------- */ 	
void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);

/* -------------------------------------------------------------------------
	함수 : void fillMemory(unsigned char(*memory)[COL_SIZE], int start, int end, int value);
	목적 : 번째 인수(시작)과 두번째 인수(끝) 사이를 세번째 인수(값)으로 수정
	한다.
	반환값 : 없음
	------------------------------------------------------------------------- */
void fillMemory(unsigned char(*memory)[COL_SIZE], int start, int end, int value);

/* --------------------------------------------------------------------------
	함수 : int checkRange(char* arg);
	목적 : memory 관련 함수 중 dump,deit,fill을 수행할 때 
	직접적으로 입력된 값이 범위내에 있는지 확인
	반환 값 : 정상범위일 경우 1, 아니면 0을 반환한다.
	-------------------------------------------------------------------------- */ 
int checkRange(char* arg);


/* ============ opcode 관련 함수 ============ */

/* -------------------------------------------------------------------------
	함수 : void readOp(OpcodeTable** opTable);
	목적 : 프로그램이 실행되면 Opcode.text를 읽어서 메모리에 입력하는 작업을
	수행한다. 실제 입력된 mnemonic 의 문자열을 다 더한뒤 
	20으로 나눈 나머지를 통해서 index를 결정하고 opcode hash table 내에 
	알맞은 index에 optable node(linked list)의 형태로 저장한다.
	반환값 : 없음
	----------------------------------------------------------------------- */ 
void readOp(OpcodeTable** opTable);

/* ------------------------------------------------------------------------
	함수 : void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT);
	목적 : Opcode mnemonic을 입력하면 findOpcode 함수를 호출하고
	결과에 맞게 출력해주는 함수이다.
	반환값 : 없음 
	----------------------------------------------------------------------- */
void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT);

/* -------------------------------------------------------------------------
	함수 : int findOpcode(OpcodeTable* opcodeT, char* first);
	목적 : mnemonic의 index를 찾고 입력된 mnemonic과 동일한 optable node를 찾아
	opcode를 반환한다.
	반환값 : mnemonic의 opcode를 반환, 만약 없을 경우 -1를 출력한다.
	------------------------------------------------------------------------ */ 
int findOpcode(OpcodeTable* opcodeT, char* first);

/* ------------------------------------------------------------------------
	함수 : void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable);
	목적 : opcodetable에 저장된 모든 opcodenode를 출력한다.
	반환값 : 없음
	---------------------------------------------------------------------- */ 
void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable);

/* ------------------------------------------------------------------------
	함수 : void freeOpcode(OpcodeTable** opTable);
	목적 : opcode table에 저장된 opcode node를 free하는 함수이다.
	반환값 : 없음.
	----------------------------------------------------------------------- */ 
void freeOpcode(OpcodeTable** opTable);


#endif
