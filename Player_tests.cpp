#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(stuff)
{
    Player * Jim = Player_factory("Jimothy", "Simple");
    Player * Dwight = Player_factory("Dwigt", "Simple");
    Player * Micheal = Player_factory("Agent Scarn", "Simple");
    Player * Pam = Player_factory("Pamela", "Simple");

    ASSERT_EQUAL("Jimothy", Jim->get_name());
    ASSERT_EQUAL("Dwigt", Dwight->get_name());
    ASSERT_EQUAL("Agent Scarn", Micheal->get_name());
    ASSERT_EQUAL("Pamela", Pam->get_name());

    Jim->add_card(Card(ACE, SPADES));
    Jim->add_card(Card(NINE, CLUBS));
    Jim->add_card(Card(JACK, DIAMONDS));
    Dwight->add_card(Card(QUEEN, HEARTS));
    Dwight->add_card(Card(KING, CLUBS));
    Micheal->add_card(Card(TEN, HEARTS));
    Micheal->add_card(Card(JACK, SPADES));
    Micheal->add_card(Card(KING, DIAMONDS));
    Pam->add_card(Card(KING, HEARTS));
    Pam->add_card(Card(NINE, HEARTS));

    Jim->add_card(Card(QUEEN, CLUBS));
    Jim->add_card(Card(TEN, DIAMONDS));
    Dwight->add_card(Card(NINE, SPADES));
    Dwight->add_card(Card(ACE, CLUBS));
    Dwight->add_card(Card(NINE, DIAMONDS));
    Micheal->add_card(Card(KING, SPADES));
    Micheal->add_card(Card(JACK, CLUBS));
    Pam->add_card(Card(TEN, SPADES));
    Pam->add_card(Card(QUEEN, DIAMONDS));
    Pam->add_card(Card(ACE, HEARTS));

    //SPADES:   ACE, KING, JACK, TEN, NINE (TAKEN) QUEEN (UNTAKEN)
    //HEARTS:   ACE, KING, QUEEN, TEN, NINE (TAKEN) JACK (UNTAKEN)
    //CLUBS:    ACE, KING, QUEEN, JACK, NINE (TAKEN) TEN (UNTAKEN)
    //DIAMONDS: KING, QUEEN, JACK, TEN, NINE (TAKEN) ACE (UNTAKEN)

    Suit order_up = SPADES;

    //round 1 testing
    ASSERT_FALSE(Jim->make_trump(Card(JACK, HEARTS), false, 1, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_TRUE(Dwight->make_trump(Card(TEN, CLUBS), false, 1, order_up));
    ASSERT_TRUE(order_up == CLUBS);
    ASSERT_TRUE(Micheal->make_trump(Card(QUEEN, SPADES), false, 1, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_FALSE(Pam->make_trump(Card(ACE, DIAMONDS), false, 1, order_up));
    ASSERT_FALSE(order_up == DIAMONDS);

    //round 2 testing
    ASSERT_FALSE(Jim->make_trump(Card(ACE, DIAMONDS), false, 2, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_TRUE(Dwight->make_trump(Card(JACK, HEARTS), true, 2, order_up));
    ASSERT_FALSE(order_up == HEARTS);
    ASSERT_TRUE(Micheal->make_trump(Card(TEN, CLUBS), false, 2, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_TRUE(Pam->make_trump(Card(QUEEN, SPADES), true, 2, order_up));
    ASSERT_FALSE(order_up == SPADES);

    Jim->add_and_discard(Card(ACE, DIAMONDS));      //removes NINE CLUBS
    Dwight->add_and_discard(Card(TEN, CLUBS));      //removes NINE SPADES
    Micheal->add_and_discard(Card(JACK, HEARTS));   //removes JACK SPADES
    Pam->add_and_discard(Card(QUEEN, SPADES));      //removes NINE HEARTS

    //JIM HAND      {(ACE SPADES),  (ACE DIAMONDS), (JACK DIAMONDS),(QUEEN CLUBS),   (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),(KING CLUBS),   (TEN CLUBS),    (ACE CLUBS),     (NINE DIAMONDS)}
    //MICHEAL HAND  {(TEN HEARTS),  (JACK HEARTS),  (KING DIAMONDS),(KINGN SPADES),  (JACK CLUBS)}
    //PAM HAND      {(KING HEARTS), (QUEEN SPADES), (TEN SPADES),   (QUEEN DIAMONDS),(ACE HEARTS)}

    ASSERT_EQUAL(Jim->lead_card(SPADES), Card(ACE, DIAMONDS));
    ASSERT_EQUAL(Dwight->play_card(Card(ACE, DIAMONDS), SPADES), Card(NINE, DIAMONDS));
    ASSERT_EQUAL(Micheal->play_card(Card(ACE, DIAMONDS), SPADES), Card(KING, DIAMONDS));
    ASSERT_EQUAL(Pam->play_card(Card(ACE, DIAMONDS), SPADES), Card(QUEEN, DIAMONDS));

    //JIM HAND      {(ACE SPADES),  (JACK DIAMONDS),(QUEEN CLUBS),  (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),(KING CLUBS),   (TEN CLUBS),    (ACE CLUBS)}
    //MICHEAL HAND  {(TEN HEARTS),  (JACK HEARTS),  (KING SPADES),  (JACK CLUBS)}
    //PAM HAND      {(KING HEARTS), (QUEEN SPADES), (TEN SPADES),   (ACE HEARTS)}

    ASSERT_EQUAL(Dwight->lead_card(HEARTS), Card(ACE, CLUBS));
    ASSERT_EQUAL(Micheal->play_card(Card(ACE, CLUBS), HEARTS), Card(JACK, CLUBS));
    ASSERT_EQUAL(Pam->play_card(Card(ACE, CLUBS), HEARTS), Card(TEN, SPADES));
    ASSERT_EQUAL(Jim->play_card(Card(ACE, CLUBS), HEARTS), Card(QUEEN, CLUBS));

    //JIM HAND      {(ACE SPADES),  (JACK DIAMONDS),(TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),(KING CLUBS),   (TEN CLUBS)}
    //MICHEAL HAND  {(TEN HEARTS),  (JACK HEARTS),  (KING SPADES)}
    //PAM HAND      {(KING HEARTS), (QUEEN SPADES), (ACE HEARTS)}

    ASSERT_EQUAL(Micheal->lead_card(DIAMONDS), Card(KING, SPADES));
    ASSERT_EQUAL(Pam->play_card(Card(KING, SPADES), DIAMONDS), Card(QUEEN, SPADES));
    ASSERT_EQUAL(Jim->play_card(Card(KING, SPADES), DIAMONDS), Card(ACE, SPADES));
    ASSERT_EQUAL(Dwight->play_card(Card(KING, SPADES), DIAMONDS), Card(TEN, CLUBS));

    //JIM HAND      {(JACK DIAMONDS),   (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),    (KING CLUBS)}
    //MICHEAL HAND  {(TEN HEARTS),      (JACK HEARTS)}
    //PAM HAND      {(KING HEARTS),     (ACE HEARTS)}

    ASSERT_EQUAL(Pam->lead_card(CLUBS), Card(ACE, HEARTS));
    ASSERT_EQUAL(Jim->play_card(Card(ACE, HEARTS), CLUBS), Card(TEN, DIAMONDS));
    ASSERT_EQUAL(Dwight->play_card(Card(ACE, HEARTS), CLUBS), Card(QUEEN, HEARTS));
    ASSERT_EQUAL(Micheal->play_card(Card(ACE, HEARTS), CLUBS), Card(JACK, HEARTS));

    //JIM HAND      {(JACK DIAMONDS)}
    //DWIGHT HAND   {(KING CLUBS)}
    //MICHEAL HAND  {(TEN HEARTS)}
    //PAM HAND      {(KING HEARTS)}

    ASSERT_EQUAL(Jim->lead_card(DIAMONDS), Card(JACK, DIAMONDS));
    ASSERT_EQUAL(Dwight->play_card(Card(JACK, DIAMONDS), DIAMONDS), Card(KING, CLUBS));
    ASSERT_EQUAL(Micheal->play_card(Card(JACK, DIAMONDS), DIAMONDS), Card(TEN, HEARTS));
    ASSERT_EQUAL(Pam->play_card(Card(JACK, DIAMONDS), DIAMONDS), Card(KING, HEARTS));

    delete Jim;
    delete Dwight;
    delete Micheal;
    delete Pam;
}   

// Add more tests here

TEST_MAIN()
