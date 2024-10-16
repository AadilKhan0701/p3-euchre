#include <iostream>
#include <istream>
#include <fstream>
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
using namespace std;

class Game {
 public:
    Game(Pack * cp, bool s, int ptw, vector<Player*> pl);
    void play();
 private:
    void shuffle();
    void deal(int dealer);
    void make_trump(int dealer, Suit* trump);
    void play_hand(int leading, Suit* trump);
    void awarding_point(vector<int> trick_points, vector<Card*> trick_hand, Card* leadCard, Suit* trump);
    
    Pack * pack;
    bool shuffling;
    int points;
    vector<Player*> players;
};

int main(int argc, char* argv[]) 
{
    if(argc != 12)
    {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    ifstream pack_name(argv[1]);
    string shuffling = argv[2];
    int points = stoi(argv[3]);
    Pack pack = Pack(pack_name);
    bool shuffle = (shuffling == "shuffle") ? true: false;
    Player * player_1 = Player_factory(argv[4], argv[5]);
    Player * player_2 = Player_factory(argv[6], argv[7]);
    Player * player_3 = Player_factory(argv[8], argv[9]);
    Player * player_4 = Player_factory(argv[10], argv[11]);
    vector<Player*> players = {player_1, player_2, player_3, player_4};
    Game game = Game(&pack, shuffle, points, players);
    game.play();
}

Game::Game(Pack * cp, bool s, int ptw, vector<Player*> pl)
    : pack(cp), shuffling(s), points(ptw), players(pl) {}

//Game play
void Game::play()
{
    int c, dealer;
    Suit trump;

    for(c= 0; c< points; c++)
    {
        /* **Setup Table** */
        shuffle();
        dealer = c%4;
        deal(dealer);
        cout << "Hand " << c << endl;
        cout << players.at(dealer)->get_name() << " deals" << endl;

        /* **Making Trump** */
        make_trump(dealer, &trump);

        /* **Playing Hand** */
        play_hand(dealer, &trump);
    }
}

//picking trump card for that round
void Game::make_trump(int dealer, Suit* trump)
{   
    Card upCard = pack->deal_one();
    cout << upCard << " turned up" << endl;
    bool picked = false; //if the trump suit has been picked yet
    int c, next; //c is used for rounds; next is used to keep track player turn
    bool is_dealer; //if the player is the dealer or not
    
    for(c= 1; c< 3; c++)
    {
        next = dealer;
        do{
            next = (next+1)%4;
            is_dealer = (next == dealer);
            picked = players.at(next)->make_trump(upCard, is_dealer, c, *trump);
            
            if(picked)
            {
                cout << players.at(next)->get_name() << " orders up " << *trump << endl;
                break;
            }
            else
            {
                cout << players.at(next)->get_name() << " passes" << endl;
            }

        }while(next != dealer);
        if(picked){break;}
    }
}

void Game::play_hand(int leading, Suit* trump)
{
    vector<int> trick_points = {0, 0, 0, 0}; //vector of the points accrued by each player
    vector<Card*> trick(4);  //array of the cards played that trick
    int current = leading;
    Card leadCard;
    int c;

    for(c= 0; c< 5; c++)
    {   
        //leading the trick
        current = (current+1)%4;
        leadCard = players.at(current)->lead_card(*trump);
        trick.at(current) = &leadCard;
        cout << leadCard << " led by " << players.at(current)->get_name() << endl;

        //following the led card
        current = (current+1)%4;
        trick.at(current) = &players.at(current)->play_card(leadCard, *trump);
        cout << trick.at(current) << " played by " << players.at(current)->get_name() << endl;

        current = (current+1)%4;
        trick.at(current) = &players.at(current)->play_card(leadCard, *trump);
        cout << trick.at(current) << " played by " << players.at(current)->get_name() << endl;

        current = (current+1)%4;
        trick.at(current) = &players.at(current)->play_card(leadCard, *trump);
        cout << trick.at(current) << " played by " << players.at(current)->get_name() << endl;

        awarding_point(trick_points, trick, &leadCard, trump);
    }
}

//determining who the point goes to for each trick
void Game::awarding_point(vector<int> trick_points, vector<Card*> trick_hand, Card* leadCard, Suit* trump)
{
    //index of the highest, first Card, for loop to find highest, then add points to person who wins trick 

}

//if shuffling is turned on (true), shuffle the deck
void Game::shuffle()
{
    if(shuffling){pack->shuffle();}
    else{pack->reset();}
}

//deal the cards in the pack to each Player
void Game::deal(int dealer)
{
    int first = (dealer+1)%4;
    int second = (dealer+2)%4;
    int third = (dealer+3)%4;

    //3-2-3-2
    players.at(first)->add_card(pack->deal_one());
    players.at(first)->add_card(pack->deal_one());
    players.at(first)->add_card(pack->deal_one());
    players.at(second)->add_card(pack->deal_one());
    players.at(second)->add_card(pack->deal_one());
    players.at(third)->add_card(pack->deal_one());
    players.at(third)->add_card(pack->deal_one());
    players.at(third)->add_card(pack->deal_one());
    players.at(dealer)->add_card(pack->deal_one());
    players.at(dealer)->add_card(pack->deal_one());

    //2-3-2-3
    players.at(first)->add_card(pack->deal_one());
    players.at(first)->add_card(pack->deal_one());
    players.at(second)->add_card(pack->deal_one());
    players.at(second)->add_card(pack->deal_one());
    players.at(second)->add_card(pack->deal_one());
    players.at(third)->add_card(pack->deal_one());
    players.at(third)->add_card(pack->deal_one());
    players.at(dealer)->add_card(pack->deal_one());
    players.at(dealer)->add_card(pack->deal_one());
    players.at(dealer)->add_card(pack->deal_one());
}