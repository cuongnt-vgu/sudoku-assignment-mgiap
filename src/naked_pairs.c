#include "naked_pairs.h"

bool is_in_list_naked_pairs(NakedPairs *pairs_array, Cell *cell)
{
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        if ((pairs_array[i].p_cell_1 == cell) || (pairs_array[i].p_cell_2 == cell))
        {
            return true;
        }
    }
    return false;
}

void find_naked_pairs(Cell **cells, int *pairs_count, int *unset_count, NakedPairs *pairs_array, UnsetDataPairs *unset_pairs)
{
    for (int i = 0; i < BOARD_SIZE - 1; i++)
    {
        if (cells[i]->num_candidates == 2)
        {
            for (int j = i + 1; j < BOARD_SIZE; j++)
            {
                if (cells[j]->num_candidates == 2)
                {
                    int *candidates_1 = get_candidates(cells[i]);
                    int *candidates_2 = get_candidates(cells[j]);
                    if ((candidates_1[0] == candidates_2[0]) && (candidates_1[1] == candidates_2[1]))
                    {
                        for (int k = 0; k < BOARD_SIZE; k++)
                        {
                            if ((k != i) && (k != j) && (cells[k]->num_candidates > 1))
                            {
                                if (!is_in_list_naked_pairs(pairs_array, cells[k]))
                                {
                                    pairs_array[*pairs_count].p_cell_1 = cells[i];
                                    pairs_array[*pairs_count].p_cell_2 = cells[j];
                                    (*pairs_count)++;
                                }

                                if (is_candidate(cells[k], candidates_1[0]) || is_candidate(cells[k], candidates_1[1]))
                                {
                                    unset_pairs[*unset_count].p_cell = cells[k];
                                    unset_pairs[*unset_count].candidate_1 = candidates_1[0];
                                    unset_pairs[*unset_count].candidate_2 = candidates_1[1];
                                    (*unset_count)++;
                                }
                            }
                        }
                    }
                    free(candidates_1);
                    free(candidates_2);
                }
            }
        }
    }
}

int naked_pairs(SudokuBoard *board)
{
    NakedPairs pairs_array[BOARD_SIZE * BOARD_SIZE];
    int pairs_count = 0;
    UnsetDataPairs unset_pairs[BOARD_SIZE * BOARD_SIZE];
    int unset_count = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_pairs(board->p_rows[i], &pairs_count, &unset_count, pairs_array, unset_pairs);
        find_naked_pairs(board->p_cols[i], &pairs_count, &unset_count, pairs_array, unset_pairs);
        find_naked_pairs(board->p_boxes[i], &pairs_count, &unset_count, pairs_array, unset_pairs);
    }

    for (int i = 0; i < unset_count; i++)
    {
        if (is_candidate(unset_pairs[i].p_cell, unset_pairs[i].candidate_1))
        {
            unset_candidate(unset_pairs[i].p_cell, unset_pairs[i].candidate_1);
        }
        if (is_candidate(unset_pairs[i].p_cell, unset_pairs[i].candidate_2))
        {
            unset_candidate(unset_pairs[i].p_cell, unset_pairs[i].candidate_2);
        }
    }
    
    return pairs_count;
}