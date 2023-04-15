//
//  Blackjack.cpp
//  Demo
//
//  Created by 张开斌 on 2023/4/13.
//

#include "Blackjack.hpp"
#include<iostream>
#include<stdlib.h>
#include<cstdlib>
#include<algorithm>
#include<random>
#include <iomanip>
#include<unistd.h>
using namespace std;
enum Suit{Clubs,Hearts,Spades,Diamonds};
string Values[]={"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
class Card{
private:
    int type;
    string sign;
    string value;
    bool visible;
    void setSign(){
        switch (this->type) {
            case 0:
                this->sign="♣";
                break;
            case 1:
                this->sign="♥";
                break;
            case 2:
                this->sign="♠";
                break;
            case 3:
                this->sign="♦";
                break;
            default:
                break;
        }
    }
public:
    Card(int type,string value){
        this->type=type;
        this->value=value;
        this->visible=true;
        setSign();
    }
    void setVisible(bool visible){
        this->visible=visible;
    }
    int getValue(){
        if(this->value=="J"||this->value=="Q"||this->value=="K")
            return 10;
        else if(this->value=="A"){
            return -1;
        }
        else{
            return stoi(this->value);
        }
    }
    void toString(){
        if(this->visible){
            cout<<" "<<this->sign<<" "<<this->value<<endl;
            cout<<"-------"<<endl;
        }
        else{
            cout<<" ***** "<<endl;
            cout<<"-------"<<endl;
        }
    }
};

class Poker{
private:
    vector<Card> cards;
public:
    void initPoker(){
        for(int i=0;i<4;i++){
            for(int j=0;j<13;j++){
                cards.push_back(Card(i, Values[j]));
            }
        }
    }
    void showAllCards(){
        for(int i=0;i<this->cards.size();i++){
            this->cards[i].toString();
        }
    }
    Card getTopCard(){
        return this->cards[0];
    }
    void dealACard(){
        this->cards.erase(this->cards.begin());
    }
    void shuffle(){
        random_device rd;
        mt19937 rng(rd());
        std::shuffle(this->cards.begin(),this->cards.end(),rng);
    }
    void pokerDestroy(){
        vector<Card>().swap(this->cards);
    }
};


class Player{
protected:
    vector<Card> cards;
public:
    vector<Card> getCards(){
        return this->cards;
    }
    void showCards(){
        for(int i=0;i<this->cards.size();i++){
            this->cards[i].toString();
        }
    }
    void hit(Card card){//拿牌
        this->cards.push_back(card);
    }
    void backCard(){
        vector<Card>().swap(cards);
    }
   
};
class Dealer:public Player{
public:
    void hideFirstCard(){
        if(this->cards.size()!=0){
            this->cards[0].setVisible(false);
        }
    }
    void showFirstCard(){
        if(this->cards.size()!=0){
            this->cards[0].setVisible(true);
        }
    }
};
class Game{
private:
    int turn;//1:玩家  0:庄家
    bool isGameOver=false;
    int playerPoint;
    int dealerPoint;
    int rounds=0;
    Poker poker;
    Player player;
    Dealer dealer;
public:
    void initData(){
        isGameOver=false;
        playerPoint=0;
        dealerPoint=0;
        player.backCard();
        dealer.backCard();
        poker.pokerDestroy();
        poker.initPoker();
        poker.shuffle();
        turn=1;
    }
    void gameStart(){
        while(!isGameOver){
            oneRound();
        }
    }
    void oneRound(){//一轮游戏
        if(rounds==0){
            cout<<"Welcome to BlackJack.Press any key to start:"<<endl;
            cin.get();
            gameProcess();
        }
        else{
            cout<<"Do you want to continue?(y/n):"<<endl;
            char c;
            cin>>c;
            if(c=='y'){
                system("clear");//清屏
                gameProcess();
            }
            else{
                isGameOver=true;
            }
        }
       
    }
    void gameProcess(){
        initData();
        dealFirstTwoCard();
        show();
        while(this->turn==1){
            askHitOrStand();
        }
        dealerTurn();
        comparePoint();
        rounds++;
    }
    void dealFirstTwoCard(){
        for(int i=0;i<4;i++){
            if(i%2==1){
                dealer.hit(poker.getTopCard());
                poker.dealACard();
                if(dealer.getCards().size()==1){
                    dealer.hideFirstCard();
                }
            }
            else{
                player.hit(poker.getTopCard());
                poker.dealACard();
            }
        }
    }
    void calculate(){
        int sum[2]={0,0};
        vector<Card> pc;
        if(this->turn==1){
            pc=this->player.getCards();
        }
        else{
            pc=this->dealer.getCards();
        }
        for(int i=0;i<pc.size()&&(sum[0]<=21||sum[1]<=21);i++){
            if(pc[i].getValue()!=-1){
                sum[0]+=pc[i].getValue();
                sum[1]+=pc[i].getValue();
            }
            else{
                sum[0]+=1;
                sum[1]+=11;
            }
        }
        if(sum[0]==sum[1]){
            cout<<(this->turn==1?"your points is:":"dealer\'s points is:")<<sum[0]<<endl;
        }
        else{
            cout<<(this->turn==1?"your points is:":"dealer\'s points is:")<<sum[0]<<(sum[1]>21?"":"/"+to_string(sum[1]))<<endl;
        }
       
       
        if(this->turn==1){
            if(sum[1]<=21&&sum[0]<=21){
                this->playerPoint=sum[1];
            }
            else if(sum[1]>21&&sum[0]<=21){
                this->playerPoint=sum[0];
            }
            else if(sum[1]>21&&sum[0]>21){
                this->playerPoint=0;
            }
        }
        else{
            if(sum[1]<=21&&sum[0]<=21){
                this->dealerPoint=sum[1];
            }
            else if(sum[1]>21&&sum[0]<=21){
                this->dealerPoint=sum[0];
            }
            else if(sum[1]>21&&sum[0]>21){
                this->dealerPoint=0;
            }
        }
        if(sum[0]>21&&sum[1]>21){
            cout<<"Burst!!!"<<endl;
            this->turn=0;
        }
        if(sum[1]==21&&pc.size()==2){
            cout<<"BlackJack!!!"<<endl;
            this->turn=0;
        }
                    
    }
    void changeTurn(){
        this->turn=0;
    }
    void show(){
        cout<<"dealer\'s cards:"<<endl;
        this->dealer.showCards();
        cout<<"yours cards:"<<endl;
        this->player.showCards();
    }
    void askHitOrStand(){
        //问玩家是否要牌
        char choice;
        calculate();
        cout<<"Hit or Stand?(h/s):";
        cin>>choice;
        if(choice=='h'||choice=='H'){
            player.hit(poker.getTopCard());
            poker.dealACard();
            show();
            calculate();
        }
        else if(choice=='S'||choice=='s'){
            changeTurn();
        }
        else{
            cout<<"invalid input!"<<endl;
        }
      
    }
    void dealerTurn(){//庄家翻牌
        sleep(1);
        cout<<"Now to Dealer."<<endl;
        sleep(2);
        dealer.showFirstCard();
        show();
        calculate();
        dealerMustHit();
    }
    void dealerMustHit(){
        while(this->dealerPoint<17&&this->dealerPoint!=0){
            sleep(3);
            dealer.hit(poker.getTopCard());
            poker.dealACard();
            show();
            calculate();
        }
    }
    void comparePoint(){
        if(this->playerPoint>this->dealerPoint){
            cout<<"You win!!!"<<endl;
        }
        else if(this->playerPoint<this->dealerPoint||(this->playerPoint==0&&this->dealerPoint==0)){
            cout<<"You lose!!!"<<endl;
        }
        else{
            cout<<"Push"<<endl;
        }
    }
};
int main(){
    Game g;
    g.gameStart();
    return 0;
}
