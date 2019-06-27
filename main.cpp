#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include<ctime>

using namespace std;


enum Direction {Top=1, Down, Right , Left};
struct Map{int Width, Height , FruitX, FruitY; };
struct Snake{int HeadX, HeadY, Tail_N,  Tail_X[50], Tail_Y[50]; Direction dir;};
struct Player{int Score; bool Lose; };

Map Map1; Snake Snake1; Player Player1;

void Generate_fruit(){
    srand(time(NULL));
    Map1.FruitX=rand()%(Map1.Width-2)+1;
    Map1.FruitY=rand()%(Map1.Height-1)+1;
}
void Sheft_Right(int arr[], int size)
{
    for(int i=size-2; i>=0;i--)
        arr[i+1]=arr[i];
}
char getch(){

    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");

    return buf;
}

int _kbhit(void) {

    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void Setup()
{
   Map1.Width=40;
   Map1.Height=20;
   Generate_fruit();

   Snake1.HeadX=Map1.Width/2;
   Snake1.HeadY=Map1.Height/2;
   Snake1.Tail_N=0;

   Player1.Score=0;
   Player1.Lose=false;

}

void Drow()
{
     system("clear");
    for(int i=0; i<Map1.Height; i++)
    {
        for(int j=0; j<Map1.Width; j++)
        {
            if(i==0 || i==Map1.Height-1 ||j==0 || j==Map1.Width-1 )cout<<"*";
            else if(i==Snake1.HeadY && j==Snake1.HeadX)cout<<"O";
            else if(i==Map1.FruitY && j==Map1.FruitX)cout<<"$";
            else
            {
                bool printed= false;
                for(int k=0; k<Snake1.Tail_N; k++)
                {
                    if(Snake1.Tail_X[k]==j && Snake1.Tail_Y[k]==i)
                    {
                        cout<<"o";
                        printed=true;
                    }
                }
                if(!printed)cout<<" ";
            }
        }
        cout<<endl;
    }
    cout<<"Score : "<<Player1.Score<<endl;

}

void Input()
{
    if(_kbhit())
    {
        switch(getch())
        {
            case 'w': Snake1.dir=Top; break;
            case 's': Snake1.dir=Down; break;
            case 'd': Snake1.dir=Right; break;
            case 'a': Snake1.dir=Left; break;
            case 'x': exit(0);
        }
    }

}

void Move()
{
    Sheft_Right(Snake1.Tail_X, 50);
    Sheft_Right(Snake1.Tail_Y, 50);

    Snake1.Tail_X[0]=Snake1.HeadX;
    Snake1.Tail_Y[0]=Snake1.HeadY;

    switch(Snake1.dir)
    {
        case Top: Snake1.HeadY--;break;
        case Down: Snake1.HeadY++;break;
        case Right:Snake1.HeadX++;break;
        case Left: Snake1.HeadX--;break;
    }

    if(Snake1.HeadY>=Map1.Height-1 || Snake1.HeadY<=0 || Snake1.HeadX>=Map1.Width-1|| Snake1.HeadX<=0)
        Player1.Lose=true;
    if(Snake1.HeadX==Map1.FruitX && Snake1.HeadY==Map1.FruitY)
    {
        Player1.Score++;
        Snake1.Tail_N++;
        Generate_fruit();
    }
}
int main(void)
{

    Setup();

    while(!Player1.Lose)
    {
        Drow();
        Input();
        Move();
    }


    if(Player1.Lose)cout<<"Game over\n";
    system("pause");
    return 0;

}