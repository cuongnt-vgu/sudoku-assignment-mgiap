#include "hidden_pairs.h"

bool is_in_cell1(Cell **p_cells, int a, int b)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if ((p_cells[i]->candidates[a - 1] == 1 && p_cells[i]->candidates[b - 1] != 1) ||
            (p_cells[i]->candidates[b - 1] == 1 && p_cells[i]->candidates[a - 1] != 1))
        {
            return false;
        }
    }
    return true;
}
// find hidden pair values in a row, in a column or in a box
int find_hidden_pair_values(Cell **p_cells, int *hidden_pair_values)
{
    int counter = 0;
    int array[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        array[i] = 0;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates > 1)
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
        if (array[i] == 2)
        {
            hidden_pair_values[counter++] = i + 1;
        }
    }
    return counter;
}

// find hidden pair cells in a row, in a column or in a box
void find_hidden_pair(Cell **p_cells, HiddenPair *p_hidden_pairs, int *p_counter)
{
    int hidden_pair_values[BOARD_SIZE];
    int counter = find_hidden_pair_values(p_cells, hidden_pair_values);
    for (int i = 0; i < counter - 1; i++)
    {
        for (int j = i + 1; j < counter; j++)
        {
            if (is_in_cell1(p_cells, hidden_pair_values[i], hidden_pair_values[j]))
            {
                for (int k = 0; k < BOARD_SIZE - 1; k++)
                {
                    for (int l = k + 1; l < BOARD_SIZE; l++)
                    {
                        if (p_cells[k]->candidates[hidden_pair_values[i] - 1] == 1 &&
                            p_cells[k]->candidates[hidden_pair_values[j] - 1] == 1 &&
                            p_cells[l]->candidates[hidden_pair_values[i] - 1] == 1 &&
                            p_cells[l]->candidates[hidden_pair_values[j] - 1] == 1)
                        {
                            p_hidden_pairs[(*p_counter)++] = (HiddenPair){p_cells[k], p_cells[l], hidden_pair_values[i], hidden_pair_values[j]};
                        }
                    }
                }
            }
        }
    }
}

int hidden_pairs(SudokuBoard *p_board) {
    int overlap = 0;
    int count = 0;
    HiddenPair hidden_pairs[BOARD_SIZE * BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {   
        find_hidden_pair(p_board->p_rows[i], hidden_pairs, &count);
        find_hidden_pair(p_board->p_cols[i], hidden_pairs, &count);
        find_hidden_pair(p_board->p_boxes[i], hidden_pairs, &count);
    }

    for (int i = 0; i < count; i++)
    {
        if (hidden_pairs[i].p_cell1->num_candidates == 2 && hidden_pairs[i].p_cell2->num_candidates == 2)
        {
            overlap++;
        }
        
        int num1 = hidden_pairs[i].p_cell1->num_candidates;
        int num2 = hidden_pairs[i].p_cell2->num_candidates;
        int *candidates1 = get_candidates(hidden_pairs[i].p_cell1);
        int *candidates2 = get_candidates(hidden_pairs[i].p_cell2);

        for (int j = 0; j < num1; j++)
        {
            if (candidates1[j] != hidden_pairs[i].value1 && candidates1[j] != hidden_pairs[i].value2)
            {
                unset_candidate(hidden_pairs[i].p_cell1, candidates1[j]);
            }
        }

        for (int j = 0; j < num2; j++)
        {
            if (candidates2[j] != hidden_pairs[i].value1 && candidates2[j] != hidden_pairs[i].value2)
            {
                unset_candidate(hidden_pairs[i].p_cell2, candidates2[j]);
            }
        }

        free(candidates1);
        free(candidates2);
    }


    return count - overlap;
}