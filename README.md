# BlackJack

## Goal: 
The goal of this MDP based BlackJack player is to find a solution to a real problem using ideas from sequential
decision making under uncertainty (Markov Decision Processes).

## Scenario: 
The game of Blackjack. Terminology
cards: a standard deck of playing cards is used, i.e., four suits (clubs, diamonds, spades, and hearts) and
13 different cards within each suit (the numbers 2 through 10, jack, queen, king, and ace). In this
assignment, we will replace 10, jack, queen and king with a generic 'face card'. We will assume an
infinite number of decks available in the pack.
card values: the numbered cards (2 through 9) count as their numerical value. The generic face card
(replacing 10, jack, queen, and king) counts as 10, and the ace may count as either 1 or 11 (whichever
works in the player's favor).
hand value: the value of a hand is the sum of the values of all cards in the hand. The values of the aces in
a hand are such that they produce the highest value that is 21 or under (if possible). A hand where any
ace is counted as 11 is called a soft hand. The suits of the cards do not matter in blackjack.
pair: the two card hand where both cards have the same value. (example, two aces, a pair of sixes, and
for our assignment, a pair of face cards).
blackjack: is a two-card hand where one card is an ace and the other card is any face card.
busted: the value of the hand has exceeded 21.

## Rules of Play
There are some slight variations on the rules and procedure of blackjack. Below is the simplified
procedure that we will use for this assignment. We will not be using insurance, surrender or dealer
peeking, which are options in a standard casino game.
1. Each player places a bet on the hand.
2. The dealer deals two cards to each player, including himself. The player's cards will be face-up. One of
the dealers cards is face-up, but the other is face-down.
3. The player must do one of the following:
H - Hit: the player receives one additional card (face up). A player can receive as many cards as he or she
wants, but if the value of the player's hand exceeds 21, the player busts and loses the bet on this hand
irrespective of dealer's hand.
S - Stand: the player does not want any additional cards
D - Double-down: before the player has received any additional cards, she may double-down. This
means that the player doubles her bet on the hand and will receive exactly one additional card. The
disadvantage of doubling-down is that the player cannot receive any more cards (beyond the one
additional card); the advantage is that the player can use this option to increase the bet when conditions
are favorable.
P - sPlit: before the player has received any additional cards, if the original hand is a pair, then the player
may split the hand. This means that instead of playing one hand the player will be playing two
independent hands. She puts in the bet amount for the second hand. Two additional cards are drawn,
one for each hand. The play goes on as if the player was playing two hands instead of one. If the drawn
cards result in more pairs she is allowed to resplit. The player is allowed endless resplits in our version of
the game. [There is an exception associated with a pair of Aces, see below]
5. Once the player stands, the dealer turns over his face-down card. The dealer then hits or stands
according to the following deterministic policy: If the value of the hand is less than 17 (or soft 17), the
dealer must hit. Otherwise, the dealer must stand. This means, that the dealer stands if his Cards are
(A,2,4) because that makes a soft 17. If the dealer busts, then he loses the bets with the non-busted
players.
6. PayOffs (in this order)
(a) If the player has a blackjack then she receives 2.5 times her bet (profit of 1.5). The only exception is if
the dealer also got a blackjack, which is a push (player gets money back – no profit no loss).
(b) If the player busted, she lost her bet.
(c) If the dealer busted, he lost and the dealer pays the player double her bet, i.e., the player makes a
profit equal to her bet.
(d) If the value of dealer's hand is greater than player's the player loses her bet.
(e) If the value of player's hand is greater than dealer's the player won and dealer pays double her bet.
(f) If the dealer has blackjack and the player has non-blackjack 21 the dealer wins.
(g) If the value of the two hands is equal, it is a push and the player gets back her bet money. That is, no
profit no loss.
7. Other rules and exceptions.
(a) Doubling is allowed after split. That means, after splitting the pair, the player is allowed to double
down either or both her hands if she wishes to.
(b) Player can resplit as many times as she desires (whenever allowed).
(c) Splitting Aces. This is an exception to the rule. If the player gets a pair of aces that is a very strong
hand. She can split this but she will only get one additional card per split hand, and she will not be
allowed to resplit. Moreover, if the card is a face card, it will not be counted as blackjack, and will be
treated as a regular 21.
To familiarize yourself with the game, play it online for a few minutes. There are many online applets
available, for example, http://www.hitorstand.net/game_m.html. The rules they follow may be slight
variations of those used in the assignment, but you will get the general idea.

## Problem Statement: 
In this work the task was to compute the policy for an optimal player. As usual
the first step is to carefully think of the state space that you will need. Design a state transition function,
and reward model to encode the dynamics of the play. Solve the game to compute the best play in each
state. The best play is defined as the action (hit/stand/double/split) that maximizes the expected return.
Make sure you double or split only in the states it is allowed. Assume that the player bets $1.
Program for a BlackJack(p) game. Assume that the probability of getting a face card is p (an input to the
program) and the probability of getting all other cards, 2-9 and Ace, is uniformly (1-p)/9. Note that p =
4/13 captures the standard Blackjack game.
After you solve the problem, the solution to BlackJack(4/13) should look very close to this. In the first
column, representing your hand, a single integer represents the sum of the two cards, and indicates that
they are not a pair and that neither is an ace. For the output of this assignment, you need to return only
the first action that you will take. Thus your output need not distinguish between "D" and "DS".

## Output format:
The output format is very close to the policy in the link. The first value in a row is your hand – it goes
from 5 to 19. Then we have special cases, first when one of the cards is an ace, A2 to A9 and finally pairs,
22 to 1010. After your hand there is a tab ‘\t’ and then there are 10 values for different open cards of
the dealer (2 to 10 then ace). The letter will indicate your first action if you get this hand. Here is a
sample output:<br>
5 H H H H H H H H H H <br>
6 H H H H H H H H H H<br>
7 H H H H H H H H H H<br>
8 H H H H H H H H H H<br>
9 H D D D D H H H H H<br>
10 D D D D D D D D H H<br>
11 D D D D D D D D H H<br>
12 H H S S S H H H H H<br>
13 S S S S S H H H H H<br>
14 S S S S S H H H H H<br>
15 S S S S S H H H H H<br>
16 S S S S S H H H H H<br>
17 S S S S S S S S S S<br>
18 S S S S S S S S S S<br>
19 S S S S S S S S S S<br>
A2 H H H H D H H H H H<br>
A3 H H H D D H H H H H<br>
A4 H H H D D H H H H H<br>
A5 H H D D D H H H H H<br>
A6 H D D D D H H H H H<br>
A7 S D D D D S S H H H<br>
A8 S S S S S S S S S S<br>
A9 S S S S S S S S S S<br>
22 P P P P P P H H H H<br>
33 P P P P P P H H H H <br>
44 H H H P P H H H H H<br>
55 D D D D D D D D H H<br>
66 P P P P P H H H H H<br>
77 P P P P P P H H H H<br>
88 P P P P P P P P H H<br>
99 P P P P P S P P S S<br>
1010 S S S S S S S S S S<br>
AA P P P P P P P P P H<br>
As an example, the table above suggests that if you get a pair of 5s and the dealer got a 9 then you
should double.

