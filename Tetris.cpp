#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
 
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27 //  
#define BY 2
#define BX 3
#define BH 24
#define BW 10
#define max(a,b) (((a)>(b))?(a):(b))
 
 void PrintBlock(int show); // 블록 출력 
 void gamepage(); // 전체 게임 화면 출력 
 int PressKey(); // 키 입력 
 int GetAround(int x,int y,int z,int r); // 주변 검사 
 int Movedown(); // 한칸 내리기 
 void TestBlocks(); // 블록 줄 검사 
 void inpage(); //  벽 안의 블록 상황 출력 
 void PrintNBlock(int show); // 다음 블록 출력 
 void Gamescore(int x,int y); // 게임 점수 출력 
 void PrintShadow(int show); // 그림자 출력 
 void Printlevel(); // 스테이지 출력 
 void ComboPrint(int x); // 콤보 효과 출력 
 void BreakPrint(); // 파괴한 블럭 출력 
 void levelupPrint(); // 스테이지 난이도 출력
 void pausePrint(); // 일시 정지 
 void startpage(); // 게임 초기 화면 출력
  
 typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
 
 void setcursortype(CURSOR_TYPE c){ // 커서 모양 변경 
    CONSOLE_CURSOR_INFO CurInfo;
    switch (c) {
         case NOCURSOR:
              CurInfo.dwSize=1;
              CurInfo.bVisible=FALSE;
              break;
         case SOLIDCURSOR:
              CurInfo.dwSize=100;
              CurInfo.bVisible=TRUE;
              break;
         case NORMALCURSOR:
              CurInfo.dwSize=20;
              CurInfo.bVisible=TRUE;
              break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
 } 
 
void gotoxy(int x,int y){  //커서 위치 이동 
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

enum {EMPTY, BLOCK, nBLOCK, WALL, Shadow};

char *Tiles[5]={"  ","■","▩","▦","□"};
int blocks[7][4][8] = {{{0,-1,1,-1,-1,0,0,0},{-1,-1,-1,0,0,0,0,1},{0,-1,1,-1,-1,0,0,0},{-1,-1,-1,0,0,0,0,1}}, 
	                  {{0,0,1,0,0,-1,-1,-1},{-1,1,-1,0,0,0,0,-1},{0,0,1,0,0,-1,-1,-1},{-1,1,-1,0,0,0,0,-1}},
	                  {{-2,1,-1,1,0,1,1,1},{-1,2,-1,1,-1,0,-1,-1},{-2,1,-1,1,0,1,1,1},{-1,2,-1,1,-1,0,-1,-1}}, // ㅣ 
	                  {{-1,1,0,1,1,1,-1,0},{-1,1,0,1,0,0,0,-1},{0,1,0,0,-1,0,-2,0},{-1,2,-1,1,-1,0,0,0}}, // ㄴ 
	                  {{-1,1,0,1,-1,0,0,0},{-1,1,0,1,-1,0,0,0},{-1,1,0,1,-1,0,0,0},{-1,1,0,1,-1,0,0,0}}, // ㅁ 
	                  {{-2,1,-1,1,0,1,0,0},{0,2,0,1,0,0,-1,0},{-1,1,-1,0,0,0,1,0},{-1,1,0,1,-1,0,-1,-1}},
	                  {{0,1,-1,0,0,0,1,0},{0,1,0,0,1,0,0,-1},{-1,0,0,0,1,0,0,-1},{0,1,0,0,-1,0,0,-1}}}; // ㅗ 
int board[BW+2][BH+2];
int Block,Shape,next_Block,next_Shape; // 블록 종류, 모양 
int nx,ny; // 좌표값 
int Frame=20,Stay; // 블록 내려오는 속도
int score=0;
int level=1,deblock=9; // 게임 스테이지
int Execute = TRUE; // 그림자 출력 

int main(){
	int i,j;
	char ch;
	
	startpage();
	
	setcursortype(NOCURSOR);
    srand((unsigned)time(NULL));
    system("cls");
	
	for(i=0;i<BW+2;i++){
		for(j=0;j<BH+2;j++)board[i][j]=(i==0 || i==BW+1 || j==BH+1)?WALL:EMPTY;
	}
	
	gamepage();
	Block = rand()%7;
	Shape = rand()%4;
	
	for(;1;){
		next_Block = rand()%7;
	    next_Shape = rand()%4; // 그다음 블럭 
	    
		nx = BW/2;
		ny = 1;
		PrintNBlock(TRUE);
		if(Execute==TRUE)PrintShadow(TRUE);
		PrintBlock(TRUE);
		
		if(GetAround(nx,ny,Block,Shape)!=0)break;
		
		Stay = Frame;
		
		for(;2;){
			if(--Stay==0){
				Stay = Frame;
				if(Movedown())break;
			}
			
			if(PressKey())break;
			Sleep(1000/20);
		}
	
		PrintNBlock(FALSE);
		Block = next_Block;
		Shape = next_Shape;
	}
	
	char over;
 	gotoxy(BX+BW+3,BY+7);
 	printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
 	gotoxy(BX+BW+3,BY+8);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+9);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+10);
 	printf("▣      G a m e    O v e r      ▣");
 	gotoxy(BX+BW+3,BY+11);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+12);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+13);
 	printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
	
	over = getch();
	gotoxy(1,28);

	return 0;
}

void PrintBlock(int show){
	int i;
	
		for(i=0;i<8;i+=2){
		gotoxy(BX+(blocks[Block][Shape][i]+nx)*2,BY+blocks[Block][Shape][i+1]+ny);
		puts(Tiles[show?BLOCK:EMPTY]); // show값이 참이면 블럭, 거짓이면 지움 
	}
}

void gamepage(){  // 게임화면 출력 
	int i,j;
	for(i=0;i<BW+2;i++){
		for(j=0;j<BH+2;j++){
		    gotoxy(BX+i*2,BY+j);
		    puts(Tiles[board[i][j]]);
		}
	}
	
	gotoxy(34,2);
	printf("┌─── Next Block ───┐\n");
	gotoxy(34,3);
	printf("│                        │\n");
	gotoxy(34,4);
	printf("│                        │\n");
	gotoxy(34,5);
	printf("│                        │\n");
	gotoxy(34,6);
	printf("│                        │\n");
	gotoxy(34,7);
	printf("│                        │\n");
	gotoxy(34,8);
	printf("│                        │\n");
	gotoxy(34,9);
	printf("└────────────┘\n");
	
	gotoxy(36,13);
	printf("←→ : 방향키");
	gotoxy(36,15);
	printf("↑ : 블럭 회전");
	gotoxy(36,17);
	printf("↓ : 소프트드랍"); 
	gotoxy(36,19);
    printf("SPACE : 하드드랍");
    gotoxy(36,21);
    printf("S : 일시정지 ");
    
    Gamescore(0,0);
    Printlevel();
    BreakPrint();
    
}

int PressKey(){ // 키 누르기 
	char ch;
	
	if(kbhit()){
		ch = getch();
	switch(ch){
		case 'S':
		case 's':
			pausePrint();
			break;
		case LEFT:
		    if(GetAround(nx-1,ny,Block,Shape)==EMPTY){
			    PrintBlock(FALSE);
			   if(Execute==TRUE) PrintShadow(FALSE);
			    
			    nx--;
			    
			    if(Execute==TRUE)PrintShadow(TRUE);
			    PrintBlock(TRUE);
		    }
			break;
		case RIGHT:
			if(GetAround(nx+1,ny,Block,Shape)==EMPTY){
				PrintBlock(FALSE);
				if(Execute==TRUE)PrintShadow(FALSE);
				
				nx++;
				
				if(Execute==TRUE)PrintShadow(TRUE);
				PrintBlock(TRUE);
			}
			break;
		case UP:
			if(GetAround(nx,ny,Block,(Shape+1)%4)==EMPTY){
				PrintBlock(FALSE);
				if(Execute==TRUE)PrintShadow(FALSE);
				
				Shape=(Shape+1)%4;
				
				if(Execute==TRUE)PrintShadow(TRUE);
				PrintBlock(TRUE);
			}
			break;
		case DOWN:
			if(Movedown()){
				return TRUE;
			}
			break;
		case ' ':
			while(Movedown()==FALSE);
			return TRUE;
	}}
    return FALSE;	
}

 int GetAround(int x,int y,int z,int r){ // 주변 블럭 검사 
 	int i,k=EMPTY;
 	
 	for(i=0;i<8;i+=2){
 		k=max(k,board[x+blocks[z][r][i]][y+blocks[z][r][i+1]]); // 두개를 비교하여 블럭이있다면 양수값 반환 
 	}
 	
 	return k; // 0이면 거짓 그외값이면 참 
 }

int Movedown(){ // 아래로 한칸 내리기 
	
	if(GetAround(nx,ny+1,Block,Shape) != EMPTY){
		  TestBlocks();
          return TRUE;
		  }
	
	PrintBlock(FALSE);
	ny++;
	PrintBlock(TRUE);
	
	return FALSE;
}

void TestBlocks(){ // 줄 검사 
	int i,j,x,y,combo=0,n=0;
	for(i=0;i<8;i+=2){
		board[nx+blocks[Block][Shape][i]][ny+blocks[Block][Shape][i+1]]=nBLOCK;
		gotoxy(BX+(blocks[Block][Shape][i]+nx)*2,BY+blocks[Block][Shape][i+1]+ny);
		puts(Tiles[nBLOCK]);
	}
	
    for(i=1;i<BH+1;i++){
	    for(j=1;j<BW+1;j++){
		    if(board[j][i]!=nBLOCK)break;
			} // 줄 검사 
	    
	    if(j==BW+1){n++;
	    
	        ComboPrint(++combo);
		
		    for(x=i;x>1;x--){
		        for(y=1;y<BW+1;y++){
		        	board[y][x]=board[y][x-1];
		        }
		    }
		    deblock++;
		    BreakPrint();
		    Sleep(400);
		    inpage();
		    
	    }
	

	}
	combo = 0;
	if(deblock>=10)Printlevel();
}

void inpage(){ // 게임 박스 안 블럭들 출력 
	int i,j;
	
	for(i=1;i<BW+1;i++){
		for(j=1;j<BH+1;j++){
			gotoxy(BX+i*2,BY+j);
			puts(Tiles[board[i][j]]);
		}
	}
}

void PrintNBlock(int show){ // 다음 블럭 출력 
	int i;
	
	for(i=0;i<8;i+=2){
		gotoxy(48+blocks[next_Block][next_Shape][i]*2,5+blocks[next_Block][next_Shape][i+1]);
		puts(Tiles[show?BLOCK:EMPTY]); // show값이 참이면 블럭, 거짓이면 지움 
	}
}

void Gamescore(int x,int y){ // 게임 점수 
	score+=x;
	score+=y;
	gotoxy(30,24);
	printf("Score : %d",score);
}

void PrintShadow(int show){
	int i;
	int yy;
	
	for(yy=ny;GetAround(nx,yy,Block,Shape)==EMPTY;yy++);
	
	for(i=0;i<8;i+=2){
		gotoxy(BX+(blocks[Block][Shape][i]+nx)*2,BY+blocks[Block][Shape][i+1]+yy-1);
		puts(Tiles[show?Shadow:EMPTY]);
	}
}

void Printlevel(){
	int i,j;
	if(deblock>=10){
		
		gotoxy(BX+8,8);
		printf("Next Stage!");
		levelupPrint();
		
		deblock=0;
		
		for(i=BH;i>BH-level;i--){
		        for(j=1;j<BW+1;j++){
		        	gotoxy(BX+j*2,BY+i);
		        	puts("◈");
					Sleep(40); 
		        }
		    }
		    Sleep(100);
		for(i=BH;i>level-1;i--){
		        for(j=1;j<BW+1;j++){
		        	board[j][i]=board[j][i-level];
		        }
		    }
		
		Sleep(300);
		
		inpage();
		if(Frame>4)Frame-=4;
		else Frame=2;
		
		level++;
		if(level==7)Execute=FALSE; // 그림자 출력 제거 
	}
	
	gotoxy(30,26);
	printf("Stage : %d",level);
	fflush(stdin);
	BreakPrint();
}

 void ComboPrint(int x){
 	gotoxy(BX+8,8);
	printf("%d combo!",x); // 콤보
	gotoxy(BX+16,9);
	printf("+%d",x*x*100);
	gotoxy(BX+16,10);
	printf("+%d",level*x*100);
	Gamescore(x*x*100,level*x*100);
 }
 
 void BreakPrint(){
 	gotoxy(30,27);
    printf("Break block :%2d/10",deblock);
 }
 
 void levelupPrint(){
 	if(level <= 6){
	 gotoxy(BX+8,9);
	 printf("Speed up !");
    }
    else if(level == 6){
    	gotoxy(BX+8,9);
    	printf("Delete Shadow !");
    }
    else if(level == 7){
    	gotoxy(BX+8,9);
    	printf("Block generate !");
    }
 }
 
 void pausePrint(){
 	char pause;
 	gotoxy(BX+BW+3,BY+7);
 	printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
 	gotoxy(BX+BW+3,BY+8);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+9);
 	printf("▣         P  A  U  S  E        ▣");
 	gotoxy(BX+BW+3,BY+10);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+11);
 	printf("▣      아무 키나 누르세요      ▣");
 	gotoxy(BX+BW+3,BY+12);
 	printf("▣                              ▣");
 	gotoxy(BX+BW+3,BY+13);
 	printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
 	pause = getch();
 	system("cls");
 	gamepage();
 	inpage();
 	PrintNBlock(TRUE);
 	PrintBlock(TRUE);
 	PrintShadow(TRUE);
 }
 
 void startpage(){
 	
 	system("cls");
 }
    
	
