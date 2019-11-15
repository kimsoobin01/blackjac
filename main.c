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

//int 값 따로 빼줘도 소용 있는지
//serve 함수 따로 만들고 밑에 삽입  

//card tray object
int CardTray[N_CARDSET*N_CARD]; //card set 지정, []은 배열 (이처럼 문자를 넣어도 되는지?)
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

int cardIndex = 0; // index: 배열 요소 번호 index=0; 배열이 끝났을 	때 


//player info
int dollar[N_MAX_USER];						//dollars that each player has 달러 나눠줌  
int n_user;									//number of users 유저 수 입력값  


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
//카드 값 계산 진 행 (게임 상) 
int getCardNum(int cardnum) {
}

//print the card information (e.g. DiaA)
//카드 정보 프린 트 
void printCard(int cardnum) {
	
}


//card array controllers -------------------------------

//mix the card sets and put in the array
//카드 섞음, tray에 넣음 
int mixCardTray(void) {

}

//get one card from the tray
//카드 선택하는 함수  
int pullCard(void) {
}


//playing game functions -----------------------------

//player settiing
int configUser(void) {
	printf("Input the number of players(MAX: 5) : \n"); 
	input = getIntegerInput(); //플레이어 수 입력 받음 
	printf("-------------------------------------\n");
	printf("-------------ROUND 1-----------------\n"); 
	printf("-------------------------------------\n");
}


//betting 
int betDollar(void) {
	int i;
	
	printf("------------BETTING STEP-------------\n");
	printf("your betting (total 50$): \n");
	input = getIntegerInput("%d", &i); //베팅 값 입력 받음  
	printf("player1 bets $%d (out of $50)\n"); //다른 플레이어들 입력 값 플레이어 수 어떻게? 
	printf("-------------------------------------\n");
}


//offering initial 2 cards (두 장씩 초기 카드 배포)  
void offerCards(void) {
	int i;
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard(); //cardhold: 현재 선수들이 가진 카드  
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

//현재 turn player에게 go/stop 여부 한 번 결정 
int getAction(void) {
	//0은 go, 다른 키는 stop
}

//현재 턴 플레이어 카드 상태 출력  
void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : "); //출력은 2개인데 int는 하나만 해도 괜찮은지  
	for (i=0;i<cardcnt;i++)
		printCard(cardhold[user][i]);
	printf("\t ::: "); //요 \t는 뭐다냐  
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
//합산 계산 및 승패 여부 판단 
//turn이 종료 될 때도 승패 판단 
//언제 정확히 승패 결정 되는지 중요 
//밑에 값 중요  
int calcStepResult() {
	
}

//턴 종료  후 플레이어 최종 결과 출력 
int checkResult() {
	printf("  -> your result : ");  // 이길 시 카드 합산 값, 베팅 값 
	printf("1'th player's result : "); // 넘어 갈 시, lose due to overflow, 베팅 값 (받는 순간 무조건 패)  
	printf("2'th player's result : "); // 블랙잭 시, blackjac! win, 베팅 값  블랙잭 시 그 자리에서 바로 승 (남의 턴) 
	
}

//게임 종료 후 최종 승리자 판단 
int checkWinner() {
	printf("game end! your money :$ %d, players's money :$%d $%d $%d $%d", );
	printf("~~ win");
}


//게임 규칙 지정 가장 위  
int main(int argc, char *argv[]) {
	int roundIndex = 0;
	int max_user;
	int i;
	
	srand((unsigned)time(NULL));
	
	//set the number of players
	configUser();


	//Game initialization -------- (초기화) 
	//1. players' dollar 다 떨어졌을 경우  
	
	//2. card tray 카드가 다 떨어졌을 경우  
	mixCardTray();



	//Game start -------- 게임 시작시, 위에 들어가야함  (전체 구조 짜주심) 
	do {
		
		betDollar();  //베팅  
		offerCards(); //1. give cards to all the players 
		
		printCardInitialStatus();
		printf("\n------------------ GAME start --------------------------\n"); //앞에 \n 있으면 어떻게 되는지.  
		
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
