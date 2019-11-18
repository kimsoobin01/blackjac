#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N_CARDSET         1
#define N_CARD            52
#define N_DOLLAR         50


#define N_MAX_CARDNUM      13
#define N_MAX_USER         5
#define N_MAX_CARDHOLD      10
#define N_MAX_GO         17
#define N_MAX_BET         5

#define N_MIN_ENDCARD      30


//card tray object
int CardTray[N_CARDSET*N_CARD];
int cardIndex = 0;


//player info
int dollar[N_MAX_USER];                  //dollars that each player has
int n_user;                           //number of users


//play yard information
int cardhold[N_MAX_USER + 1][N_MAX_CARDHOLD];   //cards that currently the players hold
int cardSum[N_MAX_USER];               //sum of the cards
int bet[N_MAX_USER];                  //current betting 
int gameEnd = 0;                      //game end flag

//카드 숫자 종류
int cardNum[13] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
//카드 문양 종류
int cardShape[4] = { 100,200,300,400 };

//some utility functions

//get an integer input from standard input (keyboard)
//return : input integer value
//         (-1 is returned if keyboard input was not integer)
int getIntegerInput(void) {
   int input, num;

   num = scanf("%d", &input);

   getchar();
   fflush(stdin);

   if (num != 1) //if it fails to get integer
      input = -1;

   return input;
}


//card processing functions ---------------

//calculate the actual card number in the blackjack game
int getCardNum(int cardnum) {
   //받아온 수를 100으로 나눈 나머지 값이 실제 수가 된다.
   int realNum = cardnum % 100;

   //만약  realNum이 10보다 큰경우 realNum은 10이 된다.
   if (realNum > 10)
   {
      realNum = 10;
   }

   return realNum;
}

//print the card information (e.g. DiaA)
void printCard(int cardnum) {

   //100의 자리수는 "Hart","Dia","Spade","Club" 를 뜻하고
   //뒤의 0~13의 숫자는 수를 나타낸다.

   //모양 번호
   int shapeNum = cardnum / 100;
   //숫자 번호
   int realNum = cardnum % 100;

   //카드 모양 담을 문자열
   char shapes[40];
   //카드 숫자를 담을 문자열
   char nums[10];

   //모양 번호에 따라서 문자열에 모양을 넣어준다.
   switch (shapeNum)
   {
      //1이면 하트
   case 1:
      strcpy(shapes, "Hart");
      break;
      //2이면 다이아
   case 2:
      strcpy(shapes, "Dia");
      break;
      //3이면 스페이드
   case 3:
      strcpy(shapes, "Spade");
      break;
      //4이면 클로버
   case 4:
      strcpy(shapes, "Club");
      break;
      //그외의 경우
   default:
      printf("모양 에러");
      break;
   }

   //숫자중에서 변환해야할 것들 바꿔주기
   switch (realNum)
   {
      //1이면 A
   case 1:
      strcpy(nums, "A");
      break;
      //11이면 J
   case 11:
      strcpy(nums, "J");
      break;
      //12이면 Q
   case 12:
      strcpy(nums, "Q");
      break;
      //13이면 K
   case 13:
      strcpy(nums, "K");
      break;
      //그외의 숫자들은 문자열로 바꿔준다.
   default:
      sprintf(nums, "%d", realNum);
      break;
   }

   //shapes의 문자열에 nums를 합쳐준다.
   strcat(shapes, nums);

   //합쳐진 문자열 출력
   printf("%s ",shapes);
}


//card array controllers -------------------------------

//mix the card sets and put in the array
int mixCardTray(void) {

   int temp;
   int randomNum;
   int i;
   int j; 

   //4가지 카드 문양과 13가지 카드 숫자를 곱한 52장의 카드를 CardTray에 넣어준다.
   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < N_MAX_CARDNUM; j++)
      {
         CardTray[i*N_MAX_CARDNUM + j] = cardShape[i] + cardNum[j];
      }
   }

   //섞어준다.
   for (i = 0; i < N_CARD; i++)
   {
      randomNum = rand() % (N_CARD-i)+i;

      temp = CardTray[i];
      CardTray[i] = CardTray[randomNum];
      CardTray[randomNum] = temp;

   }
}

//cardTray의 제일 위에 순서를 가리키는 숫자
int top = N_CARD-1;

//get one card from the tray
int pullCard(void) {

   if (top <= 0)
   {
      gameEnd = 1;
      return -1;
   }

   //CardTray에서 맨 뒷자리를 뽑아서
   return CardTray[top--];
}


//playing game functions -----------------------------

//인풋 전역 변수
int input;

//player settiing
int configUser(void) {
	

   int i;
   //인풋 내용 출력 
   printf("input the number of players (Max: %d): ", N_MAX_USER);

   //유저 인풋값 받기
   input = getIntegerInput();

   //만약 input 값이 0이라면
   if (input <= 0)
   {
      printf("invalid input players (%d)\n", input);

      configUser();
   }
   //input 값이 N_MAX_USER 값보다 크다면
   else if(input > N_MAX_USER)
   {
      printf("Too many players!\n");

      configUser();
   }
   //적절한 범위 안이라면
   else
   {
      //플레이 유저수에 인풋 값을 넣어준다.
      n_user = input;
      //플레이 유저수만큼 초기 자본을 N_DOLLAR로 설정해준다.
      for (i = 0; i < n_user; i++)
      {
         dollar[i] = N_DOLLAR;
      }
   }
}


//betting
int betDollar(void) {

   int i;
   printf("------ Betting Step ------\n");

   //input이 0이 아닐경우에 벗어난다.
   do
   {
      //현재 내 금액과 얼마나 베팅할지 문구 출력
      printf("yout betting(total:$%d) : ", dollar[0]);

      //내가 베팅할 금액 받아오기
      input = getIntegerInput();

      //만약 내가 베팅할 금액이 현재 내 금액보다 크다면
      if (input > dollar[0])
      {
         printf("you only have $%d! bet again\n", dollar[0]);

         input = 0;
      }
      //금액이 0보다 작거나 같다면
      else if (input <= 0)
      {
         printf("invalid input players (%d)\n", input);

         input = 0;
      }
      //적정한 금액이면
      else
      {
         //인풋을 베팅한다.
         bet[0] = input;
      }
   } while (input == 0);


   //나머지 플레이어들은 N_MAX_BET 가 최대인 범위에서 랜덤하게 베팅한다.
   for (i = 1; i < n_user; i++)
   {
      //1~N_MAX_BET만큼의 랜덤한  수를 생성해서 베팅 금액에 넣어준다.
      bet[i] = (rand() % N_MAX_BET) + 1;

      //각 플레이어 베팅한 금액과 그 금액을 포함한 총액을 출력
      printf("player%d bets $%d (out of $%d)\n",i, bet[i], dollar[i]);
   }

   printf("--------------------------\n");
}


//offering initial 2 cards
void offerCards(void) {
   int i;
   //1. give two card for each players
   for (i = 0; i < n_user; i++)
   {
      cardhold[i][0] = pullCard();
      cardhold[i][1] = pullCard();
   }
   //2. give two card for the operator
   cardhold[n_user][0] = pullCard();
   cardhold[n_user][1] = pullCard();

   return;
}

//print initial card status
void printCardInitialStatus(void) {
   //딜러 카드 스테이터스 출력
   printf("------------CARD OFFERING--------------\n");
   printf("--- server         : X ");
   printCard(cardhold[n_user - 1][1]);
   printf("\n");

   //유저 초기 스테이터스 출력
   printf("   -> you          : ");
   printCard(cardhold[0][0]);
   printCard(cardhold[0][1]);
   printf("\n");

   //플레이어 초기 스테이터스 출력
   int user;
   for (user = 1; user < n_user; user++)
   {
      printf("   -> player %d     : ", user);
      printCard(cardhold[user][0]);
      printCard(cardhold[user][1]);
      printf("\n");
   }
}

int getAction(void) {

}


void printUserCardStatus(int user, int cardcnt) {
   int i;

   printf("   -> card : ");
   for (i = 0; i < cardcnt; i++)
      printCard(cardhold[user][i]);
   printf("\t ::: ");
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
int calcStepResult() {

}

int checkResult() {

}

int checkWinner() {

}



int main(int argc, char *argv[]) {
   int roundIndex = 0;
   int max_user;
   int i;

   srand((unsigned)time(NULL));

   //set the number of players
   configUser();


   //Game initialization --------
   //1. players' dollar

   //2. card tray
   mixCardTray();

   //Game start --------
   do {
      cardIndex = N_CARD - (top + 1);
      printf("cardIndex:%d\n", cardIndex);

      betDollar();
      offerCards(); //1. give cards to all the players

      //만약 gameEnd 플래그가 0이 아니라면
      if (gameEnd != 0)
      {
         //건너뛰어서 게임 종료한다.
         continue;
      }

      printCardInitialStatus();
      printf("\n------------------ GAME start --------------------------\n");

      ////each player's turn
      //for () //each player
      //{
      //   while () //do until the player dies or player says stop
      //   {
      //      //print current card status printUserCardStatus();
      //      //check the card status ::: calcStepResult()
      //      //GO? STOP? ::: getAction()
      //      //check if the turn ends or not
      //   }
      //}

      //result
      checkResult();
   } while (gameEnd == 0);

   checkWinner();


   return 0;
}
