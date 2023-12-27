#pragma once

#include "sudoku.h"

typedef struct {
    Cell *p_cell1;
    Cell *p_cell2;
    Cell *p_cell3;  // Pointers to the cells
    int value1;     // First value of the hidden triple
    int value2;     // Second value of the hidden triple
    int value3;     // Third value of the hidden triple
} HiddenTriple;

int find_hidden_triple_values(Cell **p_cells, int *hidden_triple_values);
void find_hidden_triple(Cell **p_cells, HiddenTriple *p_hidden_triples, int *p_counter);
int hidden_triples(SudokuBoard *p_board);