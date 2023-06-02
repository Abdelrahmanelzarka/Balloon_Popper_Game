#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include<stdio.h>
#include<bits/stdc++.h>



using namespace std;

int phyWidth = 700;
char buffer[50]; // for writing time
char buffer2[50]; // for writing score
int phyHeight = 700;
int logWidth = 200;
int logHeight = 200;
int centerX = logWidth/2;
int centerY = logHeight/2;
int radius = 5;
int score = 0;
int delta = 0; //for balloon animation
int alphaX = 0; //for basket animation

int tim = 419; // time as a counter
int time_since_burst[20] = {0};   // 20 millisecond timer



class Balloon {

float generate_color () {
    float index = (float) rand()/RAND_MAX + 0.5 ;
    return index;
}
float generate_ypos (){
    float y = (float) rand()*500/(RAND_MAX) * -1 ;
    return y ;
}

float generate_xpos (){
    float x = (float) rand() * 195/RAND_MAX + 0.5;
    return x ;
}


public:
    int Xpos ;//initial X position
    int Ypos;//initial Y position
    bool state ; //Dead or Alive
    bool direction; //Up or Down
    int color ; //Y:yellow R:red
    bool burst; // for clicked balls



    Balloon (){
        Ypos = generate_ypos();
        Xpos = generate_xpos();
        color = generate_color ();
        state = true;
        direction = true;
       burst=false;
    }

} ;


Balloon characters[20] ;


void create_characters (){
    srand(time (nullptr));

    for (int i =0 ; i<20 ; i++){
            characters [i] = Balloon() ;
    }
}


void drawCircle(float cx, float cy, float r, int num_of_seg){
    glBegin(GL_POLYGON);
    for(int i=0;i<num_of_seg;i++){
        float theta= 2.0f*3.1415926f*float(i)/float(num_of_seg); // get current angle in radians
        float x= r * cosf(theta);
        float y= r * sinf(theta);
        glVertex2f(x+cx, y+cy);
    }
    glEnd();

}



void init2D (float r , float g , float b){
    //determines the color of background takes a normalized rgb value
    glClearColor(r,g,b,1);
    //GL_PROJECTION di m3naha enk shaghal 2d
    glMatrixMode(GL_PROJECTION);
    //Divides the window into an orthogonal plane(left , right , bottom , top)
    gluOrtho2D(0.0,logWidth,0.0,logHeight);
}


//timer of the game
void Timer (int v){
    glutTimerFunc(60 , Timer , 0);
    glutPostRedisplay();
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        // check for the position of mouse click within any ball or not
        for (int i=0 ; i < 20 ; i++){
                if(1.0*x*logWidth*1.0/phyWidth <= characters[i].Xpos +5 && characters[i].direction && characters[i].state==true && 1.0*x*logWidth*1.0/phyWidth>= characters[i].Xpos -5 && logHeight-1.0*y*logHeight*1.0/phyWidth>= characters[i].Ypos + delta-5 &&  logHeight-1.0*y*logHeight*1.0/phyWidth<= characters[i].Ypos + delta +5)
                {
               characters[i].state=false;
            if (characters[i].color == 1 ) {  // Red, going up
                score += 1;
                characters[i].burst = true;

            }
            else if (characters[i].color != 1 ) { // Yellow, going up
                score += 2;
                characters[i].burst = true;

            }
                }

        }
    }
}



// animate the basket with delta variable
void Movebasket (int key, int x, int y)
{
    if(key== GLUT_KEY_LEFT && alphaX-5>-10 )
    alphaX-=5;
    if(key== GLUT_KEY_RIGHT && alphaX+5<160)
    alphaX+=5;



}

void printSome( int timee, int x, int y){ // print time on the screen

    glColor3f (0, 0, 0);
    glRasterPos2d(x,y);
    sprintf(buffer, "time is:%d",timee/20);
    for (int i=0;i<strlen(buffer);i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    glFlush();

}

void printSome2( int score, int x, int y){ // print score on the screen

    glColor3f (0, 0, 0);
    glRasterPos2d(x,y);
    sprintf(buffer2, "score is:%d",score);
    for (int i=0;i<strlen(buffer2);i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer2[i]);
    glFlush();

}


void display (void){

    //clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);




  for (int i=0 ; i < 20 ; i++){

        if (characters[i].Ypos + delta >= 200) {  // Bounce off top
            characters[i].direction = false;
        }

    }




// specify color for drawing with
    for (int i=0 ; i < 20 ; i++){
        if (characters[i].color == 1)
            glColor3f(1,0,0);
        else
            glColor3f(1,1,0);


// determine the next move for any ball depends on its direction
          if(characters[i].state)
            drawCircle(characters[i].Xpos , characters[i].direction ?characters[i].Ypos + delta : 400+abs(characters[i].Ypos) - delta, radius , 100);



           // check if the ball dropped in the basket or not
    if (alphaX+30 <= characters[i].Xpos +20 && alphaX+30 >= characters[i].Xpos -20 && characters[i].state==true && characters[i].direction==false && 400+abs(characters[i].Ypos) - delta >45 && 400+abs(characters[i].Ypos) - delta <60)
    {

        score+=1;
         characters[i].burst = true;

        characters[i].state=false;
    }
     }


    delta += 2;

    glColor3f(0 , 1, 0);



    glBegin(GL_QUADS);
        glVertex2d(0,0);
        glVertex2d(200,0);
        glVertex2d(200,50);
        glVertex2d(0,50);
    glEnd();



// the basket
    glColor3f(0.882812,0.527343,0.261718);
        glBegin(GL_QUADS);
        glVertex2d(5+alphaX,25);
        glVertex2d(45+alphaX,25);
        glVertex2d(45+alphaX,55);
        glVertex2d(5+alphaX,55);
    glEnd();


    if (tim >=0) {  // Count down timer

        printSome(tim ,10,5);
    }
    else
    {
        glColor3f (0, 0, 0);
    glRasterPos2d(85,150);



    // last screen
      char* str="Game Over!";


    for (int i=0;i<strlen(str);i++)
     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]);
    glFlush();
    printSome2(score ,90,135);

    }
    //start screen for 20 millisecond
      if (tim > 390)
      {
    glColor3f (0, 0, 0);
    glRasterPos2d(100,150);

      char* str="Start!";

    for (int i=0;i<strlen(str);i++)
     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]);
    glFlush();
      }

       printSome2(score ,10,10);

       tim--;



        for (int i=0; i<20; i++) {
        if (characters[i].burst) {
            // Draw burst animation centered at characters[i].Xpos, characters[i].Ypos
            // ...if (characters[i].color == 1)
        if (characters[i].color == 1)
            glColor3f(1,0,0);
         else
            glColor3f(1,1,0);


            for (int j=0; j<rand()%5 + 10; j++) {
                float theta = 2*3.14*j/10; // 0 to 2pi in 10 steps
                float x = radius * cos(theta);
                float y = radius * sin(theta);


                drawCircle(characters[i].Xpos + x, 400+abs(characters[i].Ypos) - delta + y, 1, 100);
                drawCircle(characters[i].Xpos + x, characters[i].Ypos + delta + y, 1, 100); // Small circle
            }



            // After 1 second, remove the balloon
            time_since_burst[i] += 1;
            if (time_since_burst[i] > 10) {
                characters[i].state = false;
                characters[i].burst = false;

            }
        }
    }


    glutSwapBuffers();

    //Process all routines
    glFlush();
}


// Q button to restart the game
void keyboard(unsigned char key, int x, int y) {
    if (key == 'q') {
        // Restart game
        score = 0;
        tim = 419;
        // Re-position balloons
        create_characters(); // create new set of balloons
         for (int i =0 ; i<20 ; i++){
        cout << characters[i].color << " "<< characters[i].direction<<" "<< characters[i].Ypos << " " << characters[i].Xpos <<endl;
    }
    delta=0;




    }
}


/* Program entry point */

int main(int argc, char *argv[])
{

    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

    glutInitWindowSize(phyWidth,phyHeight);

    glutInitWindowPosition(100,100);

    glutCreateWindow("Test 1");
    init2D(0.384313,0.77647,0.866666);

    create_characters();

    glutDisplayFunc(display);

    Timer (0);

    glutMouseFunc(mouseClick);

    glutSpecialFunc(Movebasket);

    glutKeyboardFunc(keyboard);

    glutMainLoop();

}
