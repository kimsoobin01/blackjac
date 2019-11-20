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
//�� ������ �÷���
int turnEnd = 0;
//ī�� ���� ����
int handCardNum = 2;
//���� ī�� ���� ��
int serverCardNum = 0;
//���� ���� ��
int serverCardSum = 0;
//��ǲ ���� ����
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
      printf("   -> your betting (total:$%d) : ", dollar[0]);

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
      do
      {

         //1~N_MAX_BET��ŭ�� ������  ���� �����ؼ� ���� �ݾ׿� �־��ش�.
         bet[i] = (rand() % N_MAX_BET) + 1;

         //���� ���� ������ �ݾ��� ���� �� �ݾ׺��� ũ�ٸ�
         if (N_MAX_BET > dollar[i])
         {
            //�� ������ ���ؼ�
            int difBet = N_MAX_BET - dollar[i];

            //���� ��ŭ ���� ������ �����Ѵ�.
            bet[i] = (rand() % N_MAX_BET - difBet) + 1;
            continue;
         }
      } while (bet[i] == 0);

      //�� �÷��̾� ������ �ݾװ� �� �ݾ��� ������ �Ѿ��� ���
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
   
   //���� ī�� �������ͽ� ���
   printf("------------CARD OFFERING--------------\n");
   printf("--- server         : X ");
   printCard(cardhold[n_user][1]);
   printf("\n");

   //���� �ʱ� �������ͽ� ���
   printf("   -> you          : ");
   printCard(cardhold[0][0]);
   printCard(cardhold[0][1]);
   printf("\n");

   //�÷��̾� �ʱ� �������ͽ� ���
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
   
   //Aī�� ��ȭ �÷���
   int change = 0;
   //ī�� �� �հ� �ʱ�ȭ
   cardSum[user] = 0;

   //������ �ִ� ��� ī�带 �ϳ��� ������ ���Ѵ�.
   for (i = 0; i < handCardNum; i++)
   {
      //ī�� ���� ���ڸ� �޾ƿ´�.
       int getNum = getCardNum(cardhold[user][i]);

       cardSum[user] += getNum;

       //getNum �� ���� 1�� ���
       if (getNum == 1)
       {
          //chage 1�� �ٲ��ش�.
          change = 1;
       }
   }

   //change�� 1�̵Ǹ�
   if (change == 1)
   {
      //ī�� ������ 11���� �۰ų� �������
      if (cardSum[user] <= 11)
      {
         cardSum[user] += 10;
      }
       
   }

   //���� 21�� ��쿡
   if (cardSum[user] == 21)
   {
      dollar[user] += bet[user] *2;
      printf("Black jack! you win!! --> +$%d ($%d)", bet[user]*2, dollar[user]);
      //�� ����
      turnEnd = 1;
   }
   //���� 21 �ʰ��� ��쿡
   else if (cardSum[user] > 21)
   {
      dollar[user] -= bet[user];
      printf("Dead (sum:%d) --> -$%d ($%d)", cardSum[user], bet[user], dollar[user]);
      //�� ����
      turnEnd = 1;
   }

}

//���� ī�� �� �� ���
int calcsServerStepResult() {

   int i;
   //Aī�� ��ȭ �÷���
   int change = 0;
   //ī�� �� �հ� �ʱ�ȭ
   serverCardSum = 0;

   //������ �ִ� ��� ī�带 �ϳ��� ������ ���Ѵ�.
   for (i = 0; i < handCardNum; i++)
   {
      //ī�� ���� ���ڸ� �޾ƿ´�.
      int getNum = getCardNum(cardhold[n_user][i]);

      serverCardSum += getNum;

      //getNum �� ���� 1�� ���
      if (getNum == 1)
      {
         //chage 1�� �ٲ��ش�.
         change = 1;
      }
   }

   //change�� 1�̵Ǹ�
   if (change == 1)
   {
      //ī�� ������ 11���� �۰ų� �������
      if (serverCardSum <= 11)
      {
         serverCardSum += 10;
      }

   }

   //���� 21�� ��쿡
   if (serverCardSum == 21)
   {
      printf("Black jack! server win!!\n");
      printf("[[[[[[[server result is.... Blackjack ]]]]]]]");
      //�� ����
      turnEnd = 1;
   }
   //���� 21 �ʰ��� ��쿡
   else if (serverCardSum > 21)
   {
      printf("serever Dead (sum:%d)\n", serverCardSum);
      printf("[[[[[[[server result is.... Overflow!! ]]]]]]]");
      //�� ����
      turnEnd = 1;
   }

}

//���� �׼�
int getAction(void) {

   //�� ���� �÷��װ� 1�̶�� ����
   if (turnEnd == 1)
   {
      printf("\n\n\n");
      return -1;
   }

   printf("Action? (0 - go, others - stay) :");

   input = getIntegerInput();

   //input �� 0�̸� GO
   //ī�带 ���� �� �޴´�.
   if (input == 0)
   {
      cardhold[0][handCardNum] = pullCard();
      handCardNum++;
   }
   //Stay
   else
   {
      //�� ����
      turnEnd = 1;
      printf("\n\n");
   }
   

}

//��ǻ�� �÷��̾� �׼�
int AIGetAction(int user)
{
   //�� ���� �÷��װ� 1�̶�� ����
   if (turnEnd == 1)
   {
      printf("\n\n\n");
      return -1;
   }

   //ī�� ���� 17 �̻��̸�
   if (cardSum[user] >= 17)
   {
      //�� ����
      turnEnd = 1;

      printf("STAY!");
      printf("\n\n\n");
   }
   //17�̸��̸�
   else
   {
      printf("GO!");
      cardhold[user][handCardNum] = pullCard();
      handCardNum++;
      printf("\n");
   }
}

//���� �׼�
int ServerAction()
{
   //�� ���� �÷��װ� 1�̶�� ����
   if (turnEnd == 1)
   {
      printf("\n\n\n");
      return -1;
   }

   //ī�� ���� 17 �̻��̸�
   if (serverCardSum >= 17)
   {
      //�� ����
      turnEnd = 1;

      printf("STAY!\n");
      printf("[[[[[[[server result is.... %d ]]]]]]]", serverCardSum);
      printf("\n\n\n");
   }
   //17�̸��̸�
   else
   {
      printf("GO!\n");
      cardhold[n_user][handCardNum] = pullCard();
      handCardNum++;
   }
}

int checkResult() {
   
   int i;
   //������ �հ谡 21��  �ʰ��Ҷ�
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

         //ī�尡 21 �ʰ���
         if (cardSum[i] > 21)
         {
            printf("lose due to overflow! ($%d)\n", dollar[i]);
         }
         //�����ϰ��
         else if (cardSum[i] == 21)
         {
            printf("BlackJack! Win! ($%d)\n", dollar[i]);
         }
         //21���� ������ ������ �¸�
         else
         {
            dollar[i] += bet[i];
            printf("Win! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
         }
      }

   }
   //������ �հ谡 21�� ���
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
         //21���� ������ ������ �й�
         else
         {
            dollar[i] -= bet[i];
            printf("lose! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
         }
      }
   }
   //������ �հ谡 21���� �������
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
         //21���� ������
         else
         {
            //�������� ���ų� ũ�� �¸�
            if (cardSum[i] >= serverCardSum)
            {
               dollar[i] += bet[i];
               printf("Win! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
            }
            //�������� ������ �й�
            else if (cardSum[i] < serverCardSum)
            {
               dollar[i] -= bet[i];
               printf("lose! (sum:%d) --> ($%d)\n", cardSum[i], dollar[i]);
            }


         }
      }
   }

   //�÷��̾��߿� �Ѹ��̶� 0���� �ȴٸ�
   for (i = 0; i < n_user; i++)
   {
      if (dollar[i] == 0)
      {
         //���� ���� �÷��� 1�� �����.
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

   //������ �ٸ� �÷��̾ �����Ѵٸ�
   if (n_user > 2)
   {
      printf("player's money : ");
      for (i = 1; i < n_user; i++)
      {
         printf("$%d ", dollar[i]);
      }
      printf("\n\n\n");
   }
   //���ٸ�
   else
   {
      printf("\n\n\n");
   }

   //�ִ밪�� ����� �޷��� �־��ش�.
   int maxValue = dollar[0];
   //�ִ� �ε����� ����� �ε����� ����
   int maxIndex = 0;

   //�ִ밪�� ���Ѵ�.
   for (i = 1; i < n_user; i++)
   {
      if (maxValue < dollar[i])
      {
         maxValue = dollar[i];
         maxIndex = i;
      }
   }

   //���� maxIndex �� 0�̸� ����ڰ� win 
   if (maxIndex == 0)
   {
      printf("you win!!!\n");
   }
   //�ƴ϶�� maxIndex +1 �� �÷��̾ win
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

   //������ ������ �ִ� ������
   max_user = n_user + 1;

   //Game initialization --------
   //1. players' dollar

   //2. card tray
   mixCardTray();
   printf(" --> card is mixed and put into the tray\n\n");
   //Game start --------
   do {
      //�� ���� �� ���� ī�� ���
      cardIndex = N_CARD - (top + 1);
      printf("\n-------------------------------------------------\n");
      printf("------------- Round %d (cardIndex:%d) ---------------------------\n",roundIndex+1,cardIndex);
      printf("-------------------------------------------------\n\n");

      betDollar();
      offerCards(); //1. give cards to all the players

      //���� gameEnd �÷��װ� 0�� �ƴ϶��
      if (gameEnd != 0)
      {
         printf("Card ran out of the tray!! finishing the game.");
         //�ǳʶپ ���� �����Ѵ�.
         continue;
      }

      printCardInitialStatus();
      printf("\n------------------ GAME start --------------------------\n");

      //each player's turn
      for (i = 0; i < max_user;i++) //each player
      {
         //turnEnd �÷��� �ʱ�ȭ
         turnEnd = 0;
         //���� �÷��̾� �Ͽ� ī�� ���� Ƚ�� �ʱ�ȭ
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


            //�������
            if (i == 0)
            {
               //print current card status printUserCardStatus();
               printUserCardStatus(i, handCardNum);
               //check the card status ::: calcStepResult()
               calcStepResult(i);
               //GO? STOP? ::: getAction()
               getAction();
            }
            //�������
            else if (i == max_user-1)
            {
               //print current card status printUserCardStatus();
               printUserCardStatus(i, handCardNum);
               //���� ī�� ���
               calcsServerStepResult();
               //���� �׼�
               ServerAction();
            }
            //������ �÷��̾���̶��
            else
            {
               //print current card status printUserCardStatus();
               printUserCardStatus(i, handCardNum);
               //check the card status ::: calcStepResult()
               calcStepResult(i);
               //�÷��̾� ai �׼�
               AIGetAction(i);
            }

            //���� gameEnd �÷��װ� 0�� �ƴ϶��
            if (gameEnd != 0)
            {
               printf("Card ran out of the tray!! finishing the game.");
               //�ǳʶپ ���� �����Ѵ�.
               goto END;
            }

            //check if the turn ends or not
         }
      }

      //result
      printf("------------------------ Round %d result ....\n", roundIndex+1);
      checkResult();

      //���� gameEnd �÷��װ� 0�� �ƴ϶��
      if (gameEnd != 0)
      {
         //�ǳʶپ ���� �����Ѵ�.
         goto END;
      }

      //���� �� ����
      roundIndex++;
   } while (gameEnd == 0);

   //�� �������� �ǳʶڴ�.
   END:

   //���� �÷��̾��� �¸��ڸ� �̴´�.
   checkWinner();


   return 0;
}
