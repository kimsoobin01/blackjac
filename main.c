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

//ī�� ���� ����
int cardNum[13] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
//ī�� ���� ����
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
   //�޾ƿ� ���� 100���� ���� ������ ���� ���� ���� �ȴ�.
   int realNum = cardnum % 100;

   //����  realNum�� 10���� ū��� realNum�� 10�� �ȴ�.
   if (realNum > 10)
   {
      realNum = 10;
   }

   return realNum;
}

//print the card information (e.g. DiaA)
void printCard(int cardnum) {

   //100�� �ڸ����� "Hart","Dia","Spade","Club" �� ���ϰ�
   //���� 0~13�� ���ڴ� ���� ��Ÿ����.

   //��� ��ȣ
   int shapeNum = cardnum / 100;
   //���� ��ȣ
   int realNum = cardnum % 100;

   //ī�� ��� ���� ���ڿ�
   char shapes[40];
   //ī�� ���ڸ� ���� ���ڿ�
   char nums[10];

   //��� ��ȣ�� ���� ���ڿ��� ����� �־��ش�.
   switch (shapeNum)
   {
      //1�̸� ��Ʈ
   case 1:
      strcpy(shapes, "Hart");
      break;
      //2�̸� ���̾�
   case 2:
      strcpy(shapes, "Dia");
      break;
      //3�̸� �����̵�
   case 3:
      strcpy(shapes, "Spade");
      break;
      //4�̸� Ŭ�ι�
   case 4:
      strcpy(shapes, "Club");
      break;
      //�׿��� ���
   default:
      printf("��� ����");
      break;
   }

   //�����߿��� ��ȯ�ؾ��� �͵� �ٲ��ֱ�
   switch (realNum)
   {
      //1�̸� A
   case 1:
      strcpy(nums, "A");
      break;
      //11�̸� J
   case 11:
      strcpy(nums, "J");
      break;
      //12�̸� Q
   case 12:
      strcpy(nums, "Q");
      break;
      //13�̸� K
   case 13:
      strcpy(nums, "K");
      break;
      //�׿��� ���ڵ��� ���ڿ��� �ٲ��ش�.
   default:
      sprintf(nums, "%d", realNum);
      break;
   }

   //shapes�� ���ڿ��� nums�� �����ش�.
   strcat(shapes, nums);

   //������ ���ڿ� ���
   printf("%s ",shapes);
}


//card array controllers -------------------------------

//mix the card sets and put in the array
int mixCardTray(void) {

   int temp;
   int randomNum;
   int i;
   int j; 

   //4���� ī�� ����� 13���� ī�� ���ڸ� ���� 52���� ī�带 CardTray�� �־��ش�.
   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < N_MAX_CARDNUM; j++)
      {
         CardTray[i*N_MAX_CARDNUM + j] = cardShape[i] + cardNum[j];
      }
   }

   //�����ش�.
   for (i = 0; i < N_CARD; i++)
   {
      randomNum = rand() % (N_CARD-i)+i;

      temp = CardTray[i];
      CardTray[i] = CardTray[randomNum];
      CardTray[randomNum] = temp;

   }
}

//cardTray�� ���� ���� ������ ����Ű�� ����
int top = N_CARD-1;

//get one card from the tray
int pullCard(void) {

   if (top <= 0)
   {
      gameEnd = 1;
      return -1;
   }

   //CardTray���� �� ���ڸ��� �̾Ƽ�
   return CardTray[top--];
}


//playing game functions -----------------------------

//��ǲ ���� ����
int input;

//player settiing
int configUser(void) {
	

   int i;
   //��ǲ ���� ��� 
   printf("input the number of players (Max: %d): ", N_MAX_USER);

   //���� ��ǲ�� �ޱ�
   input = getIntegerInput();

   //���� input ���� 0�̶��
   if (input <= 0)
   {
      printf("invalid input players (%d)\n", input);

      configUser();
   }
   //input ���� N_MAX_USER ������ ũ�ٸ�
   else if(input > N_MAX_USER)
   {
      printf("Too many players!\n");

      configUser();
   }
   //������ ���� ���̶��
   else
   {
      //�÷��� �������� ��ǲ ���� �־��ش�.
      n_user = input;
      //�÷��� ��������ŭ �ʱ� �ں��� N_DOLLAR�� �������ش�.
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

   //input�� 0�� �ƴҰ�쿡 �����.
   do
   {
      //���� �� �ݾװ� �󸶳� �������� ���� ���
      printf("yout betting(total:$%d) : ", dollar[0]);

      //���� ������ �ݾ� �޾ƿ���
      input = getIntegerInput();

      //���� ���� ������ �ݾ��� ���� �� �ݾ׺��� ũ�ٸ�
      if (input > dollar[0])
      {
         printf("you only have $%d! bet again\n", dollar[0]);

         input = 0;
      }
      //�ݾ��� 0���� �۰ų� ���ٸ�
      else if (input <= 0)
      {
         printf("invalid input players (%d)\n", input);

         input = 0;
      }
      //������ �ݾ��̸�
      else
      {
         //��ǲ�� �����Ѵ�.
         bet[0] = input;
      }
   } while (input == 0);


   //������ �÷��̾���� N_MAX_BET �� �ִ��� �������� �����ϰ� �����Ѵ�.
   for (i = 1; i < n_user; i++)
   {
      //1~N_MAX_BET��ŭ�� ������  ���� �����ؼ� ���� �ݾ׿� �־��ش�.
      bet[i] = (rand() % N_MAX_BET) + 1;

      //�� �÷��̾� ������ �ݾװ� �� �ݾ��� ������ �Ѿ��� ���
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
   //���� ī�� �������ͽ� ���
   printf("------------CARD OFFERING--------------\n");
   printf("--- server         : X ");
   printCard(cardhold[n_user - 1][1]);
   printf("\n");

   //���� �ʱ� �������ͽ� ���
   printf("   -> you          : ");
   printCard(cardhold[0][0]);
   printCard(cardhold[0][1]);
   printf("\n");

   //�÷��̾� �ʱ� �������ͽ� ���
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

      //���� gameEnd �÷��װ� 0�� �ƴ϶��
      if (gameEnd != 0)
      {
         //�ǳʶپ ���� �����Ѵ�.
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
