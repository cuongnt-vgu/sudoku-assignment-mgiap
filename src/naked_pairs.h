#pragma once

#include "sudoku.h"

typedef struct NakedPairs_impl
{
    Cell *p_cell_1;
    Cell *p_cell_2;
} NakedPairs;

typedef struct UnsetDataPairs_impl
{
    Cell *p_cell;
    int candidate_1;
    int candidate_2;
} UnsetDataPairs;

bool is_in_list_naked_pairs(NakedPairs *p_array, Cell *p);
void find_naked_pairs(Cell **p_cells, int *p_counter, int *p_unset, NakedPairs *p_naked_pairs, UnsetDataPairs *p_unset_array);
int naked_pairs(SudokuBoard *p_board);