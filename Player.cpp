#include "Player.hpp"
#include "Card.hpp"
#include <string>
#include <vector>
#include <cassert>

class SimplePlayer : public Player
{
public:

    SimplePlayer(const std::string &name)
        :name(name) {}

    //EFFECTS returns player's name
    const std::string & get_name() const override
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
        Card highest = hand.at(0);
        bool only_trump = num_suit_hand(trump, 'n') == 0;

        for(Card c : hand)
        {
            if(!only_trump)
            {
                if(highest < c && c.get_suit() != trump)
                    highest = c;
            }
            else
            {
               if(Card_less(highest, c, trump))
                    highest = c; 
            }
        }

        return highest;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override
    {
        assert(!hand.empty());
        Card h_or_l = hand.at(0); //highest for lead suit, or lowest card
        bool led = num_suit_hand(led_card.get_suit(), 'p') > 0;

        for(Card c : hand)
        {
            if(led) //highest led card
            {
                if(Card_less(h_or_l, c, led_card, trump) &&
                    c.get_suit() == led_card.get_suit())
                        h_or_l = c;
            }
            else //lowest card
            {
                if(Card_less(c, h_or_l, led_card, trump))
                    h_or_l = c;
            }
        }
        return h_or_l;
    }
    

private:
    std::string name;
    std::vector<Card> hand;

    int check_hand(Suit check) const
    {
        int count = 0;

        for(Card c : hand)
        {
            if(c.get_suit() == check &&
                (c.is_face_or_ace() ||
                 c.is_left_bower(check)))
                    count++;
        }
        return count;
    }

    int num_suit_hand(Suit check, char pn) const
    {
        /* 
           p is for how many cards of that suit
            is in the players hand
           n is for how many cards of suits other
            than check is in the players hand
        */

        int p_count = 0;
        int n_count = 0;

        for(Card c : hand)
        {
            if(c.get_suit() == check)
                p_count++;
            else
                n_count++;
        }
        
        if(pn == 'n')
            return n_count;
        return p_count;        
    }
};

//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, const std::string &strategy)
{
    if(strategy == "Simple")
        return new SimplePlayer(name);
    //dont have other rn
    return new SimplePlayer(name);
}

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p)
{
    os << p.get_name();
    return os;
}