// Simple blackjack game with some basic logic
// Created May 15, 2024
// Created by: Kyle Koeller

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

struct Card {
	enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
	enum class Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

	Suit suit;
	Rank rank;

	void print() const {
		switch (rank) {
			case Rank::ACE: cout << "Ace"; break;
			case Rank::TWO: cout << "2"; break;
			case Rank::THREE: cout << "3"; break;
			case Rank::FOUR: cout << "4"; break;
			case Rank::FIVE: cout << "5"; break;
			case Rank::SIX: cout << "6"; break;
			case Rank::SEVEN: cout << "7"; break;
			case Rank::EIGHT: cout << "8"; break;
			case Rank::NINE: cout << "9"; break;
			case Rank::TEN: cout << "10"; break;
			case Rank::JACK: cout << "10"; break;
			case Rank::QUEEN: cout << "10"; break;
			case Rank::KING: cout << "10"; break;
		}

		switch (suit) {
			case Suit::HEARTS: cout << " of Hearts"; break;
			case Suit::DIAMONDS: cout << " of Diamonds"; break;
			case Suit::SPADES: cout << " of Spades"; break;
			case Suit::CLUBS: cout << " of Clubs"; break;
		}
	}
};

int getRandomNumber(int min, int max) {
	static random_device rd;
	static mt19937 rng(rd());
	uniform_int_distribution<int> uni(min, max);

	return uni(rng);
}

int getCardValue(const Card& card) {
	switch (card.rank) {
	case Card::Rank::JACK:
	case Card::Rank::QUEEN:
	case Card::Rank::KING:
		return 10;
	case Card::Rank::ACE:
		return 11;
	default: 
		return static_cast<int>(card.rank);
	}
}

vector<Card> initializeDeck(int numDecks) {
	vector<Card> deck;
	// create the deck of cards from the number of decks 
	for (int deckCount = 0; deckCount < numDecks; ++deckCount) {
		for (int suit = static_cast<int>(Card::Suit::HEARTS); suit <= static_cast<int>(Card::Suit::SPADES); ++suit) {
			for (int rank = static_cast<int>(Card::Rank::ACE); rank <= static_cast<int>(Card::Rank::KING); ++rank) {
				deck.push_back({ static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank) });
			}

		}
	}

	// Use a random number generator to shuffle the deck
	random_device rd; // Obtain a random seed from the OS entropy device
	mt19937 rng(rd()); // Initialize the random number generator with the seed
	shuffle(deck.begin(), deck.end(), rng); // Shuffle the deck using the random number generator
	return deck;
}

Card dealCard(vector<Card>& deck) {
	// if the deck is empty initialize a new deck
	if (deck.empty()) {
		cout << "\n\nDeck is empty. Reshuffling...\n\n" << endl;
		deck = initializeDeck(7);
	}

	// after a card is picked and returned, remove that card from the deck
	int index = getRandomNumber(0, static_cast<int>(deck.size()) - 1);
	Card card = deck[index];
	deck.erase(deck.begin() + index);

	return card;
}

// checks if the player wants to hit or stand
bool playerWantsHit() {
	char choice;
	while (true) {
		cout << "Do you want to hit or stand (h/s): ";
		cin >> choice;
		if (choice == 'h' || choice == 'H') {
			return true;
		}
		else if (choice == 's' || choice == 'S') {
			return false;
		}
		else {
			cout << "Invalid Input. Please enter 'h' to hit or 's' to stand." << endl;
		}
	}
}

// returns the value of the hand in numeric form
int getHandValue(const vector<Card>& hand) {
	int value = 0;
	int numAces = 0;

	for (const auto& card : hand) {
		int cardValue = getCardValue(card);
		if (cardValue == 11) {
			numAces++;
		}
		value += cardValue;
	}

	while (value > 21 && numAces > 0) {
		value -= 10;
		numAces--;
	}
	return value;
}

// displays the hands of the player and the dealer, can be optional to reveal both dealer cards
void displayHands(const vector<Card>& playerHand, const vector<Card>& dealerHand, bool showDealerCard) {
	cout << "\nYour Hand: ";
	for (const auto& card : playerHand) {
		card.print();
		cout << ", ";
	}
	cout << "Total: " << getHandValue(playerHand) << endl;

	cout << "Dealer's Hand: ";
	for (size_t i = 0; i < dealerHand.size(); ++i) {
		if (i == 1 && !showDealerCard) {
			cout << "[Hidden Card], ";
		}
		else {
			dealerHand[i].print();
			cout << ", ";
		}
	}

	if (!showDealerCard) {
		cout << "Total: ??";
	}
	else {
		cout << "Total: " << getHandValue(dealerHand);
	}
	cout << endl;
}

// checks if the player wants to split their hand
bool playerWantsSplit() {
	char choice;

	while (true) {
		cout << "\nDo you want to split? (y/n): ";
		cin >> choice;

		if (choice == 'y' || choice == 'Y') {
			return true;
		}
		else if (choice == 'n' || choice == 'N') {
			return false;
		}
		else {
			cout << "\nInvalid input. Please enter 'y' to split or 'n' to continue.\n" << endl;
		}
	}
}

// checks if the player wants to double down their initial hit
bool playerWantsDoubleDown() {
	char choice;

	while (true) {
		cout << "\nDo you want to double down? (y/n): ";
		cin >> choice;

		if (choice == 'y' || choice == 'Y') {
			return true;
		}
		else if (choice == 'n' || choice == 'N') {
			return false;
		}
		else {
			cout << "\nInvalid input. Please enter 'y' to split or 'n' to continue.\n" << endl;
		}
	}
}

// basic strategy for the player to hit or stand
char getStrategyAdvice(const vector<Card>& playerHand, const Card& dealerCard) {
	int playerValue = getHandValue(playerHand);
	int dealerValue = getCardValue(dealerCard);

	if (playerValue >= 17) {
		return 's';
	}
	else if (playerValue <= 11) {
		return 'h';
	}
	else {
		if (dealerValue >= 7) {
			return 'h';
		}
		else {
			return 's';
		}
	}
}

// determines the winner of the hand
int determineWinner(const vector<Card>& playerHand, const vector<Card>& dealerHand) {
	int playerValue = getHandValue(playerHand);
	int dealerValue = getHandValue(dealerHand);

	if (playerValue > 21) {
		return -1; //dealer wins
	}
	else if (dealerValue > 21) {
		return 1; //player wins
	}
	else if (playerValue > dealerValue) {
		return 1; // player wins
	}
	else if (dealerValue > playerValue) {
		return -1; // dealer wins
	}
	else {
		return 0; // tie
	}
}

// Function to handle player's decision to hit or stand
bool playerDecision(const vector<Card>& playerHand, const vector<Card>& dealerHand, bool canDoubleDown) {
	char choice;
	while (true) {
		char advice = getStrategyAdvice(playerHand, dealerHand[0]);
		// if the advice is to stand
		if (advice == 's') {
			cout << "\nPlayer should stand (s/h): ";
		}
		// if the advice is to hit
		else if (advice == 'h') {
			cout << "\nPlayer should hit (s/h): ";
		}
		cin >> choice;

		// Process player's choice
		if (choice == 's' || choice == 'S') {
			return false; // Stand
		}
		else if (choice == 'h' || choice == 'H') {
			return true; // Hit
		}
		else {
			cout << "\nInvalid input. Please enter 's' to stand or 'h' to hit.\n" << endl;
		}
	}
}

int main() {
	int wins = 0;
	int losses = 0;
	int ties = 0;
	const int MIN_BET = 10;
	const int MAX_BET = 100;
	int balance = 100;

	vector<Card> deck = initializeDeck(7);

	while (true) {
		if (balance <= 0) {
			cout << "\n\nYou have run out of money.\n\n" << endl;
			break;
		}
		int betAmount;
		cout << "Current Balance: " << balance << endl;
		cout << "Place your bet (minimum: " << MIN_BET << " , maximum: " << MAX_BET << "): ";
		cin >> betAmount;

		if (betAmount < MIN_BET || betAmount > MAX_BET) {
			cout << "Invalid bet amount. Please enter a bet amount between " << MIN_BET << " and " << MAX_BET << "." << endl;
			continue;
		}

		balance -= betAmount;

		vector<Card> playerHand;
		vector<Card> dealerhand;
		bool canDoubleDown = true;
		bool blackJack = false;
		bool dealer21 = false;
		bool bust = false;
		bool didDoubleDown = false;

		playerHand.push_back(dealCard(deck));
		dealerhand.push_back(dealCard(deck));
		playerHand.push_back(dealCard(deck));
		dealerhand.push_back(dealCard(deck));

		displayHands(playerHand, dealerhand, false);

		if (getHandValue(playerHand) == 21) {
			cout << "\nBlack Jack!\n" << endl;
			canDoubleDown = false;
			blackJack = true;
		}
		else if (getHandValue(dealerhand) == 21) {
			cout << "\nDealer has 21!\n" << endl;
			canDoubleDown = false;
			dealer21 = true;
		}
		else if (playerHand[0].rank == playerHand[1].rank && playerWantsSplit()) {
			// Logic for splitting cards into two hands, only works on the first 2 cards. 
			// If the split cards could be split in real life, there is not an option to split those
			cout << "\nPlayer wants to split.\n" << endl;
			vector<Card> splitHand1 = { playerHand[0], dealCard(deck) };
			vector<Card> splitHand2 = { playerHand[1], dealCard(deck) };

			vector<vector<Card>> splitHands = { splitHand1, splitHand2 };
			for (int i = 0; i < splitHands.size(); ++i) {
				// Splitting cards logic
				cout << "Playing split hand " << (i + 1) << endl;
				playerHand = splitHands[i];

				displayHands(playerHand, dealerhand, false);

				while (true) {
					// determine if the player stands or hits
					bool hit = playerDecision(playerHand, dealerhand, canDoubleDown);
					if (!hit) {
						// Player Stands
						canDoubleDown = false;
						break;
					}
					else {
						// Player hits
						playerHand.push_back(dealCard(deck));
						displayHands(playerHand, dealerhand, false);
						canDoubleDown = false;

						if (getHandValue(playerHand) > 21) {
							// Player busts
							cout << "\nPlayer busts.\n" << endl;
							bust = true;
							break;
						}
						if (getHandValue(playerHand) == 21) {
							break;
						}
					}
				}
			}
		}
		else if (canDoubleDown && playerWantsDoubleDown()) {
			// Double down logic
			cout << "\nPlayer doubles down.\n" << endl;
			playerHand.push_back(dealCard(deck));
			canDoubleDown = false;
			didDoubleDown = true;
			balance -= betAmount;
			cout << balance << endl;

			if (getHandValue(playerHand) > 21) {
				// Player busts
				cout << "\nPlayer busts.\n" << endl;
				bust = true;
			}
		}
		else {
			// Logic for when the player decided to not split or double down
			while (true) {
				bool hit = playerDecision(playerHand, dealerhand, canDoubleDown);
				if (!hit) {
					// Player stands
					canDoubleDown = false;
					break;
				}
				else {
					// Player hits
					playerHand.push_back(dealCard(deck));
					displayHands(playerHand, dealerhand, false);
					canDoubleDown = false;

					if (getHandValue(playerHand) > 21) {
						// Player busts
						cout << "\nPlayer busts.\n" << endl;
						bust = true;
						break;
					}
					if (getHandValue(playerHand) == 21) {
						break;
					}
				}
			}
		}
		// if the dealer does not have 21 on first 2 cards or the player did not bust
		if (!dealer21 || !bust) {
			// no reason for the dealer to keep hitting if the player got Black Jack
			if (!blackJack) {
				// keep hitting until the dealer gets to 17 or goes above 17
				while (getHandValue(dealerhand) <= 17) {
					dealerhand.push_back(dealCard(deck));
				}
			}
			displayHands(playerHand, dealerhand, true);

			int result = determineWinner(playerHand, dealerhand);
			if (result == -1) {
				cout << "\nDealer wins.\n" << endl;
				losses++;
			}
			else if (result == 1) {
				cout << "\nPleayer wins.\n" << endl;
				wins++;
				if (didDoubleDown) {
					balance += betAmount * 4;
				}
				else if (blackJack) {
					balance += betAmount * 1.5; // black jack pays out 3:2
				}
				else {
					balance += betAmount * 2;
				}
			}
			else {
				cout << "\nResult is a push between dealer and player.\n" << endl;
				ties++;
				if (didDoubleDown) {
					balance += betAmount * 2;
				}
				else {
					balance += betAmount;
				}
			}
		}
		//  if the dealer has 21 on first two cards or the plyer busts the dealer wins
		else {
			displayHands(playerHand, dealerhand, true);
			cout << "Dealer wins.\n" << endl;
			losses++;
		}

		cout << "Wins: " << wins << ", Losses: " << losses << ", Ties: " << ties << endl;

		char playAgain;
		cout << "Do you want to play again? (y/n): ";
		cin >> playAgain;
		cout << "\n\n\n\n" << endl;

		if (playAgain != 'y' && playAgain != 'Y') {
			break;
		}
	}
	return 0;
}
