#include "minitel.h"
#include "snake.h"
#include "tetris.h"

// réservé au minitel
void setup() {
  initialiser_minitel();
}

void loop() {
  boucle_minitel();
}

extern struct Evenements minitel_events;

// code
static char curgame = 0;

static union {
  struct SnakeGame snake;
  struct TetrisGame tetris;
} game;

static void init_game() {  
  switch (curgame) {
    case 1:
      init_snake((SnakeGame*)&game.snake, &minitel_events);
      break;
    case 2:
      init_tetris((TetrisGame*)&game.tetris, &minitel_events);
      break;
  }
}

static void selection_jeu(void* data, unsigned char code) {
  switch (code) {
    case '1':
      curgame = 1;
      break;

    case '2':
      curgame = 2;
      break;

    default:
      return;
  }
  
  reset_events(&minitel_events);
  init_game();
}

void setup_minitel() {
  reset_events(&minitel_events);
  minitel_events.on_caractere = selection_jeu;

  curgame = 0;
  
  reset_minitel();
  aiguillage_off(CR_ECRAN, CE_MODEM);
  effacer_ecran();

  Serial.print("[1] SNAKE\r\n[2] TETRIS");
}

void loop_minitel() {
  switch (curgame) {
    case 1:
      loop_snake((SnakeGame*)&game.snake);
      break;
    case 2:
      loop_tetris((TetrisGame*)&game.tetris);
      break;
  }
}
