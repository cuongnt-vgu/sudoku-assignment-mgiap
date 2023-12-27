#include "hidden_singles.h"

int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int counter = 0;
    int array[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        array[i] = 0;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates != 1)
        {
            int *candidates = get_candidates(p_cells[i]);
            for (int j = 0; j < p_cells[i]->num_candidates; j++)
            {
                array[candidates[j] - 1]++;
            }
            free(candidates);
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (array[i] == 1)
        {
            hidden_single_values[counter++] = i + 1;
        }
    }
    return counter;
}

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter)
{
    int hidden_single_values[BOARD_SIZE];
    int counter = find_hidden_single_values(p_cells, hidden_single_values);
    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (is_candidate(p_cells[j], hidden_single_values[i]))
            {
                p_hidden_singles[(*p_counter)++] = (HiddenSingle){p_cells[j], hidden_single_values[i]};
            }
        }
    }
}

int hidden_singles(SudokuBoard *p_board) {
    int count = 0;
    int overlap = 0;
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &count);
        find_hidden_single(p_board->p_cols[i], hidden_singles, &count);
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &count);
    }

    for (int i = 0; i < count; i++)
    {
        if (hidden_singles[i].p_cell->num_candidates == 1)
        {
            overlap++;
        }
        int num = hidden_singles[i].p_cell->num_candidates;
        int *candidates = get_candidates(hidden_singles[i].p_cell);
        for (int j = 0; j < num; j++)
        {
            if (candidates[j] != hidden_singles[i].value)
            {
                unset_candidate(hidden_singles[i].p_cell, candidates[j]);
            }
        }
        free(candidates);

    }
    return count - overlap;
}