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
#define MAX_MNEM 7
#define TABLE_SIZE 20

/* ���� */ 
unsigned char memory[ROW_SIZE][COL_SIZE]; 	// �޸� ���� (65536 * 16)

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
	char mnemonic[7];			// mnemonic
	int form;					// format
	struct opcodeNode* next;	// ���� node pointer
} OpcodeNode;

/* 2. opcode���� �����ϴ� hashTable */
typedef struct opcodeHashTable {
	int cnt;					// table�� ����� node �� 
	OpcodeNode* node;			// node pointer
} OpcodeTable;


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


#endif
