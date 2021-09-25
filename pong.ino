#include <Gamebuino-Meta.h>

// ball attributes
static int ball_posX = 20;
static int ball_posY = 20;
static int ball_speedX = 1;
static int ball_speedY = 1;
static const int ball_size = 4;


// Dimension of both paddles
static const int paddle_height = 10;
static const int paddle_width = 3;

// paddle1 attributes
static const int paddle1_posX = 0;
static int paddle1_posY = 30;

// paddle2 attributes
static const int paddle2_posX = gb.display.width() - paddle_width;
static int paddle2_posY = 30;
static int paddle2_speed = 0;

// score
static int score_1 = 0;
static int score_2 = 0;

// difficulty
enum Difficulty {easy=5, medium=3, hard=2};
static Difficulty upper_limit = easy; // defines the difficulty of the game (the lower, the more difficult)

// Functions
inline static void update_difficulty();
inline static void update_paddle1_position();
inline static void update_paddle2_position();
inline static void update_ball_position();
inline static void check_collisions();
inline static void check_game_over();
inline static void display();

void setup() 
{
  gb.begin();
}


void loop() 
{
  while(!gb.update());
  gb.display.clear();

  update_difficulty();
  update_paddle1_position();
  update_paddle2_position();
  update_ball_position();
  check_collisions();
  check_game_over();
  display();

}


inline static void update_difficulty()
{
  if(gb.buttons.pressed(BUTTON_MENU))
  {
    // Toggle difficulty
    switch(upper_limit)
    {
    case easy:
      upper_limit = medium;
      break;
    case medium:
      upper_limit = hard;
      break;
    case hard:
      upper_limit = easy;
      break;
    }
  }
}

inline static void update_paddle1_position()
{
    // Update paddle1 position
  if (gb.buttons.repeat(BUTTON_UP, 0)) 
  {
    paddle1_posY = paddle1_posY - 1;
  }
  if (gb.buttons.repeat(BUTTON_DOWN, 0)) 
  {
    paddle1_posY = paddle1_posY + 1;
  }
}

inline static void update_paddle2_position()
{
   // Update paddle2 position
  if (ball_posY < (paddle2_posY + (paddle_height/2))
      && random(0, upper_limit) == 1) 
  {
    paddle2_speed = -2;
  }
  else if (ball_posY > (paddle2_posY + (paddle_height/2))
      && random(0, upper_limit) == 1)
  {
    paddle2_speed = +2;
  }

    paddle2_posY = paddle2_posY + paddle2_speed;
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
    && (ball_posY <= paddle1_posY + paddle_height) ) {
    ball_speedX = 1;
  }

  // Ball-paddle2 collisions
  if ( ((ball_posX+ball_size) == paddle2_posX)
    && (ball_posY + ball_size >= paddle2_posY) 
    && (ball_posY <= paddle2_posY + paddle_height) ) {
    ball_speedX = -1;
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
    // Display ball
  gb.display.fillRect(ball_posX, ball_posY, ball_size, ball_size);
  // Display paddle1
  gb.display.fillRect(paddle1_posX, paddle1_posY, paddle_width, paddle_height);
    // Display paddle2
  gb.display.fillRect(paddle2_posX, paddle2_posY, paddle_width, paddle_height);
  // Display score
  gb.display.setCursor(30, 3);
  gb.display.print(score_1);
  gb.display.print(" - ");
  gb.display.print(score_2);

  gb.display.setCursor(30, gb.display.height()-8);
  switch(upper_limit)
  {
  case easy:
    gb.display.print("EASY");
    break;
  case medium:
    gb.display.print("MEDIUM");
    break;
  case hard:
    gb.display.print("HARD");
    break;
  }
}