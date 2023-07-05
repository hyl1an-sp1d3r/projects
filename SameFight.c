#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_OF_PLAYERS 4
#define NUM_CARDS_DECK 52


struct handInit{
  int card;
};

pthread_t dealer;
pthread_t playerNum[NUM_OF_PLAYERS];
int r = 1, p;
struct handInit player_hand1[4], player_hand2[4];
int counter = 0;
pthread_mutex_t mutex, lock;


void dealAndShuffle();
void *trythisP(void *);
void *trythisD(void *);
void dealCard();
void drawCard(int);
int outputWin(int, int);
void discardCard(int);
void getNextRound();

void dealAndShuffle(){
  pthread_create(&dealer, NULL, trythisD, (void *)&dealer);
  pthread_join(dealer, NULL);
  dealCard();
  pthread_cancel(dealer);
}



void *trythisP(void *arg){
  pthread_mutex_lock(&mutex);
  unsigned long i = 0;
  counter += 1;
  drawCard(p);
  for(i = 0; i < 100000000; i++);
	;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
  return NULL;
}

void *trythisD(void *arg){
  pthread_mutex_lock(&lock);
  int n;
  unsigned long i = 0;
  counter += 1;
  printf("Dealer deals\n");
  for(i = 0; i < 100000000; i++);
	;
  printf("Dealer shuffles\n");
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
  return NULL;

}

void dealCard(){
  int play, n = 1;
  time_t t;
  srand((unsigned) time(&t));
  int c = (rand() % 13) + 1;
  for(play = 0; play < NUM_OF_PLAYERS; play++){
     player_hand1[play].card = (rand() % 13) + 1;  
    if(player_hand1[play].card == 1)
      printf("Player %d has an Ace\n", n);
    else if(player_hand1[play].card == 11)
      printf("Player %d has a Jack\n", n);
    else if(player_hand1[play].card == 12)
      printf("Player %d has a Queen\n", n);
    else if(player_hand1[play].card == 13)
      printf("Player %d has a King\n", n);
    else
      printf("Player %d has a %d card\n", n, player_hand1[play].card);
    n++;
  }
  //return NULL;
}



void drawCard(int play){
  sleep(1);
  int n = play, m;
  time_t t;
  srand((unsigned) time(&t));
  player_hand2[n].card = (rand() % 13) + 1;
  play++;
  if(player_hand2[n].card == 1)
    printf("PLAYER %d drew an Ace\n", play);
  else if(player_hand2[n].card == 11)
    printf("PLAYER %d drew a Jack\n", play);
  else if(player_hand2[n].card == 12)
    printf("PLAYER %d drew a Queen\n", play);
  else if(player_hand2[n].card == 13)
    printf("PLAYER %d drew a King\n", play);
  else
    printf("PLAYER %d drew a %d card\n", play, player_hand2[n].card);
  //return NULL;
  if((play % 2) == 1){
    if(n < 2)
      m = n + 2;
    else
      m = n - 2;
    if(player_hand2[n].card == player_hand1[m].card){
      printf("PLAYER %d: (%d,%d), <>", n+1, player_hand1[n].card, player_hand2[n].card);
      printf(" PLAYER %d has %d\n", m+1, player_hand1[m].card);
      printf("ODD team wins round %d\n", r);
      outputWin(n, m);
      r++;
      //pthread_exit((void *)playerNum[n]);
    } else {
      discardCard(n);
    }
  } else {
    if(n < 3)
      m = n + 2;
    else
      m = n - 2;
    if(player_hand2[n].card == player_hand1[m].card){
      printf("PLAYER %d: (%d,%d), <>", n+1, player_hand1[n].card, player_hand2[n].card);
      printf(" PLAYER %d has %d\n", m+1, player_hand1[m].card);
      printf("EVEN team wins round %d\n", r);
      outputWin(n, m);
      r++;
      //pthread_exit((void *)playerNum[n]);
    } else {
      discardCard(n);
    }
  }
}

int outputWin(int n, int m){
  int i;
  FILE *fp;
  //opens file
  fp = fopen("Wins.txt", "a");				//append
  //checks if successful
  if(fp == NULL){
    perror("ERROR! UNABLE TO OPEN FILE");
    return (-1);
  }
  //input text to the file
  fprintf(fp, "PLAYER %d hand:", n+1);
  if(player_hand2[n].card != 0){
    fprintf(fp, "%d, %d\n", player_hand1[n].card, player_hand2[n].card);
  } else {
    fprintf(fp, "%d\n", player_hand1[n].card);
  }

  fprintf(fp, "PLAYER %d hand:", m+1);
  if(player_hand2[m].card != 0){
    fprintf(fp, "%d, %d\n", player_hand1[m].card, player_hand2[m].card);
  } else {
    fprintf(fp, "%d\n", player_hand1[m].card);
  }
  fprintf(fp, "CARDS USED:");
  for(i = 0; i < NUM_OF_PLAYERS; i++){
    if(player_hand2[i].card != 0){
      fprintf(fp, "%d, %d ", player_hand1[i].card, player_hand2[i].card);
    } else {
      fprintf(fp, "%d ", player_hand1[i].card);
    }
  }
  fprintf(fp, "\n");
  //closes file
  fclose(fp);

}

void discardCard(int n){
  time_t t;
  srand((unsigned) time(&t));
  int x = (rand() % 2);
  if(x == 0){
    player_hand1[n].card = player_hand2[n].card;
    player_hand2[n].card = 0;
  } else {
    player_hand2[n].card = 0;
  }
}

void getNextRound(){
  int i;
}



int main(){
  int i, j;
  //struct player[NUM_OF_PLAYERS];
  printf("Begin Round %d\n", r);
  dealAndShuffle();
  while(r <= 2){
    i = r;
    for(p = 0; p < NUM_OF_PLAYERS; p++){
      pthread_create(&playerNum[p], NULL, trythisP, (void *)&playerNum[p]);
      pthread_join(playerNum[p], NULL);
      if(r > i && r != 3){
	printf("Begin Round %d\n", r);
	dealAndShuffle();
	break;
      }  
      else if(r > i && r ==3){
	break;
      }
    }
  }
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&lock);
  pthread_exit(NULL);
  return 0;
}