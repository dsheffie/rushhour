#include "rushhour.hh"
#include <cstdint>
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


piece* place(piece *p, piece *last) {
  for(int i = 0; i < p->sz; i++) {
    if(p->horiz) {
      grid[p->y][p->x+i] = p;
    }
    else {
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
  last =  place(new car(true, 0, 2), nullptr);
  red_car = last;
  
  /* yellow bus */
  last = place(new bus(false, 2, 0), last);
  
  /* green car */
  last = place(new car(true, 4, 0), last);
  
  /* purple bus */
  last = place(new bus(true, 0, 3), last);
  
  /* blue bus */
  last = place(new bus(false, 5, 3), last);
  
  print_grid();
  bool x = search();
  printf("search %s\n", x ? "was successful" : "failed");
  if(x) {
    print_grid();
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
