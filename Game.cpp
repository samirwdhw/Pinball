#include<simplecpp>
#include<conio.h>
#define pi 3.14159265358979323846264338327950288419716939937510
#define r_2 1.41421356237309504880168872420969807856967187537694807317667973799
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_ESC 27

float radius = 10, width = 10, points = 0;

bool allchecks();

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

class triangle
{
    Line* hyp1=NULL;
    Line* hyp2=NULL;
    float x,y,height,base,angle;
    int s2;

    void draw()
    {
        Line *l1;
        l1=new Line;
        *l1=Line(x,y,x,y-height);
        l1->imprint();
        *l1=Line(x,y,x+base*cosine(angle-90),y+base*sine(angle-90));
        l1->imprint();
        drawhyp();
    }

    void drawhyp()
    {
        hyp1=new Line;
        *hyp1 =Line(x,y-height,x+base*cosine(angle-90),y+base*sine(angle-90));
        hyp2=new Line;
        *hyp2 =Line(x,y-height,x+base*cosine(angle-90),y+base*sine(angle-90));
    }

    void changehyp()
    {
        *hyp1=Line(x,y-height,x+height/2,y-height/2);
        *hyp2=Line(x+base*cosine(angle-90),y+base*sine(angle-90),x+height/2,y-height/2);
        delete hyp1; delete hyp2;
        drawhyp();
    }

    bool bet(float x, float a, float b)
    {
 //                   cout<<x<<" "<<a<<" "<<b<<endl;
        if(x>=a&&x<=b||x>=b&&x<=a)
        {

            return 1;
        }
        else
        {
 //           cout<<x<<" "<<a<<" "<<b<<" 0"<<endl;
            return 0;
        }
    }

    float line1(float x2,float y2)
    {    return s2*(x2-x);}
    float line2(float x2,float y2)
    {    return (r_2*(y2-y)-s2*(x2-x));}
    float line3(float x2,float y2)
    {    return ((y2-y+height)-s2*(1+r_2)*(x2-x));}
    void detregion(float x2,float y2)
    {
        int s1=s2;
//        cout<<"Ball "<<s2;
        if(line1(x2,y2)<=0)
        {    linecol(-1,0,x,y-height-radius,y+radius,s2,x,x-s2*radius,s2,0);}
        else if(line1(x2,y2)>0&&line3(x2,y2)>=0)
//      {cout<<"2 ";    linecol(-s2*base*cosine(angle),-base*sine(angle),y*base*sine(angle)-x*s2*(-base*cosine(angle)),-1,s2);}
        {    linecol(-s2*(1+r_2),1,-y+x*s2*(1+r_2),y-radius/r_2,y+(radius+height)/r_2,s2,x-s2*radius/r_2,x+s2*(radius+height)/r_2,s1);}
        else if(line3(x2,y2)<0&&line1(x2,y2)>0)
 //     {cout<<"3 ";    linecol(s2*(height-base*cosine(angle)),-base*sine(angle),(y-height)*base*sine(angle)-s2*x*(height-base*cosine(angle)),1,s2);}
        {    if(linecol(s2*(1+r_2),-1,y-height-x*s2*(1+r_2),y-height-0.382*radius,y+0.382*radius+height/r_2,s2,x+s2*0.924*radius,x+s2*(0.924*radius+height/r_2),1)==1)
                changehyp();}

    }
    void collision(vect para,vect norm)
{
   vect tpara = para, tnorm = norm;
    //cout<<para.x<<" "<<para.y;
   tpara.changemod((p->givedir()*para)); tnorm.changemod(-1*(p->givedir()*norm));            //See collisions in physics
   //cout<<1;
   p->collision(tpara + tnorm);
   //cout<<norm.x<<" "<<norm.y<<endl;
   //cout<<p->givedir().x<<" "<<p->givedir().y<<" "<<-1*(p->givedir()*norm);
 }

    public:

    triangle()
    {x=y=height=base=angle=0;}

    triangle(float x1,float y1,float height1,float base1,float angle1)
    {
        x=x1;y=y1;height=height1,base=base1;angle=angle1;s2=angle/abs(angle);
        draw();
    }

    bool linecol(float A,float B, float C,float h1,float h2,int s2,float b1=0,float b2=0,int s1=0,int s3=1)
    {
        vect n1(s1*A,B);
//        float x2,y2,d;
        float x2=p->givex(),y2=p->givey(),d=p->givedir()*n1;
        float D=abs((A*x2+B*y2+C)/sqrt(A*A+B*B));
//        cout<<D<<" "<<d<<endl;
        if(D<=radius&&bet(y2,h1,h2)&&d<0&&(bet(x2,b1,b2)||(s3==0&&bet(x2,b1,b2))))
        {
//            cout<<"COLLISION"<<endl;
            vect p1(-B,A);
            n1.changemod(1);p1.changemod(1);
            collision(p1,n1);
            return 1;
        }
        return 0;
    }
    void collision()
    {
//        collisionheight();
        float x2=p->givex(),y2=p->givey();
        detregion(x2,y2);
    }


}*t1,*t2;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

 float dist = abs(temp*norm);
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
 float dist = abs(temp*norm);
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
 t1=new triangle;
 *t1=triangle(80,300,60,60,120);
 t2=new triangle;
 *t2=triangle(200,300,60,60,-120);
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
  t1->collision();
  t2->collision();

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
