#pragma once

struct Position {
  char x;
  char y;
};

enum Direction {
  D_GAUCHE,
  D_DROITE,
  D_HAUT,
  D_BAS
};

void banner_begin(const char *banner);
void banner_step();
