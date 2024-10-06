#include "Pack.hpp"
#include "Card.hpp"
#include <array>
#include <string>

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on. 
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack()
    : next(0) 
    {
        cards.at(0) = Card(NINE, SPADES);
        cards.at(1) = Card(TEN, SPADES);
        cards.at(2) = Card(JACK, SPADES);
        cards.at(3) = Card(QUEEN, SPADES);
        cards.at(4) = Card(KING, SPADES);
        cards.at(5) = Card(ACE, SPADES);
        cards.at(6) = Card(NINE, HEARTS);
        cards.at(7) = Card(TEN, HEARTS);
        cards.at(8) = Card(JACK, HEARTS);
        cards.at(9) = Card(QUEEN, HEARTS);
        cards.at(10) = Card(KING, HEARTS);
        cards.at(11) = Card(ACE, HEARTS);
        cards.at(12) = Card(NINE, CLUBS);
        cards.at(13) = Card(TEN, CLUBS);
        cards.at(14) = Card(JACK, CLUBS);
        cards.at(15) = Card(QUEEN, CLUBS);
        cards.at(16) = Card(KING, CLUBS);
        cards.at(17) = Card(ACE, CLUBS);
        cards.at(18) = Card(NINE, DIAMONDS);
        cards.at(19) = Card(TEN, DIAMONDS);
        cards.at(20) = Card(JACK, DIAMONDS);
        cards.at(21) = Card(QUEEN, DIAMONDS);
        cards.at(22) = Card(KING, DIAMONDS);
        cards.at(23) = Card(ACE, DIAMONDS);
    }

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack(std::istream& pack_input)
    : next(0) 
    {
        Rank r;
        Suit s;
        std::string of;
        int c;

        for(c= 0; c< PACK_SIZE; c++)
        {
            pack_input >> r >> of >> s;
            cards.at(c) = Card(r, s);
        }
    }

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one()
{
    next++;
    return cards.at(next-1);
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset()
{
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle()
{
    std::array<Card, PACK_SIZE> temp;
    int shuffle, c, k;

    for(shuffle= 0; shuffle < 7; shuffle++)
    {       k= 0; //k< pack_size
        for(c= 0; c< PACK_SIZE/2; c++)
        {   
            if(k< PACK_SIZE)
            {
                temp.at(k) = cards.at(c+12);
                k++;
            }
            temp.at(k) = cards.at(c);
            k++;
        }
        cards = temp;
    }
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const
{
    if(next == PACK_SIZE)
        return true;
    return false;
}