#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

const char myName[] = "now";
//const char deployment[] = "Ba2a3a4a5 Cb9c9d9 Ch6i6j6 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";
const char deployment[] = "Bi3i4i5i6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";
int next_x=0,next_y=0;

int inside(int x,int y){
if(0<=x && x<9){
if(0<=y && y<9){
return 1;
}
}
return 0;
}
enum ship {
  UNKNOWN,
  ROCK,
  NOSHIP,
  BSHIP,
  CSHIP,
  DSHIP,
  SSHIP
};

int quex[81];
int quey[81];

int rockx[12]={0,0,0,0,1,1,7,7,8,8,8,8};
int rocky[12]={0,1,7,8,0,8,0,8,0,1,7,8};

int dx[8]={-1,-1,-1,0,0,1,1,1};
int dy[8]={-1,0,1,1,-1,0,-1,1};

int verx[4]={1,1,-1,-1};
int very[4]={1,-1,1,-1};



int value[9][9];
int set[9][9];

int sidex[4]={1,0,-1,0};
int sidey[4]={0,1,0,-1};

int cur_x,cur_y;
enum ship enemy_board[BD_SIZE][BD_SIZE];    // BD_SIZE is 9 (defined in public.h)

void respond_with_name(void)
{
  char *str = (char *)malloc(sizeof(myName));
  strcpy(str, myName);
  send_to_ref(str);
  free(str);
}

void respond_with_deployment(void)
{
  char *str = (char *)malloc(sizeof(deployment));
  strcpy(str, deployment);
  send_to_ref(str);
  free(str);
}


void init_board(void)
{
 int ix, iy;

  for(ix = 0; ix < (BD_SIZE); ix++)
  {
    for(iy = 0; iy < (BD_SIZE); iy++)
    {

      enemy_board[ix][iy]=UNKNOWN;
      value[ix][iy]=1;
      
      set[ix][iy]=0;
    }
  }
  for(int i=0;i<81;i++){
  quex[i]=-1;
  quey[i]=-1;
  }

  

 
  for(int i=0;i<12;i++){
    int rx=rockx[i];
    int ry=rocky[i];
    enemy_board[rx][ry]=ROCK;
    value[rx][ry]=-1;
  }

}


void respond_with_shot(void)
{
  char shot_string[MSG_LEN];
  int x, y;
  
  while (TRUE)
  {
   int best_val=-1;
   x=3;
   y=3;
    for(int i=0;i<81;i++){
    if(best_val<value[i/9][i%9]){
    	x=i/9;
    	y=i%9;
    	best_val=value[i/9][i%9];
    }
    }
    if(best_val==1){
    while(TRUE){
    int box=rand()%81;
    x=box/9;
    y=box%9;
    if( value[x][y]>=0)break;
   
    }
  }  
 
    if( value[x][y]>=0)break;
  }
  
   

  printf("[%s] shooting at %d%d ... ", myName, x, y);
  sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
  cur_x = x;
  cur_y = y;


}
void close(int x,int y){

	for(int i=0;i<8;i++){
	
	int nx=dx[i]+x;
	int ny=dy[i]+y;
	value[nx][ny]=-1;
	
	} 
}
void side_add(int x,int y,int v){

	for(int i=0;i<4;i++){
	int nx=sidex[i]+x;
	int ny=sidey[i]+y;
	if(value[nx][ny]<0 || inside(nx,ny))continue;
	value[nx][ny]+=v;
	}

}


void ship_b(int x,int y){
 //4
 	for(int i=0;i<4;i++){
	 int nx=verx[i]+x;
	 int ny=very[i]+y;
	 value[nx][ny]=-1;
	 }
	 for(int i=0;i<4;i++){
	// int settle=0;
	 for(int t=1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	if(inside(nx,ny)==0 || value[nx][ny]<0)continue;
	 value[nx][ny]++;
	 }
	 
	 }
	 
}

void ship_c(int x,int y){
//3
	  for(int i=0;i<4;i++){
	 int nx=verx[i]+x;
	 int ny=very[i]+y;
	 value[nx][ny]=-1;
	 }
	 
	 
 
	 for(int i=0;i<4;i++){
	 int settle=0;
	 for(int t=1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 
	if(inside(nx,ny)==0 || value[nx][ny]<0)continue;
	 if(enemy_board[nx][ny]==CSHIP){
	 settle+=t;
	 }
	 }
	 if(settle==3){
	 for(int t=0;t<=2;t++){
	  int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 set[nx][ny]=1;
	 close(nx,ny);
	 }
	 
	 }
	 else if(settle==2){
	 side_add(x,y,1);
	 int nx=sidex[i]*2+x;
	 int ny=sidey[i]*2+y;
	 side_add(nx,ny,1);
	 }
	 else if(settle==1){
	 
	 }
	 else{
	 int nx=sidex[i]*2+x;
	 int ny=sidey[i]*2+y;
	 value[nx][ny]++;
	 }
	 
	 }
	 
	 
}

void ship_d(int x,int y){
//2	
	 for(int i=0;i<4;i++){
	 int nx=verx[i]+x;
	 int ny=very[i]+y;
	 value[nx][ny]=-1;
	 }
	 
	 int settle=0;
	 for(int i=0;i<4;i++){
	 int nx=sidex[i]+x;
	 int ny=sidey[i]+y;
	 
	 if(inside(nx,ny)==0 || value[nx][ny]<0)continue;
	 
	 if(enemy_board[nx][ny]==DSHIP){
	 settle=1;
	 set[nx][ny]=1;
	 set[x][y]=1;
	 close(nx,ny);
	 }
	 }
	 
	 if(settle==1)close(x,y);
	 
	 
	 for(int i=0;i<4;i++){
	 int nx=sidex[i]+x;
	 int ny=sidey[i]+y;
	 
	 if(inside(nx,ny)==0 || value[nx][ny]<0)continue;
	 
	 if( settle==1){
	 value[nx][ny]=-1;
	 }
	 else{
	 value[nx][ny]++;
	 }
	 }
	 
}

void ship_s(int x,int y){
	set[x][y]=1;
	 close(x,y);
}



void record_result(int x,int y,char line[])
{
  if(line[13]=='B')
  {
 
enemy_board[x][y]=BSHIP;
value[x][y]=-1;
 ship_b(x,y);
 
  }
  else if(line[13]=='C')
  {
 
enemy_board[x][y]=CSHIP;
value[x][y]=-1;
 ship_c(x,y);
  }
  else if(line[13]=='D')
  {
enemy_board[x][y]=DSHIP;
value[x][y]=-1;
  ship_d(x,y);
  }
  else if(line[13]=='S')
  {
 
enemy_board[x][y]=SSHIP;
value[x][y]=-1;
  ship_s(x,y);
  }
  else if(line[13]=='R')
  {
  value[x][y]=-1;
enemy_board[x][y]=ROCK;
 
  }
  else
  {
 
    value[x][y]=-1;
enemy_board[x][y]=NOSHIP;

 
  }
 
}

void print_board(void){
  int ix, iy;

  for (iy = BD_SIZE - 1; iy >= 0; iy--)
  {
    printf("%2d ", iy);
    for (ix = 0; ix < BD_SIZE; ix++)
    {
      switch(enemy_board[ix][iy])
      {
        case UNKNOWN:
          printf("U ");
          break;
        case NOSHIP:
          printf("N ");
          break;
        case ROCK:
          printf("R ");
          break;
        case BSHIP:
          printf("B ");
          break;
        case CSHIP:
          printf("C ");
          break;
        case DSHIP:
          printf("D ");
          break;
        case SSHIP:
          printf("S ");
          break;
        default:
          break;
      }
    }
    printf("\n");
  }

  printf("  ");
  for (ix = 0; ix < BD_SIZE; ix++)
  {
    printf("%2d", ix);
  }
  printf("\n\n");
}

void handle_messages(void)
{
  char line[MSG_LEN];

  srand(getpid());
  init_board();
  
  while (TRUE)
  {
    receive_from_ref(line);

    if(message_has_type(line, "name?"))
    {
      respond_with_name(); 
    }
    else if(message_has_type(line, "deployment?"))
    {
       respond_with_deployment(); 
    }
    else if(message_has_type(line, "shot?"))
    {
      respond_with_shot(); 
    }
    else if(message_has_type(line, "shot-result:"))
    {
      record_result(cur_x,cur_y,line);
      printf("[%s] result: %c\n", myName, line[13]);
      print_board();
    }
    else if(message_has_type(line, "end:"))
    {
      break;
    }
    else
    {
      printf("[%s] ignoring message: %s", myName, line);
    }
  }
}

int main()
{
  client_make_connection();
  handle_messages();
  client_close_connection();
  return 0;
}
