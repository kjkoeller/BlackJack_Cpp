# BlackJack_Cpp

This is a C++ implementation of Black Jack that gives the player the ability to split pairs, double down, and hit or stand.

The code also automatically ends the hand when the player gets a blackjack on card deal. The implementation also knows that the dealer must keep hitting until busting or getting to >=17 in total card value. The distinction for Aces to be 1 or 11 is also included so that if the player/dealer goes over 21 with an Ace, the game automatically converts that Ace to a value of 1.

Most Black Jack players know that there are mathematical "rules" for hitting or standing. This does not implement all of those, but offers basic advice for hitting or standing (i.e. if the player has less than 17, hit or the dealer has >= 7 on the known card to hit).
