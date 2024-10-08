#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(name_func)
{
    Player * Jim = Player_factory("Jimothy", "Simple");
    Player * Dwight = Player_factory("Dwigt", "Simple");
    Player * Micheal = Player_factory("Agent Scarn", "Simple");
    Player * Pam = Player_factory("Pamela", "Simple");

    ASSERT_EQUAL("Jimothy", Jim->get_name());
    ASSERT_EQUAL("Dwigt", Dwight->get_name());
    ASSERT_EQUAL("Agent Scarn", Micheal->get_name());
    ASSERT_EQUAL("Pamela", Pam->get_name());

    delete Jim;
    delete Dwight;
    delete Micheal;
    delete Pam;
}

TEST(trump_func)
{
    Player * Jim = Player_factory("Jimothy", "Simple");
    Player * Dwight = Player_factory("Dwigt", "Simple");
    Player * Micheal = Player_factory("Agent Scarn", "Simple");
    Player * Pam = Player_factory("Pamela", "Simple");

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

    //JIM HAND      {(ACE SPADES),      (NINE CLUBS),   (JACK DIAMONDS),    (QUEEN CLUBS),      (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),    (KING CLUBS),   (NINE SPADES),      (ACE CLUBS),        (NINE DIAMONDS)}
    //MICHEAL HAND  {(TEN HEARTS),      (JACK SPADES),  (KING DIAMONDS),    (KING SPADES),      (JACK CLUBS)}
    //PAM HAND      {(KING HEARTS),     (NINE HEARTS),  (TEN SPADES),       (QUEEN DIAMONDS),   (ACE HEARTS)}
    //round 1 testing
    ASSERT_FALSE(Jim->make_trump(Card(JACK, HEARTS), false, 1, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_TRUE(Dwight->make_trump(Card(TEN, CLUBS), false, 1, order_up));
    ASSERT_TRUE(order_up == CLUBS);
    ASSERT_TRUE(Micheal->make_trump(Card(QUEEN, SPADES), false, 1, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_FALSE(Pam->make_trump(Card(ACE, DIAMONDS), false, 1, order_up));
    ASSERT_TRUE(order_up == SPADES);

    //round 2 testing
    ASSERT_TRUE(Jim->make_trump(Card(ACE, DIAMONDS), false, 2, order_up));
    ASSERT_TRUE(order_up == HEARTS);
    ASSERT_TRUE(Dwight->make_trump(Card(JACK, HEARTS), true, 2, order_up));
    ASSERT_TRUE(order_up == DIAMONDS);
    ASSERT_TRUE(Micheal->make_trump(Card(TEN, CLUBS), false, 2, order_up));
    ASSERT_TRUE(order_up == SPADES);
    ASSERT_TRUE(Pam->make_trump(Card(QUEEN, SPADES), true, 2, order_up));
    ASSERT_TRUE(order_up == CLUBS);

    delete Jim;
    delete Dwight;
    delete Micheal;
    delete Pam;
}

TEST(add_discard_func)
{
    Player * Jim = Player_factory("Jimothy", "Simple");
    Player * Dwight = Player_factory("Dwigt", "Simple");
    Player * Micheal = Player_factory("Agent Scarn", "Simple");
    Player * Pam = Player_factory("Pamela", "Simple");

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

    Jim->add_and_discard(Card(ACE, DIAMONDS));      //removes NINE CLUBS
    Dwight->add_and_discard(Card(TEN, CLUBS));      //removes NINE SPADES
    Micheal->add_and_discard(Card(JACK, HEARTS));   //removes JACK SPADES
    Pam->add_and_discard(Card(QUEEN, SPADES));      //removes NINE HEARTS   

    delete Jim;
    delete Dwight;
    delete Micheal;
    delete Pam; 
}

TEST(lead_func)
{
    Player * Jim = Player_factory("Jimothy", "Simple");
    Player * Dwight = Player_factory("Dwigt", "Simple");
    Player * Micheal = Player_factory("Agent Scarn", "Simple");
    Player * Pam = Player_factory("Pamela", "Simple");

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

    //JIM HAND {(ACE SPADES), (NINE CLUBS), (JACK DIAMONDS), (QUEEN CLUBS), (TEN DIAMONDS)}
    ASSERT_EQUAL(Jim->lead_card(SPADES), Card(QUEEN, CLUBS));
    ASSERT_EQUAL(Jim->lead_card(CLUBS), Card(ACE, SPADES));
    ASSERT_EQUAL(Jim->lead_card(HEARTS), Card(TEN, DIAMONDS));
    ASSERT_EQUAL(Jim->lead_card(DIAMONDS), Card(NINE, CLUBS));

    //DWIGHT HAND {(QUEEN HEARTS), (KING CLUBS), (NINE SPADES), (ACE CLUBS), (NINE DIAMONDS)}
    ASSERT_EQUAL(Dwight->lead_card(SPADES), Card(ACE, CLUBS));
    ASSERT_EQUAL(Dwight->lead_card(CLUBS), Card(QUEEN, HEARTS));
    ASSERT_EQUAL(Dwight->lead_card(HEARTS), Card(KING, CLUBS));
    ASSERT_EQUAL(Dwight->lead_card(DIAMONDS), Card(NINE, SPADES));

    //MICHEAL HAND {(TEN HEARTS), (JACK SPADES), (KING DIAMONDS), (KING SPADES), (JACK CLUBS)}
    ASSERT_EQUAL(Micheal->lead_card(SPADES), Card(KING, DIAMONDS));
    ASSERT_EQUAL(Micheal->lead_card(SPADES), Card(TEN, HEARTS));
    ASSERT_EQUAL(Micheal->lead_card(SPADES), Card(KING, SPADES));
    ASSERT_EQUAL(Micheal->lead_card(SPADES), Card(JACK, CLUBS));
    //wrong both highest trump or highest no trump
    //PAM HAND  {(KING HEARTS), (NINE HEARTS), (TEN SPADES), (QUEEN DIAMONDS), (ACE HEARTS)}
    ASSERT_EQUAL(Pam->lead_card(HEARTS), Card(QUEEN, DIAMONDS));
    ASSERT_EQUAL(Pam->lead_card(HEARTS), Card(TEN, SPADES));
    ASSERT_EQUAL(Pam->lead_card(HEARTS), Card(QUEEN, CLUBS));
    ASSERT_EQUAL(Pam->lead_card(HEARTS), Card(QUEEN, CLUBS));
}

TEST(stuff)
{
    Player * Jim = Player_factory("Jimothy", "Simple");
    Player * Dwight = Player_factory("Dwigt", "Simple");
    Player * Micheal = Player_factory("Agent Scarn", "Simple");
    Player * Pam = Player_factory("Pamela", "Simple");

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

    //JIM HAND      {(ACE SPADES),      (NINE CLUBS),   (JACK DIAMONDS),    (QUEEN CLUBS),      (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),    (KING CLUBS),   (NINE SPADES),      (ACE CLUBS),        (NINE DIAMONDS)}
    //MICHEAL HAND  {(TEN HEARTS),      (JACK SPADES),  (KING DIAMONDS),    (KING SPADES),      (JACK CLUBS)}
    //PAM HAND      {(KING HEARTS),     (NINE HEARTS),  (TEN SPADES),       (QUEEN DIAMONDS),   (ACE HEARTS)}
    ASSERT_EQUAL(Jim->lead_card(SPADES), Card(QUEEN, CLUBS));
    ASSERT_EQUAL(Dwight->play_card(Card(QUEEN, CLUBS), SPADES), Card(ACE, CLUBS));
    ASSERT_EQUAL(Micheal->play_card(Card(QUEEN, CLUBS), SPADES), Card(JACK, CLUBS));
    ASSERT_EQUAL(Pam->play_card(Card(QUEEN, CLUBS), SPADES), Card(NINE, HEARTS));

    //JIM HAND      {(ACE SPADES),      (NINE CLUBS),   (JACK DIAMONDS),    (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),    (KING CLUBS),   (NINE SPADES),      (NINE DIAMONDS)}
    //MICHEAL HAND  {(TEN HEARTS),      (JACK SPADES),  (KING DIAMONDS),    (KING SPADES)}
    //PAM HAND      {(KING HEARTS),     (TEN SPADES),   (QUEEN DIAMONDS),   (ACE HEARTS)}
    ASSERT_EQUAL(Dwight->lead_card(SPADES), Card(KING, CLUBS));
    ASSERT_EQUAL(Micheal->play_card(Card(KING, CLUBS), SPADES), Card(TEN, HEARTS));
    ASSERT_EQUAL(Pam->play_card(Card(KING, CLUBS), SPADES), Card(QUEEN, DIAMONDS));
    ASSERT_EQUAL(Jim->play_card(Card(KING, CLUBS), SPADES), Card(NINE, CLUBS));

    //JIM HAND      {(ACE SPADES),      (JACK DIAMONDS),    (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),    (NINE SPADES),      (NINE DIAMONDS)}
    //MICHEAL HAND  {(JACK SPADES),     (KING DIAMONDS),    (KING SPADES)}
    //PAM HAND      {(KING HEARTS),     (TEN SPADES),       (ACE HEARTS)}
    ASSERT_EQUAL(Micheal->lead_card(SPADES), Card(KING, DIAMONDS));
    ASSERT_EQUAL(Pam->play_card(Card(KING, DIAMONDS), SPADES), Card(KING, HEARTS));
    ASSERT_EQUAL(Jim->play_card(Card(KING, DIAMONDS), SPADES), Card(JACK, DIAMONDS));
    ASSERT_EQUAL(Dwight->play_card(Card(KING, DIAMONDS), SPADES), Card(NINE, DIAMONDS));

    //JIM HAND      {(ACE SPADES),      (TEN DIAMONDS)}
    //DWIGHT HAND   {(QUEEN HEARTS),    (NINE SPADES)}
    //MICHEAL HAND  {(JACK SPADES),     (KING SPADES)}
    //PAM HAND      {(TEN SPADES),      (ACE HEARTS)}
    ASSERT_EQUAL(Pam->lead_card(SPADES), Card(ACE, HEARTS));
    ASSERT_EQUAL(Jim->play_card(Card(ACE, HEARTS), SPADES), Card(TEN, DIAMONDS));
    ASSERT_EQUAL(Dwight->play_card(Card(ACE, HEARTS), SPADES), Card(QUEEN, HEARTS));
    ASSERT_EQUAL(Micheal->play_card(Card(ACE, HEARTS), SPADES), Card(KING, SPADES));

    //JIM HAND      {(ACE SPADES)}
    //DWIGHT HAND   {(NINE SPADES)}
    //MICHEAL HAND  {(JACK SPADES)}
    //PAM HAND      {(TEN SPADES)}
    ASSERT_EQUAL(Jim->lead_card(SPADES), Card(ACE, SPADES));
    ASSERT_EQUAL(Dwight->play_card(Card(ACE, SPADES), SPADES), Card(NINE, SPADES));
    ASSERT_EQUAL(Micheal->play_card(Card(ACE, SPADES), SPADES), Card(JACK, SPADES));
    ASSERT_EQUAL(Pam->play_card(Card(ACE, SPADES), SPADES), Card(TEN, SPADES));


    delete Jim;
    delete Dwight;
    delete Micheal;
    delete Pam;
}   

// Add more tests here

TEST_MAIN()
