#include "rushhour.hh"
#include <cstdint>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <set>

static piece* grid[6][6] = {{nullptr}};

std::string grid_to_string() {
  std::stringstream ss;
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 6; j++) {
      if(grid[i][j]==nullptr)
	ss << "--,";      
      else {
	if(grid[i][j]->id < 10) {
	  ss << 0;
	}
	ss << grid[i][j]->id << ",";
      }
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
//#define VERBOSE 1
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
  
  for(piece *p = red_car; p != nullptr; p=p->next) {
    //printf("piece %d\n", p->id);
    if(p->canMoveLeft(grid)) {
      moveLeft(p);
      if(search()) {
#ifdef VERBOSE
	moveRight(p);      
	printf("move %d left\n", p->id);
	print_grid();
#endif
	return true;
      }
      moveRight(p);      
    }
    if(p->canMoveRight(grid)) {
      moveRight(p);
      if(search()) {
#ifdef VERBOSE
	moveLeft(p);            	
	printf("move %d right\n", p->id);
	print_grid();
#endif
	return true;
      }
      moveLeft(p);            
    }
    if(p->canMoveUp(grid)) {
      moveUp(p);
      if(search()) {
#ifdef VERBOSE
	moveDown(p);            
	printf("move %d up\n", p->id);
	print_grid();
#endif
	return true;
      }
      moveDown(p);            
    }
    if(p->canMoveDown(grid)) {
      moveDown(p);
      if(search()) {
#ifdef VERBOSE
	moveUp(p);                  		
	printf("move %d down\n", p->id);
	print_grid();
#endif
	return true;
      }
      moveUp(p);                  
    }
  }
  return false;
}


piece* place(piece *p, piece *last) {
  for(int i = 0; i < p->sz; i++) {
    if(p->horiz) {
      assert(grid[p->y][p->x+i] == nullptr);
      grid[p->y][p->x+i] = p;
    }
    else {
      assert(grid[p->y+i][p->x] == nullptr);
      grid[p->y+i][p->x] = p;
    }
  }
  if(last) {
    last->next = p;
  }
  return p;
}

int piece::idcnt = 0;

int main() {
  piece *last;
  /* red car */
  last =  place(new car(true, 1, 2), nullptr);
  red_car = last;

  /* purple car */
  last = place(new car(true, 0, 3), last);

  /* dark green car */
  last = place(new car(false, 0, 4), last);

  /* light green car */
  last = place(new car(false, 1, 0), last);
  
  /* yellow bus */
  last = place(new bus(true, 2, 0), last);

  /* blue bus */
  last = place(new bus(false, 2, 3), last);

  /* light blue car */
  last = place(new car(false, 3, 2), last);

  /* pink car */
  last = place(new car(false, 4, 2), last);

  /* sliver car */
  last = place(new car(true, 3, 4), last);

  /* green bus */
  last = place(new bus(true, 3, 5), last);
    
  /* purple bus */
  last = place(new bus(false, 5, 2), last);

  /* orange car */
  last = place(new car(false, 5, 0), last);
    
  print_grid();
  bool x = search();
  printf("search %s\n", x ? "was successful" : "failed");
  if(x) {
    std::cout << "explored " << explored << " states\n";
  }

  last = red_car;
  while(true) {
    if(last == nullptr) {
      break;
    }
    piece *p = last;
    last = last->next;
    delete p;
  }

  return 0;
}
