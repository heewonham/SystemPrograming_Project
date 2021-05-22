# Assignment 1 :
본 프로젝트는 어셈블러, 링커, 로더들을 실행하게 될 shell과 컴파일을 통해서 만들어진 object코드가 적재되고 실행될 메모리공간, opcode 값으로 변환 하는 opcode 테이블과 관련 명령어들을 구현한 것이다. 이번 assignment 1에서는 프로젝트에서 다룰SIC/XE machine을 구현하기 위한 사전작업이라고 할 수 있다. 현재 구현할 셸은help, directory, quit 등의 기본적인 작업을 수행하고 다음 프로젝트의 assemble, complile 등 작업에 필요한 1MB의 가상 메모리 공간 할당과 수정을 구현한다. 또한 mnemonic에 대한 opcode의 값을 반환하거나opcodelist를 출력할 수 있다. 그리고 mnemonic은 반드시 대문자로 입력하고 help, quit 등과 같은 명령어는 소문자로 구현한다. 숫자에 대한 입력은 대소문자 가능하도록 한다. 띄어쓰기와 탭은 정제되어 처리될 수 있도록 처리 될 수 있도록 구현한다.



### 컴파일 및 실행방법

리눅스 환경에서 

1.  20172067.c

2.  20172067.h

3.  makefile

4.  opcode.txt

   상기 4가지를 동일한 폴더에 놓은 뒤

   명령어 'make'를 입력하면 컴파일과 object 파일과 생성한다.

   './20172067'을 이용하면 해당 프로그램을 실행할 수 있다.

   만약 .out , o 파일을 삭제하길 원한다면 'make clean'을 입력하면 된다.







# Assignment 2 : 
Assignment 1에서 작성한 프로그램을 확장하여 assemble, symbol, type의 기능을 추가한다. SIC/XE의 assembly 프로그램의 소스파일(asm)을 읽고 명령어를 해석하여 리스팅 파일과 오브젝트 파일을 생성하는 것을 주 목적으로 한다. 이 프로그램은 기본적인 SIC/XE의 프로그램으로 교재의 2.2까지 설명된 어셈블러 기능을 구현하였다. symbol 명령어는 마지막 assemble을 명령어를 성공적으로 끝낸 것에 대한 symbol을 저장하고 있으며 labe에 따라 알파벳 오름차순으로 정렬되어있다. 만약 assemble 명령어가 제대로 수행되지 않았을 경우에도 성공적으로 수행한 symbol을 삭제되지 않는다. 또한 프로그램을 처음 실행하거나 정상적으로 assemble이 되지 않았을 경우 symbol 값이 없으므로 에러 메세지를 출력한다. type 명령어는 파일 이름에 해당하는 파일을 읽어서 출력한다. 해당 파일이 존재하지 않는다면 에러메세지를 출력한다.  help에도 추가된 기능에 대한 명령어를 추가한다.
 	해당 보고서에는 symbol, assemble, type에 관련된 함수와 내용을 집중적으로 다루고 있으며 Assignment1의 함수가 변경된 경우에서만 설명할 것이다.  
Assemble의 알고리즘을 대략적으로 설명하면 asm 파일을 읽어와 start 주소와 program name을 저장하고 label이 있을 경우 symbol table에 저장한다. 또한 현재 loc과 line을 작성하고 현재 loc과 현재 메모리 값을 더한 다음 주소값을 저장하여 사용한다. End를 만나면 pass1을 종료하고 pass1.txt파일을 만든다. Pass2 과정을 통해서 symbol table과 pass1.txt를 이용해 인자의 수 3, 4 , 5개 인지에 따라 opcode와 operand를 구분하여object code를 계산하고 (더 자세한 내용은 주석 참조) obj, lst 파일을 작성한다. 만약 pass2가 실패하면 symbol table과 pass1.txt를 삭제하고, 성공할 경우 파일생성 문구와 pass1.txt를 삭제하고 symbol table을 전역변수인 symtab과 연결한다.



### 컴파일 및 실행방법

리눅스 환경에서  

1. 20172067.c

2. 20172067.h

3. makefile

4. opcode.txt

5. 2_5.asm

   상기 5가지를 동일한 폴더에 놓은 뒤

   명령어 'make'를 입력하면 컴파일과 object 파일과 생성한다.

   './20172067.out'을 이용하면 해당 프로그램을 실행할 수 있다.

   만약 .out , o 파일을 삭제하길 원한다면 'make clean'을 입력하면 된다.
   





# Assignment 3 : 

Assignment 3는 Assignment 1, 2에 셸(shell)의 명령어 및 기능을 추가한다. Loader는 한개에서 최대 3개의 object 파일을 불러올 수 있다. Linking은 assignment2의  생성된 object 파일 각각을 하나로 묶고, loading은 메모리에 올려서 프로그램 실행 준비를 한다. 또한 progaddr 명령어를 수행해서 프로그램의 시작주소를 지정할 수 있으며 break point를 추가하여 디버깅을 수행할 수 있다. 또한 breakpoint 확인하거나 혹은 삭제할 수 있다. 시간관계상 Run의 과정은 구현하지 못했다. 

예제 파일을 설명하자면, loader 명령어를 통해서 proga.obj, progb.obj, progc.obj의 3개의 파일을 linking, loading 과정을 수행할 수 있다. 그리고 copy.obj 파일은 loader 명령어와 run 명령어를 수행할 수 있다.



### 컴파일 및 실행방법

리눅스 환경에서 

1. 20172067.c

2. 20172067.h

3. makefile

4. opcode.txt

5. proga.obj

6. progb.obj

7. progc.obj

8. copy.obj

   상기 8가지를 동일한 폴더에 놓은 뒤

   명령어 'make'를 입력하면 컴파일과 object 파일과 생성한다.

   './20172067.out'을 이용하면 해당 프로그램을 실행할 수 있다.

   만약 .out , o 파일을 삭제하길 원한다면 'make clean'을 입력하면 된다.



# Assignment 4



