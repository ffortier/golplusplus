#include "cell.h"
#include "SDL2/SDL.h"

Cell::Cell(int x, int y, int size): x(x), y(y), size(size) {
  alive = new Cell::Alive(this);
  dead = new Cell::Dead(this);
  current = next = dead;
  neighbourCount = 0;
  neighbours = new const Cell*[8];
}

Cell::~Cell() {
  delete alive;
  delete dead;
  delete[] neighbours;
}

void Cell::addNeighbour(const Cell* neighbour) {
  neighbours[neighbourCount++] = neighbour;
}

void Cell::update() {
  int livingNeighbourCount = 0;

  for (int i = 0; i < neighbourCount; i++) {
    if (neighbours[i]->isAlive()) {
      livingNeighbourCount++;
    }
  }

  current->update(livingNeighbourCount);
}

void Cell::render(SDL_Renderer* renderer) const {
  current->render(renderer);
}

void Cell::changeState(State* state) {
  next = state;
}

void Cell::commit() {
  current = next;
}

void Cell::toggle() {
  current->toggle();
  commit();
}

bool Cell::isAlive() const {
  return current->isAlive();
}

Cell::State::State(Cell* cell): cell(cell) {

}

Cell::Alive::Alive(Cell* cell): Cell::State(cell) {

}

void Cell::Alive::update(int livingNeighbourCount) {
  if (livingNeighbourCount != 2 && livingNeighbourCount != 3) {
    cell->changeState(cell->dead);
  }
}

void Cell::Alive::render(SDL_Renderer* renderer) const {
  SDL_Rect rect = { cell->x, cell->y, cell->size, cell->size };
  SDL_SetRenderDrawColor(renderer, 51, 51, 255, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void Cell::Alive::toggle() {
  cell->changeState(cell->dead);
}

Cell::Dead::Dead(Cell* cell): Cell::State(cell) {

}

void Cell::Dead::update(int livingNeighbourCount) {
  if (livingNeighbourCount == 3) {
    cell->changeState(cell->alive);
  }
}

void Cell::Dead::render(SDL_Renderer* renderer) const {

}

void Cell::Dead::toggle() {
  cell->changeState(cell->alive);
}
