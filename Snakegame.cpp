#include <iostream>
#include <windows.h>
#include <ctime> 
#include <cstdio>
#include <cstdlib>
#include <conio.h>

#define LEFT 75   // ����Ű 
#define RIGHT 77
#define UP 72
#define DOWN 80

#define MINX 6  // ������ ���� 
#define MINY 3
#define MAXX 32
#define MAXY 20

using namespace std;

enum {HEAD, BODY, WALL, EMPTY, APPLE}; // �Ӹ�, ����, ����, ����, ���� 
char* tiles[5] = {"��", "��", "��", "  ", "��"};  // �Ӹ�, ����, ����, ����, ����

int x[100], y[100]; // ���� ��ǥ��
int Applex, Appley; // ����� ��ǥ��
int SnakeWay; // ���� ����
int sec, score;

 void gotoxy(int x,int y){  //Ŀ�� ��ġ �̵� 
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class Snake{  // �� 
	private :
		int len;
	public :
		Snake() : len(3) {} // ó�� �ʱ�ȭ�� ���̴� 3 
		
		int Getlen() const{ // ���̰� ��ȯ 
			return len;
		}
		
		void Addlen(){ // ���� ����� ���̰� 1���� 
			len+=100;
		}
			
		~Snake() {}
	};
	
class Gamemanager{  // ��Ʈ�� Ŭ���� 
	private :
		Snake* _Snake;
		int board[MAXY][MAXX]; // ������ 
	public :
		Gamemanager(){
			_Snake = new Snake(); // �÷��̾� ����(1��)
			
			for(int i=0;i<MAXY;i++){
				for(int j=0;j<MAXX;j++){
					board[i][j] = (i==0 || i==MAXY-1 || j==0 || j==MAXX-1)?WALL:EMPTY;
				}
			}
			
			for(int i=0;i<_Snake->Getlen();i++){
			x[i]=(MINX+MAXX)/2; y[i]=(MINY+MAXY)/2+i;  // ù �Ӹ��� ��ġ 
		    }
		}
		
		void PrintBoard() const{  // ���� ��� 
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
	    
	    void PrintSnake(int show) const{  // ����� 
	    	int n = _Snake->Getlen();
	    	gotoxy(x[0]*2, y[0]);
	    	puts(tiles[HEAD]);
	    	gotoxy(x[1]*2, y[1]);
	    	puts(tiles[BODY]);
	    	gotoxy(x[n]*2, y[n]);
	    	puts(tiles[EMPTY]);
		}
		
		void PrintStart() const{  // ����ȭ�� ��� 
			PrintBoard();
			
			gotoxy(MINX+18, MINY+5);
			cout<<"S N A K E   G A M E   0.0ver";
			gotoxy(MINX+45, MINY+7);
			cout<<"Made by LWG";
			gotoxy(MINX+18, MINY+14);
			cout<<"����Ϸ��� �ƹ�Ű�� ��������";
			
			getch();
		}
		
		void PrintAPPLE(){  // ���� ���
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
		
		int SnakeAround(){ // �ֺ� �˻�
		    int i;
		    if(x[0]==Applex && y[0]==Appley){_Snake->Addlen(); score+=10; 
			    gotoxy(6,24);  cout<<"score:"<<score;return APPLE;} // �Ӹ��� ���̿� �������� 
		    else if(board[y[0]-MINY][x[0]-MINX/2]==WALL)return WALL; // �Ӹ��� ���� �������� 
		    else { // �Ӹ��� ���뿡 �������� 
		    	for(i=1;i<_Snake->Getlen();i++){
		    		if(x[i]==x[0] && y[i]==y[0])break;
		    	}
		    	if(i!=_Snake->Getlen())return BODY;
		    }
		    
		    return false;
		}
		
		void Movement(int way){ // �� ������ 
			PrintSnake(false);
			for(int i=_Snake->Getlen();i>0;i--){
				x[i]=x[i-1]; y[i]=y[i-1];
			} // ������ǥ�� ��ĭ�� ��� 
			
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
				} // Ű���� ���� �Ӹ���ǥ�� ���� 
			PrintSnake(true);
		}
		
		int PressKey(){ // Ű���� 
			int key;
				key = getch(); // ������ key������ 
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
			} // ���������� �Է¹޾������ 
			return SnakeWay;
		}
		
		void gametitle(){
    	PrintStart();
	    PrintBoard();
	
	    PrintAPPLE();
	    PrintSnake(true);
	    
	    SnakeWay = UP; // ó���̵����� �� 
	    score = 0; // ���� 
	    sec = 100; // �����̴� �ӵ� �ʱⰪ 
	}

		~Gamemanager(){ // �޸����� 
				delete _Snake;
		} 
	};

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // Ŀ����� 

void setcursortype(CURSOR_TYPE c){ // Ŀ�� ��� ���� 
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
	setcursortype(NOCURSOR); // Ŀ�� ���� 
	Gamemanager player; // �÷��̾� ����
	int Around;
	
	srand((unsigned)time(NULL));
	player.gametitle();
	
	while(1){
		if(kbhit()){SnakeWay=player.PressKey();player.Movement(SnakeWay);} // � ���� �Է¹޾������ 
		else player.Movement(SnakeWay); // �Է¹����ʾ������ �״�� ���� 
		
		Around=player.SnakeAround(); // �Ӹ��� �ֺ��� �˻��� ����� ���� 
		
		if(Around==WALL || Around==BODY)break; // ���̳� �����ϰ�� ���α׷����� 
		else if(Around==APPLE)player.PrintAPPLE(); // ���� 
	
		Sleep(sec-score/10); // �̵��ӵ� 
	}
	gotoxy(0,25);	
}
