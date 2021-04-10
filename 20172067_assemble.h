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
#define MAX_BUFR 200	
#define MAX_MNEM 7
#define TABLE_SIZE 20
#define MAX 30

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
	char mnemonic[MAX_MNEM];	// mnemonic
	int form;					// format
	struct opcodeNode* next;	// 다음 node pointer
} OpcodeNode;

/* 2. opcode들을 저장하는 hashTable */
typedef struct opcodeHashTable {
	int cnt;					// table에 저장된 node 수 
	OpcodeNode* node;			// node pointer
} OpcodeTable;


/* Symbol Table 관련 구조체 */
/* 1. Symbol과 주소를 저장하는 node */
typedef struct symbolNode {
	int index;                  // hash table의 index
	int location;               // location counter
	char label[MAX_MNEM];       // label name
	struct symbolNode* next;    // 다음 node pointer
} SymbolNode;

/* 2. Symbol node를 저장하는 Symbol Table */
typedef struct symbolTable {
	int cnt;					//table에 저장된 node 수
	struct symbolNode* node;			// node pointer
} SymbolTable;


/* modification record 관련 구조체 */
/* 1. modification record와 주소를 저장하는 node */
typedef struct modifNode {
	int location;				// location
	struct modifNode* next;		// 다음 node pointer
} ModifNode;

/* 2. modification linked list */
typedef struct modifList {
	ModifNode* head;			// 시작 node pointer
	ModifNode* curr;			// 현재 node pointer
} ModifList;

/* 변수 */
unsigned char memory[ROW_SIZE][COL_SIZE]; 	// 메모리 저장 (65536 * 16)
SymbolTable* symTab = NULL;					// symbol들을 저장할 table


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


/* ============= type와 관련 함수 ============ */

/* -------------------------------------------------------------------------
	함수 : void type(char* command, char* first, HistoryList* historyL);
	목적 : type 파일명이 입력되면 파일을 읽어오는 printType파일과 호출한다.
	그리고 출력에 이상이 없을 경우 히스토리에 저장한다.
	반환값 : 없음
	----------------------------------------------------------------------- */
void type(char* command, char* first, HistoryList* historyL);

/* -------------------------------------------------------------------------
	함수 : int printType(char* filename);
	목적 : 파일을 읽어서 출력한다.
	반환값 : 파일이 없을 경우 -1, 정상일 경우 0을 반환한다.
	----------------------------------------------------------------------- */
int printType(char* filename);


/* ============ symbol 관련 함수 ============ */

/* -------------------------------------------------------------------------
	함수 : symbol(char* command, HistoryList* historyL);
	목적 : 명령어 창에 symbol을 입력할 경우 호출되는 함수이다.
	만약 전에 assemble에 성공한 경우 전역변수인 symbol table에 저장된
	symbol table을 출력해주고, 없으면 없다는 안내를 해준다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void symbol(char* command, HistoryList* historyL);

/* -------------------------------------------------------------------------
	함수 : void initSymbol(SymbolTable** symtab);
	목적 : 새로운 symbol table을 초기화시켜줄 경우 사용된다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void initSymbol(SymbolTable** symtab);

/* -------------------------------------------------------------------------
	함수 : int makeSymbol(char* label, int location, SymbolTable** symtab);
	목적 : symbol table에 label과 location을 새로 저장한다. opcode와 마찬가지로
	hash table을 사용하며 만약 이미 있을 경우 에러안내 후 -1을 반환한다.
	반환값 : symbol table에 저장할 수 있다면 0, 중복일 경우 -1 반환
	----------------------------------------------------------------------- */
int makeSymbol(char* label, int location, SymbolTable** symtab);

/* -------------------------------------------------------------------------
	함수 : int findSymbol(char* label, SymbolTable* symtab);
	목적 : label이 symbol table에 저장되어 있는지 확인한다.
	만약 해당 labe이 있을 경우 location을 반환해준다.
	반환값 : 찾을 수  없을 경우 -1, 있다면 주소를 반환한다.
	----------------------------------------------------------------------- */
int findSymbol(char* label, SymbolTable* symtab);

/* -------------------------------------------------------------------------
	함수 : void printSymbol(int count);
	목적 : symbol table을 출력한다. 임시 symbolnode을 저장해 table에 있는
	모든 노드를 저장하고 알파벳 오름차순으로 정렬하여 출력한다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void printSymbol(int count);

/* -------------------------------------------------------------------------
	함수 : int compare(const void* a, const void* b);
	목적 : qsort에 사용 정렬하기 위해 어떤 정렬방법을 사용할 것인지 사용자가
	정의하여 파라미터로 넣어준다.
	반환값 : -1  , 0 , 1
	----------------------------------------------------------------------- */
int compare(const void* a, const void* b);

/* -------------------------------------------------------------------------
	함수 : void freeSymtab(SymbolTable** symtab);
	목적 : 새로운 symbol table이 만들어질 경우 기존의 있는 symbol table을
	free 시켜준다. 혹은 프로그램을 종료할때 free한다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void freeSymtab(SymbolTable** symtab);


/* ============ assemble 관련 함수 ============ */

/* -------------------------------------------------------------------------
	함수 :void assemble(char* command, char* first, HistoryList* historyL, OpcodeTable* opTable);
	목적 : asm 파일읽고, pass1 함수를 호출하여 중간 과정인 intermediate 파일을 만들고
	symbol table을 만든다. 그 후 intermediate 파일을 사용하여 pass2 함수를 호출하고
	lst 그리고 obj 파일을 만든다. 만약 중간에 어셈블 과정이 실패할 경우,
	만들었던 symbol table, intermediate, obj, lst 파일을 삭제한다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void assemble(char* command, char* first, HistoryList* historyL, OpcodeTable* opTable);

/* -------------------------------------------------------------------------
	함수 : int pass1(char* filename, OpcodeTable* opTable, SymbolTable** symtab, char* progname, int* start, int* length);
	목적 : asm 파일읽고, start 이전에 다른 instruction이 호출되었는지 확인한다.
	그 후에 라인과 주소를 계산하고 intermediate 파일에 저장한다. 만약 중간에 주석
	이 있을 경우 라인을 형성하고 저장한다.
	END를 만날 경우 pass1을 종료한다. 주소를 계산하는 과정에서 label이 있을 경우
	symbol table을 새로 만들어 저장한다.
	이 symbol table은 pass2까지 성공할 경우 전역변수 symbol table과 교체하고,
	그렇지 않을 경우 삭제한다.
	반환값 : 중간에 실패할 경우 -1, 성공적일 경우 0을 반환한다.
	----------------------------------------------------------------------- */
int pass1(char* filename, OpcodeTable* opTable, SymbolTable** symtab, char* progname, int* start, int* length);

/* -------------------------------------------------------------------------
	함수 : int pass2(char* filename, OpcodeTable* opTable, SymbolTable* symtab, int start, int length, char* progname);
	목적 : intermediate 파일을 읽고 object code를 계산한다. 중간 중간 format 4
	를 만날경우 modificatoin을 위해 modiflist를 저장한다.
	또한 objectcode를 계산할 경우 바로 lst 파일을 작성한다. 그리고 obj 파일의 경우
	파일이 차거나 혹은 RESW, RESB를 만날 경우 한 줄씩 작성하고 END를 만나면
	모든 작얼을 끝낸다.
	반환값 : 중간에 실패할 경우 -1, 성공적일 경우 0을 반환한다.
	----------------------------------------------------------------------- */
int pass2(char* filename, OpcodeTable* opTable, SymbolTable* symtab, int start, int length, char* progname);

/* -------------------------------------------------------------------------
	함수 : int chkLocation(char* opcode, char* operand, int prev, OpcodeTable* opTable);
	목적 : WORD,RESW,RESB,BYTE 그리고 나머지 format 형식에 따른 비트수 즉, 주소
	값을 계산한다. 만약 범위를 초과하거나 opcode가 없을 경우 -1을 반환하고 종료한다.
	반환값 : 중간에 실패할 경우 -1, 성공적일 경우 주소에 바이트수롤 더하여 반환.
	----------------------------------------------------------------------- */
int chkLocation(char* opcode, char* operand, int prev, OpcodeTable* opTable);

/* -------------------------------------------------------------------------
	함수 : int chkbyte(char* operand, int* objectcode);
	목적 : BYTE를 만날 경우 해당 주소를 계산한다.
	만약 X라면 1바이트를 반환하고 추후 object code에 해당하는 16진수 값을
	포인터를 사용하여 저장한다.
	만약 C라면 그 문자에 글자 수만큼의 반환하고 추후 object code에 사용하는
	해당하는 아스키값의 16진수를 포인터를 사용하여 저장한다.
	반환값 : 중간에 실패할 경우 -1, 차지하는 바이트 값을 반환.
	----------------------------------------------------------------------- */
int chkbyte(char* operand, int* objectcode);

/* -------------------------------------------------------------------------
	함수 : int findFormat(OpcodeTable* opcodeT, char* opcode);
	목적 : opcode table을 순회하면서 해당하는 opcode를 찾아 format을 반환한다.
	탐색 전 hash 를 찾아서 노드를 탐색하고 만약 opcode 앞에 +표시가 있다면
	format 4 나머지는 opcode table의 format을 반환한다. 찾지 못할 경우 -1 반환.
	반환값 : opcode 찾지 못할 경우  -1, 해당 format 형식 값을 반환.
	----------------------------------------------------------------------- */
int findFormat(OpcodeTable* opcodeT, char* opcode);

/* -------------------------------------------------------------------------
	함수 : int checkConstantRange(char* constant);
	16진수로 표현될 수 있는 값이 저장되었는지 확인하는 함수이다. 만약 0~9 , A~F
	로 표현되었을 경우 1 아니면  0을 반환한다.
	반환값 : 만약 16진수일 경우 1 , 아닐 경우 0을 반환.
	----------------------------------------------------------------------- */
int checkConstantRange(char* constant);

/* -------------------------------------------------------------------------
	함수 : int chkRegister(char* oper);
	레지스터의 number을 반환한다.
	반환값 : A - 0, X - 1, L -2, B - 3, S - 4, T- 5, F - 6를 반환.
	----------------------------------------------------------------------- */
int chkRegister(char* oper);

/* -------------------------------------------------------------------------
	함수 : int findObject(char* opcode, char* oper1, char* oper2, char* loc,
	int baseReg, int format, OpcodeTable* opTable, SymbolTable* symtab);
	pass2에서 호출되는 함수로 실제로 object 코드를 계산한다.
	format 1일 경우 opcode만 반환하고, format 2일 경우 opcode와 레지스터의 넘버를
	계산하여 반환한다. opcode에 +가 붙어있다면 format 4로 계산하고, simple, indirect
	immediate를 확인하여 address를 계산한다.
	format 3의 경우 보통 pc -> base의 순으로 처리한다.
	반환값 : 실패할 경우 -1, 정상일 경우 object code
	----------------------------------------------------------------------- */
int findObject(char* opcode, char* oper1, char* oper2, char* loc, int baseReg, int format, OpcodeTable* opTable, SymbolTable* symtab, int* imm);

/* -------------------------------------------------------------------------
	함수 : void writeModifynode(int location, ModifList** modiflist);
	modification을 위해서 modifilist에 modfication을 위한 주소값을 저장한다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void writeModifynode(int location, ModifList** modiflist);

/* -------------------------------------------------------------------------
	함수 : void fileModifynode(FILE* fwp, ModifList* modiflist);
	pass2 과정에서 END를 만날 경우 모든 modiflist에 저장된 모든 노드들을
	obj 파일에 modification부분에 작성한다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void fileModifynode(FILE* fwp, ModifList* modiflist);

/* -------------------------------------------------------------------------
	함수 : void freeModifynode(ModifList** modiflist);
	pass2 과정 후 obj 파일을 다 작성하면 modiflist를 삭제한다.
	반환값 : 없음.
	----------------------------------------------------------------------- */
void freeModifynode(ModifList** modiflist);

#endif

