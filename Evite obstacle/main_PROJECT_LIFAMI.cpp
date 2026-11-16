// apps/start/MYPROECT/main_MYPROJECT.cpp
#include <iostream>
#include <Grapic.h>
#include <cmath>
#include <stdlib.h>
using namespace grapic;
using namespace std;
const int MAX = 500;
const int MAX_COL=10;
const int DIMW = 500;
const float G=-9.81;
const float dt=0.1;
const float lambda = 0.90;
struct vec2 {
    float x,y;

};

struct particule {
    vec2 P,F,V;
    float m;

};



struct obstacle {
    int lon,lar;
    particule p;
};

struct collecteble {
    int type; //0= coins, 1=dash power up, 2= kill obstacle, -1= reseted;
    vec2 p;

};

struct player {
    particule p;
    Image im;
    int rayon;
    int nb_coin, nb_dash, nb_kill;
};


struct world {
    obstacle T[MAX];
    vec2 collectebles[MAX];
    int nbobs,round,collects;
    player p;
    collecteble colec[MAX];
    bool spawned;
};




vec2 make_vec2 (float x, float y) {
    vec2 c;
    c.x=x;
    c.y=y;
    return c;

}
vec2 make_vec2_exp (float x, float y) {
    vec2 c;
    c.x=x*cos(y);
    c.y=x*sin(y);
    return c;

}
vec2 operator+ (vec2 a, vec2 b) {
    vec2 c;
    c.x=a.x+b.x;
    c.y=a.y+b.y;
    return c;


}
vec2 operator- (vec2 a, vec2 b) {
    vec2 c;
    c.x=a.x-b.x;
    c.y=a.y-b.y;
    return c;
}
vec2 operator* (float lamda, vec2 b) {
    vec2 c;
    c.x=lamda*b.x;
    c.y=lamda*b.y;
    return c;
}
vec2 operator/ (vec2 b,float lamda ) {
    vec2 c;
    c.x=b.x/lamda;
    c.y=b.y/lamda;
    return c;
}
vec2 scale (vec2 p, float lamda, float bx, float by) {
    vec2 c=make_vec2(bx,by);
    vec2 a = p-c;
    a=lamda*a;
    a= a+c;
    return a;
}

void addForce (particule &part) {
    part.F=part.F + make_vec2(0,part.m * G);
}


void collision(particule &part) {
    if (part.P.y <0) {
        part.P.y=-part.P.y;
        part.V.y=-part.V.y;
        part.V=lambda*part.V;

    }
    if (part.P.y >DIMW) {
        part.P.y=DIMW - (part.P.y - DIMW);
        part.V.y=-part.V.y;
        part.V=lambda*part.V;

    }
    if (part.P.x <0) {
        part.P.x=-part.P.x;
        part.V.x=-part.V.x;
        part.V=lambda*part.V;

    }
    if (part.P.x >DIMW) {
        part.P.x=DIMW - (part.P.x - DIMW);
        part.V.x=-part.V.x;
        part.V=lambda*part.V;

    }
}


void collision_obs(obstacle &obs) {
    if (obs.p.P.x - obs.lon/2 <0) {
        obs.p.P.x= obs.p.P.x + obs.lon/2 ;
        obs.p.V.x = -obs.p.V.x * lambda;

    }
    if (obs.p.P.x + obs.lon/2 >DIMW) {
        obs.p.P.x=DIMW - obs.lon / 2;
        obs.p.V.x = -obs.p.V.x * lambda;

    }
}



void partupdatePV(particule &part)
{
    part.V=part.V + dt* (part.F / part.m);
    part.P=part.P + ( dt* part.V);
    part.F.x=0;
    part.F.y=0;


}
void draw_explosion (vec2 p){
    color(153,0,0);
    circleFill(p.x,p.y,10);
    color (204,102,0);
    circleFill(p.x,p.y,18);
    color(204,204,0);
    circleFill(p.x,p.y,30);
    delay(15);


}

void controls (world &w) {

    if (isKeyPressed (SDLK_UP)) {
        w.p.p.F=0*w.p.p.F;
        if (w.p.p.V.y<0) {
            w.p.p.V.y = 15;
        } else
            w.p.p.V.y +=23;
    }
    if (isKeyPressed (SDLK_LEFT)) {
        w.p.p.F=0*w.p.p.F;
        if (w.p.p.V.x > 0) {
            w.p.p.V.x=-5;

        } else w.p.p.V.x -=10;

    }
    if (isKeyPressed (SDLK_DOWN)) {
        w.p.p.F=0*w.p.p.F;
        if (w.p.p.V.y>0) {
            w.p.p.V.y = -5;
        } else
            w.p.p.V.y -=10;
    }
    if (isKeyPressed (SDLK_RIGHT)) {
        w.p.p.F=0*w.p.p.F;
        if (w.p.p.V.x < 0) {
            w.p.p.V.x=5;

        } else w.p.p.V.x +=10;

    }
    if ((isKeyPressed('f')) && (w.p.nb_dash > 0)){
        if ((w.p.p.V.x > 0) && (w.p.p.V.y > 0)){
            w.p.p.P.x += 40;
            w.p.p.P.y +=40;
        }
        if ((w.p.p.V.x < 0) && (w.p.p.V.y > 0)){
            w.p.p.P.x -= 40;
            w.p.p.P.y +=40;
        }
        if ((w.p.p.V.x < 0) && (w.p.p.V.y < 0)){
            w.p.p.P.x -= 40;
            w.p.p.P.y -=40;
        }
        if ((w.p.p.V.x > 0) && (w.p.p.V.y < 0)){
            w.p.p.P.x += 40;
            w.p.p.P.y -=40;
        }

    }
    if ((isKeyPressed('g')&& (w.p.nb_kill > 0))){
        draw_explosion(w.p.p.P);
        for (int i=0;i < w.nbobs ; i++){
         if ((w.T[i].p.P.x >= w.p.p.P.x - 30) && (w.T[i].p.P.y >= w.p.p.P.y - 30) &&
             (w.T[i].p.P.x <= w.p.p.P.x + 30) && (w.T[i].p.P.y <= w.p.p.P.y + 30)){
             if ((w.T[i].p.P.x < w.p.p.P.x)&& (w.T[i].p.P.y < w.p.p.P.y)){
                w.T[i].p.V.x -=40;
                w.T[i].p.V.y -=40;
             }
             if ((w.T[i].p.P.x > w.p.p.P.x)&& (w.T[i].p.P.y < w.p.p.P.y)){
                w.T[i].p.V.x +=40;
                w.T[i].p.V.y -=40;
             }
             if ((w.T[i].p.P.x < w.p.p.P.x)&& (w.T[i].p.P.y > w.p.p.P.y)){
                w.T[i].p.V.x -=40;
                w.T[i].p.V.y =40;
             }
             if ((w.T[i].p.P.x > w.p.p.P.x)&& (w.T[i].p.P.y > w.p.p.P.y)){
                w.T[i].p.V.x +=40;
                w.T[i].p.V.y =40;
             }

             }
        }
        w.p.nb_kill--;
    }


}

void update_part (world &w) {

    addForce(w.p.p);
    partupdatePV(w.p.p);
    controls(w);
    collision(w.p.p);
}

void update_part_obs (obstacle &obs) {

    addForce(obs.p);
    partupdatePV(obs.p);
    collision_obs(obs);
}
void init_0 (world &w)
{
    for (int i =0; i<MAX; i++) {
        w.T[i].p.P=0*w.T[i].p.P;
        w.T[i].p.V=0*w.T[i].p.V;
        w.T[i].p.F=0*w.T[i].p.F;
        w.T[i].p.m=0;
        w.T[i].lon=0;
        w.T[i].lar=0;

    }

}


void init_colects (world &w, int nb){
    w.collects=nb;
    for (int i=0; i<nb;i++){
        w.colec[i].p.x= rand()% 500 + 10;
        w.colec[i].p.y= rand()% 500 + 10;
        w.colec[i].type=rand()% 3;
    }
    w.spawned=true;
}



void init (world &w, int nb) {
    init_0(w);
    int i;
    w.nbobs=nb;
    w.nbobs=nb;
    w.p.p.P.x=250;
    w.p.p.P.y=250;
    w.p.p.V.x=0;
    w.p.p.V.y=0;
    w.p.p.F.x=0;
    w.p.p.F.y=0;
    w.p.p.m=5;
    w.p.rayon=5;
    w.round=1;
    w.p.nb_coin=0;
    w.p.nb_dash=0;
    w.p.nb_kill=0;
    w.spawned=false;
    w.p.im= image("data/bird.png");
    for (i=0; i<nb; i++) {
        w.T[i].lon=rand()% 30 +10;
        w.T[i].lar=rand()% 30 + 10;
        w.T[i].p.P.x=rand()% DIMW ;
        w.T[i].p.P.y=501;
        w.T[i].p.V.x=rand() % 60 -30;
        w.T[i].p.V.y=-1*(rand()% 30) -10;
        w.T[i].p.F.x=0;
        w.T[i].p.F.y=0;
        w.T[i].p.m=rand()% 10 + 1;


    }
    for(i=0;i<MAX_COL;i++){
        w.colec[i].type=-1;
        w.colec[i].p.x=510;
        w.colec[i].p.y=510;
    }

}

void init_new_round (world &w,int plus_v,int vx) {
    init_0(w);
    for (int i=0; i<w.nbobs; i++) {
        w.T[i].p.P.x=rand()% DIMW;
        w.T[i].p.P.y=501;
        w.T[i].p.V.x=-30 + rand() % 60 + vx;
        w.T[i].p.V.y= (-1)*(rand()% 30) -plus_v;
        w.T[i].p.F.x=0;
        w.T[i].p.F.y=0;
        w.T[i].p.m=rand()% 10 + 1;
        w.T[i].lon=rand()% 30 +10;
        w.T[i].lar=rand()% 30 + 10;


    }
}

void draw_player (player p) {
    color (255,255,0);
    circleFill(p.p.P.x,p.p.P.y,p.rayon);
    image_draw(p.im,p.p.P.x-5,p.p.P.y-5,15,15,0);

}

void draw_pickups (world w){
        for (int i =0; i<w.collects;i++){
        if (w.colec[i].type > -1){
        if (w.colec[i].type==0)
            color(255,255,0);
        if (w.colec[i].type==1)
            color(0,255,255);
        if (w.colec[i].type==2)
            color(255,255,255);
        circleFill(w.colec[i].p.x,w.colec[i].p.y,5);
                }
    }
}

void draw_interface (world w){
    fontSize(8);
    color (255,255,0);
    circleFill (15,50,7);
    color (0,0,0);
    print(12,45,w.p.nb_coin);
    color (0,255,255);
    circleFill (15,70,7);
    color (0,0,0);
    print(12,65,w.p.nb_dash);
    color(255,255,255);
    circleFill (15,90,7);
    color (0,0,0);
    print(12,85,w.p.nb_kill);

}


void draw (world w) {
    draw_player (w.p);
    for (int i = 0; i< w.nbobs; i++) {
        color(255,255,0);
        rectangleFill (w.T[i].p.P.x-int(w.T[i].lon/2),w.T[i].p.P.y-int (w.T[i].lar/2),
                       w.T[i].p.P.x+int(w.T[i].lon/2),w.T[i].p.P.y+int (w.T[i].lar/2));
    }
    fontSize (25);
    print (10,10,w.round);
    delay(25);
    draw_pickups(w);
    draw_interface(w);

}

bool new_round (world w) {
    int counter =0;
    for (int i=0; i<w.nbobs; i++) {
        if (w.T[i].p.P.y < 0)
            counter++;
    }
    return (counter == w.nbobs);
}

void round (world &w,int &plus_v,int &vx) {
    if (new_round (w)) {
        w.round ++;
        if (w.round% 5 ==0) {
            if (plus_v > 120) {
                plus_v+=20;
            } else {
                if (w.nbobs <500) {
                    w.nbobs += 7;
                }

            }
        }

        init_new_round(w,plus_v,vx);
    }

}

void colection (world &w){
      for (int i=0;i<4;i++){
        if ((w.p.p.P.x > w.colec[i].p.x - 10) && (w.p.p.P.x < w.colec[i].p.x + 10) &&
            (w.p.p.P.y > w.colec[i].p.y - 10) &&(w.p.p.P.y < w.colec[i].p.y + 10)) {
                if (w.colec[i].type==0){
                    w.p.nb_coin++;
                    w.colec[i].type=-1;
                    }
                if (w.colec[i].type==1){
                    w.p.nb_dash++;
                    w.colec[i].type=-1;
                }
                if (w.colec[i].type==2){
                    w.p.nb_kill++;
                    w.colec[i].type=-1;
                }
            }
      }
}

void update (world &w,int &plus_v,int &vx) {
    colection(w);
    update_part (w);
    for (int i=0; i<w.nbobs; i++) {
        update_part_obs(w.T[i]);

    }

    round (w,plus_v,vx);
    if (w.round%3 == 0 && !w.spawned){
        init_colects(w,4);
    } else {
        if (w.round%3 > 0)
            w.spawned =false;

    }
}

bool game_continue (world w) {
    bool test=true;
    for (int i=0; i<w.nbobs; i++) {
        if ((w.p.p.P.x >= (w.T[i].p.P.x -int(w.T[i].lon/2))) &&
                (w.p.p.P.x <= (w.T[i].p.P.x +int(w.T[i].lon/2))) &&
                (w.p.p.P.y >= (w.T[i].p.P.y -int(w.T[i].lar/2))) &&
                (w.p.p.P.y <= (w.T[i].p.P.y +int(w.T[i].lar/2))) ) {
            test = false;
        }
    }

    return test;
}

void over (world w) {
    draw (w);
    color(255,0,0);
    fontSize (24);
    print (175,250,"GAME OVER!");
    color(255,255,255);
    rectangleFill(210,210,290,240);
    color(0,0,0);
    fontSize(12);
    print (225,222,"try again ?");

    color(255,255,255);
    rectangleFill(210,180,290,200);
    color(0,0,0);
    fontSize(12);
    print (235,185,"Quit :(");

}
void new_game (bool &stop,bool &test) {
    int x,y;
    if (isMousePressed(SDL_BUTTON_LEFT)) {
        mousePos(x,y);
        if ((x >= 210) && (x <= 290) &&
                (y >= 210) && (y <= 290))
        {
            stop=false;
            test= true;
        }

        if ((x >= 210) && (x <= 290) &&
                (y >= 180) && (y <= 200))
        {
            winQuit();
        }

    }

}

void Main_menu () {
    color(255,255,0);
    fontSize (24);
    print (175,250,"Evitez Obstacle");
    color(255,255,255);
    rectangleFill(210,210,290,240);
    color(0,0,0);
    fontSize(12);
    print (219,222,"Start Game");

    color(255,255,255);
    rectangleFill(210,180,290,200);
    color(0,0,0);
    fontSize(12);
    print (220,185,"Quit (o_O)?");

    color(255,255,255);
    rectangleFill(210,150,290,170);
    color(0,0,0);
    fontSize(12);
    print (219,155,"User Manual");


}

void start_game (bool &start, bool &manual) {
    int x,y;;
    if (isMousePressed(SDL_BUTTON_LEFT)) {
        mousePos(x,y);
        if ((x >= 210) && (x <= 290) &&
                (y >= 210) && (y <= 240))
        {
            start=true;
        }

        if ((x >= 210) && (x <= 290) &&
                (y >= 180) && (y <= 200))
        {
            winQuit();
        }
        if ((x >= 210) && (x <= 290) &&
                (y >= 150) && (y <= 170))
        {
            manual = true;
        }

    }

}

void user_manual (bool &manual,Image player,Image background){
    int x,y;
    image_draw(background,0,0,500,500,0);
    color(255,255,0);
    circleFill(250,250,5);
    fontSize(12);
    image_draw(player,245,245,15,15,0);
    print(260,240,"<--joueur les fleches le deplacent");
    rectangleFill (230,400,260,420);
    print(270,405,"<-- obstacle a eviter");
    circleFill (160,190,7);
    print(170,180,"<-- coin a collecter");
    color(255,255,255);
    circleFill(190,320,7);
    color(255,255,0);
    print(200,310,"<--Kaboom, Power up qui pousse les obstacle");
    print (230,300,"'g' pour activer");
    color (0,0,255);
    circleFill(250,170,7);
    color  (255,255,0);
    print(260,160,"<-- Dash, power up qui lance l'oiseau dans la");
    print (270,150,"dans la direction que l'oiseaux vole");
    print (270,140,"'f' pour activer");
    fontSize(24);
    print(10,10,"3");
    fontSize(12);
    print (25,15,"<-- numero de vague");
    fontSize(10);
    color (255,255,0);
    circleFill (15,50,7);
    color (0,0,0);
    print(12,45,"5");
    color (0,255,255);
    circleFill (15,70,7);
    color (0,0,0);
    print(12,65,"1");
    color(255,255,255);
    circleFill (15,90,7);
    color (0,0,0);
    print(12,85,"3");
    color(255,255,0);
    fontSize(12);
    print (25,40,"<-- nb de coin");
    print (25,60,"<-- nb de Dash");
    print (25,80,"<-- nb de Kaboom");



    color(255,255,255);
    rectangleFill(410,15,490,60);
    color(0,0,0);
    fontSize(24);
    print (430,25,"OK!");
    if(isMousePressed(SDL_BUTTON_LEFT)){
            mousePos(x,y);
        if ((x >= 410) && (x <= 490) &&
            (y >= 15) && (y <= 60))
            {
                manual = false;
            }
        }

}

void reset (world &w,bool again) {
    if(again)
    {
        init(w,10);
    }
}
int main(int, char ** )
{
    winInit("Particles", DIMW, DIMW);
    srand(time(NULL));
    bool stop=false;
    bool again=true;
    bool start =false;
    bool manual= false;
    world w;

    int nb=10;
    init (w,nb);
    int plus_v=10,vx;

    Image background= image("data/background.jpg");

    while( !stop )
    {
        winDisplay();
        winClear();
        image_draw(background,0,0,500,500,0);
        if (start) {
            if (again) {
                draw (w);
                update(w,plus_v,vx);
                again = game_continue(w);
            } else {
                over(w);
                new_game(stop, again);
                reset(w,again);

            }
        } else {
            Main_menu();
            start_game(start,manual);
            if (manual){
                user_manual(manual,w.p.im,background);
                }

        }
        stop = winDisplay();
    }
    winQuit();
    return 0;
}
