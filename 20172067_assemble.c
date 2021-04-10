#include "20172067.h"  

int main(void) {

	int address = 0;				// �ּ� ���� (dump �Լ��� �ʿ�)  

	HistoryList historyL;			// history�� ������ list 	
	OpcodeTable* opcodeT;			// opcode���� ������ table

	/* Initializing ���� */
	reset(memory);							// �޸𸮸� 0���� �ʱ�ȭ�Ѵ�. 
	historyL.head = historyL.curr = NULL;   // history list�� head�� ���� node pointer �ʱ�ȭ 
	readOp(&opcodeT);						// opcode.text�� �ҷ��� opcode table�� �����Ѵ�. 

	while (TRUE) {
		/* ��ɾ ������ �迭 : ��ɾ�, ù��° ����, �ι�° ����, ����° ���� */
		char input[MAX_STR], command[MAX_STR] = { '\0', }, arguments[MAX_STR] = { '\0', };
		char first[MAX_STR] = { '\0', }, second[MAX_STR] = { '\0', }, third[MAX_STR] = { '\0', };
		int argCount = 0;   		// ������ �� 

		printf("sicsim> ");
		scanf("%[^\n]", input);

		/* ��ɾ� ���� �Լ�
		  refineCommand�� input���� ���� �Է¾ ��ɾ�� ���ڷ� �����Ѵ�.
		  refineArgument�� ���ڵ��� ��Ȯ�ϰ� ���Դ��� �׸��� ù��° ����
		  ~ ����° ���ڱ��� �����Ͽ� �����Ѵ�. �׸��� ���ÿ� ���� ���� ��ȯ�Ѵ�. */
		refineCommand(input, command, arguments); // ��ɾ� ����
		argCount = refineArgument(arguments, first, second, third); // �μ�����

		// 0. argument�� �߸��� ��� 
		if (argCount == -1)
			printf("Please check the arguments.\n");
		// 1. help ��ɾ� ���� 
		else if (!strcmp("help", command) || !strcmp("h", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó�� 
				printf("No arguments required. Please try again.\n");
			else
				help(command, &historyL);
		}
		// 2. dir ��ɾ� ���� 
		else if (!strcmp("dir", command) || !strcmp("d", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó�� 
				printf("No arguments required. Please try again.\n");
			else
				dir(command, &historyL);
		}
		// 3. quit ��ɾ� ���� 
		else if (!strcmp("quit", command) || !strcmp("q", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó�� 
				printf("No arguments required. Please try again.\n");
			else {
				quit(&historyL, &opcodeT); break;
			}
		}
		// 4. history ��ɾ� ���� 
		else if (!strcmp("history", command) || !strcmp("hi", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó�� 
				printf("No arguments required. Please try again.\n");
			else
				history(command, &historyL);
		}
		// 5. dump ��ɾ� ���� 
		else if (!strcmp("dump", command) || !strcmp("du", command)) {
			if (argCount > 2) // argument�� ���� ��� ����ó��  
				printf("Please check the arguments.\n");
			else // (0 ~ 2�� ����)
				dump(argCount, command, first, second, &historyL, memory, &address);
		}
		// 6. edit ��ɾ� ���� 
		else if (!strcmp("edit", command) || !strcmp("e", command)) {
			if (argCount != 2) // argument�� 2���� �ƴ� ��� ����ó�� 
				printf("Please check the arguments.\n");
			else
				edit(command, first, second, &historyL, memory);
		}
		// 7. fill ��ɾ� ���� 
		else if (!strcmp("fill", command) || !strcmp("f", command)) {
			if (argCount != 3) // argument�� 3���� �ƴ� ��� ����ó�� 
				printf("Please check the arguments.\n");
			else
				fill(command, first, second, third, &historyL, memory);
		}
		// 8. reset ��ɾ� ���� 
		else if (!strcmp("reset", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó�� 
				printf("No arguments required. Please try again.\n");
			else
				reset(memory);
		}
		// 9. opcode ��ɾ� ���� 
		else if (!strcmp("opcode", command)) {
			if (argCount != 1) // argument�� 1���� �ƴ� ��� ����ó�� 
				printf("Please check the arguments.\n");
			else
				opcode(command, first, &historyL, opcodeT);
		}
		// 10. opcodelist ��ɾ� ���� 
		else if (!strcmp("opcodelist", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó��
				printf("No arguments required. Please try again.\n");
			else
				opcodeList(command, &historyL, opcodeT);
		}
		// 11. assemble ��ɾ� ����
		else if (!strcmp("assemble", command)) {
			if (argCount != 1) // argument�� 1���� �ƴ� ��� : ����ó��
				printf("Please check the arguments.\n");
			else
				assemble(command, first, &historyL, opcodeT);
		}
		// 12. type ��ɾ� ����
		else if (!strcmp("type", command)) {
			if (argCount != 1) // argument�� 1���� �ƴ� ��� : ����ó��
				printf("Please check the arguments.\n");
			else
				type(command, first, &historyL);
		}
		// 13. symbol ��ɾ� ����
		else if (!strcmp("symbol", command)) {
			if (argCount != 0) // argument�� ���� ��� : ����ó��
				printf("No arguments required. Please try again.\n");
			else
				symbol(command, &historyL);
		}
		// 14. �� �� 
		else {
			printf("Please check command.\n");
		}
		getchar();
	}

	return 0;

}

void refineCommand(char* input, char* command, char* arguments) {

	int i = 0, j = 0, index = 0;
	int len = strlen(input);		// input�� ���� 

	// �տ� �ִ� whitespace ���� 
	while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;

	for (; i < len; i++) {
		// ���� �� ������ �����ٸ� �ݺ����� ����������. 
		if (input[i] == '\0' || input[i] == '\n') break;
		// �߰��� whitespace�� ������ �� �κк��� argument �迭�� �����Ѵ�. 
		if (input[i] == ' ' || input[i] == '\t') {
			while (i < len) {
				arguments[j++] = input[i]; i++;
			}
		}
		// input�� ����� ���� command�� �����Ѵ�. 
		command[index++] = input[i];
	}
}

int refineArgument(char* arguments, char* first, char* second, char* third) {

	int args = 1;					// ���� �� 
	int index = 0;					// ���ο� �迭�� ������ �� �ʿ��� �ε���  
	int i = 0, comma = 0;			// comma �� ���� 
	int len = strlen(arguments);	// arguments �迭�� ���� 

	// �տ� �ִ� whitespace ����
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;

	/* ���� white space�� ������ �� �̿ܿ� argument�� ����� ���� ���ٸ�,
	   ���ڰ� ���� ���̹Ƿ� 0�� ��ȯ�Ѵ�. */
	if (i >= len)  return 0;

	// ù ��° argument�� first �迭�� �����Ѵ�. 
	index = 0;
	for (; i < len && args == 1; i++) {
		// �߰��� �޸��� ������ ���ڿ� �޸� ���� �����Ѵ�. 
		if (arguments[i] == ',') {
			args++;	comma++;
		}
		// �߰��� whitespace�� ������ ���� ������ �����Ѵ�. 
		else if (arguments[i] == ' ' || arguments[i] == '\t') args++;
		// �� �ܿ��� first �迭�� �����Ѵ�. 
		else
			first[index++] = arguments[i];
	}

	/* ���� ���� �ٲ�鼭 �ݺ����� ���������� �ǰ� �߰��� 2��° ���� ���� ���
	 ���ؼ� �߰��� �ִ� whitespace�� �����ϰ�, �߰��� �޸��� ������ �޸� ����
	 ���� ��Ų��. �޸� ���Ŀ� whitespace�� ���ܽ�Ű�� �۾��� �����Ѵ�. */
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;
	if (arguments[i] == ',') {
		comma++; i++;
	}
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;

	// �� ��° argument�� second �迭�� �����Ѵ�.(first�� ����) 
	index = 0;
	for (; i < len && args == 2; i++) {
		if (arguments[i] == ',') {
			args++;	comma++;
		}
		else if (arguments[i] == ' ' || arguments[i] == '\t') args++;
		else
			second[index++] = arguments[i];
	}

	// first �ؿ� �۾��� ����	
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;
	if (arguments[i] == ',') {
		comma++; i++;
	}
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;

	// �� ��° arument�� third �迭�� �����Ѵ� (first�� ����)  
	index = 0;
	for (; i < len && args == 3; i++) {
		if (arguments[i] == ',') {
			args++;	comma++;
		}
		else if (arguments[i] == ' ' || arguments[i] == '\t') 	args++;
		else
			third[index++] = arguments[i];
	}

	/* ���ڿ� �޸����� �˸°� ���鼭 ���ڰ� 3�� ���� �϶�,
	���ݱ��� ����� args ���� ��ȯ�Ѵ�. */
	if (args < 4 && comma + 1 == args) return args;
	// �� �ܴ� �߸��� ���̹Ƿ� -1�� ��ȯ�Ѵ�. 
	else return -1;

}

/* ============ shell ���� �Լ� ============ */

/* 1. help : ���డ���� ��� ��ɾ� ���*/
void help(char* input, HistoryList* historyL) {

	writeHistory(input, &historyL);		// history List�� ���� 

	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");

	return;

}

/* 2. dir : ���� ���͸��� ���� ���*/
void dir(char* input, HistoryList* historyL) {
	writeHistory(input, &historyL);		// history List�� ����

	DIR *di;
	struct dirent *direntPtr;
	struct stat info;
	int line = 0;
	char name[100];

	di = opendir(".");

	while((direntPtr = readdir(di)) != NULL){
		strcpy(name, direntPtr->d_name);

		// '.' , '..' ������ ��쿡�� ����� �����Ѵ�.
		if(strcmp(name,".") == 0 || strcmp(name,"..") == 0)
			continue;
		lstat(name,&info);
		printf("%20s",name);
		line++;

		// ���͸��� ��쿡�� �����̸� ���� '/' �� ���δ�.
		if(S_ISDIR(info.st_mode))
			printf("/");
		// ���������� ��쿡��  �����̸� ���� '*' �� ���δ�.
		else if(S_IEXEC & info.st_mode)
			printf("*");
		if(line % 3 == 0)
			printf("\n");

	}

	printf("\n");
	closedir(di);

}

/* 3. quit : ���� =====  �����ʿ� */
void quit(HistoryList* historyL, OpcodeTable** opcodeT) {

	deleteHistory(&historyL);		// ����� history linked list�� �����Ѵ�. 
	freeOpcode(opcodeT);			// ����� opcode table�� �����Ѵ�.  
	freeSymtab(&symTab);			// ����� symbol table�� �����Ѵ�.

}

/* 4. history : ����� ��ɾ �����ؼ� linked list�� �����Ѵ�. */
void history(char* input, HistoryList* historyL) {

	writeHistory(input, &historyL);		// history List�� ����
	printHistory(historyL);				// ����� history�� ����Ѵ�. 

}

/* 4-1. history ��� ��ɾ� ���� */
void writeHistory(char* content, HistoryList** historyL) {

	// ������ ������ ���ο� �������
	HistoryNode* insertNode;
	insertNode = (HistoryNode*)malloc(sizeof(HistoryNode));
	insertNode->next = NULL;
	strcpy(insertNode->input, content);

	// Linked List ����
	// head �� ����ִٸ� ���ο� head�� ���� 
	if ((*historyL)->head == NULL) {
		(*historyL)->head = (*historyL)->curr = insertNode;
	}
	// ���� ��� next�� �� ��� ���� 
	else {
		(*historyL)->curr->next = insertNode;
		(*historyL)->curr = insertNode;
	}

	return;

}

/* 4-2. history ������� ����� ��ɾ� ����Ʈ ���*/
void printHistory(HistoryList* historyL)
{
	int i = 1;
	HistoryNode* tmpNode = historyL->head;	// ����Ʈ�ϱ� ���� �ӽ� history node pointer�� head�� ����

	// node�� null �ƴҶ����� ����ؼ� ����Ѵ�. 
	while (tmpNode != NULL) {
		printf("%-4d %s\n", i++, tmpNode->input);
		tmpNode = tmpNode->next;
	}
	return;

}

/* 4-3. history list ����*/
void deleteHistory(HistoryList** historyL) {

	HistoryNode* deleteNode;	 				// ������ histroy node pointer
	HistoryNode* tmpNode = (*historyL)->head; 	// �ӽ� history node pointer�� head�� ����

	// node�� null �ƴҶ����� ����. 
	while (tmpNode != NULL) {
		deleteNode = tmpNode;
		tmpNode = deleteNode->next;
		free(deleteNode);
	}

	return;

}

/* ============ memory ���� �Լ� ============ */

/* 1. reset : memory�� �ʱ��ϴ� �Լ�. */
void reset(unsigned char(*memory)[COL_SIZE]) {

	int i, j;

	// ��� �޸� 0���� �ʱ�ȭ 
	for (i = 0; i < ROW_SIZE; i++)
		for (j = 0; j < COL_SIZE; j++)
			memory[i][j] = 0;

	return;
}

/* 2. dump : �޸𸮸� ����Ѵ�.
		 - dump : ���ڰ� ���� ���, �⺻������ 10 line�� ����Ѵ�.
		 - dump start : ���ڰ� �ϳ��� ���, start�� �������� 10 line�� ����Ѵ�.
		 - dump start , end : ���ڰ� 2���� ���, start���� end���� ����Ѵ�. */
void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address) {

	int start, end;				// ���۰� �� index 

	// history�� �����ϱ� ���� ��ɾ command + argument ���ļ� ����
	char input[MAX_STR];
	strcpy(input, command);
	if (argCount >= 1) {
		strcat(input, " ");
		strcat(input, first);
	}
	if (argCount >= 2) {
		strcat(input, ", ");
		strcat(input, second);
	}

	// argument�� 0�� ��� 
	if (argCount == 0) {

		start = *address;	// ������ address�� ����� �� �ҷ�����
		// 10������ ���� address�� 0xfffff�� �ʰ��� ��� oxfffff���� ��� 
		end = *address + 159 > 0xfffff ? 0xfffff : *address + 159;

		writeHistory(input, &historyL);			// history�� ���� 
		printMemory(memory, start, end);		// memory�� �� ��� 

		// 10������ ���� address�� 0xfffff�� �ʰ��� ��� 0���� reset
		*address = (end + 1 > 0xfffff) ? 0 : end + 1;

	}
	// argument�� 1�� ��� 
	else if (argCount == 1) {

		if (checkRange(first) == FALSE) // argument�� �������� ���� ���� ��
			printf("Please check the arguments.\n");
		else {
			sscanf(first, "%x", &start);	// 16������ ��ȯ
			// start���� 10������ ������ ��� ��� ������ �������� ��� 
			end = ((((start / COL_SIZE) + 10) * 16) - 1) > 0xfffff ? 0xfffff : ((((start / COL_SIZE) + 10) * 16) - 1);

			// ���� ������ 0 ~1048575 �̳� ���� Ȯ��
			if (start < 0x00 || start > 0xfffff)
				printf("Memory overflow.\n");
			else {

				writeHistory(input, &historyL); // history�� ���� 
				printMemory(memory, start, end); // memory�� �� ��� 

			}
		}
	}
	// argument�� 2�� ���
	else if (argCount == 2) {

		if (checkRange(first) == FALSE || checkRange(second) == FALSE) // argument�� �������� ���� ���� ��
			printf("Please check the arguments.\n");
		else {
			// 16������ ��ȯ
			sscanf(first, "%x", &start);
			sscanf(second, "%x", &end);

			// ���۰� �� ������ 0 ~1048575 �̳� ���� Ȯ��
			if (start < 0x00 || start > 0xfffff || end < 0x00 || end > 0xfffff)  // 0 ~1048575 ���� �ʰ���
				printf("Memory overflow.\n");
			// ���� ���ۺ��� Ŭ ��� 
			else if (start > end)
				printf("Start address is greater than end address!.\n");
			else {
				writeHistory(input, &historyL); // history�� ���� 
				printMemory(memory, start, end); // memory�� �� ��� 
			}
		}
	}
	return;
}

/* 2-1. dump�� ����� �޸��� ���� ����Ʈ�Ѵ�. */
void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end) {

	int sRow, sCol; 	// ���� ��, ���� �� 
	int eRow, eCol;		// �� ��, �� �� 
	int i, j;

	sRow = start / COL_SIZE;	// ���� ���� 16�� ���� �� 
	sCol = start % COL_SIZE;	// ���� ���� 16�� ���� ������  
	eRow = end / COL_SIZE;		// ���� ���� 16�� ���� �� 
	eCol = end % COL_SIZE;		// ���� ���� 16�� ���� ������ 

	for (i = sRow; i <= eRow; i++) {

		// address �κ� ��� 
		printf("%05X ", i * 16);

		// memory ���� ��� 
		for (j = 0; j < 16; j++) {
			// ������ ���̸鼭 ������ ������ ���� ��� 
			// ���� ���̸鼭 ���� ������ Ŭ ��� 
			if ((i == sRow && j < sCol) || (i == eRow && j > eCol))
				printf("   "); // �� ���� ��� 
			else // �������� �޸��� �� ��� 
				printf("%02X ", memory[i][j]);
		}

		// �޸� ������ ASCII �ڵ� ����
		printf(";");
		for (j = 0; j < 16; j++) {
			printf("%c", asciiCode(memory[i][j]));
		}

		printf("\n");
	}
	return;

}

/* 2-2. memory�� ������ �ƽ�Ű�ڵ�� ��ȯ�ϴ� �۾��� �����Ѵ�. */
char asciiCode(int hex) {

	// 0x20 ~ 0x7E ������ ���
	if (hex >= 0x20 && hex <= 0x7E)
		return hex;
	else // �� ��
		return '.';

}

/* 3. edit : ù��° ����(�ּ�)�� �ι�° ����(value)�� �ش� �޸� ���� �����Ѵ�. */
void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE])
{
	int address, value;		// �ּҿ� ���� �� 
	int col, row;			// ��� 

	// history�� �����ϱ� ���� ��ɾ command + argument ���ļ� ����
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);
	strcat(input, ", ");
	strcat(input, second);

	// argument���� ������ ���� ���� ���� ��� - ���� ó�� 
	if (checkRange(first) == FALSE || checkRange(second) == FALSE)
		printf("Please check the arguments.\n");
	else {

		// 16������ ��ȯ
		sscanf(first, "%x", &address);
		sscanf(second, "%x", &value);

		// �ּ� ���� 0 ~ 1048575 ���� �ʰ���
		if (address < 0x00 || address > 0xfffff)
			printf("Memory overflow.\n");
		// ���� ������ 0~255 ���� �ʰ��� 
		else if (value < 0x00 || value > 0xff)
			printf("Please check the value.\n");
		// ������ ���	
		else {
			writeHistory(input, &historyL); // history ��� 
			row = address / COL_SIZE;		// �ּҸ� 16 ���� �� 
			col = address % COL_SIZE;		// �ּҸ� 16 ���� ������ 
			memory[row][col] = value;		// �ش� memory ���� 
		}
	}

	return;

}

/* 4. fill : ù��° ����(����)�� �ι�° ����(��) ���̸� ����° ����(��)���� �����Ѵ�. */
void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE])
{
	int start, end, value;	// ����, ��, ���� �� 

	// history�� �����ϱ� ���� ��ɾ command + argument ���ļ� ����
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);
	strcat(input, ", ");
	strcat(input, second);
	strcat(input, ", ");
	strcat(input, third);

	// argument���� ������ ���� ���� ���� ��� - ���� ó�� 
	if (checkRange(first) == FALSE || checkRange(second) == FALSE || checkRange(third) == FALSE)
		printf("Please check the arguments.\n");
	else {

		// 16������ ��ȯ
		sscanf(first, "%x", &start);
		sscanf(second, "%x", &end);
		sscanf(third, "%x", &value);

		// �ּ� ���� 0 ~ 1048575 ���� �ʰ���
		if (start < 0x00 || start > 0xfffff || end < 0x00 || end > 0xfffff)  // 0 ~1048575 ���� �ʰ���
			printf("Memory overflow.\n");
		// ���� ���ۺ��� Ŭ ��� - ����ó��	
		else if (start > end)
			printf("Start address is greater than end address!.\n");
		// ���� ������ 0~255 ���� �ʰ��� 	
		else if (value < 0x00 || value > 0xff)
			printf("Please check the value.\n");
		else {
			writeHistory(input, &historyL);			// history ���
			fillMemory(memory, start, end, value);	// fillmemory �Լ� ȣ�� 
		}
	}

	return;

}

/* 4-1. fill ��ɾ� ����� ���� ������ �����ϴ� �Լ� */
void fillMemory(unsigned char(*memort)[COL_SIZE], int start, int end, int value) {

	int row, col, i; 	// ��, �� 

	for (i = start; i <= end; i++) {
		// ��� ���� ��ȯ 
		row = i / COL_SIZE;
		col = i % COL_SIZE;
		// �� ���� 
		memory[row][col] = value;
	}

	return;

}

/* 5. memory ���� �Լ� �� dump,deit,fill�� �Էµ� ���� �������� �ִ��� Ȯ�� */
int checkRange(char* arg)
{
	int i, len = strlen(arg) - 1;
	for (i = 0; i < len; i++) {
		if (arg[i] >= 0x30 && arg[i] <= 0x39) {} // 0 ~ 9
		else if (arg[i] >= 0x41 && arg[i] <= 0x46) {} // A ~ F
		else if (arg[i] >= 0x61 && arg[i] <= 0x66) {} // a ~ f
		else { return FALSE; }
	}
	return TRUE;
}

/* ============ opcode ���� �Լ� ============ */

/* 1. Opcode.text�� �д� �۾��� ���� */
void readOp(OpcodeTable** opTable) {

	int i, index;			// opcode table�� �ε����� ������ 
	char tmp[32];			// �ӽù��ڿ� 
	OpcodeNode* tmpNode;	// opcodeNode pointer ����	
	FILE* fp;

	// 20�� opcode�� ������ 20���� Hash table�� �����. 
	*opTable = (OpcodeTable*)malloc(sizeof(OpcodeTable) * TABLE_SIZE);

	// opcode Hash Table �ʱ�ȭ 
	for (i = 0; i < TABLE_SIZE; i++) {
		(*opTable)[i].cnt = 0;
		(*opTable)[i].node = NULL;
	}

	fp = fopen("opcode.txt", "r");

	// ������ ���� ��� - ���� ó��  
	if (fp == NULL) {
		printf("There is no opcode.text!\n");
		exit(-1);
	}

	while (!feof(fp)) {
		fgets(tmp, 32, fp);

		// �ҷ��� ���ڿ��� ������ �ݺ����� ����������. 
		if (strlen(tmp) == 0) break;

		// ���ο� ���� ������ opcode ���� 
		tmpNode = (OpcodeNode*)malloc(sizeof(OpcodeNode));
		tmpNode->next = NULL;

		// ������ Ÿ������ ��ȯ�Ͽ� opcode�� 16���� , ��ɾ�� ���ڿ�, form�� ������ ����  
		sscanf(tmp, "%x %s %d", &tmpNode->opcode, tmpNode->mnemonic, &tmpNode->form);

		/* ���߿� mnemonic���� opcode�� ã�� �� �ְ�
		��ɾ� ���ڵ��� ���Ͽ� 20���� ���� �������� �ε����� ����*/
		index = 0;
		for (i = 0; i < (int)strlen(tmpNode->mnemonic); i++)
		{
			index += tmpNode->mnemonic[i];
		}
		index = index % TABLE_SIZE;
		tmpNode->index = index;

		// opcode Table�� �ε����� ������. 
		(*opTable)[index].cnt++;
		if ((*opTable)[index].node == NULL) {
			(*opTable)[index].node = tmpNode;
		}
		else {
			tmpNode->next = (*opTable)[index].node;
			(*opTable)[index].node = tmpNode;
		}
	}

	fclose(fp);
	return;
}

/* 2. Opcode mnemonic�� �Է��ϸ� ������ִ� �Լ� */
void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT) {

	int opcode;	 	// opcode�� ���� 

	// history�� �����ϱ� ���� ��ɾ command + argument ���ļ� ����
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);

	// opcode�� ã�� �Լ� ���� 
	opcode = findOpcode(opcodeT, first);

	// opcode�� ���� ��� - ����ó�� 
	if (opcode < 0)
		printf("Please check mnemonic\n");
	else {
		writeHistory(input, &historyL); 	// history link�� ���� 
		printf("opcode is %02X\n", opcode); // opcode ��� 
	}
}

/* 2-1. mnemonic�� ����� opcodeTable�� �����ϴ� �Լ� */
int findOpcode(OpcodeTable* opcodeT, char* first) {

	int i, index = 0;		// �ε��� ���� 
	OpcodeNode* tmp;		// opcodenode pointer ���� 

	// �ε��� �� ã��: �����ϴ� ����� ���� 
	for (i = 0; i < (int)strlen(first); i++)
		index += first[i];
	tmp = opcodeT[index % TABLE_SIZE].node;

	// opcode �ش� index�� ��� ��� Ž�� 
	while (TRUE) {
		// ������ ��ɾ ������ �ش� opcode�� ��ȯ 
		if (!strcmp(first, tmp->mnemonic))
			return tmp->opcode;

		if (tmp->next != NULL)
			tmp = tmp->next;
		else
			break;
	}
	return -1; // ����
}

/* 3. opcodelist ����ϴ� �Լ� */
void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable) {

	int i;
	OpcodeNode* tmp;	// opcode node pointer ���� 

	writeHistory(command, &historyL);

	// opcode table ���   
	for (i = 0; i < TABLE_SIZE; i++) {
		tmp = opTable[i].node;
		printf("%2d : ", i);

		// opcode table �ش� �ε����� ��� ��带 ���   
		while (TRUE) {
			// ��� ��� Ž���� ������ ���� 
			if (tmp == NULL) break;

			// ��ɾ�� opcode ��� 
			printf("[%s,%02X] ", tmp->mnemonic, tmp->opcode);

			if (tmp->next != NULL) {
				printf("-> "); 		tmp = tmp->next;
			}
			else break;
		}
		printf("\n");
	}
	return;
}

/* 4. opcode table �� �����ϴ� �Լ� */
void freeOpcode(OpcodeTable** opTable) {

	int i;
	OpcodeNode* deleteN;	// ������ opcodenode pointer ����

	for (i = 0; i < TABLE_SIZE; i++) {

		while ((*opTable)[i].cnt != 0) {
			deleteN = (*opTable)[i].node;
			(*opTable)[i].node = deleteN->next;
			free(deleteN);
			(*opTable)[i].cnt--;
		}
	}

	free(*opTable);
}

/* ============ type ���� �Լ� ============ */

/* 1. printType �Լ� ��°� �����丮 �����ϴ� �۾� */
void type(char* command, char* first, HistoryList* historyL) {

	// history�� �����ϱ� ���� ��ɾ command + argument ���ļ� ����
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);

	// printType �Լ��� �ҷ��´�.
	if (printType(first) == -1) return;

	writeHistory(input, &historyL); 	// history link�� ���� 
}

/* 2. ���Ͽ� �ִ� ������ ��� */
int printType(char* filename) {

	FILE* fp = fopen(filename, "r");			// ���� �б� ���� ���� ������
	char  buffer[MAX_BUFR];					// ���� ������ �����ϴ� �ӽù迭

	if (fp == NULL) {
		printf(" '%s' does not exist.\n ", filename);
		return -1;
	}

	while (fgets(buffer, MAX_BUFR, fp)) {
		printf("%s", buffer);
	}
	fclose(fp);
	return 0;
}

/* ============ symbol ���� �Լ� ============ */

/* 1. symbol �Լ� ȣ��� �����丮 �����ϴ� �۾� */
void symbol(char* command, HistoryList* historyL) {

	int i, count = 0;									// symbol Table�� ����� symbol node�� ����
	writeHistory(command, &historyL); 	// history link�� ���� 

	// ���������� symbol Table ������� ��� ����ó���Ѵ�. 
	if ((symTab) == NULL) {
		printf("SYMTAB is empty.\n");
		return;
	}
	else {
		// symbol�� ������ ����.
		for (i = 0; i < TABLE_SIZE; i++)
			count += (symTab)[i].cnt;
		// symbol�� ������ 0���� Ŭ ��� print �Լ��� ȣ���Ѵ�. 
		if (count > 0) {
			printSymbol(count);
			return;
		}
	}
}

/* 2. symbol table�� �ʱ�ȭ�ϴ� �Լ� */
void initSymbol(SymbolTable** symtab) {

	int i;
	*symtab = (SymbolTable*)malloc(sizeof(SymbolTable) * TABLE_SIZE);

	// symbol Hash Table �ʱ�ȭ 
	for (i = 0; i < TABLE_SIZE; i++) {
		(*symtab)[i].cnt = 0;
		(*symtab)[i].node = NULL;
	}
	return;
}

/* 3. symbol table�� symbol�� �߰��ϴ� �Լ� */
int makeSymbol(char* label, int location, SymbolTable** symtab) {

	int i, index;							// hash index �� ����
	SymbolNode* tmpNode;	// ���ο� ���� ������ SymbolNode pointer ����
	SymbolNode* findNode;	// symTab�� �� ��带 Ž���� SymbolNode pointer ����

	// symbol node�� ���ο� ���� ����  
	tmpNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	tmpNode->next = NULL;
	strcpy(tmpNode->label, label);
	tmpNode->location = location;

	/* ���߿� label���� symbol�� ã�� �� �ְ�
	label ���ڵ��� ���Ͽ� 20���� ���� �������� �ε����� ����*/
	index = 0;
	for (i = 0; i < (int)strlen(label); i++) {
		index += label[i];
	}
	index = index % TABLE_SIZE;
	tmpNode->index = index;

	// ���ο� symbol�� ������ �ִ��� Ȯ��
	for (findNode = (*symtab)[index].node; findNode != NULL; findNode = findNode->next) {
		if (!strcmp(label, findNode->label)) {
			printf("%s is Duplicate symbol.\n", label);
			return -1;
		}
	}

	// ������ ��� symTab�� ���ο� ��带 �߰��Ѵ�.
	(*symtab)[index].cnt++;
	if ((*symtab)[index].node == NULL) {	// ������ ����� node�� ���� ���
		(*symtab)[index].node = tmpNode;
	}
	else {
		tmpNode->next = (*symtab)[index].node;
		(*symtab)[index].node = tmpNode;
	}
	return 0;
}

/* 4. symbol table�� �ش� symbol�� ã�� �Լ� */
int findSymbol(char* label, SymbolTable* symtab) {

	SymbolNode* findNode;		// symTab�� �� ��带 Ž���� SymbolNode pointer ����
	int i, index = 0;						// label�� ���ڿ��� ����� index( symTab ������ key)

	/* ���߿� label���� symbol�� ã�� �� �ְ�
	label ���ڵ��� ���Ͽ� 20���� ���� �������� �ε����� ����*/
	for (i = 0; i < (int)strlen(label); i++) {
		index += label[i];
	}
	index = index % TABLE_SIZE;

	// symbol table�� ��ȸ�Ͽ� label�� �ּҰ��� ��ȯ�Ѵ�.
	for (findNode = symtab[index].node; findNode != NULL; findNode = (findNode)->next) {
		if (!strcmp(label, (findNode)->label)) {
			return (findNode)->location;
		}
	}
	return -1;
}

/* 5. symbol table�� ��� ��带 label ���ĺ� ������������ ������ ����ϴ� �Լ� */
void printSymbol(int count) {

	int i, index = 0;				// �ӽ� symbol node�� index
	SymbolNode* temp;		// symTab�� �� ��带 Ž���� SymbolNode pointer ����
	// symbol ����� ������ŭ �޸𸮸� �Ҵ��� ������ ���� symbol node
	SymbolNode* desSym = (SymbolNode*)malloc(sizeof(SymbolNode) * count);

	// symTab�� ��� ��带 ��ȸ�ϸ� desSym node�� �����Ѵ�.
	for (i = 0; i < TABLE_SIZE; i++) {
		temp = (symTab)[i].node;
		while (temp != NULL) {
			strcpy(desSym[index].label, temp->label);
			desSym[index++].location = temp->location;
			temp = temp->next;
		}
	}

	// demSym node�� label�� ���ĺ� ������������ ����
	qsort(desSym, count, sizeof(desSym[0]), compare);

	// ���ĵ� symbol node�� ����Ѵ�.
	for (i = 0; i < count; i++)
		printf("\t%s\t%04X\n", desSym[i].label, desSym[i].location);

	// �ӽ÷� ���� desSym node���� �����Ѵ�.
	free(desSym);
	return;
}

/* 6. symbol table �������� ������ ���� ���� */
int compare(const void* a, const void* b) {

	// �񱳸� ���� ����
	SymbolNode A = *(SymbolNode*)a;
	SymbolNode B = *(SymbolNode*)b;

	// �������� ����
	return strcmp(A.label, B.label);
}

/* 7. symbol table�� free ���ִ� �Լ� */
void freeSymtab(SymbolTable** symtab) {

	int i;
	SymbolNode* deleteN;		// ������ symbol node pointer ����

	// ���� symbol table�� ������� ��� �ش� �Լ��� ����������.
	if (*symtab == NULL)
		return;

	// �ƴ� ��� ��� table�� ��ȸ�ϸ� �����Ѵ�.
	for (i = 0; i < TABLE_SIZE; i++) {
		while ((*symtab)[i].cnt != 0) {
			deleteN = (*symtab)[i].node;
			(*symtab)[i].node = deleteN->next;
			free(deleteN);
			(*symtab)[i].cnt--;
		}
	}
	free(*symtab);
	return;
}

/* ============ assemble ���� �Լ� ============ */

/* 1. assemble - pass1, pass2 �������� */
void assemble(char* command, char* first, HistoryList* historyL, OpcodeTable* opTable) {

	int i, j = 0;
	char input[MAX_STR] = { '\0', };			// history�� �����ϱ� ���� ��ɾ� ����迭
	char filename[MAX] = { '\0', };			// argument�� file name�� �и��ؼ� �����ϴ� �迭
	char type[MAX] = { '\0', };					// argument�� file type���� �и��ؼ� �����ϴ� �迭

	// pass 2�� �ʿ��� �͵�
	char progname[MAX] = { '\0', };		// program name ����
	int start, length;										// object program�� start address�� byte length
	SymbolTable* symtab;							// pass1 �������� �����ϰԵ� symbol table

	// ù��° ���ڸ� file name �� type���� �и��ϴ� ����
	for (i = 0; i < (int)strlen(first); i++) {
		if (first[i] == '.') break;
		filename[i] += first[i];
	}
	for (++i; i < (int)strlen(first); i++) {
		type[j++] += first[i];
	}

	// assemble ����
	// assemble ������ asm�� ��� - pass1 , pass2 ����
	if (!strcmp(type, "asm")) {
		// pass1 ���� ����
		if (pass1(first, opTable, &symtab, progname, &start, &length) == 0) {
			// pass2 ���� ����
			if (pass2(filename, opTable, symtab, start, length, progname) != -1) {
				// ������ ���
				printf("[%s.lst], [%s.obj]\n", filename, filename);
			}
			else {
				// pass 2 ������ ���, intermediate ������ pass1.txt ����
				remove("pass1.txt");
				freeSymtab(&symtab);			// pass1 �� ���� symbol table ����
				return;
			}
		}
		else return;
	}
	else {	// assemble ������ asm �ƴ� ��� �����޼��� ���
		printf("This file type is not 'asm'. please check again.\n");
		return;
	}

	// pass1 , pass2 ������ ������ ���
	freeSymtab(&symTab);			// ���������� ����� symbol table ����
	symTab = symtab;					// pass1 �������� ���� symbol table�� ���������� ����
	// history�� �����ϱ� ���� ��ɾ command + argument ���ļ� ����
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);
	writeHistory(input, &historyL); 	// history link�� ���� 
}

/* 2. pass1 ������ ���ؼ� �߰� ���ϰ� symbol table ����, programe name�� start �ּ�, length ��� */
int pass1(char* filename, OpcodeTable* opTable, SymbolTable** symtab, char* progname, int* start, int* length) {

	FILE* fp = fopen(filename, "r");					// ���� �б� ���� ���� ������
	FILE* fwp = fopen("pass1.txt", "w");			// ������ ���� ���� ���� ������

	char input[MAX_BUFR];								// ���� ������ �����ϴ� �ӽù迭
	// �и��� ���� ������ ������ �ӽù迭 
	char tmp1[MAX] = { '\0', }, tmp2[MAX] = { '\0', }, tmp3[MAX] = { '\0', };
	int args = 0, line = 0;									// argument ������ ���� ��
	int locCurr = 0, locNext = 0;						// ���� �ּҿ� byte�� ���� ���� �ּ�

	initSymbol(symtab);

	// ������ ���� ��� ���� �޼��� ��� 
	if (fp == NULL) {
		printf("%s does not exist!\n", filename);
		fclose(fp);
		fclose(fwp);
		remove("pass1.txt");			// �߰� ���� ����
		return -1;
	}

	/* start ������ �ּ�ó���� start ������ ��ɾ� ���ý� ���� ó�� */
	while (fgets(input, MAX_BUFR, fp)) {
		if (input[0] == '.') {				// �ּ�ó��
			line += 5;
			fprintf(fwp, "%d\t%s", line, input);
		}
		else {
			memset(tmp1, '\0', MAX);
			memset(tmp2, '\0', MAX);
			memset(tmp3, '\0', MAX);
			args = sscanf(input, "%s %s %s", tmp1, tmp2, tmp3);
			if (args == -1)		 // �� ������ ��� �Ѿ 
				continue;
			else if (!strcmp(tmp2, "START") && args == 3) {		// start�� ���� ��� �ּ� �߰����� ���� �Ѿ 
				locNext = atoi(tmp3);												// loc ����
				line += 5;
				*start = locNext;														// start �ּ� ����
				strcpy(progname, tmp1);										// program name ����
				// �߰� ���Ͽ� ����
				fprintf(fwp, "%4d\t%04X\t%-7s\t%-7s\t%-10s\t\n", line, locNext, tmp1, tmp2, tmp3);
				break;
			}
			else {
				// ������ ���
				fclose(fp);
				fclose(fwp);
				remove("pass1.txt");
				printf("There is other opcode before start. - pass1\n");
				return -1;
			}
		}
	}

	// start ���� ���� ó��
	while (fgets(input, MAX_BUFR, fp)) {
		if (input[0] == '.') {		// �ּ� ó��
			line += 5;
			fprintf(fwp, "%4d\t%s", line, input);
		}
		else {
			memset(tmp1, '\0', MAX);
			memset(tmp2, '\0', MAX);
			memset(tmp3, '\0', MAX);
			args = sscanf(input, "%s %s %s", tmp1, tmp2, tmp3);
			if (args == -1) // �� ���� 
				continue;
			else if (args == 3) {
				locCurr = locNext;
				// operand�� 2�� �� ���
				if (tmp2[strlen(tmp2) - 1] == ',') {
					// ���� �ּ� ����
					locNext = chkLocation(tmp1, tmp2, locCurr, opTable);
					if (locNext != -1) {
						line += 5;
						fprintf(fwp, "%4d\t%04X\t\t%s\t%s%s\t\n", line, locCurr, tmp1, tmp2, tmp3);
					}
					else
						break;
				}
				else {
					locNext = chkLocation(tmp2, tmp3, locCurr, opTable);
					if (locNext != -1 && makeSymbol(tmp1, locCurr, symtab) != -1) {
						line += 5;
						fprintf(fwp, "%4d\t%04X\t%-7s\t%-7s\t%-10s\t\n", line, locCurr, tmp1, tmp2, tmp3);
					}
					else
						break;
				}
			}
			else if (args == 2) {
				if (!strcmp(tmp1, "END")) {			// diretive end ó��
					line += 5;
					*length = locNext - (*start);		// ���α׷� ���� ����
					fprintf(fwp, "%4d\t\t\t%-7s\t%-10s\t\n", line, tmp1, tmp2);
					fclose(fp);
					fclose(fwp);
					return 0;
				}
				else if (!strcmp(tmp1, "BASE")) {			// diretive base ó��
					line += 5;
					fprintf(fwp, "%4d\t\t\t%-7s\t%-10s\t\n", line, tmp1, tmp2);
				}
				else {
					locCurr = locNext;
					locNext = chkLocation(tmp1, tmp2, locCurr, opTable);
					if (locNext != -1) {
						line += 5;
						fprintf(fwp, "%4d\t%04X\t\t%-7s\t%-10s\t\n", line, locCurr, tmp1, tmp2);
					}
					else
						break;
				}
			}
			else if (args == 1) {
				locCurr = locNext;
				locNext = chkLocation(tmp1, tmp2, locCurr, opTable);
				if (locNext != -1) {
					line += 5;
					fprintf(fwp, "%4d\t%04X\t\t%-7s\n", line, locCurr, tmp1);
				}
				else
					break;
			}
			else
				break;
		}
	}

	// ���α׷� ���� ����
	fclose(fp);
	fclose(fwp);
	remove("pass1.txt");
	printf("- pass 1 failed.\n");
	return -1;
}

/* 3. pass2 ������ ���ؼ� object code�� ����ϰ� lst ���ϰ� obj ������ �����. */
int pass2(char* filename, OpcodeTable* opTable, SymbolTable* symtab, int start, int length, char* progname) {

	FILE* fp = fopen("pass1.txt", "r");		// ������ �б� ���� ������
	FILE* fwlst;											// lst ������ ���� ���� ������
	FILE* fwobj;											// obj ������ ���� ���� ������

	// modification�� ���� list ����
	ModifList* modiflist = (ModifList*)malloc(sizeof(ModifList*));
	modiflist->head = NULL;
	modiflist->curr = NULL;
	int immflag = 0;						// immediate flag, 1�� ��� �̹� ����� �͹Ƿ� modiflist�� �����ʴ´�

	char lstfile[MAX] = { '\0', };					// filename�� lst Ȯ���ڴ��� �̸��� ������ �迭 
	char objfile[MAX] = { '\0', };				// filename�� obj Ȯ���ڴ��� �̸��� ������ �迭 
	char buffer[MAX_BUFR] = { '\0' , };		//	���� ������ �����ϴ� �ӽù迭


	// ���� ������ �и��Ͽ� �����ϴµ� �ʿ��� �迭
	char line[7], loc[7];								// ���뿡 line�� loc�� ����
	// �ӽù迭
	char tmp1[MAX] = { '\0', }, tmp2[MAX] = { '\0', };
	char tmp3[MAX] = { '\0', }, tmp4[MAX] = { '\0', };
	char* oper1, * oper2;							// operand�� �� ���� ��� ������ �迭 ������
	char* content;										// '.' �ּ��� ������ ������ �迭 ������

	int args, format;										// ������ ������ format ����
	int opc, baseReg = 0;								// opcode ����� base register �ּҰ�

	// obj ������ �ۼ��ϱ� ���� ����
	int byte = 0, startAddr, startflag = 0;		// ����Ʈ ����, obj �����ּ�, �����ּ� ���� flag (0�� ��� ���� loc�� �����Ѵ�.)
	char objectcode[10] = { '\0', };				// opc�� objectcode �迭�� �ӽ�����
	char objectline[MAX_STR] = { '\0', };		// object line�� ������ ����Ʈ ���� ���� obj ���Ͽ� ����.

	// ������ �� �� ���� ��� - ���� ó��
	if (fp == NULL) {
		printf("'pass1.txt' file does not exist.\n");
		return -1;
	}

	// lst�� obj ����+ Ȯ���� �Է�
	strcpy(lstfile, filename);
	strcat(lstfile, ".lst");
	strcpy(objfile, filename);
	strcat(objfile, ".obj");

	// ���� �����Ϳ� ����
	fwlst = fopen(lstfile, "w");
	fwobj = fopen(objfile, "w");

	// start �κ�ã��
	while (fgets(buffer, MAX_BUFR, fp)) {
		memset(line, '\0', 7);
		memset(loc, '\0', 7);
		memset(tmp1, '\0', MAX);
		memset(tmp2, '\0', MAX);
		memset(tmp3, '\0', MAX);
		args = sscanf(buffer, "%s\t%s\t%s\t%s\t%s", line, loc, tmp1, tmp2, tmp3);
		if (loc[0] == '.') {		// �ּ�ó��
			strtok(buffer, ".");
			content = strtok(NULL, "");
			fprintf(fwlst, "%4s\t\t  %s\n", line, content);
			continue;
		}
		else if (!strcmp("START", tmp2)) {
			startAddr = start;			// obj ���� �ּ� ����
			startflag = 1;					// ���� �ּҸ� ������ �ʿ���� flag , 0�� ��� ���� loc ����
			fprintf(fwlst, "%s", buffer);
			fprintf(fwobj, "H%-6s%06X%06X\n", progname, start, length);
			break;
		}
		else {
			printf("There is other opcode before start. - pass2.\n");
			fclose(fp);
			fclose(fwobj);
			fclose(fwlst);
			remove("pass1.txt");
			return -1;
		}
	}

	// start ����
	while (fgets(buffer, MAX_BUFR, fp)) {
		memset(line, '\0', 7);
		memset(loc, '\0', 7);
		memset(tmp1, '\0', MAX);
		memset(tmp2, '\0', MAX);
		memset(tmp3, '\0', MAX);
		memset(tmp4, '\0', MAX);
		memset(objectcode, 7, '\0');
		immflag = 0;
		args = sscanf(buffer, "%s\t%s\t%s\t%s\t%s", line, loc, tmp1, tmp2, tmp3);
		if (loc[0] == '.') {	// �ּ�ó��
			strtok(buffer, ".");
			content = strtok(NULL, "");
			fprintf(fwlst, "%4s\t\t  %s", line, content);
			continue;
		}
		if (args == 3) {
			if (!strcmp("BASE", loc)) {	// direvtive BASE ó��
				baseReg = findSymbol(tmp1, symtab);
				fprintf(fwlst, "%s", buffer);
				continue;
			}
			else if (!strcmp("END", loc)) { // direvtive END ó��
				// obj , lst ���� ����
				fprintf(fwlst, "%s", buffer);
				fprintf(fwobj, "T%06X%02X%s\n", startAddr, byte, objectline);
				// modfi obj ���Ͽ� �ۼ�
				fileModifynode(fwobj, modiflist);
				fprintf(fwobj, "E%06X\n", start);
				freeModifynode(&modiflist);			// modfiynode free
				fclose(fp);
				fclose(fwobj);
				fclose(fwlst);
				remove("pass1.txt");
				return 0;
			}
			else {	// BASE, END�� �ƴ� ��� ( ���ڰ� 3���� ���)
				format = findFormat(opTable, tmp1);
				opc = findOpcode(opTable, tmp1);
				if (opc != -1) {
					if (format != -1) {
						oper1 = strtok(tmp2, ",");
						oper2 = strtok(NULL, "\n");
						opc = findObject(tmp1, oper1, oper2, loc, baseReg, format, opTable, symtab, &immflag);
						fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t\t\t%X\n", line, loc, tmp1, opc);
					}
					else break;
				}
				else {
					printf("%s opcode not found.", tmp1);
					break;
				}
			}
		}
		else if (args == 4) { // ���ڰ� 4���� ���
			format = findFormat(opTable, tmp1);
			oper1 = strtok(tmp2, ",");
			oper2 = strtok(NULL, "\n");
			opc = findObject(tmp1, oper1, oper2, loc, baseReg, format, opTable, symtab, & immflag);
			if ((format != -1) && opc != -1) {
				strcpy(tmp3, oper1);
				if (oper2 != NULL) {
					strcat(tmp3, ", ");
					strcat(tmp3, oper2);
				}
				// ���Ŀ� ���� lst ������ �ٸ��� ����
				if (format == 4)
					fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t%-10s\t%08X\n", line, loc, tmp1, tmp3, opc);
				else if (format == 3)
					fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t%-10s\t%06X\n", line, loc, tmp1, tmp3, opc);
				else
					fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t%-10s\t%X\n", line, loc, tmp1, tmp3, opc);
			}
			else break;
		}
		else if (args == 5) { // ���ڰ� 5���� ���
			if (!strcmp(tmp2, "RESW") || !strcmp(tmp2, "RESB")) {
				fprintf(fwlst, "%s", buffer);		// �״�� ����
				if (byte > 0) {		// obj�� ���� �����ϴ� �κ��� �ƴ� ��� obj�� �ۼ��Ѵ�.
					fprintf(fwobj, "T%06X%02X%s\n", startAddr, byte, objectline);
					byte = 0;				// byte �� 0���� �ʱ�ȭ
					startflag = 0;		// start address flag�� 0���� �����Ѵ�.
					memset(objectline, '\0', MAX_STR);
				}
				continue;
			}
			if (!strcmp(tmp2, "WORD")) {
				int word = atoi(tmp3);
				format = 3;
				if (word >= 0x000000 && word <= 0xFFFFFF) {
					fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%06X\n", line, loc, tmp1, tmp2, tmp4, word);
				}
				else {	// ���� �ʰ���
					printf("WORD range overflow.");
					break;
				}
			}
			if (!strcmp(tmp2, "BYTE")) {
				opc = 0;
				format = chkbyte(tmp3, &opc);
				if (format != -1) {
					// format�� ���� lst�� �����ڸ� �ٸ��� �Ѵ�. 
					if (format == 1)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%02X\n", line, loc, tmp1, tmp2, tmp3, opc);
					else if (format == 2)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%04X\n", line, loc, tmp1, tmp2, tmp3, opc);
					else if (format == 3)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%06X\n", line, loc, tmp1, tmp2, tmp3, opc);
				}
				else	break;
			}
			else {	// directive �̿��� ���
				format = findFormat(opTable, tmp2);
				oper1 = strtok(tmp3, ",");
				oper2 = strtok(NULL, "\n");
				opc = findObject(tmp2, oper1, oper2, loc, baseReg, format, opTable, symtab, &immflag);
				if (format != -1 && opc != -1) {
					strcpy(tmp4, oper1);
					if (oper2 != NULL) {
						strcat(tmp4, ", ");
						strcat(tmp4, oper2);
					}
					if (format == 4)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%08X\n", line, loc, tmp1, tmp2, tmp4, opc);
					else if (format == 3)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%06X\n", line, loc, tmp1, tmp2, tmp4, opc);
					else
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%X\n", line, loc, tmp1, tmp2, tmp4, opc);
				}
				else
					break;
			}
		}
		else { // �׿��� ��� 
			printf("intermediate file(pass1.txt) format is not correct.");
			break;
		}

		// ���Ŀ� ���� object code�� 08~02X �������� �����Ѵ�. 
		if (format == 4) {
			sprintf(objectcode, "%08X", opc);
			int curloc = strtol(loc, NULL, 16);
			if(immflag == 0)
				writeModifynode(curloc - start + 1, &modiflist);		// modification list�� �����Ѵ�.
		}
		else if (format == 3)	sprintf(objectcode, "%06X", opc);
		else if (format == 2) sprintf(objectcode, "%04X", opc);
		else if (format == 1) sprintf(objectcode, "%02X", opc);

		// RESW, RESB�� ���� �� �ٽ� objectcode�� ���Ǿ��� ��� ���� �ּҸ� start�� �����Ѵ�.
		if (startflag == 0) {
			startAddr = strtol(loc, NULL, 16);
			startflag = 1;
		}
		// obj ���� ���� 
		if (format != -1) {
			if ((byte + format) > 30) {
				// byte���� 30����Ʈ�� �ʰ��� ��� ������ ����� objectline�� obj ���Ͽ� �ۼ��ϰ�, �ʱ�ȭ�Ѵ�.
				fprintf(fwobj, "T%06X%02X%s\n", startAddr, byte, objectline);
				byte = 0;
				startAddr = strtol(loc, NULL, 16);
				memset(objectline, '\0', MAX_STR);
				strcat(objectline, objectcode);
				byte += format;
			}
			else { // �̿ܿ� objectcode�� ����� ���� objectline�� �����Ѵ�.
				strcat(objectline, objectcode);
				byte += format;
			}
		}

	}

	// pass2 ���� ����
	fclose(fp);
	fclose(fwobj);
	fclose(fwlst);
	remove("pass1.txt");
	remove(objfile);
	remove(lstfile);
	printf("- pass2 fail.\n");
	return -1;
}

/* 4. ���� �ּҰ��� format ����, operand�� ����Ʈ ���� ����Ͽ� ������ ����� �ּҰ��� ����Ѵ�. */
int chkLocation(char* opcode, char* operand, int prev, OpcodeTable* opTable) {

	int format;					// format ���� ����
	int temp = 0;				// chkLocation���� �ʿ������ chkbyte�� ���� �ӽ� ����

	if (!strcmp("WORD", opcode)) {
		// word ����üũ
		if (checkConstantRange(operand) == TRUE && (atoi(operand) <= 0xFFFFFF))
			return prev + 3;
		else {
			printf("WORD Range overflow.");
			return -1;
		}
	}
	else if (!strcmp("RESW", opcode)) {
		// �ش� ���ڿ��� 16������ ����� ǥ���Ǿ����� Ȯ��, ���������� Ȯ��
		if (checkConstantRange(operand) == TRUE && (atoi(operand) <= 0xFFFFFF))
			return prev + 3 * atoi(operand);
		else {
			printf("RESW Range overflow.");
			return -1;
		}
	}
	else if (!strcmp("RESB", opcode)) {
		// �ش� ���ڿ��� 16������ ����� ǥ���Ǿ����� Ȯ��, ���������� Ȯ��
		if (checkConstantRange(operand) == TRUE && (atoi(operand) <= 0xFFFFFF))
			return prev + atoi(operand);
		else {
			printf("RESB Range overflow.");
			return -1;
		}
	}
	else if (!strcmp("BYTE", opcode)) {
		// BYTE üũ
		int tp = chkbyte(operand, &temp);
		if (tp != -1)
			return prev + tp;
		else
			return -1;
	}
	else {
		// �� ���� ��� format ���� �褿��
		format = findFormat(opTable, opcode);
		if (format == -1)
			return -1;
		else
			return prev + format;
	}
}

/* 5.  BYTE�� �����ϴ� �޸� ���� object code�� ����Ѵ�. */
int chkbyte(char* operand, int* objectcode) {

	char tmp[MAX] = { '\0' , };			// operand�� ������ �迭
	char* type, * value;						// operand ���� type�� value�� �����ϴ� �迭������ 
	int i;

	// operand�� type�� value�� �и�
	strcpy(tmp, operand);
	type = strtok(tmp, "'");
	value = strtok(NULL, "'");

	// ���� type X�� ���
	if (!strcmp(type, "X")) {
		// ���� Ȯ���Ѵ�.
		if (checkConstantRange(value) == FALSE) {
			printf("BYTE Range overflow.");
			return -1;
		}
		*objectcode = strtol(value, NULL, 16);
		return 1;
	}
	else if (!strcmp(type, "C")) {
		// type C�� ���, value �ȿ� �ִ� ���� ���ڸ� �ƽ�Ű������ �����ϰ� �ٽ� 16������ �����Ѵ�.
		for (i = 0; i < (int)strlen(value) - 1; i++) {
			*objectcode += value[i];
			*objectcode = *objectcode << 8;
		}
		*objectcode += value[i];
		return strlen(value);
	}

	printf("There is no %s operand.", operand);
	return -1;
}

/* 6. opcode�� format�� Ȯ���Ͽ� �˷��ش�. */
int findFormat(OpcodeTable* opcodeT, char* opcode) {

	int i, index = 0;				// �ε��� ���� 
	OpcodeNode* tmp;		// opcode node pointer ���� 
	char* mne;						// '+' �� �и��Ͽ� ������ �迭 ������
	char opc[MAX];				// opcode�� ������ �迭

	// opcode�� opc �迭�� �����Ѵ�.
	strcpy(opc, opcode);
	// '+' �� �и��Ͽ� ����
	mne = strtok(opc, "+");

	// �ε��� �� ã��: �����ϴ� ����� ���� 
	for (i = 0; i < (int)strlen(mne); i++)
		index += mne[i];
	tmp = opcodeT[index % TABLE_SIZE].node;

	// opcode �ش� index�� ��� ��� Ž�� 
	while (TRUE) {
		// ������ ��ɾ ������ �ش� opcode�� ��ȯ 
		if (!strcmp(mne, tmp->mnemonic)) {
			if (opcode[0] == '+')
				return 4;
			else
				return tmp->form;
		}

		// NULL�� �ƴҶ����� next �Ѿ��.
		if (tmp->next != NULL)
			tmp = tmp->next;
		else
			break;
	}

	//������ ���
	printf("There is no %s opcode.", opcode);
	return -1;
}

/* 7. 16���� �̳� ���� ����Ǿ����� Ȯ���Ѵ�. */
int checkConstantRange(char* constant) {

	int i;
	for (i = 0; i < (int)strlen(constant); i++) {
		// 0 ~ 9 , A ~ F
		if ((constant[i] >= 0x30 && constant[i] <= 0x39) || (constant[i] >= 0x41 && constant[i] <= 0x46))
			continue;
		else
			return FALSE;
	}
	return TRUE;
}

/* 8. ���������� number�� Ȯ���Ѵ�.  */
int chkRegister(char* oper) {

	if (!strcmp("A", oper)) 		return 0;
	else if (!strcmp("X", oper)) 		return 1;
	else if (!strcmp("L", oper))		return 2;
	else if (!strcmp("B", oper))		return 3;
	else if (!strcmp("S", oper))		return 4;
	else if (!strcmp("T", oper))		return 5;
	else if (!strcmp("F", oper))		return 6;

	return -1;
}

/* 9. objectcode�� ����ϴ� �Լ�  */
int findObject(char* opcode, char* oper1, char* oper2, char* loc, int baseReg, int format, OpcodeTable* opTable, SymbolTable* symtab,int * imm) {

	int objcode = 0;											// opcode mnem�� ������ ����
	int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;	// nixbpe bit�� ������ ����
	int r1 = 0, r2 = 0;											// register�� number ����

	char* opr1, * opc;											// �پ��ִ� +, @, # �и��Ͽ� ������ �迭 ������	
	char opertmp[MAX], opctmp[MAX];			// operand�κа� opcode�� ������ �迭

	int objAddr = 0;											// -	format 3�� disp �κ�, format 4�� address�κп� ����object code ����
	int bit = 0;													// format 3�� ��� 12, 4�ϰ�� 20 ���� 
	int location = strtol(loc, NULL, 16);			// ���� location�� 16������ ����

	// operand�� opcode�� opertmp�� opctmp�� ����
	if (oper1 != NULL)
		strcpy(opertmp, oper1);
	strcpy(opctmp, opcode);

	// format 1�� ��� : opcode ��ȯ(1 ����Ʈ)
	if (format == 1) {
		objcode = findOpcode(opTable, opcode);
		if (objcode == -1) {
			printf("There is no %s opcode.", opcode);
			return -1;
		}
		return objcode;
	}
	else if (format == 2) {	// format 2�� ��� : opcode�� register ���

		objcode = findOpcode(opTable, opcode);
		if (objcode == -1) {
			printf("There is no %s opcode.", opcode);
			return -1;
		}

		objcode = objcode << 8;		 // 1 ����Ʈ �̵� (8bit)

		// register 1 üũ
		if (oper1 != NULL) {
			r1 = chkRegister(oper1);
			r1 = r1 << 4;
			objcode += r1;
		}

		// register 2 üũ
		if (oper2 != NULL) {
			r2 = chkRegister(oper2);
			objcode += r2;
		}
		return objcode;
	}

	// format 3~4
	// @, #, + addressing mode Ȯ���Ѵ�. == > n , i
	if (opertmp != NULL && opertmp[0] == '@') { // @: indirect 
		n = 1;
		opr1 = strtok(opertmp, "@");
	}
	else if (opertmp != NULL && opertmp[0] == '#') { // #: immediate 
		i = 1;
		opr1 = strtok(opertmp, "#");
		if (checkConstantRange(opr1) == TRUE) {
			*imm = 1;
			objAddr = atoi(opr1);
		}
	}
	else {			// simple
		n = 1;
		i = 1;
		opr1 = strtok(opertmp, "");
	}

	// operand�� symbol�� ��� �ּҰ��� �����Ѵ�.
	if (oper1 != NULL && *imm == 0) {
		objAddr = findSymbol(opr1, symtab);
		if (objAddr == -1) {
			printf("%s Operand not exist in symbol table.", opr1);
			return -1;
		}

	}


	// opctmp�� +�� �и� �����Ѵ�.
	opc = strtok(opctmp, "+");

	// operand 1�� 2�� X�� �ִ��� Ȯ��
	if ((oper1 != NULL) && !strcmp("X", oper1) || (oper2 != NULL && !strcmp("X", oper2)))
		x = 1;

	// format 3
	if (format == 3) {
		bit = 12;									// disp�� ���� �Ŀ� 12��Ʈ �̵�
		if (oper1 == NULL)					// operand�� ������� ��� 0���� ����
			objAddr = 0;
		else if (oper1[0] != '#' || checkConstantRange(opr1) == FALSE) {
			// PC relative addressing ����Ѵ�
			if (objAddr - (location + format) >= -2048 && objAddr - (location + format) <= 2047) {
				p = 1;
				objAddr -= (location + format);

				if (objAddr < 0) // ������ 2�� ������ ����.
					objAddr &= 0x00000fff;
			}

			// pc ���� �ʰ��� BASE relative addressing ���
			else if (objAddr - baseReg >= 0 && objAddr - baseReg <= 4095) {
				b = 1;
				objAddr -= baseReg;
			}
			else {	// ���� �޼����� ����Ѵ�. 
				printf("Range overflow.");
				return -1;
			}
		}
	}
	else { // format 4
		bit = 20;								// address�� ���� �Ŀ� 20��Ʈ �̵�
		e = 1;									// format 4�� e = 1

		if (n == 1 && i == 1 && x == 1) {  // simple + x bit
			if (x == 1) 						// x�� number �߰� (direct address)
				objAddr = 1;
		}
	}

	// opcode mnemi ã��
	objcode = findOpcode(opTable, opc);
	if (objcode == -1) { // ����
		printf("%s opcode is not correct.", opc);
		return -1;
	}

	// object code�� opcode + nixbpe + address �κ��� ���� �ϼ��Ѵ�.
	n = n << 1;
	objcode = objcode + n + i;	// opcode�� n, i�� ����.
	objcode = objcode << 4;		// 4��Ʈ �̵��Ͽ� +xbpe�� ���Ѵ�. 
	x = x << 3;
	b = b << 2;
	p = p << 1;
	objcode = objcode + x + b + p + e;
	objcode = objcode << bit;			// �ش� format��ŭ �̵���Ű�� ����� objAddr�� �����Ѵ�.
	return objcode += objAddr;

}

/* 10. modification �ּҸ� ModifList�� ������  */
void writeModifynode(int location, ModifList** modiflist) {

	// ModifNode�� ������ ���ο� ��� ����
	ModifNode* newNode = (ModifNode*)malloc(sizeof(ModifNode));
	newNode->next = NULL;
	newNode->location = location;

	/* ���� modify node�� ó���� ���, ����� �����ϰ�,
	cur�� head�� �����ϰ� �����Ѵ�. */
	if ((*modiflist)->head == NULL) {
		(*modiflist)->head = newNode;
		(*modiflist)->curr = newNode;
	}
	else { // �ƴ� ��� , current next�� �����ϰ� , curr �� �������ش�.
		(*modiflist)->curr->next = newNode;
		(*modiflist)->curr = newNode;
	}
	return;
}

/* 11. ModifList�� ����� ��� ��带 obj ���Ͽ� ������  */
void fileModifynode(FILE* fwp, ModifList* modiflist) {

	// �� ��带 ��ȸ�ϰ� �� modifnode
	ModifNode* tmpNode = modiflist->head;

	// ��� ��带 ��ȸ�ϸ� ����Ѵ�.
	for (; tmpNode != NULL; tmpNode = tmpNode->next)
		fprintf(fwp, "M%06X05\n", tmpNode->location);

	return;
}

/* 12. ModifList�� ����� ��� ��带 free�Ѵ�.  */
void freeModifynode(ModifList** modiflist) {

	// ������ ���� modifnode
	ModifNode* deleteNode = (*modiflist)->head;

	// list�� ����� ��� ��带 �����Ѵ�. head�� �� ���� ���� ����, delete ���� 
	while (TRUE) {
		deleteNode = (*modiflist)->head;
		if (deleteNode != NULL) {
			(*modiflist)->head = deleteNode->next;
			free(deleteNode);
		}
		else
			break;
	}
	return;
}
