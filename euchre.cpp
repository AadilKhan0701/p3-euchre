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
    void deal(const int dealer);
    void make_trump(const int dealer, Suit* trump);
    void play_hand(const int leading, const Suit* trump);
    void awarding_points_hand(const vector<int> trick_points);
    
    Pack * pack;
    bool shuffling;
    int points;
    vector<Player*> players;
    vector<pair<int, bool>> team_points;
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

    cout << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " " 
         << argv[4] << " " << argv[5] << " " << argv[6] << " " << argv[7] << " "
         << argv[8] << " " << argv[9] << " " << argv[10]<< " " << argv[11]<< " "<<endl;

    Game game = Game(&pack, shuffle, points, players);
    game.play();
}

Game::Game(Pack * cp, bool s, int ptw, vector<Player*> pl)
    : pack(cp), shuffling(s), points(ptw), players(pl), 
      team_points({{0, false}, {0, false}}) {}

//Game play
void Game::play()
{
    int c, dealer;
    Suit trump;
    c = 0;
    while(team_points.at(0).first < points && team_points.at(1).first < points)
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
        c++;
    }

    //Winner is...
    if(team_points.at(0).first > team_points.at(1).first)
    {
        cout << players.at(0)->get_name() << " and "
             << players.at(2)->get_name() << " win!" << endl;
    }
    else
    {
        cout << players.at(1)->get_name() << " and "
             << players.at(3)->get_name() << " win!" << endl;
    }
}

//picking trump card for that round
void Game::make_trump(const int dealer, Suit* trump)
{   
    Card upCard = pack->deal_one();
    cout << upCard << " turned up" << endl;
    bool picked = false; //if the trump suit has been picked yet
    int c, next; //c is used for rounds; next is used to keep track player turn
    bool is_dealer; //if the player is the dealer or not

    //resets which team ordered up trump 
    team_points.at(0).second = false;
    team_points.at(1).second = false;
    
    for(c= 1; c< 3; c++)
    {
        next = dealer;
        do{
            next = (next+1)%4;
            is_dealer = (next == dealer);
            picked = players.at(next)->make_trump(upCard, is_dealer, c, *trump);
            
            if(picked)
            {
                cout << players.at(next)->get_name() << " orders up " 
                     << *trump << endl;
                if(next%2 == 0)
                    team_points.at(0).second = true;
                else
                    team_points.at(1).second = true;
                break;
            }
            else
            {
                cout << players.at(next)->get_name() << " passes" << endl;
            }

        }while(next != dealer);
        if(picked && c == 1)
        {
            (players.at(dealer)->add_and_discard(upCard));
            cout << endl;
        }
        if(picked){break;}
    }
}

void Game::play_hand(const int leading, const Suit* trump)
{
    vector<int> trick_points = {0, 0, 0, 0}; //vector of the points accrued by each player
    vector<Card*> trick(4);  //array of the cards played that trick
    int current = (leading+1)%4;
    Card leadCard, card_1, card_2, card_3, highest;
    int c, k, index;

    for(c= 0; c< 5; c++)
    {   
        //leading the trick
        leadCard = players.at(current)->lead_card(*trump);
        trick.at(current) = &leadCard;
        cout << leadCard << " led by " << players.at(current)->get_name() << endl;

        //following the led card
        current = (current+1)%4;
        card_1 = players.at(current)->play_card(leadCard, *trump);
        trick.at(current) = &card_1;
        cout << card_1 << " played by " << players.at(current)->get_name() << endl;

        current = (current+1)%4;
        card_2 = players.at(current)->play_card(leadCard, *trump);
        trick.at(current) = &card_2;
        cout << card_2 << " played by " << players.at(current)->get_name() << endl;

        current = (current+1)%4;
        card_3 = players.at(current)->play_card(leadCard, *trump);
        trick.at(current) = &card_3;
        cout << card_3 << " played by " << players.at(current)->get_name() << endl;

        //awarding_points_trick(trick_points, trick, &leadCard, trump, current);
        highest = *trick.at(0);
        index = 0;

        for(k= 1; k< trick.size(); k++)
        {
            if(Card_less(highest, *trick.at(k), leadCard, *trump))
            {
                highest = *trick.at(k);
                index = k;
            }
        }
        trick_points.at(index)++;
        current = index;
        cout << players.at(index)->get_name() << " takes the trick" << endl << endl;
    }
    awarding_points_hand(trick_points);
}

//determing which team gets how many points for each hand
void Game::awarding_points_hand(const vector<int> trick_points)
{
    int sum_team_1 = trick_points.at(0) + trick_points.at(2);
    int sum_team_2 = trick_points.at(1) + trick_points.at(3);

    if(sum_team_1 > sum_team_2)
    {
        cout << players.at(0)->get_name() << " and " << players.at(2)->get_name()
             << " win the hand" << endl;
        //determines if march or euchred and adds points
        if(team_points.at(0).second && sum_team_1 == 5)
        {
            team_points.at(0).first += 2;
            cout << "march!" << endl;
        }
        else if(!team_points.at(0).second && sum_team_1 > 2) 
        {
            team_points.at(0).first +=2;
            cout << "euchred!" << endl;
        }
        else {team_points.at(0).first++;}
    }
    else
    {
        cout << players.at(1)->get_name() << " and " << players.at(3)->get_name()
             << " win the hand" << endl;
        //determines if march or euchred and adds points
        if(team_points.at(1).second && sum_team_2 == 5)
        {
            team_points.at(1).first += 2;
            cout << "march!" << endl;
        }
        else if(!team_points.at(1).second && sum_team_2 > 2) 
        {
            team_points.at(1).first +=2;
            cout << "euchred!" << endl;
        }
        else {team_points.at(1).first++;}
    }

    cout << players.at(0)->get_name() << " and " << players.at(2)->get_name()
         << " have " << team_points.at(0).first << " points" << endl;
    cout << players.at(1)->get_name() << " and " << players.at(3)->get_name()
         << " have " << team_points.at(1).first << " points" << endl << endl;
}

//if shuffling is turned on (true), shuffle the deck
void Game::shuffle()
{
    pack->reset();
    if(shuffling){pack->shuffle();}
}

//deal the cards in the pack to each Player
void Game::deal(const int dealer)
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