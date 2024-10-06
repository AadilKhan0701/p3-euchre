#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) 
{
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
    Card k;
    ASSERT_EQUAL(TWO, k.get_rank());
    ASSERT_EQUAL(SPADES, k.get_suit());
}

// Add more test cases here
TEST(Stream_operator_func)
{
    //testing ostream
    ostringstream os1;
    ostringstream os2;
    ostringstream os3;
    ostringstream os4;
    Card c1 = Card(ACE, SPADES);
    Card c2 = Card(QUEEN, HEARTS);
    Card c3 = Card(SEVEN, CLUBS);
    Card c4 = Card(FOUR, DIAMONDS);
    os1 << c1;
    ASSERT_EQUAL(os1.str(), "Ace of Spades");
    os2 << c2;
    ASSERT_EQUAL(os2.str(), "Queen of Hearts");
    os3 << c3;
    ASSERT_EQUAL(os3.str(), "Seven of Clubs");
    os4 << c4;
    ASSERT_EQUAL(os4.str(), "Four of Diamonds");

    // istream
    Card c5;
    Card c6;
    Card c7;
    Card c8;
    istringstream is5("Ten of Spades");
    is5 >> c5;
    istringstream is6("Jack of Clubs");
    is6 >> c6;
    istringstream is7("Five of Hearts");
    is7 >> c7;
    istringstream is8("King of Diamonds");
    is8 >> c8;
    ASSERT_EQUAL(c5, Card(TEN, SPADES));
    ASSERT_EQUAL(c6, Card(JACK, CLUBS));
    ASSERT_EQUAL(c7, Card(FIVE, HEARTS));
    ASSERT_EQUAL(c8, Card(KING, DIAMONDS));
}

TEST(All_base_func)
{
    Card c1;
    Card c2 = Card(TWO, DIAMONDS);
    Card c3 = Card(SIX, CLUBS);
    Card c4 = Card(EIGHT, HEARTS);
    Card c5 = Card(JACK, SPADES);
    Card c6 = Card(JACK, CLUBS);
    Card c7 = Card(NINE, DIAMONDS);
    Card c8 = Card(ACE, SPADES);

    //testing rank and suit
    ASSERT_EQUAL(c1.get_rank(), TWO);
    ASSERT_EQUAL(c2.get_suit(), DIAMONDS);
    ASSERT_EQUAL(c3.get_rank(), SIX);
    ASSERT_EQUAL(c4.get_suit(), HEARTS);
    ASSERT_EQUAL(c5.get_rank(), JACK);
    ASSERT_EQUAL(c6.get_suit(), CLUBS);
    ASSERT_EQUAL(c7.get_rank(), NINE);
    ASSERT_EQUAL(c8.get_suit(), SPADES);

    //testing bower, face, trump
    ASSERT_FALSE(c1.is_face_or_ace());
    ASSERT_FALSE(c2.is_left_bower(DIAMONDS));
    ASSERT_FALSE(c3.is_right_bower(CLUBS));
    ASSERT_FALSE(c4.is_trump(SPADES));
    ASSERT_TRUE(c5.is_trump(CLUBS));
    ASSERT_TRUE(c6.is_right_bower(CLUBS));
    ASSERT_FALSE(c7.is_left_bower(CLUBS));
    ASSERT_TRUE(c8.is_face_or_ace());

    //testing Card less
    ASSERT_FALSE(Card_less(c1, c2, SPADES));
    ASSERT_TRUE(Card_less(c3, c4, c4, DIAMONDS));
    ASSERT_FALSE(Card_less(c5, c6, SPADES));
    ASSERT_TRUE(Card_less(c7, c8, c1, CLUBS));
    ASSERT_TRUE(Card_less(c1, c6, SPADES));
    ASSERT_TRUE(Card_less(c2, c3, c6, HEARTS));
    ASSERT_TRUE(Card_less(c4, c7, SPADES));
    ASSERT_FALSE(Card_less(c5, c8, c3, CLUBS));
}

TEST(Card_less_func)
{
    Card rb = Card(JACK, SPADES);   //right bower
    Card lb = Card(JACK, CLUBS);    //left bower
    Card ts = Card(QUEEN, SPADES);  //trump suit
    Card ls = Card(TEN, HEARTS);   //lead suit
    Card ns = Card(KING, CLUBS);    //not special

    ASSERT_FALSE(Card_less(rb, lb, SPADES));
    ASSERT_FALSE(Card_less(rb, lb, ls, SPADES));
    ASSERT_FALSE(Card_less(rb, ts, SPADES));
    ASSERT_FALSE(Card_less(rb, ts, ls, SPADES));
    ASSERT_FALSE(Card_less(rb, ls, SPADES));
    ASSERT_FALSE(Card_less(rb, ls, ls, SPADES));
    ASSERT_FALSE(Card_less(rb, ns, SPADES));
    ASSERT_FALSE(Card_less(rb, ns, ls, SPADES));

    ASSERT_TRUE(Card_less(lb, rb, SPADES));
    ASSERT_TRUE(Card_less(lb, rb, ls, SPADES));
    ASSERT_FALSE(Card_less(lb, ts, SPADES));
    ASSERT_FALSE(Card_less(lb, ts, ls, SPADES));
    ASSERT_FALSE(Card_less(lb, ls, SPADES));
    ASSERT_FALSE(Card_less(lb, ls, ls, SPADES));
    ASSERT_FALSE(Card_less(lb, ns, SPADES));
    ASSERT_FALSE(Card_less(lb, ns, ls, SPADES));

    ASSERT_TRUE(Card_less(ts, rb, SPADES));
    ASSERT_TRUE(Card_less(ts, rb, ls, SPADES));
    ASSERT_TRUE(Card_less(ts, lb, SPADES));
    ASSERT_TRUE(Card_less(ts, lb, ls, SPADES));
    ASSERT_FALSE(Card_less(ts, ls, SPADES));
    ASSERT_FALSE(Card_less(ts, ls, ls, SPADES));
    ASSERT_FALSE(Card_less(ts, ns, SPADES));
    ASSERT_FALSE(Card_less(ts, ns, ls, SPADES));

    ASSERT_TRUE(Card_less(ls, rb, SPADES));
    ASSERT_TRUE(Card_less(ls, rb, ls, SPADES));
    ASSERT_TRUE(Card_less(ls, lb, SPADES));
    ASSERT_TRUE(Card_less(ls, lb, ls, SPADES));
    ASSERT_TRUE(Card_less(ls, ts, SPADES));
    ASSERT_TRUE(Card_less(ls, ts, ls, SPADES));
    ASSERT_TRUE(Card_less(ls, ns, SPADES));
    ASSERT_FALSE(Card_less(ls, ns, ls, SPADES));

    ASSERT_TRUE(Card_less(ns, rb, SPADES));
    ASSERT_TRUE(Card_less(ns, rb, ls, SPADES));
    ASSERT_TRUE(Card_less(ns, lb, SPADES));
    ASSERT_TRUE(Card_less(ns, lb, ls, SPADES));
    ASSERT_TRUE(Card_less(ns, ts, SPADES));
    ASSERT_TRUE(Card_less(ns, ts, ls, SPADES));
    ASSERT_FALSE(Card_less(ns, ls, SPADES));
    ASSERT_TRUE(Card_less(ns, ls, ls, SPADES));
}

TEST_MAIN()
