#include <iostream>
#include <istream>
#include <fstream>
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
using namespace std;

class Game {
 public:
  Game(/* game details */);
  void play();
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
    ifstream s(argv[2]);
    ifstream p(argv[3]);
    string sh;
    s >> sh;
    int pi;
    p >> pi;
    //Pack pack = Pack(pack_name);
    bool shuffle = (sh == "shuffle") ? true: false;
    int points = pi; //not true or complete
    Player * player_1 = Player_factory(argv[4], argv[5]);
    Player * player_2 = Player_factory(argv[6], argv[7]);
    Player * player_3 = Player_factory(argv[8], argv[9]);
    Player * player_4 = Player_factory(argv[10], argv[11]);

    cout << "pack :: " << &pack_name << endl;
    cout << "shuffle :: " << shuffle <<" :: " << argv[2] << endl;
    cout << "points :: " << points << " :: " << argv[3] << endl;
    cout << "Player :: " << player_1->get_name() << endl;
    cout << "Player :: " << player_2->get_name() << endl;
    cout << "Player :: " << player_3->get_name() << endl;
    cout << "Player :: " << player_4->get_name() << endl;
}