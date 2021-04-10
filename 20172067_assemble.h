#ifndef __MAIN_H__
#define __MAIN_H__

/* ���ԵǴ� ���� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

/* ���ǵǴ� ��� */
#define TRUE	1
#define FALSE	0
#define ROW_SIZE 65536
#define COL_SIZE 16
#define MAX_STR 100	
#define MAX_BUFR 200	
#define MAX_MNEM 7
#define TABLE_SIZE 20
#define MAX 30

/* History ���� ����ü */
/* 1. history node     */
typedef struct historyNode {
	char input[MAX_STR];		// �Է³������� 
	struct historyNode* next;	// ���� node pointer
} HistoryNode;

/* 2. history linked list */
typedef struct historyList {
	HistoryNode* head;			// ���� node pointer
	HistoryNode* curr;			// ���� node pointer
} HistoryList;


/* Opcode Hash Table ���� ����ü */
/* 1. opcode�� �����ϴ� node */
typedef struct opcodeNode {
	int index;					// hash table�� index
	int opcode;					// opcode
	char mnemonic[MAX_MNEM];	// mnemonic
	int form;					// format
	struct opcodeNode* next;	// ���� node pointer
} OpcodeNode;

/* 2. opcode���� �����ϴ� hashTable */
typedef struct opcodeHashTable {
	int cnt;					// table�� ����� node �� 
	OpcodeNode* node;			// node pointer
} OpcodeTable;


/* Symbol Table ���� ����ü */
/* 1. Symbol�� �ּҸ� �����ϴ� node */
typedef struct symbolNode {
	int index;                  // hash table�� index
	int location;               // location counter
	char label[MAX_MNEM];       // label name
	struct symbolNode* next;    // ���� node pointer
} SymbolNode;

/* 2. Symbol node�� �����ϴ� Symbol Table */
typedef struct symbolTable {
	int cnt;					//table�� ����� node ��
	struct symbolNode* node;			// node pointer
} SymbolTable;


/* modification record ���� ����ü */
/* 1. modification record�� �ּҸ� �����ϴ� node */
typedef struct modifNode {
	int location;				// location
	struct modifNode* next;		// ���� node pointer
} ModifNode;

/* 2. modification linked list */
typedef struct modifList {
	ModifNode* head;			// ���� node pointer
	ModifNode* curr;			// ���� node pointer
} ModifList;

/* ���� */
unsigned char memory[ROW_SIZE][COL_SIZE]; 	// �޸� ���� (65536 * 16)
SymbolTable* symTab = NULL;					// symbol���� ������ table


/* =============  ��ɾ� �����Լ�  ============= */

/* -----------------------------------------------------------------------
	�Լ� : refineCommand(char* input, char* command, char* arguments);
	���� : �Էµ� input�� command �κа� argument �κ��� ������.
	�� �ܿ��� command �κ��� ����ó���Ѵ�. ('\t', ' ' �� ���� ���� ���ش�)
	���ϰ� : ����
	---------------------------------------------------------------------*/
void refineCommand(char* input, char* command, char* arguments);

/* ----------------------------------------------------------------------
	�Լ� : refineArgument(char* arguments, char* first, char* second, char* third);
	���� : argument �κе��� 1,2,3�� ���� �����Ѵ�.
	�� �ܿ��� '\t', ' ' �� ���ִ� ���� �۾��� ó���Ѵ�.
	���� ����� �� ���ΰ� �ԷµǾ����� ���� ���,
	�޸� ���� �μ� ���� Ȯ���Ѵ�.
	���ϰ� : ���������� �μ����� ����Ѵ� .(0~3 ������)
	���� �μ��� ���������� �Էµ��� �������, (���� ��� �޸����� �μ��� �����������)
	�� -1�� ����Ѵ�.
	---------------------------------------------------------------------*/
int refineArgument(char* arguments, char* first, char* second, char* third);


/* ============ shell ���� �Լ� ============ */

/* -----------------------------------------------------------------------
	�Լ� : help(char* input, HistoryList* historyL);
	���� : ���� ������ ��� ��ɾ ����Ѵ�.
	���ϰ� : ����
   ----------------------------------------------------------------------	*/
void help(char* input, HistoryList* historyL);

/* ----------------------------------------------------------------------
	�Լ� : dir(char* input, HistoryList* historyL);
	���� : ���� ���͸��� �ִ� ���ϵ��� ����Ѵ�.
	���� ���Ͽ��� '*'�� ���̰�, ���͸����� '/'�� ǥ���Ѵ�.
	���ϰ� : ����
	---------------------------------------------------------------------  */
void dir(char* input, HistoryList* historyL);

/* ----------------------------------------------------------------------
	�Լ� : quit(HistoryList* historyL, OpcodeTable** opcodeT);
	���� : ���α׷��� �����Ѵ�.
	�̶� historynode�� opcodetable�� free �Ѵ�.
	��ȯ�� : ����
	--------------------------------------------------------------------- */
void quit(HistoryList* historyL, OpcodeTable** opcodeT);

/* ----------------------------------------------------------------------
	�Լ� : history(char* input, HistoryList* historyL);
	���� : writeHistory �Լ��� �ҷ�����, printHistory�Լ��� �ҷ��´�.
	��ȯ�� : ����
	---------------------------------------------------------------------- */
void history(char* input, HistoryList* historyL);

/* -----------------------------------------------------------------------
	�Լ� : void writeHistory(char* content, HistoryList** historyL);
	���� : ������ ����� ��ɾ history node�� �����ؼ�
	history list�� �����Ѵ�. (linked list�� ����)
	��ȯ�� : ����
	----------------------------------------------------------------------- */
void writeHistory(char* content, HistoryList** historyL);

/* ------------------------------------------------------------------------
	�Լ� : void printHistory(HistoryList* historyL);
	���� : history list�� ����� head node���� ������
	history node�� ����� ��ɾ ����Ѵ�.
	��ȯ�� : ����
	----------------------------------------------------------------------- */
void printHistory(HistoryList* historyL);

/* ------------------------------------------------------------------------
	�Լ� : void deleteHistory(HistoryList** historyL);
	���� : history list�� ����� head node �κк��� ������
	history node�� free�Ѵ�.
	��ȯ�� : ����
	---------------------------------------------------------------------- */
void deleteHistory(HistoryList** historyL);


/* ============ memory ���� �Լ� ============ */

/* -----------------------------------------------------------------------
	�Լ� : void reset(unsigned char(*memory)[COL_SIZE]);
	���� : memory �迭�� ��� ��� ���� 0���� �ʱ�ȭ �Ѵ�.
	��ȯ�� : ����.
	---------------------------------------------------------------------- */
void reset(unsigned char(*memory)[COL_SIZE]);

/* -----------------------------------------------------------------------
	�Լ� : void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address);
	���� : dump(�μ��� ���� ���), address�κк��� �⺻������ 10 line�� ����Ѵ�.
		   dump (start) - �μ��� �ϳ��� ���, start�� �������� 10 line�� ����Ѵ�.
		   dump (start, end  - �μ��� 2���� ���, start���� end���� ����Ѵ�.
	�ش� �μ����� ������ Ȯ���ϰ� ����ó���� �ѵ� ������ ��� printMemory�� ȣ���Ͽ�
	���������� start ~ end���� ����Ѵ�.
	��ȯ�� : ����
	----------------------------------------------------------------------- */
void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address);

/* ------------------------------------------------------------------------
	�Լ� : void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end);
	���� : ���������� memory�� ����� ���� ����Ѵ�.
	ù��° �κ��� �ּҸ� ����ϰ� �ι�° �κ��� �޸� ������ ����ϰ�,
	����° �κ��� �޸𸮿� �����ϴ� ASCII�ڵ带 ����Ѵ�.
	��ȯ�� : ����
	------------------------------------------------------------------------ */
void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end);

/* ------------------------------------------------------------------------
	�Լ� : char asciiCode(int hex)
	���� : memory ���� �� ����° �޸𸮿� �����ϴ� ASCII �� ��ȯ�ϴ� �۾���
	�����Ѵ�.
	���� 16���� 20~7E������ �ʰ��� ���� '.'���� ����Ѵ�.
	��ȯ�� : ����(������ �����ϴ� �ƽ�Ű�ڵ�� ��ȯ�� ����)
	----------------------------------------------------------------------- */
char asciiCode(int hex);

/* ------------------------------------------------------------------------
	�Լ� : void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);
	���� : ù��° �μ�(�ּ�)�� �ι�° �μ�(value)�� �ش� �޸� ���� �����ϴ� �۾���
	�����Ѵ�. ���� ������ �ʰ��� ��� ����ó���� �Ѵ�.
	��ȯ �� : ����
	----------------------------------------------------------------------- */
void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);

/* -------------------------------------------------------------------------
	�Լ� : void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);
	���� : fill �����ϱ� �� �޸𸮰� ������ Ȯ���ϰ� ���ܸ� ó���Ѵ�.
	������ ���ٸ� fill memory�� ȣ���Ͽ� �޸� ���� �ٲ۴�.
	��ȯ�� : ����.
	------------------------------------------------------------------------- */
void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]);

/* -------------------------------------------------------------------------
	�Լ� : void fillMemory(unsigned char(*memory)[COL_SIZE], int start, int end, int value);
	���� : ��° �μ�(����)�� �ι�° �μ�(��) ���̸� ����° �μ�(��)���� ����
	�Ѵ�.
	��ȯ�� : ����
	------------------------------------------------------------------------- */
void fillMemory(unsigned char(*memory)[COL_SIZE], int start, int end, int value);

/* --------------------------------------------------------------------------
	�Լ� : int checkRange(char* arg);
	���� : memory ���� �Լ� �� dump,deit,fill�� ������ ��
	���������� �Էµ� ���� �������� �ִ��� Ȯ��
	��ȯ �� : ��������� ��� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	-------------------------------------------------------------------------- */
int checkRange(char* arg);


/* ============ opcode ���� �Լ� ============ */

/* -------------------------------------------------------------------------
	�Լ� : void readOp(OpcodeTable** opTable);
	���� : ���α׷��� ����Ǹ� Opcode.text�� �о �޸𸮿� �Է��ϴ� �۾���
	�����Ѵ�. ���� �Էµ� mnemonic �� ���ڿ��� �� ���ѵ�
	20���� ���� �������� ���ؼ� index�� �����ϰ� opcode hash table ����
	�˸��� index�� optable node(linked list)�� ���·� �����Ѵ�.
	��ȯ�� : ����
	----------------------------------------------------------------------- */
void readOp(OpcodeTable** opTable);

/* ------------------------------------------------------------------------
	�Լ� : void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT);
	���� : Opcode mnemonic�� �Է��ϸ� findOpcode �Լ��� ȣ���ϰ�
	����� �°� ������ִ� �Լ��̴�.
	��ȯ�� : ����
	----------------------------------------------------------------------- */
void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT);

/* -------------------------------------------------------------------------
	�Լ� : int findOpcode(OpcodeTable* opcodeT, char* first);
	���� : mnemonic�� index�� ã�� �Էµ� mnemonic�� ������ optable node�� ã��
	opcode�� ��ȯ�Ѵ�.
	��ȯ�� : mnemonic�� opcode�� ��ȯ, ���� ���� ��� -1�� ����Ѵ�.
	------------------------------------------------------------------------ */
int findOpcode(OpcodeTable* opcodeT, char* first);

/* ------------------------------------------------------------------------
	�Լ� : void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable);
	���� : opcodetable�� ����� ��� opcodenode�� ����Ѵ�.
	��ȯ�� : ����
	---------------------------------------------------------------------- */
void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable);

/* ------------------------------------------------------------------------
	�Լ� : void freeOpcode(OpcodeTable** opTable);
	���� : opcode table�� ����� opcode node�� free�ϴ� �Լ��̴�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void freeOpcode(OpcodeTable** opTable);


/* ============= type�� ���� �Լ� ============ */

/* -------------------------------------------------------------------------
	�Լ� : void type(char* command, char* first, HistoryList* historyL);
	���� : type ���ϸ��� �ԷµǸ� ������ �о���� printType���ϰ� ȣ���Ѵ�.
	�׸��� ��¿� �̻��� ���� ��� �����丮�� �����Ѵ�.
	��ȯ�� : ����
	----------------------------------------------------------------------- */
void type(char* command, char* first, HistoryList* historyL);

/* -------------------------------------------------------------------------
	�Լ� : int printType(char* filename);
	���� : ������ �о ����Ѵ�.
	��ȯ�� : ������ ���� ��� -1, ������ ��� 0�� ��ȯ�Ѵ�.
	----------------------------------------------------------------------- */
int printType(char* filename);


/* ============ symbol ���� �Լ� ============ */

/* -------------------------------------------------------------------------
	�Լ� : symbol(char* command, HistoryList* historyL);
	���� : ��ɾ� â�� symbol�� �Է��� ��� ȣ��Ǵ� �Լ��̴�.
	���� ���� assemble�� ������ ��� ���������� symbol table�� �����
	symbol table�� ������ְ�, ������ ���ٴ� �ȳ��� ���ش�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void symbol(char* command, HistoryList* historyL);

/* -------------------------------------------------------------------------
	�Լ� : void initSymbol(SymbolTable** symtab);
	���� : ���ο� symbol table�� �ʱ�ȭ������ ��� ���ȴ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void initSymbol(SymbolTable** symtab);

/* -------------------------------------------------------------------------
	�Լ� : int makeSymbol(char* label, int location, SymbolTable** symtab);
	���� : symbol table�� label�� location�� ���� �����Ѵ�. opcode�� ����������
	hash table�� ����ϸ� ���� �̹� ���� ��� �����ȳ� �� -1�� ��ȯ�Ѵ�.
	��ȯ�� : symbol table�� ������ �� �ִٸ� 0, �ߺ��� ��� -1 ��ȯ
	----------------------------------------------------------------------- */
int makeSymbol(char* label, int location, SymbolTable** symtab);

/* -------------------------------------------------------------------------
	�Լ� : int findSymbol(char* label, SymbolTable* symtab);
	���� : label�� symbol table�� ����Ǿ� �ִ��� Ȯ���Ѵ�.
	���� �ش� labe�� ���� ��� location�� ��ȯ���ش�.
	��ȯ�� : ã�� ��  ���� ��� -1, �ִٸ� �ּҸ� ��ȯ�Ѵ�.
	----------------------------------------------------------------------- */
int findSymbol(char* label, SymbolTable* symtab);

/* -------------------------------------------------------------------------
	�Լ� : void printSymbol(int count);
	���� : symbol table�� ����Ѵ�. �ӽ� symbolnode�� ������ table�� �ִ�
	��� ��带 �����ϰ� ���ĺ� ������������ �����Ͽ� ����Ѵ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void printSymbol(int count);

/* -------------------------------------------------------------------------
	�Լ� : int compare(const void* a, const void* b);
	���� : qsort�� ��� �����ϱ� ���� � ���Ĺ���� ����� ������ ����ڰ�
	�����Ͽ� �Ķ���ͷ� �־��ش�.
	��ȯ�� : -1  , 0 , 1
	----------------------------------------------------------------------- */
int compare(const void* a, const void* b);

/* -------------------------------------------------------------------------
	�Լ� : void freeSymtab(SymbolTable** symtab);
	���� : ���ο� symbol table�� ������� ��� ������ �ִ� symbol table��
	free �����ش�. Ȥ�� ���α׷��� �����Ҷ� free�Ѵ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void freeSymtab(SymbolTable** symtab);


/* ============ assemble ���� �Լ� ============ */

/* -------------------------------------------------------------------------
	�Լ� :void assemble(char* command, char* first, HistoryList* historyL, OpcodeTable* opTable);
	���� : asm �����а�, pass1 �Լ��� ȣ���Ͽ� �߰� ������ intermediate ������ �����
	symbol table�� �����. �� �� intermediate ������ ����Ͽ� pass2 �Լ��� ȣ���ϰ�
	lst �׸��� obj ������ �����. ���� �߰��� ����� ������ ������ ���,
	������� symbol table, intermediate, obj, lst ������ �����Ѵ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void assemble(char* command, char* first, HistoryList* historyL, OpcodeTable* opTable);

/* -------------------------------------------------------------------------
	�Լ� : int pass1(char* filename, OpcodeTable* opTable, SymbolTable** symtab, char* progname, int* start, int* length);
	���� : asm �����а�, start ������ �ٸ� instruction�� ȣ��Ǿ����� Ȯ���Ѵ�.
	�� �Ŀ� ���ΰ� �ּҸ� ����ϰ� intermediate ���Ͽ� �����Ѵ�. ���� �߰��� �ּ�
	�� ���� ��� ������ �����ϰ� �����Ѵ�.
	END�� ���� ��� pass1�� �����Ѵ�. �ּҸ� ����ϴ� �������� label�� ���� ���
	symbol table�� ���� ����� �����Ѵ�.
	�� symbol table�� pass2���� ������ ��� �������� symbol table�� ��ü�ϰ�,
	�׷��� ���� ��� �����Ѵ�.
	��ȯ�� : �߰��� ������ ��� -1, �������� ��� 0�� ��ȯ�Ѵ�.
	----------------------------------------------------------------------- */
int pass1(char* filename, OpcodeTable* opTable, SymbolTable** symtab, char* progname, int* start, int* length);

/* -------------------------------------------------------------------------
	�Լ� : int pass2(char* filename, OpcodeTable* opTable, SymbolTable* symtab, int start, int length, char* progname);
	���� : intermediate ������ �а� object code�� ����Ѵ�. �߰� �߰� format 4
	�� ������� modificatoin�� ���� modiflist�� �����Ѵ�.
	���� objectcode�� ����� ��� �ٷ� lst ������ �ۼ��Ѵ�. �׸��� obj ������ ���
	������ ���ų� Ȥ�� RESW, RESB�� ���� ��� �� �پ� �ۼ��ϰ� END�� ������
	��� �۾��� ������.
	��ȯ�� : �߰��� ������ ��� -1, �������� ��� 0�� ��ȯ�Ѵ�.
	----------------------------------------------------------------------- */
int pass2(char* filename, OpcodeTable* opTable, SymbolTable* symtab, int start, int length, char* progname);

/* -------------------------------------------------------------------------
	�Լ� : int chkLocation(char* opcode, char* operand, int prev, OpcodeTable* opTable);
	���� : WORD,RESW,RESB,BYTE �׸��� ������ format ���Ŀ� ���� ��Ʈ�� ��, �ּ�
	���� ����Ѵ�. ���� ������ �ʰ��ϰų� opcode�� ���� ��� -1�� ��ȯ�ϰ� �����Ѵ�.
	��ȯ�� : �߰��� ������ ��� -1, �������� ��� �ּҿ� ����Ʈ���� ���Ͽ� ��ȯ.
	----------------------------------------------------------------------- */
int chkLocation(char* opcode, char* operand, int prev, OpcodeTable* opTable);

/* -------------------------------------------------------------------------
	�Լ� : int chkbyte(char* operand, int* objectcode);
	���� : BYTE�� ���� ��� �ش� �ּҸ� ����Ѵ�.
	���� X��� 1����Ʈ�� ��ȯ�ϰ� ���� object code�� �ش��ϴ� 16���� ����
	�����͸� ����Ͽ� �����Ѵ�.
	���� C��� �� ���ڿ� ���� ����ŭ�� ��ȯ�ϰ� ���� object code�� ����ϴ�
	�ش��ϴ� �ƽ�Ű���� 16������ �����͸� ����Ͽ� �����Ѵ�.
	��ȯ�� : �߰��� ������ ��� -1, �����ϴ� ����Ʈ ���� ��ȯ.
	----------------------------------------------------------------------- */
int chkbyte(char* operand, int* objectcode);

/* -------------------------------------------------------------------------
	�Լ� : int findFormat(OpcodeTable* opcodeT, char* opcode);
	���� : opcode table�� ��ȸ�ϸ鼭 �ش��ϴ� opcode�� ã�� format�� ��ȯ�Ѵ�.
	Ž�� �� hash �� ã�Ƽ� ��带 Ž���ϰ� ���� opcode �տ� +ǥ�ð� �ִٸ�
	format 4 �������� opcode table�� format�� ��ȯ�Ѵ�. ã�� ���� ��� -1 ��ȯ.
	��ȯ�� : opcode ã�� ���� ���  -1, �ش� format ���� ���� ��ȯ.
	----------------------------------------------------------------------- */
int findFormat(OpcodeTable* opcodeT, char* opcode);

/* -------------------------------------------------------------------------
	�Լ� : int checkConstantRange(char* constant);
	16������ ǥ���� �� �ִ� ���� ����Ǿ����� Ȯ���ϴ� �Լ��̴�. ���� 0~9 , A~F
	�� ǥ���Ǿ��� ��� 1 �ƴϸ�  0�� ��ȯ�Ѵ�.
	��ȯ�� : ���� 16������ ��� 1 , �ƴ� ��� 0�� ��ȯ.
	----------------------------------------------------------------------- */
int checkConstantRange(char* constant);

/* -------------------------------------------------------------------------
	�Լ� : int chkRegister(char* oper);
	���������� number�� ��ȯ�Ѵ�.
	��ȯ�� : A - 0, X - 1, L -2, B - 3, S - 4, T- 5, F - 6�� ��ȯ.
	----------------------------------------------------------------------- */
int chkRegister(char* oper);

/* -------------------------------------------------------------------------
	�Լ� : int findObject(char* opcode, char* oper1, char* oper2, char* loc,
	int baseReg, int format, OpcodeTable* opTable, SymbolTable* symtab);
	pass2���� ȣ��Ǵ� �Լ��� ������ object �ڵ带 ����Ѵ�.
	format 1�� ��� opcode�� ��ȯ�ϰ�, format 2�� ��� opcode�� ���������� �ѹ���
	����Ͽ� ��ȯ�Ѵ�. opcode�� +�� �پ��ִٸ� format 4�� ����ϰ�, simple, indirect
	immediate�� Ȯ���Ͽ� address�� ����Ѵ�.
	format 3�� ��� ���� pc -> base�� ������ ó���Ѵ�.
	��ȯ�� : ������ ��� -1, ������ ��� object code
	----------------------------------------------------------------------- */
int findObject(char* opcode, char* oper1, char* oper2, char* loc, int baseReg, int format, OpcodeTable* opTable, SymbolTable* symtab, int* imm);

/* -------------------------------------------------------------------------
	�Լ� : void writeModifynode(int location, ModifList** modiflist);
	modification�� ���ؼ� modifilist�� modfication�� ���� �ּҰ��� �����Ѵ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void writeModifynode(int location, ModifList** modiflist);

/* -------------------------------------------------------------------------
	�Լ� : void fileModifynode(FILE* fwp, ModifList* modiflist);
	pass2 �������� END�� ���� ��� ��� modiflist�� ����� ��� ������
	obj ���Ͽ� modification�κп� �ۼ��Ѵ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void fileModifynode(FILE* fwp, ModifList* modiflist);

/* -------------------------------------------------------------------------
	�Լ� : void freeModifynode(ModifList** modiflist);
	pass2 ���� �� obj ������ �� �ۼ��ϸ� modiflist�� �����Ѵ�.
	��ȯ�� : ����.
	----------------------------------------------------------------------- */
void freeModifynode(ModifList** modiflist);

#endif

