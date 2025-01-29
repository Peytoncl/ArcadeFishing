#include <GL/glut.h>

#include <stdio.h>
#include "stdbool.h"
#include <stdlib.h> 
#include <math.h>

#include "windows.h"

#include "graphics.h"
#include "gameMath.h"

//Key input setup
#define KEY_COUNT 256 

bool keys[KEY_COUNT];

int* gameFont = GLUT_STROKE_ROMAN;

//Game values (non customizable)
float CENTER_X = (WINDOW_W / 2);
float CENTER_Y = (WINDOW_H / 2);

float xMeterDistance = WINDOW_W / 2 / 8;
float yMeterDistance = WINDOW_H / 2 / 1.2f;


float greenScreenY;

float greenHeightY = 25;

float fishScreenY;

float fishHeight = 6;


int greenDirection;

float greenSpeed = 0.5f;


int fishDirection;

float fishSpeed;

int fishMinMove;
int fishMaxMove;

int fishProximityDistance = 100;


int score = 0;

char playerScore[1000];


int playerLives = 5;

int livesPadding = 20;
int livesWidth = 30;

//Frame timer
unsigned long lastUpdate = 0;

//Ball speed
const unsigned long FRAME_TIME = 1; 


void SpawnFish()
{

  fishDirection = (rand() % 2) ? 1 : -1;

  fishSpeed = RandomFloat(0.05f, 0.5f);

  fishMinMove = RandomInt(CENTER_Y - yMeterDistance, CENTER_Y);

  fishMaxMove = RandomInt(CENTER_Y, CENTER_Y + yMeterDistance);

  //printf("Distance: %d\n", fishMaxMove - fishMinMove);

  if (fishMaxMove - fishMinMove <= fishProximityDistance) fishMaxMove += fishProximityDistance / 2;

  fishScreenY = (int)Lerp(fishMinMove, fishMaxMove, 0.5f); //spawn in the middle of the moving boundaries

  fishHeight = RandomInt(5, 9);

}

void StartGame()
{

  greenScreenY = WINDOW_H / 2;

  greenDirection = (rand() % 2) ? 1 : -1;

  playerLives = 5;

  SpawnFish();

  score = 0;

  sprintf(playerScore, "%d", score);

}

void init()
{
  glClearColor(0, 0, 0, 0);

  RandomInitialization();

  StartGame();

  gluOrtho2D(0, WINDOW_W, WINDOW_H, 0);
}

void display() 
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glColor3f(0.3f, 0.3f, 1);

  glBegin(GL_QUADS);

  glVertex2f(CENTER_X - xMeterDistance, CENTER_Y + yMeterDistance);
  glVertex2f(CENTER_X + xMeterDistance, CENTER_Y + yMeterDistance);
  glVertex2f(CENTER_X + xMeterDistance, CENTER_Y - yMeterDistance);
  glVertex2f(CENTER_X - xMeterDistance, CENTER_Y - yMeterDistance);


  glColor3f(0.3f, 1, 0.3f);

  glVertex2f(CENTER_X - xMeterDistance, greenScreenY + greenHeightY);
  glVertex2f(CENTER_X + xMeterDistance, greenScreenY + greenHeightY);
  glVertex2f(CENTER_X + xMeterDistance, greenScreenY - greenHeightY);
  glVertex2f(CENTER_X - xMeterDistance, greenScreenY - greenHeightY);


  glColor3f(0.9f, 0.9f, 0.7f);

  glVertex2f(CENTER_X - fishHeight, fishScreenY + fishHeight);
  glVertex2f(CENTER_X + fishHeight, fishScreenY + fishHeight);
  glVertex2f(CENTER_X + fishHeight, fishScreenY - fishHeight);
  glVertex2f(CENTER_X - fishHeight, fishScreenY - fishHeight);


  glColor3f(1, 0.3f, 0.3f);

  for (int live = 0; live < playerLives; live++)
  {
    glVertex2f(CENTER_X + xMeterDistance + livesPadding, CENTER_Y - yMeterDistance + (livesPadding * live + livesWidth * live));
    glVertex2f(CENTER_X + xMeterDistance + livesPadding + livesWidth, CENTER_Y - yMeterDistance + (livesPadding * live + livesWidth * live));
    glVertex2f(CENTER_X + xMeterDistance + livesPadding + livesWidth, CENTER_Y - yMeterDistance + livesWidth + (livesPadding * live + livesWidth * live));
    glVertex2f(CENTER_X + xMeterDistance + livesPadding, CENTER_Y - yMeterDistance + livesWidth + (livesPadding * live + livesWidth * live));
  }


  glEnd();


  glColor3f(1, 1, 1);

  glLineWidth(3.0f);

  glBegin(GL_LINE_LOOP);

  glVertex2f(CENTER_X - xMeterDistance, CENTER_Y + yMeterDistance);
  glVertex2f(CENTER_X + xMeterDistance, CENTER_Y + yMeterDistance);
  glVertex2f(CENTER_X + xMeterDistance, CENTER_Y - yMeterDistance);
  glVertex2f(CENTER_X - xMeterDistance, CENTER_Y - yMeterDistance);

  glEnd();

  drawText(0 + 30, WINDOW_H - 30, gameFont, playerScore, 1, 1, 1, 1.0f);

  glutSwapBuffers();

}

bool catchPressed = false;

bool caughtFish = false;

void keyDown(unsigned char key, int x, int y) 
{

  keys[key] = true; 

  if (key == ' ' && !catchPressed) 
  {

    catchPressed = true;

    if(fishScreenY + fishHeight > greenScreenY - greenHeightY && fishScreenY - fishHeight < greenScreenY + greenHeightY)
    {
      score += 1;

      sprintf(playerScore, "%d", score);

      caughtFish = true;

      SpawnFish();
    } 
    else 
    {
      playerLives -= 1;

      if(playerLives <= 0) StartGame();
    }

  } 
  
}

void keyUp(unsigned char key, int x, int y) 
{

  keys[key] = false;

  if (key == ' ') catchPressed = false;

}

void update()
{

  // Frame timer

  unsigned long currentTime = glutGet(GLUT_ELAPSED_TIME);

  if (currentTime - lastUpdate < FRAME_TIME) return;

  lastUpdate = currentTime;

  if (caughtFish)
  {
    sleep_ms(250);

    caughtFish = false;
  }

  // Movement detection

  float nextGreenY = greenScreenY + (greenSpeed * greenDirection);

  float nextFishY = fishScreenY + (fishSpeed * fishDirection);

  if (nextGreenY + (greenHeightY * greenDirection) >= CENTER_Y + yMeterDistance || nextGreenY + (greenHeightY * greenDirection) <= CENTER_Y - yMeterDistance)
  {
    greenDirection = -greenDirection;
  }
  else
  {
    greenScreenY = nextGreenY;
  }

  if (nextFishY + (fishHeight * fishDirection) >= fishMaxMove || nextFishY + (fishHeight * fishDirection) <= fishMinMove)
  {
    fishDirection = -fishDirection;
  }
  else
  {
    fishScreenY = nextFishY;
  }

  glutPostRedisplay();

}

void windowResize(int width, int height)
{
  glutReshapeWindow(WINDOW_W, WINDOW_H);
}

int main(int argc, char** argv) 
{
  glutInit(&argc, argv);    

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);        
  glutInitWindowSize(WINDOW_W, WINDOW_H);        
    
  glutCreateWindow("Fishing Sim");  

  init();

  for (int i = 0; i < KEY_COUNT; i++) 
  {
    keys[i] = false;
  }

  glutDisplayFunc(display); 
  glutIdleFunc(update);      

  glutKeyboardFunc(keyDown);  
  glutKeyboardUpFunc(keyUp);  

  glutReshapeFunc(windowResize);

  glutMainLoop();      

  return 0;   
}