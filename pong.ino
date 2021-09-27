#include <Gamebuino-Meta.h>

#define AI_HANDICAP 2 // the higher the easier
#define FPS 40        // framerate

#define SIGN(x)  (((x) > 0) ? 1 : -1)

// ball attributes
static int ball_posX = 20;
static int ball_posY = 20;
static int ball_speedX = 1;
static int ball_speedY = 1;
static const int ball_size = 4;

// Dimension of both paddles
static const int paddle_height = 8;
static const int paddle_width = 3;

// paddle1 attributes
static const int paddle1_posX = 5;
static int paddle1_posY = 30;
static int paddle1_speed = 0;

// paddle2 attributes
static const int paddle2_posX = gb.display.width() - paddle_width - 5;
static int paddle2_posY = 30;
static int paddle2_speed = 0;

// score
static int score_1 = 0;
static int score_2 = 0;

// difficulty
enum Mode {AI=0, singleplayer, multiplayer};
static Mode mode = AI; // defines game mode

// Functions
inline static void update_mode();
inline static void update_paddle1_position();
inline static void update_paddle2_position();
inline static void update_ball_position();
inline static void check_collisions();
inline static void check_game_over();
inline static void display();

void setup() 
{
  gb.begin();
  gb.setFrameRate(FPS);
}


void loop() 
{
  gb.waitForUpdate();
  gb.display.clear();

  update_mode();
  update_paddle1_position();
  update_paddle2_position();
  update_ball_position();
  check_collisions();
  check_game_over();
  display();

}


inline static void update_mode()
{
  if(gb.buttons.pressed(BUTTON_MENU))
  {
    // Toggle game mode
    switch(mode)
    {
    case AI:
      mode = singleplayer;
      break;
    case singleplayer:
      mode = multiplayer;
      break;
    case multiplayer:
      mode = AI;
      break;
    }
  }
}

inline static void update_paddle1_position()
{

  switch(mode)
  {
    case AI:
    // control paddle 1 automatically
      if (ball_posY < (paddle1_posY + (paddle_height/2))
      && random(0, AI_HANDICAP) == 1) 
      {
        paddle1_speed = -2;
      }
      else if (ball_posY > (paddle1_posY + (paddle_height/2))
          && random(0, AI_HANDICAP) == 1)
      {
        paddle1_speed = +2;
      }
      paddle1_posY = paddle1_posY + paddle1_speed;
      break;
    case singleplayer:
    case multiplayer:
    // control paddle 1 manually
      if (gb.buttons.repeat(BUTTON_UP, 0)) 
      {
        paddle1_posY = paddle1_posY - 1;
        paddle1_speed = -1;
      }
      if (gb.buttons.repeat(BUTTON_DOWN, 0)) 
      {
        paddle1_posY = paddle1_posY + 1;
        paddle1_speed = +1;
      }
      break;
  }
}

inline static void update_paddle2_position()
{
   switch(mode)
  {
    case multiplayer:
        // control paddle 2 manually
      if (gb.buttons.repeat(BUTTON_B, 0)) 
      {
        paddle2_posY = paddle2_posY - 1;
        paddle2_speed = -1;
      }
      if (gb.buttons.repeat(BUTTON_A, 0)) 
      {
        paddle2_posY = paddle2_posY + 1;
        paddle2_speed = +1;
      }
      break;
    case singleplayer:
    case AI:
    // control paddle 2 automatically
      if (ball_posY < (paddle2_posY + (paddle_height/2))
          && random(0, AI_HANDICAP) == 1) 
      {
        paddle2_speed = -2;
      }
      else if (ball_posY > (paddle2_posY + (paddle_height/2))
          && random(0, AI_HANDICAP) == 1)
      {
        paddle2_speed = +2;
      }
      paddle2_posY = paddle2_posY + paddle2_speed;
      break;
  }
}

inline static void update_ball_position()
{
  // Update ball position
  ball_posX = ball_posX + ball_speedX;
  ball_posY = ball_posY + ball_speedY;
}

inline static void check_collisions()
{
  // Collisions with walls
  if (ball_posY < 0) {
    ball_speedY = 1;
  }
  if (ball_posY > gb.display.height() - ball_size) {
    ball_speedY = -1;
  }


  // Ball-paddle1 collisions
  if ( (ball_posX == paddle1_posX + paddle_width)
        && (ball_posY + ball_size >= paddle1_posY) 
        && (ball_posY <= paddle1_posY + paddle_height) ) 
  {
    ball_speedX = 1;
    ball_speedY = SIGN(paddle1_speed);
  }

  // Ball-paddle2 collisions
  if ( ((ball_posX+ball_size) == paddle2_posX)
    && (ball_posY + ball_size >= paddle2_posY) 
    && (ball_posY <= paddle2_posY + paddle_height) ) {
    ball_speedX = -1;
    ball_speedY = SIGN(paddle2_speed);
  }
}

inline static void check_game_over()
{
   // Check if the ball left the screen (on the left side)
  if (ball_posX < 0) 
  {
    // Reset ball
    ball_posX = 10;
    ball_posY = random(20, gb.display.height() - 20);
    ball_speedX = 1;
    if (random(0, 2) == 1) 
    {  // 50% of the time, this is true
      ball_speedY = 1;
    } 
    else 
    {  // Other 50% of the time
      ball_speedY = -1;
    }
    score_2++;
  }
  // Check if the ball left the screen on the right side
  else if (ball_posX > gb.display.width()) 
  {
    // Reset ball
    ball_posX = gb.display.width() - 10;
    ball_posY = random(20, gb.display.height() - 20);
    ball_speedX = -1;
    if (random(0, 2) == 1) 
    {  // 50% of the time, this is true
      ball_speedY = 1;
    } 
    else 
    {  // Other 50% of the time
      ball_speedY = -1;
    }
    score_1++;
  }
}

inline static void display()
{
  // Display ball and paddles
  gb.display.setColor(RED);
  gb.display.fillRect(ball_posX, ball_posY, ball_size, ball_size);
  gb.display.setColor(WHITE);
  gb.display.fillRect(paddle1_posX, paddle1_posY, paddle_width, paddle_height);
  gb.display.fillRect(paddle2_posX, paddle2_posY, paddle_width, paddle_height);
  
  // display score
  gb.display.setColor(BLUE);
  gb.display.setCursor(30, 3);
  gb.display.print(score_1);
  gb.display.print(" - ");
  gb.display.print(score_2);

  // display game mode
  switch(mode)
  {
  case AI:
    gb.display.setCursor(33, gb.display.height()-8);
    gb.display.print("AI");
    break;
  case singleplayer:
    gb.display.setCursor(20, gb.display.height()-8);
    gb.display.print("Singleplayer");
    break;
  case multiplayer:
    gb.display.setCursor(22, gb.display.height()-8);
    gb.display.print("Multiplayer");
    break;
  }
}