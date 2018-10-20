import java.util.ArrayList;

public class State{
	
	// hand
	int hand;
	int dealer_hand;
	boolean doubled;
	boolean ace_split;

	// Integer -1 for not set
	// Integer 0 for bust
	// Integer 1 to 17 are for hard totals 4 to 20
	// Integer 18 to 25 are for Ace + (rest hard total)
	// Integer 26 to 35 are for pairs
	// Integer 36 for sum 21
	// Integer 37 for blackjack

	State(){
		this.hand = -1;
		this.dealer_hand = -1;
		this.doubled = false;
		this.ace_split = false;
	}

	State(State s){
		this.hand = s.hand;
		this.dealer_hand = s.dealer_hand;
		this.doubled = s.doubled;
		this.ace_split = s.ace_split;
	}

	int stand(){
		// Return sum
		if(this.hand <= 17){
			return this.hand + 3;
		}
		else if(this.hand <= 25){
			return this.hand - 5; 
		}
		else if(this.hand <= 34){
			return 2 * (this.hand - 24);
		}
		else if(this.hand == 35){
			return 12;
		}
		else{
			return 21;
		}
	}

	void double_down(int card){
		this.doubled = true;
		this.hit(card);
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
			else if(cur_hand == 25){
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
		if(cur_card == 11){
			this.ace_split = true;
		}
		if(card == 11){
			if(cur_card == 10){
				// Blackjack
				this.hand = 37;
			}
			else if(cur_card == 11){
				// A + A
				this.hand = 35;
			}
			else{
				// A + card
				this.hand = 16 + cur_card;
			}
		}
		else{
			if(cur_card == card){
				this.hand = 24 + card;
			}
			else if(cur_card == 11){
				if(card == 10){
					// No blackjack but sum 21
					this.hand = 36;
				}
				else{
					this.hand = 16 + card;
				}
			}
			else{
				this.hand = cur_card + card - 3;
			}
		}
	}
    
}