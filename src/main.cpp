#include <SDL2/SDL.h>
#include <chrono>
#include <random>
#include "cell.h"

using namespace std;

const int CELL_SIZE = 5;
const int WIDTH = 600;
const int HEIGHT = 600;
const int DELAY_MS = 200;

bool init(SDL_Window**window, SDL_Renderer **renderer) {
  SDL_Init(SDL_INIT_EVERYTHING);

  *window = SDL_CreateWindow(
    "Game of life",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    600,
    600,
    SDL_WINDOW_SHOWN
  );

  if (window == NULL) {
    return false;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    return false;
  }

  return true;
}

void processInput(bool *running) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        *running = false;
        break;
    }
  }
}

void update(Cell **cells, int count) {
  SDL_Delay(DELAY_MS);

  for (int i = 0; i < count; i++) {
    cells[i]->update();
  }

  for (int i = 0; i < count; i++) {
    cells[i]->commit();
  }
}

void draw(Cell **cells, int count, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (int i = 0; i < count; i++) {
    cells[i]->render(renderer);
  }

  SDL_RenderPresent(renderer);
}

Cell** createCells(int rows, int cols, int size) {
  Cell** cells = new Cell*[rows * cols];

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      Cell* current = cells[r * cols + c] = new Cell(c * size, r * size, size);

      if (c > 0) {
        current->addNeighbour(cells[r * cols + c - 1]);
        cells[r * cols + c - 1]->addNeighbour(current);
      }

      if (r > 0) {
        current->addNeighbour(cells[(r - 1) * cols + c]);
        cells[(r - 1) * cols + c]->addNeighbour(current);

        if (c > 0) {
          current->addNeighbour(cells[(r - 1) * cols + c - 1]);
          cells[(r - 1) * cols + c - 1]->addNeighbour(current);
        }

        if (c < cols - 1) {
          current->addNeighbour(cells[(r - 1) * cols + c + 1]);
          cells[(r - 1) * cols + c + 1]->addNeighbour(current);
        }
      }
    }
  }

  return cells;
}

void destroyCells(Cell** cells, int count) {
  for (int i = 0; i < count; i++) {
    delete cells[i];
  }
  delete[] cells;
}

void quit(SDL_Window *window, SDL_Renderer* renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void setup(Cell **cells, int count) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  uniform_int_distribution<int> distribution(1,10);

  for (int i = 0; i < count; i++) {
    if (distribution(generator) == 1) {
      cells[i]->toggle();
    }
  }
}

int main()
{
  int rows = HEIGHT / CELL_SIZE;
  int cols = WIDTH / CELL_SIZE;

  Cell** cells = createCells(rows, cols, CELL_SIZE);
  SDL_Window* window;
  SDL_Renderer* renderer;

  bool running = init(&window, &renderer);

  setup(cells, rows * cols);

  while (running) {
    processInput(&running);
    update(cells, rows * cols);
    draw(cells, rows * cols, renderer);
  }

  quit(window, renderer);
  destroyCells(cells, rows * cols);

  return 0;
}
