#include<simplecpp>
#include<conio.h>
#define pi 3.14159265
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_ESC 27

float radius = 10, width = 10, points = 0;

bool allchecks();

float absol(float a)
{
 if(a>=0)
 {return a;}
 else
 {return -a;}

}

float dist(float x1, float y1, float x2, float y2)
{
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct vect
{
float x, y;

vect()
{x= 0; y = 0;}

vect(float a, float b)
{x = a;
 y = b;
}

void rotateby(float a)
{
 float mod = sqrt(x*x + y*y);
 float theta  = arctangent(y/x);                                 //plus due to different conventions of simplecpp
 x = mod*cosine(theta + a);
 y = mod*sine(theta + a);
}

vect operator+(vect &b)
{
 vect out;
 out.x = x + b.x;
 out.y = y + b.y;
 return out;
}

vect operator-(vect &b)
{
 vect out;
 out.x = x - b.x;
 out.y = y - b.y;
 return out;
}

float operator*(vect &b)
{
 return(x*(b.x) + y*(b.y));
}

void changemod(float mod)
{
 float temp = sqrt(x*x + y*y);
 if(temp == 0)cout<<"Division by zero";

 x = mod*x/temp;
 y = mod*y/temp;
}

float returnmod()
{
 //cout<<sqrt(x*x + y*y);
 return sqrt(x*x + y*y);
}

void print()
{cout<<x<<" "<<y<<endl;}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class pinball
{
Circle *ball;
vect dir;                                           //to determine direction of motion of ball

public:

pinball()
{
 ball = new Circle;                                         //Creation of ball
 *ball = Circle(80,105 , 10);
 dir = vect(0,1);
}

void bmove(float pixel = 2)
{
 ball->move(pixel*dir.x, pixel*dir.y);
}

void collision(vect direction)                            //In case a collision occurs
{                                                        //Whis is float x = dirx not allowed???
 dir = direction;
 dir.changemod(1);                                       //Mod is canged to maintain speed
}

float givex()
{return ball->getX();}
float givey()
{return ball->getY();}

vect givedir()
{return dir;}

}*p;



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class circularobs
{
Circle *c;
float obsrad;                                            //Radius of circle
float cenx, ceny;                                        //coordinates of center

public:

circularobs()
{ cenx = 0; ceny = 0;
  c = new Circle;
  *c = Circle(cenx, ceny, 0);
}

circularobs(float x, float y, float r = 20)
{
 obsrad = r;
 c = new Circle;
 *c = Circle(x ,y, obsrad);
 cenx = x; ceny = y;
}

bool iscollision()
{
 float x = p->givex(), y = p->givey();
 vect normal(p->givex() - cenx, p->givey() - ceny);

 if(dist(x,y,cenx,ceny) <= obsrad + radius && p->givedir()*normal <=0)
 {
  points += 100;
  return 1;}
 return 0;

}

void collision()
{
  if(iscollision())
  {
   vect normal(p->givex() - cenx, p->givey() - ceny);
   normal.changemod(1);
   vect temp = p->givedir();
   temp.changemod(-1);

   float angle = temp*normal/temp.returnmod();                        //see logic for collisionss
   //cout<<-2*arccosine(angle)<<" "<<angle<<endl;
   temp.rotateby(180-2*arccosine(angle));
   p->collision(temp);

   c->setFill(1);
   c->setScale(.5);
   c->setColor(COLOR("red"));
   c->setScale(1);                                                 //To animate
   c->setFill(0);
   c->setColor(COLOR("black"));

  }

}

}*c1 = NULL, *c2 = NULL;


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void endchecks()
{
 vect temp = p->givedir();
 if( (p->givex() <=0 + radius && temp.x<0)  || (p->givex() >=300 - radius &&temp.x > 0) )
 {
   vect temp = p->givedir();                              //collision with vertical edges
   temp.x *= -1;
   //cout<<temp.x;
   p->collision(temp);
 }
 else if( (p->givey() <= 0 + radius && temp.y < 0)  || (p->givey() >=500 - radius &&temp.y>0) )
 {                                                       //collision with horizontal edges
   vect temp = p->givedir();
   temp.y *= -1;
   p->collision(temp);
 }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class bat
{
Rectangle *r;
vect norm, para;                            //Parallel direction to bat, normal direction to bat

public:

bat()
{r = new Rectangle;
 *r = Rectangle(100, 400, 80, 20);
 norm.x = 0; norm.y = -1;
 para.x = 1; para.y = 0;
}

void moveangle(float a)                     //To move the bat by a particular angle
{
 r->rotate(a*pi/180);
 norm.rotateby(a);
 para.rotateby(a);

 p->bmove(2);                                //To maintain motion of ball
 allchecks();

 r->move(-40*(1-cosine(a)), 40*sine(a));
 r->move(.8741,0);
}

void movepad()                      //To move the bat up and down (by 60 degrees)
{
  repeat(5)
  {
    moveangle(-12);
  }

  repeat(5)
  {
   moveangle(12);
  }

}

bool batborder()
{

 if(p->givex() <60 || p->givex() >140)
 {return 0;}
 return 1;

}

bool iscollision()                      //To check for collision of the ball with bat
{
 vect temp( p->givex()-r->getX(), p->givey()-r->getY());

 float dist = absol(temp*norm);
 //cout<<dist<<" ";
 if(dist <= radius + width && p->givedir()*norm <0 && batborder())
 {
  //cout<<p->givedir().x<<" "<<p->givedir().y<<endl;
  //cout<<norm.x<<" "<<norm.y<<endl;
  return 1;
 }
 return 0;
}

void collision()
{
 if(iscollision())
 {
   vect tpara = para, tnorm = norm;
    //cout<<para.x<<" "<<para.y;
   tpara.changemod((p->givedir()*para)); tnorm.changemod(-1*(p->givedir()*norm));            //See collisions in physics
   //cout<<1;
   p->collision(tpara + tnorm);
   //cout<<norm.x<<" "<<norm.y<<endl;
   //cout<<p->givedir().x<<" "<<p->givedir().y<<" "<<-1*(p->givedir()*norm);
 }

}

}*b1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class batright
{
Rectangle *r;
vect norm, para;                            //Parallel direction to bat, normal direction to bat

public:

batright()
{r = new Rectangle;
 *r = Rectangle(200, 400, 80, 20);
 norm.x = 0; norm.y = -1;
 para.x = 1; para.y = 0;
}

void moveangle(float a)                     //To move the bat by a particular angle
{
 r->rotate(-a*pi/180);
 norm.rotateby(-a);
 para.rotateby(-a);

 //para.print();

 p->bmove(2);                                //To maintain motion of ball
 allchecks();

 r->move(-40*(1-cosine(-a)), 40*sine(a));
 r->move(.8741,0);
}

void movepad()                      //To move the bat up and down (by 60 degrees)
{ //cout<<1;
  repeat(5)
  {
    moveangle(-12);
  }

  repeat(5)
  {
   moveangle(12);
  }

}

bool batborder()
{

 if(p->givex() <=235 && p->givex() >=160)
 {return 1;}
 return 0;

}

bool iscollision()                      //To check for collision of the ball with bat
{
 vect temp( p->givex()-r->getX(), p->givey()-r->getY());
 float dist = absol(temp*norm);
 //cout<<dist()<<" ";
 if(dist <= radius + width && p->givedir()*norm <0 && batborder())
 { return 1; }

 return 0;
}

void collision()
{
 if(iscollision())
 {//cout<<1;
   vect tpara = para, tnorm = norm;

   tpara.changemod((p->givedir()*para)); tnorm.changemod(-1*(p->givedir()*norm));            //See collisions in physics

   p->collision(tpara + tnorm);

 }

}

}*b2;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void makecourse()
{
 c1 = new circularobs;                      // To make the circular obsstacle
 *c1 = circularobs(100, 200);                        //
 c2 = new circularobs;
 *c2 = circularobs(200, 200);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool allchecks()
{
  b1->collision();
  b2->collision();
  endchecks();                                   //Checks for all types of collisions
  c1->collision();
  c2->collision();

  if(p->givey()>=480)
  {Text t1(150, 250, "GAME OVER");
   t1.imprint();                                   //The GAME OVER condition
   return 0;
  }
  return 1;
}

void work()
{
 makecourse();
 XEvent e;
 b1 = new bat;
 b2 = new batright;
 p = new pinball;
 b1->moveangle(30);
 b2->moveangle(30);
 //XEvent event;                  //Event class in simplecpp

 while(1)
 {
  //wait(.01);

  Text point(40,20,points);

  if(checkEvent(e))
  {
    if(keyPressEvent(e))
    { char ch = charFromEvent(e);
      if(ch == KEY_LEFT)
      {b1->movepad();}
      if(ch == KEY_RIGHT)
      {b2->movepad();}
      if(ch == 'p')
      { Text t(150,250, "PAUSE");
        getch();

      }
      else if(ch == KEY_ESC)
      {break;}
    }

  }

  p->bmove();

  if(allchecks() == 0)
  {break;}
  b1->collision();
  endchecks();
 }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void welcome()
{
initCanvas("Welcome!!", 500, 500);



Rectangle background(250, 250, 500, 500);

background.setColor(COLOR(171,130,255));

Text(250,100, "Welcome To THE PINBALL GAME!!!");
Text(250, 200, "INSTRUCTIONS");
Text(250, 300, "CREDITS");
Text(250, 300, "PLAY GAME");

wait(5);
closeCanvas();

}

main_program{

XEvent e;

/*initCanvas("Welcome!!", 500, 500);



Rectangle background(250, 250, 500, 500);

background.setColor(COLOR(171,130,255));

Text(250,100, "Welcome To THE PINBALL GAME!!!");
Text(250, 200, "INSTRUCTIONS");
Text(250, 300, "CREDITS");
Text(250, 300, "PLAY GAME");

wait(5);
closeCanvas();

//event1
initCanvas("INSTRUCTIONS", 500, 500);
Text(250,250, "Controls\n<-: Move left bat\n->: Move right bat\n'p' - Pause/Continue Game \nPoints will be given for each collision(Shown on top left corner of game \nAll collisions follow newton's laws of physics \nThe game ends when the ball goes under the bat");


wait(5); closeCanvas();
//*/

initCanvas("Pinball", 300, 500);
work();



nextEvent(e);

delete p; delete b1; delete c1; delete c2;

getClick();
}
