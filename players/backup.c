#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

const char myName[] = "03240288";

//broken
//const char deployment[] = "Ba2a3a4a5 Cc8d8e8 Ch6g6f6 De0e1 De3e4 Dg6g7 Sg0 Sg2 Sg4 Sg6 ";


//works
const char deployment[] = "Bi3i4i5i6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";
 
//strong-ta
//const char deployment[] ="Bc8d8e8f8 Cg2g3g4 Ci2i3i4 Da2a3 De0e1 Df6g6 Sa6 Sg0 Si6 Sc0 ";

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
      value[ix][iy]=0;
      
      set[ix][iy]=0;
    }
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
  x=-1;
  y=-1;
  int best=0;
   for(int i=0;i<81;i++){
 
   int nx=i/9;
   int ny=i%9;
   
   if(value[nx][ny]>best){
   best=value[nx][ny];
   x=nx;
   y=ny;
   }
  	
   }
   if(x<0 || y<0){
   while(TRUE){
   int i=rand()%81;
    int nx=i/9;
   int ny=i%9;
   if(enemy_board[nx][ny]!=UNKNOWN)continue;
   if(inside(nx,ny)==1){
   x=nx;
   y=ny;
   break;
   }
   }
   }
  
   

  printf("[%s] shooting at %d%d ... ", myName, x, y);
  sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
  cur_x = x;
  cur_y = y;


}

void push(int x, int y){
for(int i=0;i<81;i++){
 int nx=quex[i];
 int ny=quey[i];
 if(nx==x && ny==y)break;
 if(nx==-1 && ny==-1){
 quex[i]=x;
 quey[i]=y;
 break;
 }
}
}

void close(int x,int y){

	for(int i=0;i<8;i++){
	
	int nx=dx[i]+x;
	int ny=dy[i]+y;
	if(inside(nx,ny)==0)continue;
	if(enemy_board[nx][ny]==UNKNOWN)enemy_board[nx][ny]=NOSHIP;
	
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
 	if(inside(nx,ny)==0){
	 
	 continue;
	 }
	if(enemy_board[nx][ny]==UNKNOWN)enemy_board[nx][ny]=NOSHIP;
	 }
	 
	 
	 
//oooo

 for(int i=0;i<4;i++){
	 int cnt=0;
	 for(int t=1;t<=3;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==BSHIP)cnt++;
	 }
	 if(cnt>=3){
	  for(int t=1;t<=3;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==BSHIP)close(nx,ny);
	 }
	 return;
	 }
	 cnt=0;
	for(int t=-1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==BSHIP)cnt++;
	 }
	 if(cnt>=4){
	 for(int t=-1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==BSHIP)close(nx,ny);
	 }
	 return;
	 }
	 }
	 
//ooox
for(int i=0;i<4;i++){
	 int flag=1;
	 for(int t=1;t<=3;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
 	if(inside(nx,ny)==0){
	 flag=0;
	 break;
	 }
	 if(enemy_board[nx][ny]==UNKNOWN || enemy_board[nx][ny]==BSHIP){
	 }
	 else flag=0;
	 
	 }
	 
	 if(flag==1){
	  for(int t=1;t<=3;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 value[nx][ny]++;
	 }
	 }
	 
	 }
//ooxo
for(int i=0;i<4;i++){
	 int flag=1;
	 for(int t=-1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 if(inside(nx,ny)==0){
	 flag=0;
	 break;
	 }
	 if(enemy_board[nx][ny]==UNKNOWN || enemy_board[nx][ny]==BSHIP){
	 }
	 else flag=0;
	 
	 }
	 
	 if(flag==1){
	  for(int t=-1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 value[nx][ny]++;
	 }
	 }
	 
	 }

	 
}

void ship_c(int x,int y){
//3
	  for(int i=0;i<4;i++){
	 int nx=verx[i]+x;
	 int ny=very[i]+y;
	 if(inside(nx,ny)==0)continue;
	if(enemy_board[nx][ny]==UNKNOWN)enemy_board[nx][ny]=NOSHIP;
	 }

//ooo
	 for(int i=0;i<4;i++){
	 int cnt=0;
	 for(int t=1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==CSHIP)cnt++;
	 }
	 if(cnt==2){
	  for(int t=1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==CSHIP)close(nx,ny);
	 }
	 return ;
	 }
	 cnt=0;
	 for(int t=-1;t<=1;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==CSHIP)cnt++;
	 }
	 	 if(cnt==3){
	 	 for(int t=-1;t<=1;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0)break;
	 if(  enemy_board[nx][ny]==CSHIP)close(nx,ny);
	 }
	 	 return;
	 }
	 }
	 
 //oxx
	 for(int i=0;i<4;i++){
	 int flag=1;
	 for(int t=1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0){
	 flag=0;
	 break;
	 }
	 if(enemy_board[nx][ny]==UNKNOWN || enemy_board[nx][ny]==CSHIP){
	 }
	 else flag=0;
	 
	 }
	 
	 if(flag==1){
	  for(int t=1;t<=2;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 value[nx][ny]++;
	 }
	 }
	 
	 }
 //xox
 	 for(int i=0;i<4;i++){
	 int flag=1;
	 for(int t=-1;t<=1;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	  if(inside(nx,ny)==0){
	 flag=0;
	 break;
	 }
	 if(enemy_board[nx][ny]==UNKNOWN || enemy_board[nx][ny]==CSHIP){
	 }
	 else flag=0;
	 
	 }
	 
	 if(flag==1){
	  for(int t=-1;t<=1;t++){
	 int nx=sidex[i]*t+x;
	 int ny=sidey[i]*t+y;
	 value[nx][ny]++;
	 }
	 }
	 
	 }
	 
	 
}

void ship_d(int x,int y){
//2	
	 for(int i=0;i<4;i++){
	 int nx=verx[i]+x;
	 int ny=very[i]+y;
	if(inside(nx,ny)==0)continue;
	if(enemy_board[nx][ny]==UNKNOWN)enemy_board[nx][ny]=NOSHIP;
	 }
	 
//oo	
	int cnt=0;
	for(int t=0;t<4;t++){
	   int nx=sidex[t]+x;
	   int ny=sidey[t]+y;
	    if(inside(nx,ny)==0){

	 continue;
	 }
	 if(  enemy_board[nx][ny]==DSHIP){
 	cnt++;
	 }
	  }
	  if(cnt==1){
	  for(int t=0;t<4;t++){
	   int nx=sidex[t]+x;
	   int ny=sidey[t]+y;
	    if(inside(nx,ny)==0){

	 continue;
	 }
	 if(  enemy_board[nx][ny]==DSHIP){
 	close(nx,ny);
	 }
	  }
	  return;
	  }
	
	
	 
//ox
	  for(int t=0;t<4;t++){
	   int nx=sidex[t]+x;
	   int ny=sidey[t]+y;
	    if(inside(nx,ny)==0){

	 continue;
	 }
	 if(enemy_board[nx][ny]==UNKNOWN || enemy_board[nx][ny]==DSHIP){
	 value[nx][ny]++;
	 }
	  
	  }
	 
}

void ship_s(int x,int y){
	set[x][y]=1;
	 close(x,y);
}

 
void reset_val(void){
for(int x=0;x<9;x++){
for(int y=0;y<9;y++){
value[x][y]=0;
}
}
}

void update_val(void){

reset_val();

for(int x=0;x<9;x++){
for(int y=0;y<9;y++){
if(enemy_board[x][y]!=UNKNOWN)value[x][y]=-100;
}

}
for(int x=0;x<9;x++){
for(int y=0;y<9;y++){ 
if(enemy_board[x][y]==BSHIP)ship_b(x,y);
else if(enemy_board[x][y]==CSHIP)ship_c(x,y);
else if(enemy_board[x][y]==DSHIP)ship_d(x,y);
else if(enemy_board[x][y]==SSHIP)ship_s(x,y);
}
}

}
 

 



void record_result(int x,int y,char line[])
{
  if(line[13]=='B')
  {
 
enemy_board[x][y]=BSHIP;
 
 
 
  }
  else if(line[13]=='C')
  {
 
enemy_board[x][y]=CSHIP;
 
 
  }
  else if(line[13]=='D')
  {
enemy_board[x][y]=DSHIP;
 
 
  }
  else if(line[13]=='S')
  {
 
enemy_board[x][y]=SSHIP;
 
 
  }
  else if(line[13]=='R')
  {
 
enemy_board[x][y]=ROCK;
 
  }
  else
  {
 
 
enemy_board[x][y]=NOSHIP;

 
  }
  update_val();
 
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
