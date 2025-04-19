#ifndef __rushhourhh__
#define __rushhourhh__
#include <cstdio>

struct piece {
  bool horiz;
  int sz,x,y,id;
  piece(bool horiz, int sz, int x, int y, int id) :
    horiz(horiz), sz(sz), x(x), y(y), id(id) {}
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


#endif
