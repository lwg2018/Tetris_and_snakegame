#include <iostream>
#include <windows.h>
#include <ctime> 
#include <cstdio>
#include <cstdlib>
#include <conio.h>

#define LEFT 75   // 방향키 
#define RIGHT 77
#define UP 72
#define DOWN 80

#define MINX 6  // 게임판 넓이 
#define MINY 3
#define MAXX 32
#define MAXY 20

using namespace std;

enum {HEAD, BODY, WALL, EMPTY, APPLE}; // 머리, 몸통, 벽돌, 공백, 먹이 
char* tiles[5] = {"◎", "○", "▤", "  ", "♡"};  // 머리, 몸통, 벽돌, 공백, 먹이

int x[100], y[100]; // 뱀의 좌표값
int Applex, Appley; // 사과의 좌표값
int SnakeWay; // 뱀의 방향
int sec, score;

 void gotoxy(int x,int y){  //커서 위치 이동 
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class Snake{  // 뱀 
	private :
		int len;
	public :
		Snake() : len(3) {} // 처음 초기화시 길이는 3 
		
		int Getlen() const{ // 길이값 반환 
			return len;
		}
		
		void Addlen(){ // 먹이 섭취시 길이값 1증가 
			len+=100;
		}
			
		~Snake() {}
	};
	
class Gamemanager{  // 컨트롤 클래스 
	private :
		Snake* _Snake;
		int board[MAXY][MAXX]; // 게임판 
	public :
		Gamemanager(){
			_Snake = new Snake(); // 플레이어 생성(1명)
			
			for(int i=0;i<MAXY;i++){
				for(int j=0;j<MAXX;j++){
					board[i][j] = (i==0 || i==MAXY-1 || j==0 || j==MAXX-1)?WALL:EMPTY;
				}
			}
			
			for(int i=0;i<_Snake->Getlen();i++){
			x[i]=(MINX+MAXX)/2; y[i]=(MINY+MAXY)/2+i;  // 첫 머리의 위치 
		    }
		}
		
		void PrintBoard() const{  // 보드 출력 
			system("cls");
			for(int i=0;i<MAXY;i++){
				for(int j=0;j<MAXX;j++){
					gotoxy(MINX + j*2, MINY + i);
					puts(tiles[board[i][j]]);
			}
		gotoxy(6,24);
		cout<<"score:"<<score;
	    }
	    }
	    
	    void PrintSnake(int show) const{  // 뱀출력 
	    	int n = _Snake->Getlen();
	    	gotoxy(x[0]*2, y[0]);
	    	puts(tiles[HEAD]);
	    	gotoxy(x[1]*2, y[1]);
	    	puts(tiles[BODY]);
	    	gotoxy(x[n]*2, y[n]);
	    	puts(tiles[EMPTY]);
		}
		
		void PrintStart() const{  // 시작화면 출력 
			PrintBoard();
			
			gotoxy(MINX+18, MINY+5);
			cout<<"S N A K E   G A M E   0.0ver";
			gotoxy(MINX+45, MINY+7);
			cout<<"Made by LWG";
			gotoxy(MINX+18, MINY+14);
			cout<<"계속하려면 아무키나 누르세요";
			
			getch();
		}
		
		void PrintAPPLE(){  // 먹이 출력
		    int i;
		    while(1){
			 	Applex = MINX/2+1+rand()%(MAXX-2);
	        	Appley = MINY+1+rand()%(MAXY-3);
	        	
	        	for(i=0;i<_Snake->Getlen();i++)
	        	    if(x[i]==Applex && y[i]==Appley)break;
	        	
	        	if(i==_Snake->Getlen())break;
	        }
	        
	        gotoxy(Applex*2, Appley);
	        puts(tiles[APPLE]);
	    }	
		
		int SnakeAround(){ // 주변 검사
		    int i;
		    if(x[0]==Applex && y[0]==Appley){_Snake->Addlen(); score+=10; 
			    gotoxy(6,24);  cout<<"score:"<<score;return APPLE;} // 머리가 먹이에 닿았을경우 
		    else if(board[y[0]-MINY][x[0]-MINX/2]==WALL)return WALL; // 머리가 벽에 닿았을경우 
		    else { // 머리가 몸통에 닿았을경우 
		    	for(i=1;i<_Snake->Getlen();i++){
		    		if(x[i]==x[0] && y[i]==y[0])break;
		    	}
		    	if(i!=_Snake->Getlen())return BODY;
		    }
		    
		    return false;
		}
		
		void Movement(int way){ // 뱀 움직임 
			PrintSnake(false);
			for(int i=_Snake->Getlen();i>0;i--){
				x[i]=x[i-1]; y[i]=y[i-1];
			} // 몸통좌표값 한칸씩 당김 
			
			switch(way){
				case LEFT:
					x[0]-=1;
					break;
				case RIGHT:
					x[0]+=1;
					break;
				case UP:
					y[0]-=1;
					break;
				case DOWN:
					y[0]+=1;
					break;
				} // 키값에 따라 머리좌표값 변경 
			PrintSnake(true);
		}
		
		int PressKey(){ // 키누름 
			int key;
				key = getch(); // 누른값 key에저장 
				switch(key){
					case LEFT:
					case RIGHT:
					case UP:
					case DOWN:
						SnakeWay = key;
						break; 
					default:							
					    key=SnakeWay;
				}
			if(kbhit()){
				key = getch();
			    return key;
			} // 연속적으로 입력받았을경우 
			return SnakeWay;
		}
		
		void gametitle(){
    	PrintStart();
	    PrintBoard();
	
	    PrintAPPLE();
	    PrintSnake(true);
	    
	    SnakeWay = UP; // 처음이동방향 위 
	    score = 0; // 점수 
	    sec = 100; // 움직이는 속도 초기값 
	}

		~Gamemanager(){ // 메모리해제 
				delete _Snake;
		} 
	};

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // 커서모양 

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

int main(void){
	setcursortype(NOCURSOR); // 커서 없앰 
	Gamemanager player; // 플레이어 생성
	int Around;
	
	srand((unsigned)time(NULL));
	player.gametitle();
	
	while(1){
		if(kbhit()){SnakeWay=player.PressKey();player.Movement(SnakeWay);} // 어떤 값을 입력받았을경우 
		else player.Movement(SnakeWay); // 입력받지않았을경우 그대로 진행 
		
		Around=player.SnakeAround(); // 머리의 주변을 검사한 결과값 저장 
		
		if(Around==WALL || Around==BODY)break; // 벽이나 몸통일경우 프로그램종료 
		else if(Around==APPLE)player.PrintAPPLE(); // 먹이 
	
		Sleep(sec-score/10); // 이동속도 
	}
	gotoxy(0,25);	
}
