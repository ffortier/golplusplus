#ifndef __CELL_H
#define __CELL_H

#include <SDL2/SDL.h>

class Cell {
public:
  Cell(int x, int y, int size);
  ~Cell();

  void addNeighbour(const Cell* neighbour);
  void update();
  void commit();
  void render(SDL_Renderer* renderer) const;
  void toggle();

  bool isAlive() const;

private:
  struct State;
  struct Alive;
  struct Dead;

  void changeState(State* state);

  State* current;
  State* next;
  Alive* alive;
  Dead* dead;

  const Cell** neighbours;

  int neighbourCount;
  int x;
  int y;
  int size;
};

struct Cell::State {
  State(Cell* cell);
  virtual ~State() {}

  virtual void update(int neighbourCount) = 0;
  virtual void render(SDL_Renderer* renderer) const = 0;
  virtual bool isAlive() const = 0;
  virtual void toggle() = 0;

  Cell* cell;
};

struct Cell::Alive : public Cell::State {
  Alive(Cell* cell);
  virtual ~Alive() {}

  virtual void update(int livingNeighbourCount) override;
  virtual void render(SDL_Renderer* renderer) const override;
  virtual bool isAlive() const override { return true; }
  virtual void toggle() override;
};

struct Cell::Dead : public Cell::State {
  Dead(Cell* cell);
  virtual ~Dead() {}

  virtual void update(int livingNeighbourCount) override;
  virtual void render(SDL_Renderer* renderer) const override;
  virtual bool isAlive() const override { return false; }
  virtual void toggle() override;
};

#endif
