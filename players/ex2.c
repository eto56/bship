#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

const char myName[] = "ex2";
//const char deployment[] = "Ba3a4a5a6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";




//works
//const char deployment[] = "Bi3i4i5i6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";

//weak
//const char deployment[] = "Bf6f5f4f3 Cd4d5d6 Cb5b6b7 Dg0f0 Dc0d0 Da2a3 Se8 Sg8 Si6 Si4 ";


//const char deployment[] = "Bi3i4i5i6 Cc1c2c3 Cc5c6c7 De1e2 Dg3g4 De6f6 Sg1 Se4 Se8 Sg8 ";

//const char deployment[] = "Bc8d8e8f8 Cd0e0f0 Ci2i3i4 Da5a6 Da2a3 Dg2g3 Sc5 Sg5 Se4 Sh7 ";


//strong-ta
//const char deployment[] ="Bc8d8e8f8 Cg2g3g4 Ci2i3i4 Da2a3 De0e1 Df6g6 Sa6 Sg0 Si6 Sc0 ";


//vvst
const char deployment[]="Bd0e0f0g0 Ce2f2g2 Ci2i3i4 Db2c2 Df4g4 Dh6i6 Sd4 Sd6 Sf6 Sg8 ";

int rockx[12]={0,0,0,0,1,1,7,7,8,8,8,8};
int rocky[12]={0,1,7,8,0,8,0,8,0,1,7,8};

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

void respond_with_shot(void)
{
  char shot_string[MSG_LEN];
  int x, y;

  //=====kokokara========

  while(1){
  int rx=rand()%(9);
  int ry=rand()%(9);
  x=rx;
  y=ry;
  int flag=1;
  for(int cnt=0;cnt<12;cnt++){
    if(x==rockx[cnt] && y==rocky[cnt]){
      flag=0;
      break;
    }
  }
  if(flag==1)break;
  }
  //=====kokomade========

  printf("[%s] shooting at %d%d ... ", myName, x, y);
   sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
}

void handle_messages(void)
{
  char line[MSG_LEN];

  srand(getpid());

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
     printf("[%s] result: %c\n", myName, line[13]);
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
