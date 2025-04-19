#ifndef __rushhourhh__
#define __rushhourhh__
#include <cstdio>

struct piece {
  static int idcnt;
  bool horiz;
  int sz,x,y,id;
  piece *next;
  piece(bool horiz, int sz, int x, int y) :
    horiz(horiz), sz(sz), x(x), y(y), id(idcnt++), next(nullptr) {}
  bool canMoveLeft(piece* grid[6][6]) const {
    int nx = (x - 1);
    if(not(horiz)) {
      return false;
    }
    if(nx < 0) {
      //printf("%d would be off grid on left\n", id);
      return false;
    }
    return (grid[y][nx] == nullptr);
  }
  bool canMoveUp(piece *grid[6][6]) const {
    int ny = (y + sz);
    if(horiz)
      return false;
    if(ny >= 6)
      return false;
    return (grid[ny][x] == nullptr);
  }
  bool canMoveRight(piece *grid[6][6]) const {
    int nx = (x + sz);
    if(not(horiz)) {
      return false;
    }
    if(nx > 5) {
      //printf("%d would be off grid on right\n", id);
      return false;
    }
    return (grid[y][nx] == nullptr);
  }
  bool canMoveDown(piece *grid[6][6]) const {
    int ny = (y - 1);
    if(horiz)
      return false;
    if(ny < 0)
      return false;
    return (grid[ny][x] == nullptr);
  }  
  
};

struct bus : public piece {
  bus(bool horiz, int x, int y) : piece(horiz, 3, x, y) {}
};

struct car : public piece {
  car(bool horiz, int x, int y) : piece(horiz, 2, x, y) {}
};


#endif
