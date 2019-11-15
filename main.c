#include <stdio.h>
#include <stdlib.h>

#define N_CARDSET			1
#define N_CARD				52
#define N_DOLLAR			50


#define N_MAX_CARDNUM		13
#define N_MAX_USER			5
#define N_MAX_CARDHOLD		10
#define N_MAX_GO			17
#define N_MAX_BET			5

#define N_MIN_ENDCARD		30

//int �� ���� ���൵ �ҿ� �ִ���
//serve �Լ� ���� ����� �ؿ� ����  

//card tray object
int CardTray[N_CARDSET*N_CARD]; //card set ����, []�� �迭 (��ó�� ���ڸ� �־ �Ǵ���?)
char [N_CARDSET]={Hart, Space, Dia, Club};
char [N_CARD]={A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K};
int J, Q, K = 10;
int i;
int j;	

srand((unsigned)time(NULL));
for(i; i<52; i++)	
{
	do
	{
		j=rand()%52;
	} while (CardTray[j]==0);
	
}

int cardIndex = 0; // index: �迭 ��� ��ȣ index=0; �迭�� ������ 	�� 


//player info
int dollar[N_MAX_USER];						//dollars that each player has �޷� ������  
int n_user;									//number of users ���� �� �Է°�  


//play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
int cardSum[N_MAX_USER];					//sum of the cards
int bet[N_MAX_USER];						//current betting 
int gameEnd = 0; 							//game end flag

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
//ī�� �� ��� �� �� (���� ��) 
int getCardNum(int cardnum) {
}

//print the card information (e.g. DiaA)
//ī�� ���� ���� Ʈ 
void printCard(int cardnum) {
	
}


//card array controllers -------------------------------

//mix the card sets and put in the array
//ī�� ����, tray�� ���� 
int mixCardTray(void) {

}

//get one card from the tray
//ī�� �����ϴ� �Լ�  
int pullCard(void) {
}


//playing game functions -----------------------------

//player settiing
int configUser(void) {
	printf("Input the number of players(MAX: 5) : \n"); 
	input = getIntegerInput(); //�÷��̾� �� �Է� ���� 
	printf("-------------------------------------\n");
	printf("-------------ROUND 1-----------------\n"); 
	printf("-------------------------------------\n");
}


//betting 
int betDollar(void) {
	int i;
	
	printf("------------BETTING STEP-------------\n");
	printf("your betting (total 50$): \n");
	input = getIntegerInput("%d", &i); //���� �� �Է� ����  
	printf("player1 bets $%d (out of $50)\n"); //�ٸ� �÷��̾�� �Է� �� �÷��̾� �� ���? 
	printf("-------------------------------------\n");
}


//offering initial 2 cards (�� �徿 �ʱ� ī�� ����)  
void offerCards(void) {
	int i;
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard(); //cardhold: ���� �������� ���� ī��  
		cardhold[i][1] = pullCard();
	}
	//2. give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//print initial card status
void printCardInitialStatus(void) {
	printf("--------CARD OFFERING------------\n");
	printf("dealer   :   X    %d", i);
	printf("you      :   %d    %d", a, b);
	printf("player1  :   %d    %d", c, d);
	printf("player2  :   %d    %d", e, f);

}

//���� turn player���� go/stop ���� �� �� ���� 
int getAction(void) {
	//0�� go, �ٸ� Ű�� stop
}

//���� �� �÷��̾� ī�� ���� ���  
void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : "); //����� 2���ε� int�� �ϳ��� �ص� ��������  
	for (i=0;i<cardcnt;i++)
		printCard(cardhold[user][i]);
	printf("\t ::: "); //�� \t�� ���ٳ�  
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
//�ջ� ��� �� ���� ���� �Ǵ� 
//turn�� ���� �� ���� ���� �Ǵ� 
//���� ��Ȯ�� ���� ���� �Ǵ��� �߿� 
//�ؿ� �� �߿�  
int calcStepResult() {
	
}

//�� ����  �� �÷��̾� ���� ��� ��� 
int checkResult() {
	printf("  -> your result : ");  // �̱� �� ī�� �ջ� ��, ���� �� 
	printf("1'th player's result : "); // �Ѿ� �� ��, lose due to overflow, ���� �� (�޴� ���� ������ ��)  
	printf("2'th player's result : "); // ���� ��, blackjac! win, ���� ��  ���� �� �� �ڸ����� �ٷ� �� (���� ��) 
	
}

//���� ���� �� ���� �¸��� �Ǵ� 
int checkWinner() {
	printf("game end! your money :$ %d, players's money :$%d $%d $%d $%d", );
	printf("~~ win");
}


//���� ��Ģ ���� ���� ��  
int main(int argc, char *argv[]) {
	int roundIndex = 0;
	int max_user;
	int i;
	
	srand((unsigned)time(NULL));
	
	//set the number of players
	configUser();


	//Game initialization -------- (�ʱ�ȭ) 
	//1. players' dollar �� �������� ���  
	
	//2. card tray ī�尡 �� �������� ���  
	mixCardTray();



	//Game start -------- ���� ���۽�, ���� ������  (��ü ���� ¥�ֽ�) 
	do {
		
		betDollar();  //����  
		offerCards(); //1. give cards to all the players 
		
		printCardInitialStatus();
		printf("\n------------------ GAME start --------------------------\n"); //�տ� \n ������ ��� �Ǵ���.  
		
		//each player's turn
		for () //each player
		{
			while () //do until the player dies or player says stop
			{
				//print current card status printUserCardStatus();
				//check the card status ::: calcStepResult()
				//GO? STOP? ::: getAction()
				//check if the turn ends or not
			}
		}
		
		//result
		checkResult();
	} while (gameEnd == 0);
	
	checkWinner();
	
	
	return 0;
}
