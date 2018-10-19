import java.util.ArrayList;

public class State{
	
	// hand
	int hand;

	// Integer -1 for not set
	// Integer 0 for bust
	// Integer 1 to 17 are for hard totals 4 to 20
	// Integer 18 to 25 are for Ace + (rest hard total)
	// Integer 26 to 35 are for pairs
	// Integer 36 for sum 21
	// Integer 37 for blackjack

	State(){
		this.hand = -1;
	}

	int stall(){
		return this.hand;
	}

	void hit(int card){
		int cur_hand = this.hand;
		if(card == 11){
			if(cur_hand <= 6){
				// Transition from hard to soft
				this.hand = cur_hand + 19;
			}
			else if(cur_hand < 17){
				// Hard tp hard + 1
				this.hand = cur_hand + 1;
			}
			else if(cur_hand == 17){
				// Hard total 20 to 21
				this.hand = 36;
			}
			else if(cur_hand <= 24){
				// Next Ace is hard
				this.hand = cur_hand + 1;
			}
			else if(cur_hand == 24){
				// A9 + A gives sum 21
				this.hand = 36;
			}
			else if(cur_hand <= 28){
				// Hard total + ace
				this.hand = 2 * (cur_hand - 24) + 16;
			}
			else if(cur_hand == 29){
				// 55 + A gives 21
				this.hand = 36;
			}
			else if(cur_hand <= 33){
				// Hard total + hard ace
				this.hand = 2 * (cur_hand - 24) - 2;
			}
			else if(cur_hand == 34){
				// 1010 + hard Ace
				this.hand = 36;
			}
			else if(cur_hand == 35){
				// AA + A = A+2
				this.hand = 18;
			}
		}
		else{
			if(cur_hand <= 17){
				if(cur_hand + 3 + card > 21){
					// bust
					this.hand = 0;
				}
				else if(cur_hand + 3 + card < 21){
					this.hand = cur_hand + card;
				}
				else{
					this.hand = 36;
				}
			}
			else if(cur_hand <= 25){
				if(cur_hand - 5 + card < 21){
					// A is soft
					this.hand = this.hand + card;
				}
				else if(cur_hand - 5 + card > 21){
					// A becomes hard
					this.hand = this.hand - 15 + card;
				}
				else{
					this.hand = 36;
				}
			}
		}
	}

	void split(int card){
		int cur_card = this.hand - 24;
		if(card == 11){
			if(cur_card == 10){
				// Blackjack
				this.hand = 37;
			}
			else if(cur_card == 11){
				// A + card
				this.hand = 16 + cur_card;
			}
			else if(this.hand + 3 + card > 21){
				// bust
				this.hand = 0;
			}
			else{
				this.hand = this.hand + card;
			}

		}
	}
    
}