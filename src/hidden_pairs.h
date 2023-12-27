#pragma once

#include "sudoku.h"

typedef struct {
    Cell *p_cell1;
    Cell *p_cell2;  // Pointer to the cell
    int value1;    // First value of the hidden pair
    int value2;    // Second value of the hidden pair
} HiddenPair;

int find_hidden_pair_values(Cell **p_cells, int *hidden_pair_values);
void find_hidden_pair(Cell **p_cells, HiddenPair *p_hidden_pairs, int *p_counter);
int hidden_pairs(SudokuBoard *p_board);
