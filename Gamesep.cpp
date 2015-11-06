#include<simplecpp>
#include<conio.h>
#include"gamesep.h"
#define pi 3.14159265358979323846264338327950288419716939937510
#define r_2 1.41421356237309504880168872420969807856967187537694807317667973799
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_ESC 27

float radius = 10, width = 10, points = 0,x2,y2;
int y_lights=0;
int isLaunched=0;

pinball *p = NULL;
circularobs *c1 = NULL,*c2 = NULL;
bat *b1 = NULL;
batright *b2 = NULL;
launcher *launch = NULL;
triangle *t1= NULL, *t2 = NULL;

bool allchecks();



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

float dist(float x1, float y1, float x2, float y2)
{
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

vect::vect()
{x= 0; y = 0;}

vect::vect(float a, float b)
{x = a;
 y = b;
}

void vect::rotateby(float a)
{
 float mod = sqrt(x*x + y*y);
 float theta  = arctangent(y/x);                                 //plus due to different conventions of simplecpp
 x = mod*cosine(theta + a);
 y = mod*sine(theta + a);
}

vect vect::operator+(vect &b)
{
 vect out;
 out.x = x + b.x;
 out.y = y + b.y;
 return out;
}

vect vect::operator-(vect &b)
{
 vect out;
 out.x = x - b.x;
 out.y = y - b.y;
 return out;
}

float vect::operator*(vect &b)
{
 return(x*(b.x) + y*(b.y));
}

void vect::changemod(float mod)
{
 float temp = sqrt(x*x + y*y);
 if(temp == 0)cout<<"Division by zero";

 x = mod*x/temp;
 y = mod*y/temp;
}

float vect::returnmod()
{
 //cout<<sqrt(x*x + y*y);
 return sqrt(x*x + y*y);
}

void vect::print()
{cout<<x<<" "<<y<<endl;}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

pinball::pinball()
{
 ball = new Circle;                                         //Creation of ball
 *ball = Circle(280,450 , 10);
 dir = vect(0,-1);
 gravity = .1;
}


void pinball::bmove(float pixel = 3)
{
 ball->move(pixel*dir.x, pixel*(dir.y+gravity));
}

void pinball::collision(vect direction)                            //In case a collision occurs
{                                                        //Whis is float x = dirx not allowed???
 dir = direction;
 dir.changemod(1);                                       //Mod is canged to maintain speed
}

float pinball::givex()
{return ball->getX();}
float pinball::givey()
{return ball->getY();}

vect pinball::givedir()
{return dir;}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

circularobs::circularobs()
{ cenx = 0; ceny = 0;
  c = new Circle;
  *c = Circle(cenx, ceny, 0);
  c->setFill(1); c->setColor(COLOR(124, 252, 0));
  Circle color(cenx,ceny,0); color.imprint();

}

circularobs::circularobs(float x, float y, float r = 20)
{
 obsrad = r;
 c = new Circle;
 *c = Circle(x ,y, obsrad);
 c->setFill(1); c->setColor(COLOR(124, 252, 0));
 cenx = x; ceny = y;
 Circle color(cenx,ceny,r+2); color.imprint();
}

bool circularobs::iscollision()
{
// float x = p->givex(), y = p->givey();
 vect normal(p->givex() - cenx, p->givey() - ceny);

 if(dist(x2,y2,cenx,ceny) <= obsrad + radius && p->givedir()*normal <=0)
 {
  points += 100;
  return 1;}
 return 0;

}

void circularobs::collision()
{
  if(iscollision())
  {
   vect normal(x2 - cenx, y2 - ceny);
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
   c->setFill(1);
   c->setColor(COLOR(124, 252, 0));

  }

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bat::bat()
{r = new Rectangle;
 *r = Rectangle(100, 400, 80, 20);
 norm.x = 0; norm.y = -1;
 para.x = 1; para.y = 0;
 r->setFill(1); r->setColor(COLOR(151,255,255));
}

void bat::moveangle(float a)                     //To move the bat by a particular angle
{
 r->rotate(a*pi/180);
 norm.rotateby(a);
 para.rotateby(a);

 if(p->givey()<390)
 {p->bmove(2);}
 else
 {p->bmove(1);}                                //To maintain motion of ball
 allchecks();

 r->move(-40*(1-cosine(a)), 40*sine(a));
 r->move(.8741,0);
}

void bat::movepad()                      //To move the bat up and down (by 60 degrees)
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

bool bat::batborder()
{

 if(p->givex() <60 || p->givex() >140)
 {return 0;}
 return 1;

}

bool bat::iscollision()                      //To check for collision of the ball with bat
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

void bat::collision()
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


batright::batright()
{r = new Rectangle;
 *r = Rectangle(200, 400, 80, 20);
 norm.x = 0; norm.y = -1;
 para.x = 1; para.y = 0;
 r->setFill(1); r->setColor(COLOR(151,255,255));
}

void batright::moveangle(float a)                     //To move the bat by a particular angle
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

void batright::movepad()                      //To move the bat up and down (by 60 degrees)
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

bool batright::batborder()
{

 if(p->givex() <=235 && p->givex() >=160)
 {return 1;}
 return 0;

}

bool batright::iscollision()                      //To check for collision of the ball with bat
{
 vect temp( p->givex()-r->getX(), p->givey()-r->getY());
 float dist = abs(temp*norm);
 //cout<<dist()<<" ";
 if(dist <= radius + width && p->givedir()*norm <0 && batborder())
 { return 1; }

 return 0;
}

void batright::collision()
{
 if(iscollision())
 {//cout<<1;
   vect tpara = para, tnorm = norm;

   tpara.changemod((p->givedir()*para)); tnorm.changemod(-1*(p->givedir()*norm));            //See collisions in physics

   vect tempdir = tpara + tnorm;
   tempdir.y = -abs(tempdir.y);
   p->collision(tempdir);

 }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    void launcher::draw_lights(float x,float y,int i)
    {
        r[i].reset(x,y,Llength,Lbreadth);
        r[i].rotate(Langle);
        r[i].setColor(COLOR("black"));
        r[i].setFill(1);
    }

    void launcher::changelauncher(int i)
    {
        r[i].setColor(COLOR("yellow"));
        r[i].setFill(1);
    }


    launcher::launcher()
    {
        height=0;width=0;n_lights=0;cx=0;cy=0;
        Llength=0;Lbreadth=0;Langle=0;
    }


    launcher::launcher(float w,float h,float n, float x, float y,float LL,float LB,float LA)
    {
        height=h;width=w;n_lights=n;cx=x;cy=y;
        Llength=LL;Lbreadth=LB;Langle=LA;
        isLaunched=0;
    }

    void launcher::draw_launcher()
    {
        rr.reset(cx,cy,width,height);
//        rr.imprint();
        for(int i=0;i<n_lights;i++)
            draw_lights(cx,cy-(i+1)*50+200,i);
    }

    void launcher::collision()
    {
        vect d=p->givedir(),temp(-d.x,d.y);

        if(y_lights==n_lights)
        {
            vect temp(-1,-1);
            p->collision(temp);
            isLaunched=2;
            draw_launcher();
            y_lights=0;
 //           cout<<0<<endl;
        }

        else if(isLaunched==1&&x2>(cx-width/2-radius))
        {
            vect temp(-p->givedir().x,p->givedir().y);
            p->collision(temp);
              p->bmove();
//           cout<<1<<endl;
//            temp.print();
 //           cout<<endl<<x2<<" "<<cx-width/2-radius<<endl;
        }

        else if(y2<cy-(y_lights+1)*50+200&&isLaunched==0)
            {
                r[y_lights].setColor(COLOR("yellow"));
                r[y_lights].setFill(1);
//                cout<<y2<<" "<<cy-(y_lights+1)*50+200<<endl;
                y_lights++;
//                cout<<2<<endl;
            }
        else if(x2<cx-width/2-radius&&isLaunched==2&&p->givedir().x>0)
        { isLaunched=1;
  //          cout<<3<<endl;
        }


    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    void triangle::draw()
    {
        Line *l1;
        l1=new Line;
        *l1=Line(x,y,x,y-height);
        l1->imprint();
        *l1=Line(x,y,x+base*cosine(angle-90),y+base*sine(angle-90));
        l1->imprint();
        drawhyp();
    }

    void triangle::drawhyp()
    {
        hyp1=new Line;
        *hyp1 =Line(x,y-height,x+base*cosine(angle-90),y+base*sine(angle-90));
        hyp2=new Line;
        *hyp2 =Line(x,y-height,x+base*cosine(angle-90),y+base*sine(angle-90));
    }

    void triangle::changehyp()
    {
        *hyp1=Line(x,y-height,x+s2*height/2,y-height/2);
        *hyp2=Line(x+base*cosine(angle-90),y+base*sine(angle-90),x+s2*height/2,y-height/2);
        delete hyp1; delete hyp2;
        drawhyp();
    }

    bool triangle::bet(float x, float a, float b)
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

    float triangle::line1(float x2,float y2)
    {    return s2*(x2-x);}
    float triangle::line2(float x2,float y2)
    {    return (r_2*(y2-y)-s2*(x2-x));}
    float triangle::line3(float x2,float y2)
    {    return ((y2-y+height)-s2*(1+r_2)*(x2-x));}

     bool triangle::linecol(float A,float B, float C,float h1,float h2,int s2,float b1=0,float b2=0,int s1=0,int s3=1)
    {
        vect n1(s1*A,B);
//        float x2,y2,d;
        float d=p->givedir()*n1;
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

    void triangle::detregion(float x2,float y2)
    {
        int s1=s2;
//        cout<<"Ball "<<s2;
        if(line1(x2,y2)<=0)
        {    linecol(-1,0,x,y-height-radius,y+radius,s2,x,x-s2*radius,s2,0);}
        else if(line1(x2,y2)>0&&line3(x2,y2)>=0)
//      {cout<<"2 ";    linecol(-s2*base*cosine(angle),-base*sine(angle),y*base*sine(angle)-x*s2*(-base*cosine(angle)),-1,s2);}
        {    linecol(-s2*(1+r_2),1,-y+x*s2*(1+r_2),y+radius/r_2,y+(radius+height)/r_2,s2,x-s2*radius/r_2,x+s2*(-radius+height)/r_2,s1);}
        else if(line3(x2,y2)<0&&line1(x2,y2)>0)
 //     {cout<<"3 ";    linecol(s2*(height-base*cosine(angle)),-base*sine(angle),(y-height)*base*sine(angle)-s2*x*(height-base*cosine(angle)),1,s2);}
        {    if(linecol(s2*(1+r_2),-1,y-height-x*s2*(1+r_2),y-height-0.382*radius,y+0.382*radius+height/r_2,s2,x+s2*0.924*radius,x+s2*(0.924*radius+height/r_2),1)==1)
             {
                changehyp();
                points+=200;
             }
        }

    }
    void triangle::collision(vect para,vect norm)
{
   vect tpara = para, tnorm = norm;
    //cout<<para.x<<" "<<para.y;
   tpara.changemod((p->givedir()*para)); tnorm.changemod(-1*(p->givedir()*norm));            //See collisions in physics
   //cout<<1;
   p->collision(tpara + tnorm);
   //cout<<norm.x<<" "<<norm.y<<endl;
   //cout<<p->givedir().x<<" "<<p->givedir().y<<" "<<-1*(p->givedir()*norm);
 }


    triangle::triangle()
    {x=y=height=base=angle=0;}

    triangle::triangle(float x1,float y1,float height1,float base1,float angle1)
    {
        x=x1;y=y1;height=height1,base=base1;angle=angle1;s2=angle/abs(angle);
        draw();
    }


    void triangle::collision()
    {
//        collisionheight();
        detregion(x2,y2);
    }



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void makecourse()
{
 int x1 = 100,y1 = 200, x2 = 200, y2 = 200;
 int xt1 = 80, yt1 = 300, s1 = 60;
 int xt2 = 200, yt2 = 300, s2 = 60;
 char ch = 'n';
 cout<<"Do you want to make your own course?(y,n) ";cin>>ch;

 if(ch=='y')
 {
   cout<<"CONGRATULATIONS: YOU GET TO MAKE YOUR OWN GAME"<<endl;
   cout<<"Enter the x,y coordinates of 1st circular obstacle: "; cin>>x1>>y1;
   cout<<"Enter the x,y coordinates of 2nd circular obstacle: "; cin>>x2>>y2;
   cout<<"Enter the x,y, sidelength coordinates of 1st Triangle: "; cin>>xt1>>yt1>>s1;
   cout<<"Enter the x,y, sidelength coordinates of 1st Triangle: "; cin>>xt2>>yt2>>s2;
 }

 c1 = new circularobs;                      // To make the circular obsstacle
 *c1 = circularobs(x1, y1);                        //100,200
 c2 = new circularobs;
 *c2 = circularobs(x2, y2);                        //200,200
 t1=new triangle;
 *t1=triangle(xt1,yt1,s1,s1,120);                  //80,300,60,60,120
 t2=new triangle;
 *t2=triangle(xt2,yt2,s2,s2,-120);                //200,300,60,60,-120
 launch=new launcher;
 *launch=launcher(20,400,5,280,250,10,30,-30);
 launch->draw_launcher();
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
    launch->collision();

  if(p->givey()>=480)
  {Text t1(150, 100, "GAME OVER");
   t1.imprint();                                   //The GAME OVER condition
   return 0;
  }
  return 1;
}

void showdisplay()
{

Text t1(150, 150, "WELCOME");
Text t2(150, 200, "INSTRUCTIONS");
Text t3(150, 250, "PLAY GAME");
Text t4(150, 300, "CREDITS");
Text t5(150,150, "hmm"); t5.hide();
Text t6(150,150, "hmm"); t6.hide();
Text t7(150,150, "hmm"); t7.hide();
Text t8(150,150, "hmm"); t8.hide();
Text t9(150,150, "hmm"); t9.hide();


int temp = getClick();

t1.hide(); t2.hide(); t3.hide(); t4.hide();

float x = temp/65536, y = temp%65536;

//cout<<x<<" "<<y;


// 115 185
// 145 155

if( x<=270 && x>=30 && y>=195 && y<=205 )
{
    t8.reset(150, 320, "Click Anywhere To Play");
    t1.reset(150, 150, "Controls");
    t2.reset(150, 170, "<-: Move left bat");
    t7.reset(150, 190, "->: Move right bat");
    t3.reset(150, 210, "'p' - Pause/Continue Game");
    t6.reset(150, 230, "Points will be given for each collision");
    t4.reset(150, 250, "All collisions follow newton's laws of physics");
    t5.reset(150, 270, "The game ends when ball goes under the bat");
    t9.reset(150, 290, "Terminal to choose to make your own course");
    getClick();
}

else if(  x<=185 && x>=115 && y>=295 && y<=305 )
{
    t5.reset(150, 320, "Click Anywhere To Play");
    t1.reset(150, 230, "SPECIAL THANKS TO ADITYA KUMAR AKASH :) ");
    t2.reset(150, 250, "CREATED BY: ");
    t3.reset(150, 270, "Samir Wadhwa (150100024)");
    t4.reset(150, 290, "Anish Ram Senati (150100023)");
    getClick();
}



}

void work()
{




 showdisplay();
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


  Text point(150,20,points);
  Text point2(50,20,"Points: ");

  if(checkEvent(e))
  {
    if(keyPressEvent(e))
    { char ch = charFromEvent(e);
      if(ch == KEY_LEFT)
      {b1->movepad();}
      if(ch == KEY_RIGHT)
      {b2->movepad();}
      if(ch == 'p')
      { Text t(150,100, "PAUSE");
        getch();

      }
      else if(ch == KEY_ESC)
      {break;}
    }

  }

  p->bmove();
  x2=p->givex();
  y2=p->givey();

  if(allchecks() == 0)
  {
   point.imprint();
   point2.imprint();
   break;}
  b1->collision();
  endchecks();
 }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


main_program{

XEvent e;



initCanvas("Pinball", 300, 500);

//Rectangle Back(150,250,300,500); Back.setFill(1); Back.setColor(COLOR(238, 224	,229));

work();



nextEvent(e);

delete p; delete b1; delete c1; delete c2;

//getClick();
}

