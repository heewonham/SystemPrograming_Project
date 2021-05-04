#include "20172067.h"  

int main(void) {

	int address = 0;				// 주소 저장 (dump 함수시 필요)

	HistoryList historyL;			// history를 저장할 list 	
	OpcodeTable* opcodeT;			// opcode들을 저장할 table
	BreakPointList breakP;			// break point linked list 생성 

	/* Initializing 과정 */
	reset(memory);							// 메모리를 0으로 초기화한다. 
	historyL.head = historyL.curr = NULL;   // history list의 head와 현재 node pointer 초기화 
	readOp(&opcodeT);						// opcode.text를 불러와 opcode table에 저장한다. 
	breakP.head = breakP.curr = NULL;		// break point 초기화

	while (TRUE) {
		/* 명령어를 저장할 배열 : 명령어, 첫번째 인자, 두번째 인자, 세번째 인자 */
		char input[MAX_STR], command[MAX_STR] = { '\0', }, arguments[MAX_STR] = { '\0', };
		char first[MAX_STR] = { '\0', }, second[MAX_STR] = { '\0', }, third[MAX_STR] = { '\0', };
		int argCount = 0;   		// 인자의 수 

		printf("sicsim> ");
		scanf("%[^\n]", input);

		/* 명령어 정제 함수
		  refineCommand는 input으로 들어온 입력어를 명령어와 인자로 구분한다.
		  refineArgument는 인자들이 정확하게 들어왔는지 그리고 첫번째 인자
		  ~ 세번째 인자까지 정제하여 저장한다. 그리고 동시에 인자 수를 반환한다. */
		refineCommand(input, command, arguments); // 명령어 정리
		argCount = refineArgument(arguments, first, second, third); // 인수정리

		// 0. argument가 잘못된 경우 
		if (strcmp("loader", command) && argCount == -1)
			printf("Please check the arguments.\n");
		// 1. help 명령어 수행 
		else if (!strcmp("help", command) || !strcmp("h", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리 
				printf("No arguments required. Please try again.\n");
			else
				help(command, &historyL);
		}
		// 2. dir 명령어 수행 
		else if (!strcmp("dir", command) || !strcmp("d", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리 
				printf("No arguments required. Please try again.\n");
			else
				dir(command, &historyL);
		}
		// 3. quit 명령어 수행 
		else if (!strcmp("quit", command) || !strcmp("q", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리 
				printf("No arguments required. Please try again.\n");
			else {
				quit(&historyL, &opcodeT); break;
			}
		}
		// 4. history 명령어 수행 
		else if (!strcmp("history", command) || !strcmp("hi", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리 
				printf("No arguments required. Please try again.\n");
			else
				history(command, &historyL);
		}
		// 5. dump 명령어 수행 
		else if (!strcmp("dump", command) || !strcmp("du", command)) {
			if (argCount > 2) // argument가 많을 경우 예외처리  
				printf("Please check the arguments.\n");
			else // (0 ~ 2개 정상)
				dump(argCount, command, first, second, &historyL, memory, &address);
		}
		// 6. edit 명령어 수행 
		else if (!strcmp("edit", command) || !strcmp("e", command)) {
			if (argCount != 2) // argument가 2개가 아닐 경우 예외처리 
				printf("Please check the arguments.\n");
			else
				edit(command, first, second, &historyL, memory);
		}
		// 7. fill 명령어 수행 
		else if (!strcmp("fill", command) || !strcmp("f", command)) {
			if (argCount != 3) // argument가 3개가 아닐 경우 예외처리 
				printf("Please check the arguments.\n");
			else
				fill(command, first, second, third, &historyL, memory);
		}
		// 8. reset 명령어 수행 
		else if (!strcmp("reset", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리 
				printf("No arguments required. Please try again.\n");
			else {
				reset(memory);
				execAddr = -1;
			}
		}
		// 9. opcode 명령어 수행 
		else if (!strcmp("opcode", command)) {
			if (argCount != 1) // argument가 1개가 아닐 경우 예외처리 
				printf("Please check the arguments.\n");
			else
				opcode(command, first, &historyL, opcodeT);
		}
		// 10. opcodelist 명령어 수행 
		else if (!strcmp("opcodelist", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리
				printf("No arguments required. Please try again.\n");
			else
				opcodeList(command, &historyL, opcodeT);
		}
		// 11. assemble 명령어 수행
		else if (!strcmp("assemble", command)) {
			if (argCount != 1) // argument가 1개가 아닐 경우 : 예외처리
				printf("Please check the arguments.\n");
			else
				assemble(command, first, &historyL, opcodeT);
		}
		// 12. type 명령어 수행
		else if (!strcmp("type", command)) {
			if (argCount != 1) // argument가 1개가 아닐 경우 : 예외처리
				printf("Please check the arguments.\n");
			else
				type(command, first, &historyL);
		}
		// 13. symbol 명령어 수행
		else if (!strcmp("symbol", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리
				printf("No arguments required. Please try again.\n");
			else
				symbol(command, &historyL);
		}
		// 14. progaddr 명령어 수행
		else if (!strcmp("progaddr", command)) {
			if (argCount != 1) // argument가 1개가 아닐 경우 : 예외처리
				printf("Please check the arguments.\n");
			else
				progaddr(command, first, &historyL);
		}
		// 15. loader 명령어 수행 
		else if (!strcmp("loader", command)) {
			memset(first, '\0', MAX_STR);
			memset(second, '\0', MAX_STR);
			memset(third, '\0', MAX_STR);
			argCount = sscanf(arguments, "%s %s %s", first, second, third);
			if (argCount > 3) // argument가 3개 이상일 경우 : 예외처리
				printf("There are many object files.\n");
			else if (argCount <= 0) // argument가 1개 이하일 경우 : 예외처리
				printf("Please check the arguments.\n");
			else {
				loader(argCount, command, first, second, third, &historyL);
			}
		}
		// 16. run 명령어 수행
		else if (!strcmp("run", command)) {
			if (argCount != 0) // argument가 있을 경우 : 예외처리
				printf("No arguments required. Please try again.\n");
			else
				run(); // 구현하지 못함.. 
		}
		// 17. bp
		else if (!strcmp("bp", command)) {
			if (argCount > 1) // argument가 1개 이상일 경우 : 예외처리
				printf("Please check the arguments.\n");
			else
				bp(command, first, &historyL, &breakP);
		}
		// 18. 그 외 
		else {
			printf("Please check command.\n");
		}
		getchar();
	}

	return 0;

}

void refineCommand(char* input, char* command, char* arguments) {

	int i = 0, j = 0, index = 0;
	int len = strlen(input);		// input의 길이 

	// 앞에 있는 whitespace 삭제 
	while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;

	for (; i < len; i++) {
		// 만약 끝 지점을 만난다면 반복문을 빠져나간다. 
		if (input[i] == '\0' || input[i] == '\n') break;
		// 중간에 whitespace를 만나면 그 부분부터 argument 배열에 저장한다. 
		if (input[i] == ' ' || input[i] == '\t') {
			while (i < len) {
				arguments[j++] = input[i]; i++;
			}
		}
		// input에 저장된 값을 command에 저장한다. 
		command[index++] = input[i];
	}
}

int refineArgument(char* arguments, char* first, char* second, char* third) {

	int args = 1;					// 인자 값 
	int index = 0;					// 새로운 배열에 저장할 때 필요한 인덱스  
	int i = 0, comma = 0;			// comma 수 저장 
	int len = strlen(arguments);	// arguments 배열의 길이 

	// 앞에 있는 whitespace 삭제
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;

	/* 앞의 white space를 제외한 값 이외에 argument에 저장된 값이 없다면,
	   인자가 없는 것이므로 0을 반환한다. */
	if (i >= len)  return 0;

	// 첫 번째 argument를 first 배열에 저장한다. 
	index = 0;
	for (; i < len && args == 1; i++) {
		// 중간에 콤마를 만나면 인자와 콤마 값을 증가한다. 
		if (arguments[i] == ',') {
			args++;	comma++;
		}
		// 중간에 whitespace를 만나면 인자 값만을 증가한다. 
		else if (arguments[i] == ' ' || arguments[i] == '\t') args++;
		// 그 외에는 first 배열에 복사한다. 
		else
			first[index++] = arguments[i];
	}

	/* 인자 값이 바뀌면서 반복문을 빠져나오게 되고 중간에 2번째 인자 값을 담기
	 위해서 중간에 있는 whitespace를 제외하고, 중간에 콤마를 만나면 콤마 수를
	 증가 시킨다. 콤마 이후에 whitespace도 제외시키는 작업을 수행한다. */
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;
	if (arguments[i] == ',') {
		comma++; i++;
	}
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;

	// 두 번째 argument를 second 배열에 저장한다.(first와 동일) 
	index = 0;
	for (; i < len && args == 2; i++) {
		if (arguments[i] == ',') {
			args++;	comma++;
		}
		else if (arguments[i] == ' ' || arguments[i] == '\t') args++;
		else
			second[index++] = arguments[i];
	}

	// first 밑에 작업과 동일	
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;
	if (arguments[i] == ',') {
		comma++; i++;
	}
	while (i < len && (arguments[i] == ' ' || arguments[i] == '\t')) i++;

	// 세 번째 arument를 third 배열에 저장한다 (first와 동일)  
	index = 0;
	for (; i < len && args == 3; i++) {
		if (arguments[i] == ',') {
			args++;	comma++;
		}
		else if (arguments[i] == ' ' || arguments[i] == '\t') 	args++;
		else
			third[index++] = arguments[i];
	}

	/* 인자와 콤마수가 알맞게 들어가면서 인자가 3개 이하 일때,
	지금까지 저장된 args 값을 반환한다. */
	if (args < 4 && comma + 1 == args) return args;
	// 그 외는 잘못된 값이므로 -1를 반환한다. 
	else return -1;

}

/* ============ shell 관련 함수 ============ */

/* 1. help : 실행가능한 모든 명령어 출력*/
void help(char* input, HistoryList* historyL) {

	writeHistory(input, &historyL);		// history List에 저장 

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
	printf("progaddr address\n");
	printf("loader object filename1 [object filename2] [object filename3]\n");
	printf("run\n");
	printf("bp [address]\n");

	return;

}

/* 2. dir : 현재 디렉터리의 파일 출력*/
void dir(char* input, HistoryList* historyL) {
	writeHistory(input, &historyL);		// history List에 저장

	DIR* di;
	struct dirent* direntPtr;
	struct stat info;
	int line = 0;
	char name[100];

	di = opendir(".");

	while ((direntPtr = readdir(di)) != NULL) {
		strcpy(name, direntPtr->d_name);

		// '.' , '..' 파일의 경우에는 출력을 생략한다.
		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
			continue;
		lstat(name, &info);
		printf("%20s", name);
		line++;

		// 디렉터리의 경우에는 파일이름 끝에 '/' 을 붙인다.
		if (S_ISDIR(info.st_mode))
			printf("/");
		// 실행파일의 경우에는  파일이름 끝에 '*' 을 붙인다.
		else if (S_IEXEC & info.st_mode)
			printf("*");
		if (line % 3 == 0)
			printf("\n");

	}

	printf("\n");
	closedir(di);

}

/* 3. quit : 종료 =====  수정필요 */
void quit(HistoryList* historyL, OpcodeTable** opcodeT) {

	deleteHistory(&historyL);		// 저장된 history linked list를 삭제한다. 
	freeOpcode(opcodeT);			// 저장된 opcode table을 삭제한다.  
	freeSymtab(&symTab);			// 저장된 symbol table을 삭제한다.

}

/* 4. history : 사용한 명령어를 정제해서 linked list에 저장한다. */
void history(char* input, HistoryList* historyL) {

	writeHistory(input, &historyL);		// history List에 저장
	printHistory(historyL);				// 저장된 history를 출력한다. 

}

/* 4-1. history 사용 명령어 저장 */
void writeHistory(char* content, HistoryList** historyL) {

	// 내용을 저장한 새로운 노드형성
	HistoryNode* insertNode;
	insertNode = (HistoryNode*)malloc(sizeof(HistoryNode));
	insertNode->next = NULL;
	strcpy(insertNode->input, content);

	// Linked List 연결
	// head 가 비어있다면 새로운 head로 지정 
	if ((*historyL)->head == NULL) {
		(*historyL)->head = (*historyL)->curr = insertNode;
	}
	// 현재 노드 next로 새 노드 연결 
	else {
		(*historyL)->curr->next = insertNode;
		(*historyL)->curr = insertNode;
	}

	return;

}

/* 4-2. history 현재까지 사용한 명령어 리스트 출력*/
void printHistory(HistoryList* historyL) {

	int i = 1;
	HistoryNode* tmpNode = historyL->head;	// 프린트하기 위해 임시 history node pointer를 head로 지정

	// node가 null 아닐때까지 계속해서 출력한다. 
	while (tmpNode != NULL) {
		printf("%-4d %s\n", i++, tmpNode->input);
		tmpNode = tmpNode->next;
	}
	return;

}

/* 4-3. history list 삭제*/
void deleteHistory(HistoryList** historyL) {

	HistoryNode* deleteNode;	 				// 삭제할 histroy node pointer
	HistoryNode* tmpNode = (*historyL)->head; 	// 임시 history node pointer를 head로 지정

	// node가 null 아닐때까지 삭제. 
	while (tmpNode != NULL) {
		deleteNode = tmpNode;
		tmpNode = deleteNode->next;
		free(deleteNode);
	}

	return;

}

/* ============ memory 관련 함수 ============ */

/* 1. reset : memory를 초기하는 함수. */
void reset(unsigned char(*memory)[COL_SIZE]) {

	int i, j;

	// 모든 메모리 0으로 초기화 
	for (i = 0; i < ROW_SIZE; i++)
		for (j = 0; j < COL_SIZE; j++)
			memory[i][j] = 0;

	return;
}

/* 2. dump : 메모리를 출력한다.
		 - dump : 인자가 없을 경우, 기본적으로 10 line을 출력한다.
		 - dump start : 인자가 하나인 경우, start의 번지부터 10 line을 출력한다.
		 - dump start , end : 인자가 2개인 경우, start부터 end까지 출력한다. */
void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address) {

	int start, end;				// 시작과 끝 index 

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
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

	// argument가 0일 경우 
	if (argCount == 0) {

		start = *address;	// 시작은 address에 저장된 값 불러오기
		// 10라인을 더한 address가 0xfffff를 초과할 경우 oxfffff까지 출력 
		end = *address + 159 > 0xfffff ? 0xfffff : *address + 159;

		writeHistory(input, &historyL);			// history에 저장 
		printMemory(memory, start, end);		// memory의 값 출력 

		// 10라인을 더한 address가 0xfffff를 초과할 경우 0으로 reset
		*address = (end + 1 > 0xfffff) ? 0 : end + 1;

	}
	// argument가 1일 경우 
	else if (argCount == 1) {

		if (checkRange(first) == FALSE) // argument가 범위내에 있지 않을 때
			printf("Please check the arguments.\n");
		else {
			sscanf(first, "%x", &start);	// 16진수로 변환
			// start에서 10라인이 범위를 벗어날 경우 마지막 범위까지 출력 
			end = ((((start / COL_SIZE) + 10) * 16) - 1) > 0xfffff ? 0xfffff : ((((start / COL_SIZE) + 10) * 16) - 1);

			// 시작 범위가 0 ~1048575 이내 인지 확인
			if (start < 0x00 || start > 0xfffff)
				printf("Memory overflow.\n");
			else {

				writeHistory(input, &historyL); // history에 저장 
				printMemory(memory, start, end); // memory의 값 출력 

			}
		}
	}
	// argument가 2일 경우
	else if (argCount == 2) {

		if (checkRange(first) == FALSE || checkRange(second) == FALSE) // argument가 범위내에 있지 않을 때
			printf("Please check the arguments.\n");
		else {
			// 16진수로 변환
			sscanf(first, "%x", &start);
			sscanf(second, "%x", &end);

			// 시작과 끝 범위가 0 ~1048575 이내 인지 확인
			if (start < 0x00 || start > 0xfffff || end < 0x00 || end > 0xfffff)  // 0 ~1048575 범위 초과시
				printf("Memory overflow.\n");
			// 끝이 시작보다 클 경우 
			else if (start > end)
				printf("Start address is greater than end address!.\n");
			else {
				writeHistory(input, &historyL); // history에 저장 
				printMemory(memory, start, end); // memory의 값 출력 
			}
		}
	}
	return;
}

/* 2-1. dump을 실행시 메모리의 값을 프린트한다. */
void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end) {

	int sRow, sCol; 	// 시작 행, 시작 열 
	int eRow, eCol;		// 끝 행, 끝 열 
	int i, j;

	sRow = start / COL_SIZE;	// 시작 행은 16을 나눈 몫 
	sCol = start % COL_SIZE;	// 시작 열은 16을 나눈 나머지  
	eRow = end / COL_SIZE;		// 끝의 열은 16을 나눈 몫 
	eCol = end % COL_SIZE;		// 끝의 열은 16을 나눈 나머지 

	for (i = sRow; i <= eRow; i++) {

		// address 부분 출력 
		printf("%05X ", i * 16);

		// memory 내용 출력 
		for (j = 0; j < 16; j++) {
			// 시작의 행이면서 시작의 열보다 작을 경우 
			// 끝의 행이면서 끝의 열보다 클 경우 
			if ((i == sRow && j < sCol) || (i == eRow && j > eCol))
				printf("   "); // 빈 공간 출력 
			else // 나머지는 메모리의 값 출력 
				printf("%02X ", memory[i][j]);
		}

		// 메모리 내용을 ASCII 코드 대응
		printf(";");
		for (j = 0; j < 16; j++) {
			printf("%c", asciiCode(memory[i][j]));
		}

		printf("\n");
	}
	return;

}

/* 2-2. memory의 내용을 아스키코드로 변환하는 작업을 수행한다. */
char asciiCode(int hex) {

	// 0x20 ~ 0x7E 범위내 출력
	if (hex >= 0x20 && hex <= 0x7E)
		return hex;
	else // 그 외
		return '.';

}

/* 3. edit : 첫번째 인자(주소)에 두번째 인자(value)로 해당 메모리 값을 수정한다. */
void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]) {

	int address, value;		// 주소와 변경 값 
	int col, row;			// 행렬 

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR];

	// argument들이 적절한 범위 내에 없을 경우 - 예외 처리 
	if (checkRange(first) == FALSE || checkRange(second) == FALSE)
		printf("Please check the arguments.\n");
	else {

		// 16진수로 변환
		sscanf(first, "%x", &address);
		sscanf(second, "%x", &value);

		// 주소 값이 0 ~ 1048575 범위 초과시
		if (address < 0x00 || address > 0xfffff)
			printf("Memory overflow.\n");
		// 값의 범위가 0~255 범위 초과시 
		else if (value < 0x00 || value > 0xff)
			printf("Please check the value.\n");
		// 정상을 경우	
		else {
			if (command != NULL) {
				strcpy(input, command);
				strcat(input, " ");
				strcat(input, first);
				strcat(input, ", ");
				strcat(input, second);
				writeHistory(input, &historyL); // history 기록 
			}
				
			row = address / COL_SIZE;		// 주소를 16 나눈 몫 
			col = address % COL_SIZE;		// 주소를 16 나눈 나머지 
			memory[row][col] = value;		// 해당 memory 수정 
		}
	}

	return;

}

/* 4. fill : 첫번째 인자(시작)과 두번째 인자(끝) 사이를 세번째 인자(값)으로 수정한다. */
void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE]) {

	int start, end, value;	// 시작, 끝, 변경 값 

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);
	strcat(input, ", ");
	strcat(input, second);
	strcat(input, ", ");
	strcat(input, third);

	// argument들이 적절한 범위 내에 없을 경우 - 예외 처리 
	if (checkRange(first) == FALSE || checkRange(second) == FALSE || checkRange(third) == FALSE)
		printf("Please check the arguments.\n");
	else {

		// 16진수로 변환
		sscanf(first, "%x", &start);
		sscanf(second, "%x", &end);
		sscanf(third, "%x", &value);

		// 주소 값이 0 ~ 1048575 범위 초과시
		if (start < 0x00 || start > 0xfffff || end < 0x00 || end > 0xfffff)  // 0 ~1048575 범위 초과시
			printf("Memory overflow.\n");
		// 끝이 시작보다 클 경우 - 예외처리	
		else if (start > end)
			printf("Start address is greater than end address!.\n");
		// 값의 범위가 0~255 범위 초과시 	
		else if (value < 0x00 || value > 0xff)
			printf("Please check the value.\n");
		else {
			writeHistory(input, &historyL);			// history 기록
			fillMemory(memory, start, end, value);	// fillmemory 함수 호출 
		}
	}

	return;

}

/* 4-1. fill 명령어 수행시 직접 내용을 변경하는 함수 */
void fillMemory(unsigned char(*memort)[COL_SIZE], int start, int end, int value) {

	int row, col, i; 	// 행, 열 

	for (i = start; i <= end; i++) {
		// 행과 열로 변환 
		row = i / COL_SIZE;
		col = i % COL_SIZE;
		// 값 수정 
		memory[row][col] = value;
	}

	return;

}

/* 5. memory 관련 함수 중 dump,deit,fill에 입력된 값이 범위내에 있는지 확인 */
int checkRange(char* arg) {

	int i, len = strlen(arg) - 1;
	for (i = 0; i < len; i++) {
		if (arg[i] >= 0x30 && arg[i] <= 0x39) {} // 0 ~ 9
		else if (arg[i] >= 0x41 && arg[i] <= 0x46) {} // A ~ F
		else if (arg[i] >= 0x61 && arg[i] <= 0x66) {} // a ~ f
		else { return FALSE; }
	}
	return TRUE;
}

/* ============ opcode 관련 함수 ============ */

/* 1. Opcode.text를 읽는 작업을 수행 */
void readOp(OpcodeTable** opTable) {

	int i, index;			// opcode table의 인덱스를 저장할 
	char tmp[32];			// 임시문자열 
	OpcodeNode* tmpNode;	// opcodeNode pointer 생성	
	FILE* fp;

	// 20개 opcode를 저장할 20개의 Hash table을 만든다. 
	*opTable = (OpcodeTable*)malloc(sizeof(OpcodeTable) * TABLE_SIZE);

	// opcode Hash Table 초기화 
	for (i = 0; i < TABLE_SIZE; i++) {
		(*opTable)[i].cnt = 0;
		(*opTable)[i].node = NULL;
	}

	fp = fopen("opcode.txt", "r");

	// 파일이 없을 경우 - 예외 처리  
	if (fp == NULL) {
		printf("There is no opcode.text!\n");
		exit(-1);
	}

	while (!feof(fp)) {
		fgets(tmp, 32, fp);

		// 불러올 문자열이 없으면 반복문을 빠져나간다. 
		if (strlen(tmp) == 0) break;

		// 새로운 값을 저장할 opcode 생성 
		tmpNode = (OpcodeNode*)malloc(sizeof(OpcodeNode));
		tmpNode->next = NULL;

		// 적절한 타입으로 변환하여 opcode는 16진수 , 명령어는 문자열, form은 정수로 저장  
		sscanf(tmp, "%x %s %d", &tmpNode->opcode, tmpNode->mnemonic, &tmpNode->form);

		/* 나중에 mnemonic으로 opcode를 찾을 수 있게
		명령어 문자들을 더하여 20으로 나눈 나머지를 인덱스로 구함*/
		index = 0;
		for (i = 0; i < (int)strlen(tmpNode->mnemonic); i++)
		{
			index += tmpNode->mnemonic[i];
		}
		index = index % TABLE_SIZE;
		tmpNode->index = index;

		// opcode Table의 인덱스에 연결함. 
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

/* 2. Opcode mnemonic을 입력하면 출력해주는 함수 */
void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT) {

	int opcode;	 	// opcode를 저장 

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);

	// opcode를 찾는 함수 실행 
	opcode = findOpcode(opcodeT, first);

	// opcode가 없을 경우 - 예외처리 
	if (opcode < 0)
		printf("Please check mnemonic\n");
	else {
		writeHistory(input, &historyL); 	// history link에 저장 
		printf("opcode is %02X\n", opcode); // opcode 출력 
	}
}

/* 2-1. mnemonic이 저장된 opcodeTable에 접근하는 함수 */
int findOpcode(OpcodeTable* opcodeT, char* first) {

	int i, index = 0;		// 인덱스 저장 
	OpcodeNode* tmp;		// opcodenode pointer 생성 

	// 인덱스 값 찾기: 저장하는 방법과 동일 
	for (i = 0; i < (int)strlen(first); i++)
		index += first[i];
	tmp = opcodeT[index % TABLE_SIZE].node;

	// opcode 해당 index의 모든 노드 탐색 
	while (TRUE) {
		// 동일한 명령어가 있으면 해당 opcode를 반환 
		if (!strcmp(first, tmp->mnemonic))
			return tmp->opcode;

		if (tmp->next != NULL)
			tmp = tmp->next;
		else
			break;
	}
	return -1; // 실패
}

/* 3. opcodelist 출력하는 함수 */
void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable) {

	int i;
	OpcodeNode* tmp;	// opcode node pointer 생성 

	writeHistory(command, &historyL);

	// opcode table 출력   
	for (i = 0; i < TABLE_SIZE; i++) {
		tmp = opTable[i].node;
		printf("%2d : ", i);

		// opcode table 해당 인덱스에 모든 노드를 출력   
		while (TRUE) {
			// 모든 노드 탐색이 끝나면 종료 
			if (tmp == NULL) break;

			// 명령어와 opcode 출력 
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

/* 4. opcode table 을 삭제하는 함수 */
void freeOpcode(OpcodeTable** opTable) {

	int i;
	OpcodeNode* deleteN;	// 삭제할 opcodenode pointer 생성

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

/* ============ type 관련 함수 ============ */

/* 1. printType 함수 출력과 히스토리 저장하는 작업 */
void type(char* command, char* first, HistoryList* historyL) {

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);

	// printType 함수를 불러온다.
	if (printType(first) == -1) return;

	writeHistory(input, &historyL); 	// history link에 저장 
}

/* 2. 파일에 있는 내용을 출력 */
int printType(char* filename) {

	FILE* fp = fopen(filename, "r");			// 파일 읽기 위한 파일 포인터
	char  buffer[MAX_BUFR];					// 파일 내용을 저장하는 임시배열

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

/* ============ symbol 관련 함수 ============ */

/* 1. symbol 함수 호출과 히스토리 저장하는 작업 */
void symbol(char* command, HistoryList* historyL) {

	int i, count = 0;									// symbol Table에 저장된 symbol node의 개수
	writeHistory(command, &historyL); 	// history link에 저장 

	// 전역변수인 symbol Table 비어있을 경우 에러처리한다. 
	if ((symTab) == NULL) {
		printf("SYMTAB is empty.\n");
		return;
	}
	else {
		// symbol의 개수를 센다.
		for (i = 0; i < TABLE_SIZE; i++)
			count += (symTab)[i].cnt;
		// symbol의 개수가 0보다 클 경우 print 함수를 호출한다. 
		if (count > 0) {
			printSymbol(count);
			return;
		}
	}
}

/* 2. symbol table을 초기화하는 함수 */
void initSymbol(SymbolTable** symtab) {

	int i;
	*symtab = (SymbolTable*)malloc(sizeof(SymbolTable) * TABLE_SIZE);

	// symbol Hash Table 초기화 
	for (i = 0; i < TABLE_SIZE; i++) {
		(*symtab)[i].cnt = 0;
		(*symtab)[i].node = NULL;
	}
	return;
}

/* 3. symbol table에 symbol을 추가하는 함수 */
int makeSymbol(char* label, int location, SymbolTable** symtab) {

	int i, index;							// hash index 를 저장
	SymbolNode* tmpNode;	// 새로운 값을 저장할 SymbolNode pointer 생성
	SymbolNode* findNode;	// symTab의 전 노드를 탐색할 SymbolNode pointer 생성

	// symbol node에 새로운 값을 저장  
	tmpNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	tmpNode->next = NULL;
	strcpy(tmpNode->label, label);
	tmpNode->location = location;

	/* 나중에 label으로 symbol를 찾을 수 있게
	label 문자들을 더하여 20으로 나눈 나머지를 인덱스로 구함*/
	index = 0;
	for (i = 0; i < (int)strlen(label); i++) {
		index += label[i];
	}
	index = index % TABLE_SIZE;
	tmpNode->index = index;

	// 새로운 symbol이 기존에 있는지 확인
	for (findNode = (*symtab)[index].node; findNode != NULL; findNode = findNode->next) {
		if (!strcmp(label, findNode->label)) {
			printf("%s is Duplicate symbol.\n", label);
			return -1;
		}
	}

	// 정상일 경우 symTab의 새로운 노드를 추가한다.
	(*symtab)[index].cnt++;
	if ((*symtab)[index].node == NULL) {	// 기존에 저장된 node가 없는 경우
		(*symtab)[index].node = tmpNode;
	}
	else {
		tmpNode->next = (*symtab)[index].node;
		(*symtab)[index].node = tmpNode;
	}
	return 0;
}

/* 4. symbol table에 해당 symbol을 찾는 함수 */
int findSymbol(char* label, SymbolTable* symtab) {

	SymbolNode* findNode;		// symTab의 전 노드를 탐색할 SymbolNode pointer 생성
	int i, index = 0;						// label의 문자열을 계산한 index( symTab 접근할 key)

	/* 나중에 label으로 symbol를 찾을 수 있게
	label 문자들을 더하여 20으로 나눈 나머지를 인덱스로 구함*/
	for (i = 0; i < (int)strlen(label); i++) {
		index += label[i];
	}
	index = index % TABLE_SIZE;

	// symbol table을 순회하여 label의 주소값을 반환한다.
	for (findNode = symtab[index].node; findNode != NULL; findNode = (findNode)->next) {
		if (!strcmp(label, (findNode)->label)) {
			return (findNode)->location;
		}
	}
	return -1;
}

/* 5. symbol table을 모든 노드를 label 알파벳 오름차순으로 정렬후 출력하는 함수 */
void printSymbol(int count) {

	int i, index = 0;				// 임시 symbol node의 index
	SymbolNode* temp;		// symTab의 전 노드를 탐색할 SymbolNode pointer 생성
	// symbol 노드의 개수만큼 메모리를 할당한 정렬을 위한 symbol node
	SymbolNode* desSym = (SymbolNode*)malloc(sizeof(SymbolNode) * count);

	// symTab의 모든 노드를 순회하며 desSym node에 저장한다.
	for (i = 0; i < TABLE_SIZE; i++) {
		temp = (symTab)[i].node;
		while (temp != NULL) {
			strcpy(desSym[index].label, temp->label);
			desSym[index++].location = temp->location;
			temp = temp->next;
		}
	}

	// demSym node를 label의 알파벳 오름차순으로 정렬
	qsort(desSym, count, sizeof(desSym[0]), compare);

	// 정렬된 symbol node를 출력한다.
	for (i = 0; i < count; i++)
		printf("\t%s\t%04X\n", desSym[i].label, desSym[i].location);

	// 임시로 사용된 desSym node들을 삭제한다.
	free(desSym);
	return;
}

/* 6. symbol table 오름차순 정렬을 위한 기준 */
int compare(const void* a, const void* b) {

	// 비교를 위한 변수
	SymbolNode A = *(SymbolNode*)a;
	SymbolNode B = *(SymbolNode*)b;

	// 오름차순 정렬
	return strcmp(A.label, B.label);
}

/* 7. symbol table를 free 해주는 함수 */
void freeSymtab(SymbolTable** symtab) {

	int i;
	SymbolNode* deleteN;		// 삭제할 symbol node pointer 생성

	// 만약 symbol table이 비어있을 경우 해당 함수를 빠져나간다.
	if (*symtab == NULL)
		return;

	// 아닐 경우 모든 table을 순회하며 삭제한다.
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

/* ============ assemble 관련 함수 ============ */

/* 1. assemble - pass1, pass2 과정실행 */
void assemble(char* command, char* first, HistoryList* historyL, OpcodeTable* opTable) {

	int i, j = 0;
	char input[MAX_STR] = { '\0', };			// history에 저장하기 위한 명령어 저장배열
	char filename[MAX] = { '\0', };			// argument를 file name을 분리해서 저장하는 배열
	char type[MAX] = { '\0', };					// argument를 file type으로 분리해서 저장하는 배열

	// pass 2에 필요한 것들
	char progname[MAX] = { '\0', };		// program name 저장
	int start, length;										// object program의 start address과 byte length
	SymbolTable* symtab;							// pass1 과정에서 저장하게될 symbol table

	// 첫번째 인자를 file name 과 type으로 분리하는 과정
	for (i = 0; i < (int)strlen(first); i++) {
		if (first[i] == '.') break;
		filename[i] += first[i];
	}
	for (++i; i < (int)strlen(first); i++) {
		type[j++] += first[i];
	}

	// assemble 과정
	// assemble 파일이 asm일 경우 - pass1 , pass2 실행
	if (!strcmp(type, "asm")) {
		// pass1 과정 실행
		if (pass1(first, opTable, &symtab, progname, &start, &length) == 0) {
			// pass2 과정 실행
			if (pass2(filename, opTable, symtab, start, length, progname) != -1) {
				// 성공할 경우
				printf("[%s.lst], [%s.obj]\n", filename, filename);
			}
			else {
				// pass 2 실패할 경우, intermediate 파일인 pass1.txt 삭제
				remove("pass1.txt");
				freeSymtab(&symtab);			// pass1 에 만든 symbol table 삭제
				return;
			}
		}
		else return;
	}
	else {	// assemble 파일이 asm 아닐 경우 에러메세지 출력
		printf("This file type is not 'asm'. please check again.\n");
		return;
	}

	// pass1 , pass2 과정에 성공할 경우
	freeSymtab(&symTab);			// 전역변수에 저장된 symbol table 삭제
	symTab = symtab;					// pass1 과정에서 만든 symbol table을 전연변수로 저장
	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);
	writeHistory(input, &historyL); 	// history link에 저장 
}

/* 2. pass1 과정을 통해서 중간 파일과 symbol table 저장, programe name과 start 주소, length 계산 */
int pass1(char* filename, OpcodeTable* opTable, SymbolTable** symtab, char* progname, int* start, int* length) {

	FILE* fp = fopen(filename, "r");					// 파일 읽기 위한 파일 포인터
	FILE* fwp = fopen("pass1.txt", "w");			// 파일을 쓰기 위한 파일 포인터

	char input[MAX_BUFR];								// 파일 내용을 저장하는 임시배열
	// 분리된 파일 내용을 저장할 임시배열 
	char tmp1[MAX] = { '\0', }, tmp2[MAX] = { '\0', }, tmp3[MAX] = { '\0', };
	int args = 0, line = 0;									// argument 개수와 라인 수
	int locCurr = 0, locNext = 0;						// 현재 주소와 byte를 더한 다음 주소

	initSymbol(symtab);

	// 파일이 없을 경우 에러 메세지 출력 
	if (fp == NULL) {
		printf("%s does not exist!\n", filename);
		fclose(fp);
		fclose(fwp);
		remove("pass1.txt");			// 중간 파일 삭제
		return -1;
	}

	/* start 이전에 주석처리와 start 이전에 명령어 나올시 에러 처리 */
	while (fgets(input, MAX_BUFR, fp)) {
		if (input[0] == '.') {				// 주석처리
			line += 5;
			fprintf(fwp, "%d\t%s", line, input);
		}
		else {
			memset(tmp1, '\0', MAX);
			memset(tmp2, '\0', MAX);
			memset(tmp3, '\0', MAX);
			args = sscanf(input, "%s %s %s", tmp1, tmp2, tmp3);
			if (args == -1)		 // 빈 공간일 경우 넘어감 
				continue;
			else if (!strcmp(tmp2, "START") && args == 3) {		// start를 만날 경우 주소 중간파일 쓰고 넘어감 
				locNext = atoi(tmp3);												// loc 저장
				line += 5;
				*start = locNext;														// start 주소 저장
				strcpy(progname, tmp1);										// program name 저장
				// 중간 파일에 쓰기
				fprintf(fwp, "%4d\t%04X\t%-7s\t%-7s\t%-10s\t\n", line, locNext, tmp1, tmp2, tmp3);
				break;
			}
			else {
				// 에러날 경우
				fclose(fp);
				fclose(fwp);
				remove("pass1.txt");
				printf("There is other opcode before start. - pass1\n");
				return -1;
			}
		}
	}

	// start 이후 부터 처리
	while (fgets(input, MAX_BUFR, fp)) {
		if (input[0] == '.') {		// 주석 처리
			line += 5;
			fprintf(fwp, "%4d\t%s", line, input);
		}
		else {
			memset(tmp1, '\0', MAX);
			memset(tmp2, '\0', MAX);
			memset(tmp3, '\0', MAX);
			args = sscanf(input, "%s %s %s", tmp1, tmp2, tmp3);
			if (args == -1) // 빈 공간 
				continue;
			else if (args == 3) {
				locCurr = locNext;
				// operand가 2개 일 경우
				if (tmp2[strlen(tmp2) - 1] == ',') {
					// 다음 주소 저장
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
				if (!strcmp(tmp1, "END")) {			// diretive end 처리
					line += 5;
					*length = locNext - (*start);		// 프로그램 길이 저장
					fprintf(fwp, "%4d\t\t\t%-7s\t%-10s\t\n", line, tmp1, tmp2);
					fclose(fp);
					fclose(fwp);
					return 0;
				}
				else if (!strcmp(tmp1, "BASE")) {			// diretive base 처리
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

	// 프로그램 에러 실패
	fclose(fp);
	fclose(fwp);
	remove("pass1.txt");
	printf("- pass 1 failed.\n");
	return -1;
}

/* 3. pass2 과정을 통해서 object code를 계산하고 lst 파일과 obj 파일을 만든다. */
int pass2(char* filename, OpcodeTable* opTable, SymbolTable* symtab, int start, int length, char* progname) {

	FILE* fp = fopen("pass1.txt", "r");		// 파일을 읽기 위한 포인터
	FILE* fwlst;											// lst 파일을 쓰기 위한 포인터
	FILE* fwobj;											// obj 파일을 쓰기 위한 포인터

	// modification을 위한 list 생성
	ModifList* modiflist = (ModifList*)malloc(sizeof(ModifList*));
	modiflist->head = NULL;
	modiflist->curr = NULL;
	int immflag = 0;						// immediate flag, 1일 경우 이미 상수인 것므로 modiflist에 넣지않는다

	char lstfile[MAX] = { '\0', };					// filename과 lst 확장자더한 이름을 저장할 배열 
	char objfile[MAX] = { '\0', };				// filename과 obj 확장자더한 이름을 저장할 배열 
	char buffer[MAX_BUFR] = { '\0' , };		//	파일 내용을 저장하는 임시배열


	// 파일 내용을 분리하여 저장하는데 필요한 배열
	char line[7], loc[7];								// 내용에 line과 loc이 저장
	// 임시배열
	char tmp1[MAX] = { '\0', }, tmp2[MAX] = { '\0', };
	char tmp3[MAX] = { '\0', }, tmp4[MAX] = { '\0', };
	char* oper1, * oper2;							// operand가 두 개일 경우 저장할 배열 포인터
	char* content;										// '.' 주석을 내용을 저장할 배열 포인터

	int args, format;										// 인자의 개수와 format 형식
	int opc, baseReg = 0;								// opcode 저장과 base register 주소값

	// obj 파일을 작성하기 위한 변수
	int byte = 0, startAddr, startflag = 0;		// 바이트 수와, obj 시작주소, 시작주소 변경 flag (0일 경우 현재 loc을 저장한다.)
	char objectcode[10] = { '\0', };				// opc를 objectcode 배열에 임시저장
	char objectline[MAX_STR] = { '\0', };		// object line에 저장후 바이트 수가 차면 obj 파일에 쓴다.

	// 파일을 열 수 없을 경우 - 에러 처리
	if (fp == NULL) {
		printf("'pass1.txt' file does not exist.\n");
		return -1;
	}

	// lst와 obj 파일+ 확장자 입력
	strcpy(lstfile, filename);
	strcat(lstfile, ".lst");
	strcpy(objfile, filename);
	strcat(objfile, ".obj");

	// 파일 포인터에 연결
	fwlst = fopen(lstfile, "w");
	fwobj = fopen(objfile, "w");

	// start 부분찾기
	while (fgets(buffer, MAX_BUFR, fp)) {
		memset(line, '\0', 7);
		memset(loc, '\0', 7);
		memset(tmp1, '\0', MAX);
		memset(tmp2, '\0', MAX);
		memset(tmp3, '\0', MAX);
		args = sscanf(buffer, "%s\t%s\t%s\t%s\t%s", line, loc, tmp1, tmp2, tmp3);
		if (loc[0] == '.') {		// 주석처리
			strtok(buffer, ".");
			content = strtok(NULL, "");
			fprintf(fwlst, "%4s\t\t  %s\n", line, content);
			continue;
		}
		else if (!strcmp("START", tmp2)) {
			startAddr = start;			// obj 시작 주소 저장
			startflag = 1;					// 시작 주소를 저장할 필요없는 flag , 0일 경우 현재 loc 저장
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

	// start 이후
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
		if (loc[0] == '.') {	// 주석처리
			strtok(buffer, ".");
			content = strtok(NULL, "");
			fprintf(fwlst, "%4s\t\t  %s", line, content);
			continue;
		}
		if (args == 3) {
			if (!strcmp("BASE", loc)) {	// direvtive BASE 처리
				baseReg = findSymbol(tmp1, symtab);
				fprintf(fwlst, "%s", buffer);
				continue;
			}
			else if (!strcmp("END", loc)) { // direvtive END 처리
				// obj , lst 파일 쓰기
				fprintf(fwlst, "%s", buffer);
				fprintf(fwobj, "T%06X%02X%s\n", startAddr, byte, objectline);
				// modfi obj 파일에 작성
				fileModifynode(fwobj, modiflist);
				fprintf(fwobj, "E%06X\n", start);
				freeModifynode(&modiflist);			// modfiynode free
				fclose(fp);
				fclose(fwobj);
				fclose(fwlst);
				remove("pass1.txt");
				return 0;
			}
			else {	// BASE, END도 아닌 경우 ( 인자가 3개인 경우)
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
		else if (args == 4) { // 인자가 4개인 경우
			format = findFormat(opTable, tmp1);
			oper1 = strtok(tmp2, ",");
			oper2 = strtok(NULL, "\n");
			opc = findObject(tmp1, oper1, oper2, loc, baseReg, format, opTable, symtab, &immflag);
			if ((format != -1) && opc != -1) {
				strcpy(tmp3, oper1);
				if (oper2 != NULL) {
					strcat(tmp3, ", ");
					strcat(tmp3, oper2);
				}
				// 형식에 따라 lst 구분자 다르게 저장
				if (format == 4)
					fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t%-10s\t%08X\n", line, loc, tmp1, tmp3, opc);
				else if (format == 3)
					fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t%-10s\t%06X\n", line, loc, tmp1, tmp3, opc);
				else
					fprintf(fwlst, "%4s\t%4s\t       \t%-7s\t%-10s\t%X\n", line, loc, tmp1, tmp3, opc);
			}
			else break;
		}
		else if (args == 5) { // 인자가 5개인 경우
			if (!strcmp(tmp2, "RESW") || !strcmp(tmp2, "RESB")) {
				fprintf(fwlst, "%s", buffer);		// 그대로 저장
				if (byte > 0) {		// obj가 새로 시작하는 부분이 아닐 경우 obj를 작성한다.
					fprintf(fwobj, "T%06X%02X%s\n", startAddr, byte, objectline);
					byte = 0;				// byte 수 0으로 초기화
					startflag = 0;		// start address flag를 0으로 변경한다.
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
				else {	// 범위 초과시
					printf("WORD range overflow.");
					break;
				}
			}
			if (!strcmp(tmp2, "BYTE")) {
				opc = 0;
				format = chkbyte(tmp3, &opc);
				if (format != -1) {
					// format에 따라 lst의 구분자를 다르게 한다. 
					if (format == 1)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%02X\n", line, loc, tmp1, tmp2, tmp3, opc);
					else if (format == 2)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%04X\n", line, loc, tmp1, tmp2, tmp3, opc);
					else if (format == 3)
						fprintf(fwlst, "%4s\t%4s\t%-7s\t%-7s\t%-10s\t%06X\n", line, loc, tmp1, tmp2, tmp3, opc);
				}
				else	break;
			}
			else {	// directive 이외의 경우
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
		else { // 그외의 경우 
			printf("intermediate file(pass1.txt) format is not correct.");
			break;
		}

		// 형식에 따라 object code에 08~02X 포멧으로 저장한다. 
		if (format == 4) {
			sprintf(objectcode, "%08X", opc);
			int curloc = strtol(loc, NULL, 16);
			if (immflag == 0)
				writeModifynode(curloc - start + 1, &modiflist);		// modification list에 저장한다.
		}
		else if (format == 3)	sprintf(objectcode, "%06X", opc);
		else if (format == 2) sprintf(objectcode, "%04X", opc);
		else if (format == 1) sprintf(objectcode, "%02X", opc);

		// RESW, RESB를 만난 후 다시 objectcode를 계산되었을 경우 현재 주소를 start로 저장한다.
		if (startflag == 0) {
			startAddr = strtol(loc, NULL, 16);
			startflag = 1;
		}
		// obj 파일 쓰기 
		if (format != -1) {
			if ((byte + format) > 30) {
				// byte수가 30바이트를 초과할 경우 기존에 저장된 objectline을 obj 파일에 작성하고, 초기화한다.
				fprintf(fwobj, "T%06X%02X%s\n", startAddr, byte, objectline);
				byte = 0;
				startAddr = strtol(loc, NULL, 16);
				memset(objectline, '\0', MAX_STR);
				strcat(objectline, objectcode);
				byte += format;
			}
			else { // 이외에 objectcode로 저장된 것을 objectline에 연결한다.
				strcat(objectline, objectcode);
				byte += format;
			}
		}

	}

	// pass2 과정 실패
	fclose(fp);
	fclose(fwobj);
	fclose(fwlst);
	remove("pass1.txt");
	remove(objfile);
	remove(lstfile);
	printf("- pass2 fail.\n");
	return -1;
}

/* 4. 현재 주소값에 format 형식, operand의 바이트 수를 계산하여 다음에 사용할 주소값을 계산한다. */
int chkLocation(char* opcode, char* operand, int prev, OpcodeTable* opTable) {

	int format;					// format 형식 저장
	int temp = 0;				// chkLocation에는 필요없지만 chkbyte를 위한 임시 변수

	if (!strcmp("WORD", opcode)) {
		// word 범위체크
		if (checkConstantRange(operand) == TRUE && (atoi(operand) <= 0xFFFFFF))
			return prev + 3;
		else {
			printf("WORD Range overflow.");
			return -1;
		}
	}
	else if (!strcmp("RESW", opcode)) {
		// 해당 문자열이 16진수로 제대로 표현되었는지 확인, 범위내인지 확인
		if (checkConstantRange(operand) == TRUE && (atoi(operand) <= 0xFFFFFF))
			return prev + 3 * atoi(operand);
		else {
			printf("RESW Range overflow.");
			return -1;
		}
	}
	else if (!strcmp("RESB", opcode)) {
		// 해당 문자열이 16진수로 제대로 표현되었는지 확인, 범위내인지 확인
		if (checkConstantRange(operand) == TRUE && (atoi(operand) <= 0xFFFFFF))
			return prev + atoi(operand);
		else {
			printf("RESB Range overflow.");
			return -1;
		}
	}
	else if (!strcmp("BYTE", opcode)) {
		// BYTE 체크
		int tp = chkbyte(operand, &temp);
		if (tp != -1)
			return prev + tp;
		else
			return -1;
	}
	else {
		// 그 외의 경우 format 형식 계ㅏ산
		format = findFormat(opTable, opcode);
		if (format == -1)
			return -1;
		else
			return prev + format;
	}
}

/* 5.  BYTE의 차지하는 메모리 값과 object code를 계산한다. */
int chkbyte(char* operand, int* objectcode) {

	char tmp[MAX] = { '\0' , };			// operand를 저장할 배열
	char* type, * value;						// operand 안의 type과 value를 저장하는 배열포인터 
	int i;

	// operand를 type과 value로 분리
	strcpy(tmp, operand);
	type = strtok(tmp, "'");
	value = strtok(NULL, "'");

	// 만약 type X일 경우
	if (!strcmp(type, "X")) {
		// 범위 확인한다.
		if (checkConstantRange(value) == FALSE) {
			printf("BYTE Range overflow.");
			return -1;
		}
		*objectcode = strtol(value, NULL, 16);
		return 1;
	}
	else if (!strcmp(type, "C")) {
		// type C일 경우, value 안에 있는 값의 문자를 아스키값으로 변경하고 다시 16진수로 저장한다.
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

/* 6. opcode의 format을 확인하여 알려준다. */
int findFormat(OpcodeTable* opcodeT, char* opcode) {

	int i, index = 0;				// 인덱스 저장 
	OpcodeNode* tmp;		// opcode node pointer 생성 
	char* mne;						// '+' 를 분리하여 저장할 배열 포인터
	char opc[MAX];				// opcode를 복사할 배열

	// opcode를 opc 배열에 복사한다.
	strcpy(opc, opcode);
	// '+' 를 분리하여 저장
	mne = strtok(opc, "+");

	// 인덱스 값 찾기: 저장하는 방법과 동일 
	for (i = 0; i < (int)strlen(mne); i++)
		index += mne[i];
	tmp = opcodeT[index % TABLE_SIZE].node;

	// opcode 해당 index의 모든 노드 탐색 
	while (TRUE) {
		// 동일한 명령어가 있으면 해당 opcode를 반환 
		if (!strcmp(mne, tmp->mnemonic)) {
			if (opcode[0] == '+')
				return 4;
			else
				return tmp->form;
		}

		// NULL이 아닐때까지 next 넘어간다.
		if (tmp->next != NULL)
			tmp = tmp->next;
		else
			break;
	}

	//실패할 경우
	printf("There is no %s opcode.", opcode);
	return -1;
}

/* 7. 16진수 이내 값이 저장되었는지 확인한다. */
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

/* 8. 레지스터의 number을 확인한다.  */
int chkRegister(char* oper) {

	if (!strcmp("A", oper)) 		return 0;
	else if (!strcmp("X", oper)) 		return 1;
	else if (!strcmp("L", oper))		return 2;
	else if (!strcmp("B", oper))		return 3;
	else if (!strcmp("S", oper))		return 4;
	else if (!strcmp("T", oper))		return 5;
	else if (!strcmp("F", oper))		return 6;
	else if (!strcmp("PC", oper))		return 8;
	else if (!strcmp("SW", oper))		return 9;

	return -1;
}

/* 9. objectcode를 계산하는 함수  */
int findObject(char* opcode, char* oper1, char* oper2, char* loc, int baseReg, int format, OpcodeTable* opTable, SymbolTable* symtab, int* imm) {

	int objcode = 0;											// opcode mnem을 저장할 변수
	int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;	// nixbpe bit를 저장할 변수
	int r1 = 0, r2 = 0;											// register의 number 저장

	char* opr1, * opc;											// 붙어있는 +, @, # 분리하여 저장할 배열 포인터	
	char opertmp[MAX], opctmp[MAX];			// operand부분과 opcode를 저장할 배열

	int objAddr = 0;											// -	format 3은 disp 부분, format 4는 address부분에 대한object code 저장
	int bit = 0;													// format 3일 경우 12, 4일경우 20 저장 
	int location = strtol(loc, NULL, 16);			// 현재 location을 16진수로 저장

	// operand와 opcode를 opertmp와 opctmp에 저장
	if (oper1 != NULL)
		strcpy(opertmp, oper1);
	strcpy(opctmp, opcode);

	// format 1일 경우 : opcode 반환(1 바이트)
	if (format == 1) {
		objcode = findOpcode(opTable, opcode);
		if (objcode == -1) {
			printf("There is no %s opcode.", opcode);
			return -1;
		}
		return objcode;
	}
	else if (format == 2) {	// format 2일 경우 : opcode와 register 계산

		objcode = findOpcode(opTable, opcode);
		if (objcode == -1) {
			printf("There is no %s opcode.", opcode);
			return -1;
		}

		objcode = objcode << 8;		 // 1 바이트 이동 (8bit)

		// register 1 체크
		if (oper1 != NULL) {
			r1 = chkRegister(oper1);
			r1 = r1 << 4;
			objcode += r1;
		}

		// register 2 체크
		if (oper2 != NULL) {
			r2 = chkRegister(oper2);
			objcode += r2;
		}
		return objcode;
	}

	// format 3~4
	// @, #, + addressing mode 확인한다. == > n , i
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

	// operand가 symbol일 경우 주소값을 저장한다.
	if (oper1 != NULL && *imm == 0) {
		objAddr = findSymbol(opr1, symtab);
		if (objAddr == -1) {
			printf("%s Operand not exist in symbol table.", opr1);
			return -1;
		}

	}


	// opctmp의 +을 분리 저장한다.
	opc = strtok(opctmp, "+");

	// operand 1과 2에 X가 있는지 확인
	if ((oper1 != NULL) && !strcmp("X", oper1) || (oper2 != NULL && !strcmp("X", oper2)))
		x = 1;

	// format 3
	if (format == 3) {
		bit = 12;									// disp을 위해 후에 12비트 이동
		if (oper1 == NULL)					// operand가 비어있을 경우 0으로 저장
			objAddr = 0;
		else if (oper1[0] != '#' || checkConstantRange(opr1) == FALSE) {
			// PC relative addressing 계산한다
			if (objAddr - (location + format) >= -2048 && objAddr - (location + format) <= 2047) {
				p = 1;
				objAddr -= (location + format);

				if (objAddr < 0) // 음수면 2의 보수를 취함.
					objAddr &= 0x00000fff;
			}

			// pc 범위 초과시 BASE relative addressing 계산
			else if (objAddr - baseReg >= 0 && objAddr - baseReg <= 4095) {
				b = 1;
				objAddr -= baseReg;
			}
			else {	// 오류 메세지를 출력한다. 
				printf("Range overflow.");
				return -1;
			}
		}
	}
	else { // format 4
		bit = 20;								// address를 위해 후에 20비트 이동
		e = 1;									// format 4는 e = 1

		if (n == 1 && i == 1 && x == 1) {  // simple + x bit
			if (x == 1) 						// x의 number 추가 (direct address)
				objAddr = 1;
		}
	}

	// opcode mnemi 찾기
	objcode = findOpcode(opTable, opc);
	if (objcode == -1) { // 에러
		printf("%s opcode is not correct.", opc);
		return -1;
	}

	// object code를 opcode + nixbpe + address 부분을 합쳐 완성한다.
	n = n << 1;
	objcode = objcode + n + i;	// opcode와 n, i를 더함.
	objcode = objcode << 4;		// 4비트 이동하여 +xbpe를 더한다. 
	x = x << 3;
	b = b << 2;
	p = p << 1;
	objcode = objcode + x + b + p + e;
	objcode = objcode << bit;			// 해당 format만큼 이동시키고 계산한 objAddr를 저장한다.
	return objcode += objAddr;

}

/* 10. modification 주소를 ModifList에 저장함  */
void writeModifynode(int location, ModifList** modiflist) {

	// ModifNode에 저장할 새로운 노드 생성
	ModifNode* newNode = (ModifNode*)malloc(sizeof(ModifNode));
	newNode->next = NULL;
	newNode->location = location;

	/* 만약 modify node가 처음일 경우, 헤더에 저장하고,
	cur도 head와 동일하게 저장한다. */
	if ((*modiflist)->head == NULL) {
		(*modiflist)->head = newNode;
		(*modiflist)->curr = newNode;
	}
	else { // 아닐 경우 , current next로 저장하고 , curr 도 변경해준다.
		(*modiflist)->curr->next = newNode;
		(*modiflist)->curr = newNode;
	}
	return;
}

/* 11. ModifList에 저장된 모든 노드를 obj 파일에 저장함  */
void fileModifynode(FILE* fwp, ModifList* modiflist) {

	// 전 노드를 순회하게 될 modifnode
	ModifNode* tmpNode = modiflist->head;

	// 모든 노드를 순회하며 출력한다.
	for (; tmpNode != NULL; tmpNode = tmpNode->next)
		fprintf(fwp, "M%06X05\n", tmpNode->location);

	return;
}

/* 12. ModifList에 저장된 모든 노드를 free한다.  */
void freeModifynode(ModifList** modiflist) {

	// 삭제에 사용될 modifnode
	ModifNode* deleteNode = (*modiflist)->head;

	// list에 저장된 모든 노드를 삭제한다. head는 그 다음 노드로 저장, delete 삭제 
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

/* ============ progaddr 관련 함수 ============ */
/* 1. 프로그램 시작 주소를 지정한다. */
void progaddr(char* command, char* first, HistoryList* historyL) {

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR] = { '\0', };			// history에 저장하기 위한 명령어 저장배열
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);

	if (checkConstantRange(first) == TRUE) { // 범위내에 있을 경우 
		sscanf(first, "%x", &progAddr);		// 주소값으로 지정 
		writeHistory(input, &historyL);		// history List에 저장	
	}
	else { //범위 초과시 에러 출력 
		printf("progAddr range is not correct.\n");
	}

	return;
}

/* ============ loader 관련 함수 ============ */
/* 1. external symbol table 초기화. */
void initExSymTable(ExSymbolTable** exSymT) {

	int i;
	*exSymT = (ExSymbolTable*)malloc(sizeof(ExSymbolTable) * 3);

	// symbol Hash Table 초기화 
	for (i = 0; i < 3; i++) {
		(*exSymT)[i].cnt = 0;
		strcpy((*exSymT)[i].csname, "");
		(*exSymT)[i].length = 0;
		(*exSymT)[i].address = 0;
		(*exSymT)[i].node = NULL;
		(*exSymT)[i].last = NULL;
	}
	return;
}

/* 2. external symbol table 초기화. */
int findExSymbol(int csflag, char* csname, char* label, ExSymbolTable** exSymT) {

	int i;
	SymbolNode* findNode;	// symbol 찾을 임시 node 
	
	// control section일 경우 - external table을 csname을 확인하여 찾는다. 
	if (csflag == 0) {
		for (i = 0; i < 3 && strcmp((*exSymT)[i].csname, ""); i++) {
			if (!strcmp(csname, (*exSymT)[i].csname))
				return (*exSymT)[i].address;
		}
	}
	else { // symbol일 경우, external table의 node을 순회하여 찾는다. 
		for (i = 0; i < 3; i++) {
			findNode = (*exSymT)[i].node;
			while (findNode != NULL) {
				if (!strcmp(label, (findNode)->label))
					return findNode->location;
				findNode = findNode->next;
			}
		}
	}
	return -1;
}

/* 3. external symbol table 생성. */
void makeExSymbol(int csflag, int index, char* csname, char* label, int addr, int len, ExSymbolTable** exSymT) {

	SymbolNode* tmpNode;	// 새로운 값을 저장할 SymbolNode pointer 생성
	
	// control section의 경우 : external table에 저장한다. 
	if (csflag == 0) {
		strcpy((*exSymT)[index].csname, csname);
		(*exSymT)[index].length = len;
		(*exSymT)[index].address = addr;
	}
	else {	// 아닐 경우, symbol node을 추가하고 external table의 node에 연결한다. 
		tmpNode = (SymbolNode*)malloc(sizeof(SymbolNode));
		tmpNode->next = NULL;
		strcpy(tmpNode->label, label);
		tmpNode->location = addr;
		tmpNode->index = index;

		// 기존에 저장된 node가 없는 경우  
		(*exSymT)[index].cnt++;
		if ((*exSymT)[index].node == NULL) {
			(*exSymT)[index].node = tmpNode;
			(*exSymT)[index].last = tmpNode;
		}
		else {
			((*exSymT)[index].last)->next = tmpNode;
			(*exSymT)[index].last = tmpNode;
		}
	}
	return;
}

/* 4. external symbol table free. */
void freeExSymbolTable(ExSymbolTable** exSymT) {

	SymbolNode* deleteN = NULL;		// 삭제할 symbol node 
	int i;

	// 만약 external symbol table이 비어있을 경우 해당 함수를 빠져나간다.
	if (*exSymT == NULL)
		return;

	// 아닐 경우 모든 table을 순회하며 삭제한다.
	for (i = 0; i < 3; i++) {
		while ((*exSymT)[i].cnt != 0) {
			deleteN = (*exSymT)[i].node;
			(*exSymT)[i].node = deleteN->next;
			free(deleteN);
			(*exSymT)[i].cnt--;
		}
	}

	free(*exSymT);
	return;

}

/* 5. external symbol table 출력. */
void printExSymbol(int count, ExSymbolTable* exSymT) {

	SymbolNode* tmpNode = NULL;		// symbol을 순회할 node 

	int total = 0;		// total 길이
	int i;

	// print 한다.  
	printf("control      symbol       address      length       \n");
	printf("section      name \n");
	printf("--------------------------------------------------------\n");
	
	// load한 파일의 external symbol을 출력한다.  
	for (i = 0; i < count; i++) {
		// control section 출력 
		printf("%-8s     %-8s     %04X         %04X         \n", (exSymT)[i].csname, "", (exSymT)[i].address, (exSymT)[i].length);
		total += (exSymT)[i].length;
		
		// symbol 부분 출력 
		tmpNode = (exSymT)[i].node;
		while (tmpNode != NULL) {
			printf("%-8s     %-8s     %04X\n", "", tmpNode->label, tmpNode->location);
			tmpNode = tmpNode->next;
		}
	}
	
	// 전체 길이를 출력한다. 
	printf("--------------------------------------------------------\n");
	printf("%-8s     total        length       %04X\n","",total);
	free(tmpNode);
	return;
}

/* 6. loader 함수 - loadpass1, pass2 호출 */
void loader(int arg, char* command, char* first, char* second, char* third, HistoryList* historyL) {

	ExSymbolTable* exSymT;			// external symbol table 생성 

	int i;  
	char tmp[MAX_STR] = { '\0', };	// 임시 저장 배열 
	char files[3][MAX_STR];			// argument를 file name을 분리해서 저장하는 배열
	char* filename, * type;			// 파일명과 type을 저장할 배열포인터 

	char input[MAX_STR] = { '\0', };			// history에 저장하기 위한 명령어 저장배열
	strcpy(input, command);

	initExSymTable(&exSymT);		// external table 초기화 

	// argument로 받은 배열를 파일명으로 분리 
	for (i = 0; i < arg; i++) {
		memset(tmp, '\0', MAX_STR);
		memset(files[i], '\0', MAX_STR);
		if (i == 0) {
			strcat(input, " ");
			strcat(input, first);
			strcpy(tmp, first);
		}
		else if (i == 1) {
			strcat(input, " ");
			strcat(input, second);
			strcpy(tmp, second);
		}
		else if (i == 2) {
			strcat(input, " ");
			strcat(input, third);
			strcpy(tmp, third);
		}

		filename = strtok(tmp, ".");
		strcpy(files[i], filename);
		strcat(files[i], ".");
		type = strtok(NULL, "");
		
		// type 확인 
		if (strcmp(type, "obj")) {
			printf("The file type is not '*.obj'. please check again.\n");
			return;
		}
		else { // files 배열에 저장 
			strcat(files[i], type);
		}
	}

	// pass1 호출 
	if (loadPass1(files, arg, &exSymT) != -1) {
		if (loadPass2(files, arg,&exSymT) != -1) { // pass 2 호출 
			printExSymbol(arg, exSymT);			 // external table 출력	
			writeHistory(input, &historyL);			 // history List에 저장
		}
	}

	freeExSymbolTable(&exSymT); 	// external table free 
}

/* 7. loadpass1 과정 수행 - exsymbol 생성 */
int loadPass1(char(*files)[MAX_STR], int count, ExSymbolTable** exSymT) {

	FILE* fp;				// 파일포인터 
	char input[MAX_BUFR];	// 파일 내용 저장 배열 
	char text[7], tmp[7]; 	// 임시 저장 배열 
	char csname[7];			// control section name 저장배열 
	char* head;				// 헤더 외를 지칭하는 배열 포인터 
	int address;			// 임시 주소값 저장 
	int length;				// 길이 저장 
	int csAddr = progAddr;	// control section 주소
	int i, j, k;

	for (i = 0; i < count; i++) {
		memset(csname, '\0', 7);
		fp = fopen(files[i], "r");
		// 파일이 존재하지 않을 경우 - 에러처리 
		if (fp == NULL) {
			printf("%s file does not exist.\n", files[i]);
			return -1;
		}

		while (fgets(input, MAX_BUFR, fp)) {

			memset(text, '\0', 7);
			memset(tmp, '\0', 7);
			
			// header 부분 
			if (input[0] == 'H') {
				// 1~6번째 cs name 추출 
				for (j = 1; j < 7; j++) {
					if (input[j] == ' ')
						break;
					text[j - 1] = input[j];
				}
				strcpy(csname, text);
				
				// 7~12번째 시작주소 추출 	
				for (j = 7; j < 13; j++)
					tmp[j - 7] = input[j];
				sscanf(tmp, "%x", &address);
				// control section 주소에 더하기 
				csAddr += address;

				memset(tmp, '\0', 7);
				// 13~18번째 길이 
				for (j = 13; j < 19; j++)
					tmp[j - 13] = input[j];
				sscanf(tmp, "%x", &length);
				
				// external symbol에 이미 존재할 경우 
				if (findExSymbol(0, text, NULL, exSymT) != -1) {
					printf("Duplicate external symbol.\n");
					return -1;
				}
				else {
					// 없을 경우, 추가 
					makeExSymbol(0, i, text, NULL, csAddr, length, exSymT);
				}
				break;
			}
		}

		while (fgets(input, MAX_BUFR, fp)) {
			if (input[0] == '.')
				continue;
			else if (input[0] == 'E')
				break;
			// Define Record 출력 
			else if (input[0] == 'D') {
				head = &input[1];
				for (k = 0; k < (int)strlen(head)-1; k++) {
					memset(text, '\0', 7);
					memset(tmp, '\0', 7);
					
					// name 추출 
					for (j = 0 + k; j < 6 + k; j++) {
						if (head[j] == ' ') 
							break;
						text[j - k] = head[j];
					}
					// 주소 추출 
					for (j = 6 + k; j < 12 + k; j++) {
						tmp[j - 6 - k] = head[j];
					}
					sscanf(tmp, "%x", &address);

					if (findExSymbol(1, csname, text, exSymT) != -1) {
						printf("Duplicate external symbol.\n");
						return -1;
					}
					else {
						// 추가  
						makeExSymbol(1, i, csname, text, csAddr + address, 0, exSymT);
					}
					k = j - 1;
				}
			}
		}
		csAddr += length;
		fclose(fp);
	}
	return 0;
}

/* 8. loadpass2 과정 수행 - exsymbol 생성 */
int loadPass2(char(*files)[MAX_STR], int count, ExSymbolTable** exSymT) {

	FILE* fp;						// 파일포인터 
	RefSymbolList refSymL;			// reference symbols list 생성 
	RefSymbolNode* tmpref = NULL;	// 순회할 refSymbol Node 
	
	char input[MAX_BUFR];		// 파일 내용 저장 배열 
	char text[7], tmp[7]; 		// 임시 저장 배열 
	char csname[7];				// control section name 저장배열 
	char* head;					// 헤더 외를 지칭하는 배열 포인터 
	int address;				// 임시 주소값 저장 
	int length;					// 길이 저장 
	int csAddr = progAddr;		// control section 주소

	char oper;			// operator 
	int symAddr;		// symbol의 주소값 저장
	int refNum;			// reference symbol number
	int rAddr;			// record의 주소
	int rLen;			// record의 길이 
	int objCode;		// object code 저장 
	int modifnum;		// modification 저장 
	int editnum;		// edit num 저장 
	int row, col;		// 행과 열 
	int i, j, k;


	refSymL.head = refSymL.last = NULL;
	csAddr = execAddr = progAddr;

	for (i = 0; i < count; i++) {
		fp = fopen(files[i], "r");

		while (fgets(input, MAX_BUFR, fp)) {

			memset(text, '\0', 7);
			memset(tmp, '\0', 7);
			// header 부분  
			if (input[0] == 'H') {
				// control section name 추출 
				for (j = 1; j < 7; j++) {
					if (input[j] == ' ')
						break;
					text[j - 1] = input[j];
				}
				strcpy(csname, text);
				
				// 시작 주소 추출 
				for (j = 7; j < 13; j++)
					tmp[j - 7] = input[j];
				sscanf(tmp, "%x", &address);
				csAddr += address;

				// reference symbol 추가 
				makeRefSymbol(1, text, &refSymL);
				
				// 길이 추출 
				memset(tmp, '\0', 7);
				for (j = 13; j < 19; j++)
					tmp[j - 13] = input[j];
				sscanf(tmp, "%x", &length);
			}
			break;
		}

		while (fgets(input, MAX_BUFR, fp)) {

			if (input[0] == '.')
				continue;
			else if (input[0] == 'E') {
				memset(tmp, '\0', 7);
				if (strlen(input) > 1) {
					for (j = 1; j < 7; j++)
						tmp[j - 1] = input[j];
					sscanf(tmp, "%x", &address);
					execAddr = csAddr + address;
				}
				csAddr += length;
				break;
			}
			// reference record 추출 
			else if (input[0] == 'R') {
				head = &input[1];
				for (k = 0; k < (int)strlen(head); k= k+8) {
					memset(text, '\0', 7);
					memset(tmp, '\0', 7);
					refNum = 0;
					// reference number 
					for (j = 0 + k; j < 2 + k; j++) {
						text[j - k] = head[j];
					}
					sscanf(text, "%x", &refNum);
					
					// reference name 
					for (j = 2 + k; j < 8 + k; j++) {
						if (head[j] == ' ' || head[j] == '\n'){
							break;
						}
						tmp[j - 2 - k] = head[j];
					}
					makeRefSymbol(refNum, tmp, &refSymL);
				}
			}
			// text record 메모리에 추가 
			else if (input[0] == 'T') {
				rAddr = rLen = 0;
				sscanf(input + 1, "%6x%2x", &rAddr, &rLen);
				for (j = 0; j < rLen; j++) {
					memset(text, '\0', 7);
					memset(tmp, '\0', 7);
					sscanf(input + 9 + j*2, "%2x", &objCode);
					sprintf(text, "%x", csAddr + rAddr + j);
					sprintf(tmp, "%x", objCode);
					edit(NULL, text, tmp, NULL, memory);
				}
			}
			// modification record 추출 
			else if (input[0] == 'M') {
				memset(text, '\0', 7);
				memset(tmp, '\0', 7);
				// 정제작업 
				sscanf(input + 1, "%6x%2x%c%x", &rAddr, &rLen, &oper, &refNum);
				
				// reference number으로 refer symbol list에서 symbol name 찾기  
				tmpref = refSymL.head;
				while (tmpref != NULL) {
					if (tmpref->refnum == refNum) {
						strcpy(text, tmpref->symbol);
						break;
					}
					tmpref = tmpref->next;
				}
				
				// reference number가 1일 경우 프로그램 name	
				if (refNum == 1) {
					symAddr = findExSymbol(0, text, NULL, exSymT);
				}
				else {
					// 아닐 경우 external symbol table에서 찾기 
					symAddr = findExSymbol(1, csname, text, exSymT);
				}
				
				// symbol address를 찾은 경우 
				if (symAddr != -1) {
					modifnum = 0;
					
					// half byte가 5일 경우 
					if (rLen == 5) {
						row = (csAddr + rAddr) / COL_SIZE;		// 주소를 16 나눈 몫 
						col = (csAddr + rAddr) % COL_SIZE;		// 주소를 16 나눈 나머지 
						modifnum += memory[row][col] % 16; // [csAddr+rAddr]%16;
						editnum = 0xFFFFF;
					}
					else if (rLen == 6)
						editnum = 0xFFFFFF;
				}
				else { // 찾을 수 없는 경우 
					printf("external symbol cannot found.\n");
					return -1;
				}
				
				// modify 할 부분 저장 
				for (k = csAddr + rAddr + rLen % 2; k < csAddr + rAddr + 3; k++) {
					row = k / COL_SIZE;		// 주소를 16 나눈 몫 
					col = k % COL_SIZE;		// 주소를 16 나눈 나머지 
					modifnum <<= 4;
					modifnum += memory[row][col] / 0x10;
					modifnum <<= 4;
					modifnum += memory[row][col] % 0x10;
				}
				
				
				// operator 실행 
				if (oper == '+')
					modifnum += symAddr;
				else if (oper == '-')
					modifnum -= symAddr;
				else {
					printf("operator is not correct.\n");
					return -1;
				}

				// modify 한 memory에서 수정 
				modifnum &= editnum;
				for (k = 2; k >= rLen % 2; k--) {
					row = (k + csAddr + rAddr) / COL_SIZE;		// 주소를 16 나눈 몫 
					col = (k + csAddr + rAddr) % COL_SIZE;		// 주소를 16 나눈 나머지 
					memory[row][col] = modifnum % 0x100;
					modifnum >>= 8;
				}
				// half byte가 5일 경우 추가 작업 
				if (rLen % 2 == 1) {
					row = (csAddr + rAddr) / COL_SIZE;		// 주소를 16 나눈 몫 
					col = (csAddr + rAddr) % COL_SIZE;		// 주소를 16 나눈 나머지
					memory[row][col] /= 16;
					memory[row][col] *= 16;
					memory[row][col] += modifnum % 0x10;
				}
			}
		}
		// refer symbol list 해제 
		refSymL.head = refSymL.last = NULL;
		freeRefSymbol(&refSymL);
		fclose(fp);
	}
	return 0;
}

void makeRefSymbol(int refnum, char* symbol, RefSymbolList* refSymL) {

	RefSymbolNode* tmpNode = (RefSymbolNode*)malloc(sizeof(RefSymbolNode));  // 새로 저장할 refer symbol node 
	
	// 값을 저장 
	tmpNode->next = NULL;
	tmpNode->refnum = refnum;
	strcpy(tmpNode->symbol, symbol);
	
	// 아무것도 없을 경우 추가 
	if (refSymL->head == NULL) {
		refSymL->head = refSymL->last = tmpNode;
	}
	else { // 있을 경우, last 부분에 연결 
		refSymL->last->next = tmpNode;
		refSymL->last = tmpNode;
	}
	return;
}

void freeRefSymbol(RefSymbolList* refSymL) {

	RefSymbolNode* deleteNode = refSymL->head; 	// 삭제할 refer symbol node 
	refSymL->last = NULL;
	
	// 삭제할 것이 없을 때까지 순회하며 삭제 
	while (deleteNode != NULL) {
		refSymL->head = deleteNode->next;
		free(deleteNode);
		deleteNode = refSymL->head;
	}

	return;
}


void bp(char* command, char* first, HistoryList* historyL, BreakPointList* breakP) {

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR] = { '\0', };			// history에 저장하기 위한 명령어 저장배열
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);

	int i;
	int loc;					// break point의 주소 
	
	// argument가 없을 경우 
	if (strlen(first) == 0) {
		// 출력 
		printBreakPoint(breakP);
	}
	else {
		if (!strcmp("clear", first))
			freeBreakPointList(&breakP);
		else {
			// 범위 확인 
			for (i = 0; i < (int)strlen(first); i++) { // 0 ~ 9 , A ~ F , a ~ f
				if ((first[i] >= 0x30 && first[i] <= 0x39) || (first[i] >= 0x41 && first[i] <= 0x46) || (first[i] >= 0x61 && first[i] <= 0x66))
					continue;
				else {
					printf("Argument is not correct. (address(hex) or 'clear')\n");
					return;
				}
			}
			// location 저장 
			sscanf(first, "%x", &loc);
			writeBreakPoint(loc, breakP);
			printf("	[ok] create breakpoint %4X\n", loc);
		}
	}

	writeHistory(input, &historyL);		// history List에 저장
	return;
}

void printBreakPoint(BreakPointList* breakP) {

	BreakPointNode* tmpNode = breakP->head; // 순회할 break point node 

	printf("\tbreakpoint\n");
	printf("\t----------\n");
	// 없을 때까지 순회 
	while (tmpNode != NULL) {
		printf("\t%04X\n", tmpNode->loc);
		tmpNode = tmpNode->next;
	}
	return;
}

void writeBreakPoint(int location, BreakPointList* breakP) {

	BreakPointNode* prev;	// 이전을 저장 node 
	
	 // 새로 추가할 break point node 
	BreakPointNode* tmpNode = (BreakPointNode*)malloc(sizeof(BreakPointNode));
	tmpNode->next = NULL;
	tmpNode->loc = location;

	// 없을 경우 추가한다.  
	if (breakP->head == NULL) {
		breakP->head = breakP->curr = tmpNode;
	}
	// break point의 head location보다 작을 경우 새로운 head로 지정 
	else if (breakP->head->loc >= location) {
		if (breakP->head->loc == location) {
			free(tmpNode);
			return;
		}
		tmpNode->next = breakP->head;
		breakP->head = breakP->curr = tmpNode;
	}
	else {
		// 순회하며 오름차순으로 정렬하여 추가한다. 
		breakP->curr = breakP->head->next;
		prev = breakP->head;

		while (breakP->curr != NULL) {
			if (breakP->curr->loc == location) {	// 같은 것은 추가하지 않음. 
				free(tmpNode);
				return;
			}
			else if (breakP->curr->loc > location) { 
				prev->next = tmpNode;
				tmpNode->next = breakP->curr;
				breakP->curr = tmpNode;
				return;
			}
			// 다음 노드로 이동 
			prev = breakP->curr;
			breakP->curr = prev->next;
		}
		// 새로운 노드가 가장 클 경우 마지막에 저장 
		prev->next = breakP->curr = tmpNode;
	}

	return;
}

void freeBreakPointList(BreakPointList** breakP) {

	// 삭제에 사용될 BreakPoint
	BreakPointNode* deleteNode = (*breakP)->head;

	// list에 저장된 모든 노드를 삭제한다. head는 그 다음 노드로 저장, delete 삭제 
	while (deleteNode != NULL) {
		(*breakP)->head = deleteNode->next;
		free(deleteNode);
		deleteNode = (*breakP)->head;
	}
	return;
}

