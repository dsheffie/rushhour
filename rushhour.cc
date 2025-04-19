#include "rushhour.hh"
#include <cstdint>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <set>

static piece* grid[6][6];

std::string grid_to_string() {
  std::stringstream ss;
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 6; j++) {
      if(grid[i][j]==nullptr)
	ss << "-,";      
      else
	ss << grid[i][j]->id << ",";
    }
  }
  return ss.str();
}
void print_grid() {
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 6; j++) {
      if(grid[i][j]==nullptr)
	printf(" - ");
      else
	printf(" %d ", grid[i][j]->id);
    }
    printf("\n");
  }
  printf("........\n");
}

static std::set<std::string> seen;
piece *red_car = nullptr;
static std::vector<piece*> pieces;

void moveRight(piece *p) {
  grid[p->y][p->x] = nullptr;  
  for(int i = 0; i < p->sz; i++) {
    grid[p->y][p->x+i+1] = p;
  }
  p->x += 1;
}

void moveUp(piece *p) {
  grid[p->y][p->x] = nullptr;
  for(int i = 0; i < p->sz; i++) {
    grid[p->y+i+1][p->x] = p;
  }  
  p->y += 1;
}

void moveLeft(piece *p) {
  grid[p->y][p->x+p->sz-1] = nullptr;  
  for(int i = 0; i < p->sz; i++) {
    grid[p->y][p->x + i - 1] = p;
  }
  p->x -= 1;
}

void moveDown(piece *p) {
  grid[p->y+p->sz-1][p->x] = nullptr;  
  for(int i = 0; i < p->sz; i++) {
    grid[p->y + i - 1][p->x] = p;
  }
  p->y -= 1;
}

uint64_t explored = 0;

bool search() {

  if((red_car->x + red_car->sz-1) == 5) {
    return true;
  }

  std::string h = grid_to_string();
  if(seen.find(h) != seen.end()) {
    return false;
  }
  seen.insert(h);
  
  //print_grid();
  ++explored;
  
  for(piece *p : pieces) {
    //printf("piece %d\n", p->id);
    if(p->canMoveLeft(grid)) {
      moveLeft(p);
      if(search()) {
	return true;
      }
      moveRight(p);      
    }
    if(p->canMoveRight(grid)) {
      moveRight(p);
      if(search()) {
	return true;
      }
      moveLeft(p);            
    }
    if(p->canMoveUp(grid)) {
      moveUp(p);
      if(search()) {
	return true;
      }
      moveDown(p);            
    }
    if(p->canMoveDown(grid)) {
      moveDown(p);
      if(search()) {
	return true;
      }
      moveUp(p);                  
    }
  }
  return false;
}


int main() {
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 6; j++) {
      grid[i][j] = nullptr;
    }
  }
  /* red car */
  grid[2][0] = new piece(true, 2, 0, 2, 0);
  grid[2][1] = grid[2][0];
  pieces.push_back(grid[2][0]);
  red_car = grid[2][0];
  
  /* yellow bus */
  grid[0][2] = new piece(false, 3, 2, 0, 1);
  grid[1][2] = grid[0][2];
  grid[2][2] = grid[0][2];
  pieces.push_back(grid[0][2]);
  
  /* green car */
  grid[0][4] = new piece(true, 2, 4, 0, 2);
  grid[0][5] = grid[0][4];
  pieces.push_back(grid[0][4]);
  
  /* purple bus */
  grid[3][0] = new piece(true, 3, 0, 3, 3);
  grid[3][1] = grid[3][0];
  grid[3][2] = grid[3][0];  
  pieces.push_back(grid[3][0]);
  
  /* blue bus */
  grid[3][5] = new piece(false, 3, 5, 3, 5);
  grid[4][5] = grid[3][5];
  grid[5][5] = grid[3][5];
  pieces.push_back(grid[3][5]);
  
  print_grid();
  bool x = search();
  printf("search %s\n", x ? "was successful" : "failed");
  if(x) {
    print_grid();
    printf("explored %lu states\n", explored);
  }
  for(piece *p : pieces) {
    delete p;
  }
  return 0;
}
