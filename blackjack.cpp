#include <stdio.h>

#include <iostream>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <string>

int numOfPlayers;
int startingChips;
int playedCards;
int cards[208];

enum Suit {
    clubs,
    spades,
    hearts,
    diamonds,
	invalid,
};


struct Card {
	enum Suit suit;
	char value;
};
//values
//0 1 2 3 4 5 6 7 8 9 10 11 12
//2 3 4 5 6 7 8 9 T J Q  K  A


struct Player {
	int number;
	int chips;
	int bet;
	struct Card currentHandCards[10];
	char isPlaying;
};

void getRandomCard(Card* card){
	int cardIndex = cards[playedCards];
	playedCards++;
	cardIndex = cardIndex % 52; //there are only 52 cards in a deck every number over 52 loops into the next deck of cards
	char _suit = cardIndex / 13; //the cards are orderd every 13 cards a new suit starts
	char _type = cardIndex % 13; //every 13 cards the value loops
	card->suit = Suit(_suit);
	card->value = _type;
}

std::string cardToString(struct Card card){
	char type = '0';
	switch(card.value){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			type += card.value + 2;
			break;
		case 8:
			type = 'T';
			break;
		case 9:
			type = 'J';
			break;
		case 10:
			type = 'Q';
			break;
		case 11:
			type = 'K';
			break;
		case 12:
			type = 'A';
			break;
	}
	char suit;
	switch(card.suit){
		case Suit::clubs:
			suit = 'c';
			break;
		case Suit::spades:
			suit = 's';
			break;
		case Suit::hearts:
			suit = 'h';
			break;
		case Suit::diamonds:
			suit = 'd';
			break;
		case Suit::invalid:
			suit = 'i';
			break;
	}
	std::string st;
	st += suit;
	st += type;
	return st;
	
}

char checkBlackjack(Player* player){
	Card* cards = player->currentHandCards;
	int val1 = cards[0].value;
	int val2 = cards[1].value;
	return ((val1 == 12)/*A*/ ^ (val2 == 12))/*A*/ && //If only one of the cards is an ace
			(val1 > 7 && val2 > 7);
			//val1 and val2 must be larger than the card value 9
			//the card value 9 is at place 7 of the deck
			//so any place higher has a value of 10 and creates a blackjack
}

void printHand(Player* player){
	int i;
	for(i = 0; i < 10; i++){
		std::cout << player->currentHandCards[i].suit << "<>" << (int)player->currentHandCards[i].value << "|";
	}
	std::cout << std::endl;
}

char getHandValue(Player* player){
	char handValue = 0;
	char aces = 0;
	Card* cards = player->currentHandCards;
	int i;
	for(i = 0; i < 10; i++){
		if(cards[i].suit != Suit::invalid){
			if(cards[i].value < 8)
				handValue += cards[i].value + 2;
			else if (cards[i].value == 12)
				aces += 1;
			else
				handValue +=10;
			//handValue += cards[i].value < 8 ? cards[i].value + 2 : cards[i].value == 12 ? 11 : 10;
		} else {
			break;
		}
	}
	for(i = 0; i < aces; i++){
		if(handValue + 11 <= 21)
			handValue += 11;
		else
			handValue += 1;
	}
	return handValue;
}

char checkBust(Player* player){
	return getHandValue(player) > 21;
}

void addCard(Player* player, Card* card){
	Card* cards = player->currentHandCards;
	int i;
	for(i = 0; i < 10; i++){
		if(cards[i].suit == Suit::invalid){
			cards[i].suit = card->suit;
			cards[i].value = card->value;
			break;
		}
	}
}



void clearHand(Player* player){
	int i;
	Card* cards = player->currentHandCards;
	for(i = 0; i < 10; i++){
		cards[i].suit = invalid;
	}
}


void clearAllHands(Player* players){
	int i;
	for(i = 0; i < numOfPlayers; i++){
		clearHand(&players[i]);
	}
}

void shuffleCards(){
	playedCards = 0;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(&cards[0], &cards[207], std::default_random_engine(seed));
}

int main(int argc, char** argv){
	std::cout << "Number of Players|";
	std::cin >> numOfPlayers; numOfPlayers++;
	std::cout << "Starting amount of Chips|";
	std::cin >> startingChips;
	
	struct Player players[numOfPlayers];
	/*Initialize Player*/
	players[0].isPlaying = 1;
	int i;
	for(i = 0; i < numOfPlayers; i++){
		Player* player = &players[i];
		player->number = i;
		player->chips = startingChips;
		int j;
		/*Clear Hand*/
		for(j = 0; j < 10; j++){
			player->currentHandCards[j].suit = invalid;
		}
		/**/
	}
	/*Initialized*/

	/*Shuffle Cards*/
	for(i = 0; i < 208; i++){
		cards[i]= i;
	}
	shuffleCards();
	/*Shuffled*/
	
	short gameOn = 1;
	while(gameOn){
		
		/*Bets*/
		for(i = 1; i < numOfPlayers; i++){
			int bet;
			std::cout << "bet|p" << i << "|";
			std::cin >> bet;
			players[i].isPlaying = bet; //If bet is 0 isPlaying is evaluated as false
			players[i].bet = bet;
			players[i].chips -= bet;
		}
		/*Bets closed*/

		/*Deal Card*/
		for(i = 0; i < numOfPlayers * 2/*loop 2 times*/; i++){
			if(players[i % numOfPlayers].isPlaying){
				struct Card card;
				getRandomCard(&card);
				addCard(&players[i % numOfPlayers], &card);
				std::cout << "deal|p" << i % numOfPlayers << "|" << cardToString(card) << std::endl;
			}
		}
		/*Cards dealt*/
		
		/*Check Blackjack*/
		for(i = 0; i < numOfPlayers; i++){
			if((int)checkBlackjack(&players[i])){
				if(i == 0){
					//Dealer BlackJack
					int k;
					for(k = 1; k < numOfPlayers; k++)
						players[k].isPlaying = false;
					break;
				} else {
					//Player BlackJack
					players[i].isPlaying = false;
				}
				
			}
		}
		/*Blackjack checked*/
		
		/*Player Actions*/
		char anyonePlaying = 1;
		while(anyonePlaying){
			for(i = 1; i < numOfPlayers; i++){
				Player* player = &players[i];
				if(player->isPlaying){
					std::cout << "action|p" << i << "|";
					int action;
					std::cin >> action;
					if(action == 1){//Stay
						player->isPlaying = 0;
					} else if(action == 2 || action == 3){//Hit or double
						struct Card card;
						getRandomCard(&card);
						addCard(player, &card);
						std::cout << "deal|p" << i << "|" << cardToString(card) << std::endl;
						//TODO maybe tell that the player busted
						//printHand(player);
						if(action == 3){
							player->chips -= player->bet;
							player->bet += player->bet;
							player->isPlaying = 0;
						} else {
							player->isPlaying = !checkBust(player);
						}
					}
				}
			}
			int stopped = 0;
			for(i = 1; i < numOfPlayers; i++){
				stopped += players[i].isPlaying ? 0 : 1;
			}
			anyonePlaying = numOfPlayers - 1 - stopped;
		}
		/*All Players stopped playing*/
		
		/*Dealer gets Cards*/
		char dealerHandValue = getHandValue(&players[0]);
		while(dealerHandValue < 17){
			std::cout << "action|p0|2" << std::endl;
			struct Card card; 
			getRandomCard(&card);
			addCard(&players[0], &card);
			std::cout << "deal|p0|" << cardToString(card) << std::endl;
			dealerHandValue = getHandValue(&players[0]);
		}
		if(dealerHandValue > 21){
			//std::cout << "" << std::endl;
		} else {
			std::cout << "action|p0|1" << std::endl;
		}
		/*Dealer done*/
		
		/*Showdown*/
		for(i = 1; i < numOfPlayers; i++){
			Player* player = &players[i];
			char playerHandValue = getHandValue(player);
			if(checkBlackjack(player)){
				if(checkBlackjack(&players[0])){
					std::cout << "result|p" << i << "|2" << std::endl;
					player->chips += player->bet;
				} else {
					std::cout << "result|p" << i << "|0"/*blackjack*/ << std::endl;
					player->chips += player->bet * 2.5;//Rounds automaticcly down //Should also warn, but doesn't
				}
			} else if(playerHandValue > 21){
				std::cout << "result|p" << i << "|4"/*bust*/ << std::endl;
			} else if(dealerHandValue > 21){
				std::cout << "result|p" << i << "|1" << std::endl;
				player->chips += player->bet * 2;
			} else if(playerHandValue < dealerHandValue){
				std::cout << "result|p" << i << "|3" << std::endl;
			} else if(playerHandValue == dealerHandValue && !checkBlackjack(&players[0])){
				std::cout << "result|p" << i << "|2" << std::endl;
				player->chips += player->bet;
			} else {
				std::cout << "result|p" << i << "|1" << std::endl;
				player->chips += player->bet * 2;
			}
			player->bet = 0;
		} 
		/*All Bets resolved*/
		
		/*Overview*/
		for(i = 1; i < numOfPlayers; i++){
			Player* player = &players[i];
			std::cout << "overview|p" << i << "|chips|" << player->chips << std::endl;
		}
		
		/*Clear Hands*/
		clearAllHands(players);
		std::cout << "remainingCards|" << 208 - playedCards << std::endl;
		short shuffle;
		std::cout << "shuffleCards|";
		std::cin >> shuffle;
		if(shuffle)
			shuffleCards();
		
		std::cout << "nextroud|";
		std::cin >> gameOn;
	}
	
	return 0;
}