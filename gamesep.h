#include<simplecpp>


//pinball *p, circularobs *c1,*c2; triangle *t1, *t2, launcher *launch bat *b1, batright *b2;


struct vect
{
float x, y;

vect();

vect(float a, float b);

void rotateby(float a);

vect operator+(vect &b);

vect operator-(vect &b);

float operator*(vect &b);

void changemod(float mod);

float returnmod();

void print();

};


class pinball
{
Circle *ball;
vect dir;
float gravity;                                         //to determine direction of motion of ball

public:

pinball();

void bmove(float pixel);

void collision(vect direction);                            //In case a collision occurs

float givex();

float givey();

vect givedir();

};


class circularobs
{
Circle *c;
float obsrad;                                            //Radius of circle
float cenx, ceny;                                        //coordinates of center

public:

circularobs();

circularobs(float x, float y, float r);

bool iscollision();

void collision();

};


class bat
{
Rectangle *r;
vect norm, para;                            //Parallel direction to bat, normal direction to bat

public:

bat();

void moveangle(float a);                     //To move the bat by a particular angle


void movepad();                      //To move the bat up and down (by 60 degrees)


bool batborder();

bool iscollision();                      //To check for collision of the ball with bat

void collision();


};

class batright
{
Rectangle *r;
vect norm, para;                            //Parallel direction to bat, normal direction to bat

public:

batright();

void moveangle(float a);                     //To move the bat by a particular angle

void movepad();                      //To move the bat up and down (by 60 degrees)


bool batborder();

bool iscollision();                      //To check for collision of the ball with bat


void collision();

};

class launcher
{
    float height,width,cx,cy,Llength,Lbreadth,Langle;
    int n_lights;
    Rectangle r[10],rr;


    void draw_lights(float x,float y,int i);


    void changelauncher(int i);

    public:
    launcher();

    launcher(float w,float h,float n, float x, float y,float LL,float LB,float LA);

    void draw_launcher();

    void collision();

};

class triangle
{
    Line* hyp1=NULL;
    Line* hyp2=NULL;
    float x,y,height,base,angle;
    int s2;

    void draw();

    void drawhyp();

    void changehyp();

    bool bet(float x, float a, float b);

    float line1(float x2,float y2);

    float line2(float x2,float y2);

    float line3(float x2,float y2);

    void detregion(float x2,float y2);

    void collision(vect para,vect norm);

    public:

    triangle();

    triangle(float x1,float y1,float height1,float base1,float angle1);

    bool linecol(float A,float B, float C,float h1,float h2,int s2,float b1,float b2,int s1,int s3);

    void collision();


};

