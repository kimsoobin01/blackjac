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
//턴 끝나는 플래그
int turnEnd = 0;
//카드 소지 개수
int handCardNum = 2;
//딜러 카드 숫자 합
int serverCardNum = 0;
//딜러 숫자 합
int serverCardSum = 0;
//인풋 전역 변수
int input;
//some utility functions

//get an integer input from standard input (keyboard)
//return : input integer value
//         (-1 is returned if keyboard input was not integer)
int getIntegerInput(void) {
   int input, num;

   num = scanf("%d", &input);

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
      printf("   -> your betting (total:$%d) : ", dollar[0]);

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
      do
      {

         //1~N_MAX_BET만큼의 랜덤한  수를 생성해서 베팅 금액에 넣어준다.
         bet[i] = (rand() % N_MAX_BET) + 1;

         //만약 내가 베팅할 금액이 현재 내 금액보다 크다면
         if (N_MAX_BET > dollar[i])
         {
            //그 차액을 구해서
            int difBet = N_MAX_BET - dollar[i];

            //차액 만큼 빼준 범위를 베팅한다.
            bet[i] = (rand() % N_MAX_BET - difBet) + 1;
            continue;
         }
      } while (bet[i] == 0);

      //각 플레이어 베팅한 금액과 그 금액을 포함한 총액을 출력
      printf("   -> player%d bets $%d (out of $%d)\n",i, bet[i], dollar[i]);
   }

   printf("--------------------------\n\n");
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
   
   int user;
   
   //딜러 카드 스테이터스 출력
   printf("------------CARD OFFERING--------------\n");
   printf("--- server         : X ");
   printCard(cardhold[n_user][1]);
   printf("\n");

   //유저 초기 스테이터스 출력
   printf("   -> you          : ");
   printCard(cardhold[0][0]);
   printCard(cardhold[0][1]);
   printf("\n");

   //플레이어 초기 스테이터스 출력
   for (user = 1; user < n_user; user++)
   {
      printf("   -> player %d     : ", user);
      printCard(cardhold[user][0]);
      printCard(cardhold[user][1]);
      printf("\n");
   }
}


void printUserCardStatus(int user, int cardcnt) {
   int i;

   printf("   -> card : ");
   for (i = 0; i < cardcnt; i++)
      printCard(cardhold[user][i]);
   printf("\t ::: ");
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
int calcStepResult(int user) {

   int i;
   
   //A카드 변화 플래그
   int change = 0;
   //카드 수 합계 초기화
   cardSum[user] = 0;

   //가지고 있는 모든 카드를 하나씩 꺼내서 합한다.
   for (i = 0; i < handCardNum; i++)
   {
      //카드 실제 숫자를 받아온다.
       int getNum = getCardNum(cardhold[user][i]);

       cardSum[user] += getNum;

       //getNum 이 만약 1일 경우
       if (getNum == 1)
       {
          //chage 1로 바꿔준다.
          change = 1;
       }
   }

   //change가 1이되면
   if (change == 1)
   {
      //카드 총합이 11보다 작거나 같을경우
      if (cardSum[user] <= 11)
      {
         cardSum[user] += 10;
      }
       
   }

   //합이 21일 경우에
   if (cardSum[user] == 21)
   {
      dollar[user] += bet[user] *2;
      printf("Black jack! you win!! --> +$%d ($%d)", bet[user]*2, dollar[user]);
      //턴 종료
      turnEnd = 1;
   }
   //합이 21 초과할 경우에
   else if (cardSum[user] > 21)
   {
      dollar[user] -= bet[user];
      printf("Dead (sum:%d) --> -$%d ($%d)", cardSum[user], bet[user], dollar[user]);
      //턴 종료
      turnEnd = 1;
   }

}

//딜러 카드 합 및 결과
int calcsServerStepResult() {

   int i;
   //A카드 변화 플래그
   int change = 0;
   //카드 수 합계 초기화
   serverCardSum = 0;

   //가지고 있는 모든 카드를 하나씩 꺼내서 합한다.
   for (i = 0; i < handCardNum; i++)
   {
      //카드 실제 숫자를 받아온다.
      int getNum = getCardNum(cardhold[n_user][i]);

      serverCardSum += getNum;

      //getNum 이 만약 1일 경우
      if (getNum == 1)
      {
         //chage 1로 바꿔준다.
         change = 1;
      }
   }

   //change가 1이되면
   if (change == 1)
   {
      //카드 총합이 11보다 작거나 같을경우
      if (serverCardSum <= 11)
      {
         serverCardSum += 10;
      }

   }

   //합이 21일 경우에
   if (serverCardSum == 21)
   {
      printf("Black jack! server win!!\n");
      printf("[[[[[[[server result is.... Blackjack ]]]]]]]");
      //턴 종료
      turnEnd = 1;
   }
   //합이 21 초과할 경우에
   else if (serverCardSum > 21)
   {
      printf("serever Dead (sum:%d)\n", serverCardSum);
      printf("[[[[[[[server result is.... Overflow!! ]]]]]]]");
      //턴 종료
      turnEnd = 1;
   }

}

//유저 액션
int getAction(void) {

   //턴 종료 플래그가 1이라면 리턴
   if (turnEnd == 1)
   {
      printf("\n\n\n");
      return -1;
   }

   printf("Action? (0 - go, others - stay) :");

   input = getIntegerInput();

   //input 이 0이면 GO
   //카드를 한장 더 받는다.
   if (input == 0)
   {
      cardhold[0][handCardNum] = pullCard();
      handCardNum++;
   }
   //Stay
   else
   {
      //턴 종료
      turnEnd = 1;
      printf("\n\n");
   }
   

}

//컴퓨터 플레이어 액션
int AIGetAction(int user)
{
   //턴 종료 플래그가 1이라면 리턴
   if (turnEnd == 1)
   {
      printf("\n\n\n");
      return -1;
   }

   //카드 합이 17 이상이면
   if (cardSum[user] >= 17)
   {
      //턴 종료
      turnEnd = 1;

      printf("STAY!");
      printf("\n\n\n");
   }
   //17미만이면
   else
   {
      printf("GO!");
      cardhold[user][handCardNum] = pullCard();
      handCardNum++;
      printf("\n");
   }
}

//딜러 액션
int ServerAction()
{
   //턴 종료 플래그가 1이라면 리턴
   if (turnEnd == 1)
   {
      printf("\n\n\n");
      return -1;
   }

   //카드 합이 17 이상이면
   if (serverCardSum >= 17)
   {
      //턴 종료
      turnEnd = 1;

      printf("STAY!\n");
      printf("[[[[[[[server result is.... %d ]]]]]]]", serverCardSum);
      printf("\n\n\n");
   }
   //17미만이면
   else
   {
      printf("GO!\n");
      cardhold[n_user][handCardNum] = pullCard();
      handCardNum++;
   }
}

int checkResult() {
   
   int i;
   //딜러의 합계가 21을  초과할때
   if (serverCardSum > 21)
   {
      for (i = 0; i < n_user; i++)
      {
         if (i == 0)
         {
            printf("    -> yout result : ");
         }
         else
         {
            printf("    -> %d'th player's result : ",i);
         }

         //카드가 21 초과면
         if (cardSum[i] > 21)
         {
            printf("lose due to overflow! ($%d)\n", dollar[i]);
         }
         //블랙잭일경우
         else if (cardSum[i] == 21)
         {
            printf("BlackJack! Win! ($%d)\n", dollar[i]);
         }
         //21보다 작으면 무조건 승리
         else
         {
            dollar[i] += bet[i];
            printf("Win! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
         }
      }

   }
   //딜러의 합계가 21일 경우
   else if (serverCardSum == 21)
   {
      for (i = 0; i < n_user; i++)
      {
         if (i == 0)
         {
            printf("    -> yout result : ");
         }
         else
         {
            printf("    -> %d'th player's result : ", i);
         }

         if (cardSum[i] > 21)
         {
            printf("lose due to overflow! ($%d)\n", dollar[i]);
         }
         else if (cardSum[i] == 21)
         {
            printf("BlackJack! Win! ($%d)\n", dollar[i]);
         }
         //21보다 작으면 무조건 패배
         else
         {
            dollar[i] -= bet[i];
            printf("lose! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
         }
      }
   }
   //딜러의 합계가 21보다 작을경우
   else
   {
      for (i = 0; i < n_user; i++)
      {
         if (i == 0)
         {
            printf("    -> yout result : ");
         }
         else
         {
            printf("    -> %d'th player's result : ", i);
         }

         if (cardSum[i] > 21)
         {
            printf("lose due to overflow! ($%d)\n", dollar[i]);
         }
         else if (cardSum[i] == 21)
         {
            printf("BlackJack! Win! ($%d)\n", dollar[i]);
         }
         //21보다 작으면
         else
         {
            //딜러보다 같거나 크면 승리
            if (cardSum[i] >= serverCardSum)
            {
               dollar[i] += bet[i];
               printf("Win! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
            }
            //딜러보다 작으면 패배
            else if (cardSum[i] < serverCardSum)
            {
               dollar[i] -= bet[i];
               printf("lose! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
            }


         }
      }
   }

   //플레이어중에 한명이라도 0원이 된다면
   for (i = 0; i < n_user; i++)
   {
      if (dollar[i] == 0)
      {
         //게임 종료 플래그 1로 만든다.
         gameEnd = 1;
         break;
      }
   }

}

int checkWinner() {
   
   int i;
   printf("\n\n\n--------------------------------------------\n\n");
   printf("--------------------------------------------\n\n");
   printf("--------------------------------------------\n\n");
   printf("--------------------------------------------\n\n");
   printf("game end! your money :$%d, ",dollar[0]);

   //나말고 다른 플레이어도 참가한다면
   if (n_user > 2)
   {
      printf("player's money : ");
      for (i = 1; i < n_user; i++)
      {
         printf("$%d ", dollar[i]);
      }
      printf("\n\n\n");
   }
   //없다면
   else
   {
      printf("\n\n\n");
   }

   //최대값에 사용자 달러를 넣어준다.
   int maxValue = dollar[0];
   //최대 인덱스에 사용자 인덱스로 설정
   int maxIndex = 0;

   //최대값을 구한다.
   for (i = 1; i < n_user; i++)
   {
      if (maxValue < dollar[i])
      {
         maxValue = dollar[i];
         maxIndex = i;
      }
   }

   //만약 maxIndex 가 0이면 사용자가 win 
   if (maxIndex == 0)
   {
      printf("you win!!!\n");
   }
   //아니라면 maxIndex +1 의 플레이어가 win
   else
   {
      printf("player %d's win\n", maxIndex);
   }
}



int main(int argc, char *argv[]) {
   int roundIndex = 0;
   int max_user;
   int i;

   srand((unsigned)time(NULL));
   //set the number of players
   configUser();

   //서버를 포함한 최대 유저수
   max_user = n_user + 1;

   //Game initialization --------
   //1. players' dollar

   //2. card tray
   mixCardTray();
   printf(" --> card is mixed and put into the tray\n\n");
   //Game start --------
   do {
      //각 라운드 및 사용된 카드 출력
      cardIndex = N_CARD - (top + 1);
      printf("\n-------------------------------------------------\n");
      printf("------------- Round %d (cardIndex:%d) ---------------------------\n",roundIndex+1,cardIndex);
      printf("-------------------------------------------------\n\n");

      betDollar();
      offerCards(); //1. give cards to all the players

      //만약 gameEnd 플래그가 0이 아니라면
      if (gameEnd != 0)
      {
         printf("Card ran out of the tray!! finishing the game.");
         //건너뛰어서 게임 종료한다.
         continue;
      }

      printCardInitialStatus();
      printf("\n------------------ GAME start --------------------------\n");

      //each player's turn
      for (i = 0; i < max_user;i++) //each player
      {
         //turnEnd 플래그 초기화
         turnEnd = 0;
         //다음 플레이어 턴에 카드 소지 횟수 초기화
         handCardNum = 2;
         if (i == 0)
         {
            printf(">>> my turn! -------------\n");
         }
         else if (i == max_user - 1)
         {
            printf(">>> server turn! ---------------------\n");
         }
         else
         {
            printf(">>> player %d turn! -------------\n", i);
         }

         while (turnEnd == 0) //do until the player dies or player says stop
         {


            //유저라면
            if (i == 0)
            {
               //print current card status printUserCardStatus();
               printUserCardStatus(i, handCardNum);
               //check the card status ::: calcStepResult()
               calcStepResult(i);
               //GO? STOP? ::: getAction()
               getAction();
            }
            //딜러라면
            else if (i == max_user-1)
            {
               //print current card status printUserCardStatus();
               printUserCardStatus(i, handCardNum);
               //딜러 카드 계산
               calcsServerStepResult();
               //딜러 액션
               ServerAction();
            }
            //나머지 플레이어들이라면
            else
            {
               //print current card status printUserCardStatus();
               printUserCardStatus(i, handCardNum);
               //check the card status ::: calcStepResult()
               calcStepResult(i);
               //플레이어 ai 액션
               AIGetAction(i);
            }

            //만약 gameEnd 플래그가 0이 아니라면
            if (gameEnd != 0)
            {
               printf("Card ran out of the tray!! finishing the game.");
               //건너뛰어서 게임 종료한다.
               goto END;
            }

            //check if the turn ends or not
         }
      }

      //result
      printf("------------------------ Round %d result ....\n", roundIndex+1);
      checkResult();

      //만약 gameEnd 플래그가 0이 아니라면
      if (gameEnd != 0)
      {
         //건너뛰어서 게임 종료한다.
         goto END;
      }

      //라운드 수 증가
      roundIndex++;
   } while (gameEnd == 0);

   //이 지점으로 건너뛴다.
   END:

   //최종 플레이어중 승리자를 뽑는다.
   checkWinner();


   return 0;
}
