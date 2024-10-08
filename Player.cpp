#include "Player.hpp"
#include "Card.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

class SimplePlayer : public Player
{
public:

    //constructor
    SimplePlayer(const string &name)
        :name(name) {}

    //EFFECTS returns player's name
    const string & get_name() const override
    {
        return name;
    }

    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override
    {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    } 

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override
    {
        assert(round == 1 || round == 2);
        
        //Round 1
        if(round == 1)
        {
            if(check_hand(upcard.get_suit()) > 1)
            {
                order_up_suit = upcard.get_suit();
                return true;
            }
            return false;
        }

        //Round 2
        if(round == 2)
        {
            Suit sameColor = upcard.get_suit(upcard.get_suit());

            if(check_hand(sameColor) > 0 || is_dealer)
            { 
                order_up_suit = sameColor;
                return true;
            }
            else
                return false;
        }
        return false;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override
    {
        assert(!hand.empty());
        Card least = upcard;
        int index_l = -1;
        int c;

        for(c= 0; c< MAX_HAND_SIZE; c++)
        {
            if(Card_less(hand.at(c), least, upcard.get_suit()))
            {
                least = hand.at(c);
                index_l = c;
            }
        }

        //just incase the upcard has the least value
        if(index_l != -1)
            hand.at(index_l) = upcard;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(Suit trump) override
    {
        assert(!hand.empty());
        bool only_trump = num_suit_hand(trump, trump,'n') == 0;
        Card highest;

        //ensures the first value isn't trump
        //if non trump cards are in hand
        if(!only_trump)
        {
            for(Card c : hand)
                {if(!c.is_trump(trump))
                { highest = c;  break;}  }
        }
        else
            highest = hand.at(0);

        for(Card c : hand)
        {
            if(!only_trump)
            {
                if(highest < c && !c.is_trump(trump))
                    highest = c;
            }
            else
            {
               if(Card_less(highest, c, trump))
                    highest = c; 
            }
        }
        
        auto loc = find(hand.begin(), hand.end(), highest);
        hand.erase(loc);
        return highest;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override
    {
        assert(!hand.empty());
        bool led = num_suit_hand(led_card.get_suit(), trump, 'p') > 0;
        Card select;

        if(led) //**If cards of the lead suit are in player hand
        {   //--If lead suit is the trump suit
            if(led_card.get_suit() == trump)
            {
                select = hand.at(0);
                for(Card c : hand)
                {
                    if(Card_less(select, c, led_card, trump))
                        select = c;
                }

            }
            else //--If lead suit is not trump suit
            {   //ensures the first value for select isn't trump 
                for(Card c : hand)
                {
                    if(!c.is_trump(trump))
                    {
                         select = c; 
                         break;
                    }
                }
                for(Card c : hand)
                {
                    if(Card_less(select, c, led_card, trump) && !c.is_trump(trump))
                        select = c;
                }
            }
        }
        else //**If NO cards of the lead suit are in player hand
        {
            select = hand.at(0);
            for(Card c : hand)
                {
                    if(Card_less(c, select, led_card, trump))
                        select = c;
                }
        }

        auto loc = find(hand.begin(), hand.end(), select);
        hand.erase(loc);
        return select;
    }
    

private:
    string name;
    vector<Card> hand;

    int check_hand(Suit check) const
    {
        int count = 0;

        for(Card c : hand)
        {
            if(c.is_face_or_ace() && c.is_trump(check))
                count++;
        }
        return count;
    }

    /*  p is for how many cards of the check
    suit is in the players hand
        n is for how many cards of suits other
    than check is in the players hand       */
    int num_suit_hand(Suit check, Suit trump, char pn) const
    {
        int p_count = 0;
        int n_count = 0;
        bool suitCT = check == trump;

        for(Card c : hand)
        {   
            if(suitCT && c.is_trump(trump))
                p_count++;
            else if(!suitCT && c.get_suit() == check && !c.is_trump(trump)) //left bower
                p_count++;
            else
                n_count++;
        }
        
        if(pn == 'n')
            return n_count;
        return p_count;        
    }
};





class HumanPlayer : public Player
{
public:

    //constructor
    HumanPlayer(string name)
    : name(name) {}

    //EFFECTS returns player's name
    const string & get_name() const override
    {
        return name;
    }

    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override
    {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override
    {
        string decision;

        print_hand();
        cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
        cin >> decision;

        if(decision != "pass")
        {
            order_up_suit = string_to_suit(decision);
            return true;
        }
        else
            return false;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override
    {
        int selected;
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";
        cin >> selected;
        if(selected != -1)
            hand.at(selected) = upcard;
        sort(hand.begin(), hand.end());
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(Suit trump) override
    {
        int selected;
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        cin >> selected;
        return hand.at(selected);
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override
    {
        int selected;
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        cin >> selected;
        return hand.at(selected);
    }


private:
    string name;
    vector<Card> hand;

    //prints hand of player
    void print_hand() const 
    {
        for (size_t i=0; i < hand.size(); ++i)
            cout << "Human player " << name << "'s hand: "
                << "[" << i << "] " << hand[i] << "\n";
    }
};





//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const string &name, const string &strategy)
{
    if(strategy == "Simple")
        return new SimplePlayer(name);
    if(strategy == "Human")
        return new HumanPlayer(name);
    // if strategy is an unrecognized string create Simple Player
    return new SimplePlayer(name);
}

//EFFECTS: Prints player's name to os
ostream & operator<<(ostream &os, const Player &p)
{
    os << p.get_name();
    return os;
}