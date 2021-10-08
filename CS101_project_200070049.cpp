#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  double angle_return;//Angle for coin to bounce back in parabolic motion

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    angle_return=angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void initCoinRandPos();//Initializes coin at random position
  void resetCoin();
  void resetCoinRandomly();//Once boundary condition is met, this resets coin to another random position
  void returnPathPara(double x);//Function to set return path for coin moving in parabola
  void removeCoin();//Moves coin to corner of gameboard and makes it invisible

}; // End class Coin

//Class Bomb
//To use bomb which deducts 1 from the total number of coins
class Bomb : public MovingObject {
  double bomb_start_x;
  double bomb_start_y;
  double bomb_speed;
  double release_angle_deg;
  double bomb_ax;
  double bomb_ay;

  // Moving parts
  Circle bombcircle;

 public:
 Bomb(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    bomb_speed = speed;
    release_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb();
  }

  void initBomb();
  void resetBomb();

};

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Bomb *the_bomb;
  int num_coins;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }
  void draw_lasso_band();
  void yank();
  void yankBomb();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_bomb(Bomb* bomb);
  int getNumCoins() { return num_coins; }

}; // End class Lasso
#endif


//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::initCoinRandPos(){
  coin_start_x=rand()%400 + 400;//Creates coin between x=400 and x=800
  coin_start_y=100;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {
  double coin_speed = release_speed;
  double coin_angle_deg = release_angle_deg;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::resetCoinRandomly() {
  double coin_speed = release_speed;
  double coin_angle_deg = release_angle_deg;
  coin_ax = 0;
  coin_ay = 0;
  bool paused = true, rtheta = true;
  reset_all(rand()%400+300, rand()%500, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::returnPathPara(double x){
  double coin_speed=release_speed;
  angle_return=180-angle_return;
  coin_ay=COIN_G;
  bool paused = true, rtheta = true;
  reset_all(x, coin_start_y, coin_speed, angle_return, coin_ax, coin_ay, paused, rtheta);
}

void Coin::removeCoin(){
  coin_circle.setColor(COLOR("white"));//Makes coin invisible wrt game board
  coin_circle.setFill(false);
  release_speed=0;
  coin_circle.moveTo(790,10);//Moves coin to top right corner
}

//Function to create bombs that fall from the top
void Bomb::initBomb(){
  bomb_start_x=rand()%400+300;
  bomb_start_y=10;
  bombcircle.reset(bomb_start_x,bomb_start_y,2*COIN_SIZE);
  bombcircle.setColor(COLOR("black"));
  bombcircle.setFill(true);
  addPart(&bombcircle);
}

//Resets bomb back to top when it reaches bottom
void Bomb::resetBomb(){
  double bomb_speed=rand()%20+30;
  double bomb_angle_deg = release_angle_deg;
  bomb_ax=0;
  bomb_ay=rand()%40;
  bool paused = true, rtheta = true;
  reset_all(rand()%500+250, bomb_start_y, bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
}

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("brown"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  the_bomb=NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("brown"));

  lasso_band.setColor(COLOR("blue"));
  draw_lasso_band();

} // End Lasso::initLasso()


void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin();
  }
  the_coin=NULL;
} // End Lasso::yank()

//Yank function for bomb, reduces 1 from the total coins
void Lasso::yankBomb() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_bomb != NULL) {
    num_coins--;
    the_bomb->resetBomb();
  }
  the_bomb=NULL;
}

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()

//Checks for presence of bomb
void Lasso::check_for_bomb(Bomb *bombPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb_x = bombPtr->getXPos();
  double bomb_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb_x);
  double ydiff = (lasso_y - bomb_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_bomb = bombPtr;
    the_bomb->getAttachedTo(this);
  }
}

void gameControls(char c, Lasso* lasso, Coin* coin){

      switch(c) {
      case 't':
	(*lasso).unpause();
	break;
      case 'y':
	(*lasso).yank();
	break;
      case 'l':
	(*lasso).loopit();
	(*lasso).check_for_coin(coin);
	wait(STEP_TIME*5);
	break;
      case '[':
	if((*lasso).isPaused()) { (*lasso).addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if((*lasso).isPaused()) { (*lasso).addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if((*lasso).isPaused()) { (*lasso).addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if((*lasso).isPaused()) { (*lasso).addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
    system("CLS");
    cout<<"\t"<<"THANK YOU FOR PLAYING";
    exit(0);
      default:
	break;
      }
}

//game controls for an object of bomb class
void gameControlsBomb(char c, Lasso* lasso, Bomb* bomb){

      switch(c) {
      case 't':
	(*lasso).unpause();
	break;
      case 'y':
	(*lasso).yankBomb();
	break;
      case 'l':
	(*lasso).loopit();
	(*lasso).check_for_bomb(bomb);
	wait(STEP_TIME*5);
	break;
      case '[':
	if((*lasso).isPaused()) { (*lasso).addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if((*lasso).isPaused()) { (*lasso).addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if((*lasso).isPaused()) { (*lasso).addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if((*lasso).isPaused()) { (*lasso).addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
    system("CLS");
    cout<<"\n"<<"\n";
    cout<<"\t"<<"THANK YOU FOR PLAYING";
    exit(0);
      default:
	break;
      }
}

main_program {
  //Game instructions and guidelines
  cout<<"\t"<<"\t"<<"WELCOME TO THE LASSO GAME"<<endl;
  cout<<"The objective of the game is to collect as many coins as possible using a lasso and move forward through the levels"<<endl;
  cout<<"as quickly as possible"<<endl;
  cout<<endl;
  cout<<"GAME GUIDELINES:"<<endl;
  cout<<"There are total 4 levels each with different types of situations"<<endl;
  cout<<"Level 1 consists of 1 coin moving up and down in a straight line. To pass this level you need a total of 4 coins."<<endl;
  cout<<"Level 2 consists of 1 coin moving in a parabolic path at a pretty slow speed."<<endl;
  cout<<"To move to the next level from this, you need 8 total coins."<<endl;
  cout<<"Level 3 consists of 4 coins moving in all kinds of directions, including a coin that moves and"<<endl;
  cout<<"spawns randomly on the game board"<<endl;
  cout<<"In this level smaller movements of lasso do not take place, and all movement magnitudes are quadrupled"<<endl;
  cout<<"To complete this level you need to have 13 total coins"<<endl;
  cout<<"Last but not the least comes level 4, where 2 bombs have been added along with 2 coins."<<endl;
  cout<<"Beware of these bombs as they reduce your total coins by 1 if caught"<<endl;
  cout<<"To accomplish this level and complete the game, you need a total of 17 coins."<<endl;
  cout<<"Note: In one throw of the lasso, you can get only 1 point"<<endl;
  cout<<endl;
  cout<<"CONTROLS:"<<endl;
  cout<<"Press 't' to release the lasso from its initial position"<<endl;
  cout<<"Press 'y' to bring the lasso back to its original position, once released"<<endl;
  cout<<"Press 'l' to try and loop any nearby coins or bombs into the lasso"<<endl;
  cout<<"Press '[' to reduce the angle of elevation for release of lasso"<<endl;
  cout<<"Press ']' to increase the angle of elevation for release of lasso"<<endl;
  cout<<"Press '-' to decrease the speed of the lasso"<<endl;
  cout<<"Press '=' to increase the speed of the lasso"<<endl;
  cout<<"Press 'q' to Quit the game"<<endl;
  cout<<endl;
  cout<<"\t"<<"\t"<<"GOOD LUCK"<<endl;
  //End of game instructions

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);//Default coin

  int level=1;
  char leveltext[256];
  sprintf(leveltext, "Level: %d", level);
  Text levelnumber(WINDOW_X/2,WINDOW_Y-50,leveltext);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  //Level 1 start
  for(;lasso.getNumCoins()<4;) {
    if((runTime > 0) && (currTime > runTime)) { break; }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      gameControls(c,&lasso,&coin);
    }
    lasso.nextStep(stepTime);
    coin.nextStep(stepTime);

    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  }
  coin.removeCoin();
  //end Level 1

  //Level 2 Start
  level++;
  sprintf(leveltext, "Level: %d", level);
  levelnumber.setMessage(leveltext);
  paused = true; rtheta = true;
  coin_speed=153;
  coin_angle_deg=78;
  last_coin_jump_end = 0;
  //coin that moves in motion of parabola and bounces back
  Coin coin2(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  for(;lasso.getNumCoins()>=4 && lasso.getNumCoins()<8;) {
    if((runTime > 0) && (currTime > runTime)) { break; }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      gameControls(c,&lasso,&coin2);
    }
    lasso.nextStep(stepTime);
    coin2.nextStep(stepTime);

    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin2.unpause();
      }
    }

    double x=coin2.getXPos();
    if(x>WINDOW_X) coin2.resetCoin();
    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.returnPathPara(x);
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  }
  //End Level 2

  //Level 3 Start
  level++;
  sprintf(leveltext, "Level: %d", level);
  levelnumber.setMessage(leveltext);
  coin2.resetCoin();
  paused = true; rtheta = true;
  last_coin_jump_end = 0;
  Coin coin3(30, rand()%90, coin_ax, 0, paused, rtheta);//Coin that spawns randomly
  coin3.initCoinRandPos();
  Coin coin4(COIN_SPEED, COIN_ANGLE_DEG, coin_ax, coin_ay, paused, rtheta);//Coin with default parameters of motion
  Coin coin5(80, 30, 7, coin_ay, paused, rtheta);//Coin that moves in parabola but with acceleration in x also
  for(;lasso.getNumCoins()>=8 && lasso.getNumCoins()<13;) {
    if((runTime > 0) && (currTime > runTime)) { break; }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      gameControls(c,&lasso,&coin2);
      gameControls(c,&lasso,&coin3);
      gameControls(c,&lasso,&coin4);
      gameControls(c,&lasso,&coin5);
    }
    lasso.nextStep(stepTime);
    coin2.nextStep(stepTime);
    coin3.nextStep(stepTime);
    coin4.nextStep(stepTime);
    coin5.nextStep(stepTime);

    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin2.unpause();
      }
    }
    if(coin3.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin3.unpause();
      }
    }
    if(coin4.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin4.unpause();
      }
    }
    if(coin5.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin5.unpause();
      }
    }

    double x=coin2.getXPos();
    if(x>WINDOW_X) coin2.resetCoin();
    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.returnPathPara(x);
      last_coin_jump_end = currTime;
    }
    if(coin3.getXPos() > 700 || coin3.getYPos()<0 || coin3.getYPos()>PLAY_Y_HEIGHT) {
      coin3.resetCoinRandomly();
      last_coin_jump_end = currTime;
    }
    if(coin4.getYPos() > PLAY_Y_HEIGHT) {
      coin4.resetCoin();
      last_coin_jump_end = currTime;
    }
    if(coin5.getYPos() > PLAY_Y_HEIGHT && coin5.getXPos()<WINDOW_X) {
      coin5.returnPathPara(coin5.getXPos());
      last_coin_jump_end = currTime;
    }
    if(coin5.getXPos()>WINDOW_X){
      coin5.resetCoin();
      last_coin_jump_end=currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  }
  coin4.removeCoin();
  coin5.removeCoin();
  //End Level 3

  //Level 4 Start
  level++;
  sprintf(leveltext, "Level: %d", level);
  levelnumber.setMessage(leveltext);
  coin2.resetCoin();
  coin3.resetCoinRandomly();
  paused = true; rtheta = true;
  last_coin_jump_end = 0;
  Bomb bomb1(rand()%20+30,-90,0,rand()%40,paused,rtheta);//Bomb 1
  Bomb bomb2(rand()%20+30,-90,0,rand()%40,paused,rtheta);//Bomb 2

  if(lasso.getNumCoins()>=13 && lasso.getNumCoins()<17){
    for(;;) {
      if((runTime > 0) && (currTime > runTime)) { break; }
      XEvent e;
      bool pendingEv = checkEvent(e);
      if(pendingEv) {
        char c = charFromEvent(e);
        msg[msg.length()-1] = c;
        charPressed.setMessage(msg);
        gameControls(c,&lasso,&coin2);
        gameControls(c,&lasso,&coin3);
        gameControlsBomb(c,&lasso,&bomb1);
        gameControlsBomb(c,&lasso,&bomb2);
      }
      lasso.nextStep(stepTime);
      coin2.nextStep(stepTime);
      coin3.nextStep(stepTime);
      bomb1.nextStep(stepTime);
      bomb2.nextStep(stepTime);

      if(coin2.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
    	coin2.unpause();
    	}
      }
      if(coin3.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
    	coin3.unpause();
      }
      }
      if(bomb1.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
    	bomb1.unpause();
      }
      }
      if(bomb2.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
    	bomb2.unpause();
      }
    }

    double x=coin2.getXPos();
    if(x>WINDOW_X) coin2.resetCoin();
    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.returnPathPara(x);
      last_coin_jump_end = currTime;
    }
    if(coin3.getXPos() > 700 || coin3.getYPos()<0 || coin3.getYPos()>PLAY_Y_HEIGHT) {
      coin3.resetCoinRandomly();
      last_coin_jump_end = currTime;
    }
    if(bomb1.getYPos() > PLAY_Y_HEIGHT) {
      bomb1.resetBomb();
      last_coin_jump_end = currTime;
    }
    if(bomb2.getYPos() > PLAY_Y_HEIGHT) {
      bomb2.resetBomb();
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);

    if(lasso.getNumCoins()==17) break;
  }
  }
  //Level 4 End

  wait(3);
  if(lasso.getNumCoins()==17){//Game Completion Message
    system("CLS");
    cout<<"\t"<<"CONGRATULATIONS!! YOU HAVE COMPLETED THE GAME"<<endl;
    cout<<"\t"<<"THANK YOU FOR PLAYING";
  }
} // End main_program
