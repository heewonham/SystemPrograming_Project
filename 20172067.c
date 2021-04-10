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
		if(argCount == -1)
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
			opcodeList(command, &historyL, opcodeT);
		}
		// 11. �� �� 
		else {
			printf("Please check command.\n");
		}
		getchar();
	}
	
	return 0;

}

/* =============  ��ɾ� �����Լ�  ============= */ 

/* 1. command ����ó�� : '\t', ' ' �� ���� ���� ���ְ�,
command�� arguments �迭�� ������ �����Ѵ�. */
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

/* 2. arguments���� ����ó�� : '\t', ' '�� ���ְ�,
 �޸� ���� ���� ���� Ȯ���Ͽ� first, second, third �迭�� �����Ѵ�  */
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
void help(char* input, HistoryList* historyL){
	
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

/* 3. quit : ����*/
void quit(HistoryList* historyL, OpcodeTable** opcodeT) {
	
	deleteHistory(&historyL);		// ����� history linked list�� �����Ѵ�. 
	freeOpcode(opcodeT);			// ����� opcode table�� �����Ѵ�.  
	
}

/* 4. history : ����� ��ɾ �����ؼ� linked list�� �����Ѵ�. */
void history(char* input, HistoryList* historyL){
	
	writeHistory(input, &historyL);		// history List�� ����
	printHistory(historyL);				// ����� history�� ����Ѵ�. 
	
}

/* 4-1. history ��� ��ɾ� ���� */
void writeHistory(char* content, HistoryList** historyL){
	
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
void deleteHistory(HistoryList** historyL){
	
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
void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address){
	
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
void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end){
	
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
			if((i == sRow && j < sCol) || (i == eRow && j > eCol))
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
char asciiCode(int hex){
	
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
void fillMemory(unsigned char(*memort)[COL_SIZE], int start, int end, int value){

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
	for (i = 0; i < len ; i++) {
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
void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT){
	
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
int findOpcode(OpcodeTable* opcodeT, char* first){
	
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
void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable){
	
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
