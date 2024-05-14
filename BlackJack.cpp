#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

struct Card {
    enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
    enum Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

    Suit suit;
    Rank rank;

    void print() const {
        switch (rank) {
            case ACE: cout << "Ace"; break;
            case TWO: cout << "2"; break;
            case THREE: cout << "3"; break;
            case FOUR: cout << "4"; break;
            case FIVE: cout << "5"; break;
            case SIX: cout << "6"; break;
            case SEVEN: cout << "7"; break;
            case EIGHT: cout << "8"; break;
            case NINE: cout << "9"; break;
            case TEN: cout << "10"; break;
            case JACK: cout << "Jack"; break;
            case QUEEN: cout << "Queen"; break;
            case KING: cout << "King"; break;
        }

        switch (suit) {
            case HEARTS: cout << " of Hearts"; break;
            case DIAMONDS: cout << " of Diamonds"; break;
            case SPADES: cout << " of Spades"; break;
            case CLUBS: cout << " of Clubs"; break;
        }
    }
};

int getRandomNumber(int min, int max) {
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

int getCardValue(const Card& card) {
    switch (card.rank) {
        case Card::JACK:
        case Card::QUEEN:
        case Card::KING:
            return 10;
        case Card::ACE:
            return 11;
        default:
            return static_cast<int>(card.rank);
    }
}

vector<Card> initializeDeck(int numDecks) {
    vector<Card> deck;
    for (int deckCount = 0; deckCount < numDecks; ++deckCount) {
        for (int suit = Card::HEARTS; suit <= Card::SPADES; ++suit) {
            for (int rank = Card::ACE; rank <= Card::KING; ++rank) {
                deck.push_back({ static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank) });
            }

        }
    }

    srand(static_cast<unsigned int>(time(0))); // seed random # gen
    random_shuffle(deck.begin(), deck.end()); // shuffle the deck
    return deck;
}

Card dealCard(vector<Card>& deck) {
    // if the deck is empty initialize a new deck
    if (deck.empty()) {
        cout << "\n\nDeck is empty. Reshuffling...\n\n" << endl;
        deck = initializeDeck(4);
    }

    // after a card is picked and returned, remove that card from the deck
    int index = getRandomNumber(0, deck.size() - 1);
    Card card = deck[index];
    deck.erase(deck.begin() + index);

    return card;
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
            cout << "Invalid input. Please enter 's' to stand or 'h' to hit." << endl;
        }
    }
}

int main() {
    int wins = 0;
    int losses = 0;
    int ties = 0;

    vector<Card> deck = initializeDeck(4);

    while (true) {
        vector<Card> playerHand;
        vector<Card> dealerhand;
        bool canDoubleDown = true;
        bool blackJack = false;
        bool dealer21 = false;

        playerHand.push_back(dealCard(deck));
        dealerhand.push_back(dealCard(deck));
        playerHand.push_back(dealCard(deck));
        dealerhand.push_back(dealCard(deck));

        // Display initial hands
        cout << "\n\n";
        cout << "Your Hand: ";
        for (const auto& card : playerHand) {
            card.print();
            cout << ", ";
        }
        cout << "Total: " << getHandValue(playerHand) << endl;

        cout << "Dealer's Hand: ";
        cout << "[Hidden Card], ";
        dealerhand[1].print();
								cout << ",Total: ??\n" << endl;
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
    else {
        // Player decision
        while (true) {
            bool hit = playerDecision(playerHand, dealerhand, canDoubleDown);
            if (!hit) {
                // Player stands
                break;
            }
            else {
                // Player hits
                playerHand.push_back(dealCard(deck));

                // Display player's hand
                cout << "Your Hand: ";
                for (const auto& card : playerHand) {
                    card.print();
                    cout << ", ";
                }
                cout << "Total: " << getHandValue(playerHand) << endl;

                if (getHandValue(playerHand) >= 21) {
                    cout << "\nYou bust! Dealer Wins.\n" << endl;
                    break;
                }
            }
        }
    }

    // Rest of the game logic...
