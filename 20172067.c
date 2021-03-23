#include "20172067.h"  

int main(void) {
	
	int address = 0;				// 주소 저장 (dump 함수시 필요)  
	
	HistoryList historyL;			// history를 저장할 list 	
	OpcodeTable* opcodeT;			// opcode들을 저장할 table 

	/* Initializing 과정 */
	reset(memory);							// 메모리를 0으로 초기화한다. 
	historyL.head = historyL.curr = NULL;   // history list의 head와 현재 node pointer 초기화 
	readOp(&opcodeT);						// opcode.text를 불러와 opcode table에 저장한다. 

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
		if(argCount == -1)
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
			else
				reset(memory);
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
			opcodeList(command, &historyL, opcodeT);
		}
		// 11. 그 외 
		else {
			printf("Please check command.\n");
		}
		getchar();
	}
	
	return 0;

}

/* =============  명령어 정제함수  ============= */ 

/* 1. command 정제처리 : '\t', ' ' 와 같은 것을 없애고,
command와 arguments 배열에 나눠서 저장한다. */
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

/* 2. arguments들을 정제처리 : '\t', ' '을 없애고,
 콤마 수와 인자 수를 확인하여 first, second, third 배열에 저장한다  */
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
void help(char* input, HistoryList* historyL){
	
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
	
	return;
	
}

/* 2. dir : 현재 디렉터리의 파일 출력*/
void dir(char* input, HistoryList* historyL) {

	writeHistory(input, &historyL);		// history List에 저장 

	DIR *di;
	struct dirent *direntPtr;
	struct stat info;
	int line = 0;
	char name[100];

	di = opendir(".");

	while((direntPtr = readdir(di)) != NULL){
		strcpy(name, direntPtr->d_name);
		
		// '.' , '..' 파일의 경우에는 출력을 생략한다.
		if(strcmp(name,".") == 0 || strcmp(name,"..") == 0)
			continue;
		lstat(name,&info);
		printf("%20s",name);
		line++;
		
		// 디렉터리의 경우에는 파일이름 끝에 '/' 을 붙인다.
		if(S_ISDIR(info.st_mode))
			printf("/");
		// 실행파일의 경우에는  파일이름 끝에 '*' 을 붙인다.
		else if(S_IEXEC & info.st_mode)
			printf("*");
		if(line % 3 == 0)
			printf("\n");
			
	}
	
	printf("\n");
	closedir(di);
	
}

/* 3. quit : 종료*/
void quit(HistoryList* historyL, OpcodeTable** opcodeT) {
	
	deleteHistory(&historyL);		// 저장된 history linked list를 삭제한다. 
	freeOpcode(opcodeT);			// 저장된 opcode table을 삭제한다.  
	
}

/* 4. history : 사용한 명령어를 정제해서 linked list에 저장한다. */
void history(char* input, HistoryList* historyL){
	
	writeHistory(input, &historyL);		// history List에 저장
	printHistory(historyL);				// 저장된 history를 출력한다. 
	
}

/* 4-1. history 사용 명령어 저장 */
void writeHistory(char* content, HistoryList** historyL){
	
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
void printHistory(HistoryList* historyL)
{
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
void deleteHistory(HistoryList** historyL){
	
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
void dump(int argCount, char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE], int* address){
	
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
void printMemory(unsigned char(*memory)[COL_SIZE], int start, int end){
	
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
			if((i == sRow && j < sCol) || (i == eRow && j > eCol))
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
char asciiCode(int hex){
	
	// 0x20 ~ 0x7E 범위내 출력
	if (hex >= 0x20 && hex <= 0x7E)
		return hex;
	else // 그 외
		return '.';
		
}

/* 3. edit : 첫번째 인자(주소)에 두번째 인자(value)로 해당 메모리 값을 수정한다. */
void edit(char* command, char* first, char* second, HistoryList* historyL, unsigned char(*memory)[COL_SIZE])
{
	int address, value;		// 주소와 변경 값 
	int col, row;			// 행렬 

	// history에 저장하기 위해 명령어를 command + argument 합쳐서 저장
	char input[MAX_STR];
	strcpy(input, command);
	strcat(input, " ");
	strcat(input, first);
	strcat(input, ", ");
	strcat(input, second);

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
			writeHistory(input, &historyL); // history 기록 
			row = address / COL_SIZE;		// 주소를 16 나눈 몫 
			col = address % COL_SIZE;		// 주소를 16 나눈 나머지 
			memory[row][col] = value;		// 해당 memory 수정 
		}
	}
	
	return;
	
}

/* 4. fill : 첫번째 인자(시작)과 두번째 인자(끝) 사이를 세번째 인자(값)으로 수정한다. */ 
void fill(char* command, char* first, char* second, char* third, HistoryList* historyL, unsigned char(*memory)[COL_SIZE])
{
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
void fillMemory(unsigned char(*memort)[COL_SIZE], int start, int end, int value){

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
void opcode(char* command, char* first, HistoryList* historyL, OpcodeTable* opcodeT){
	
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
int findOpcode(OpcodeTable* opcodeT, char* first){
	
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
void opcodeList(char* command, HistoryList* historyL, OpcodeTable* opTable){
	
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
