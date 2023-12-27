#include "hidden_triples.h"

bool is_in_cell2(Cell **p_cells, int a, int b, int c)
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->candidates[a - 1] == 1 || p_cells[i]->candidates[b - 1] == 1 || p_cells[i]->candidates[c - 1] == 1)
        {
            count++;
        }
    }
    return count == 3;
}

int find_hidden_triple_values(Cell **p_cells, int *hidden_triple_values)
{
    int counter = 0;
    int array[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        array[i] = 0;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates >= 2)
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
        if (array[i] == 2 || array[i] == 3)
        {
            hidden_triple_values[counter++] = i + 1;
        }
    }
    return counter;
}

// find hidden triple cells in a row, in a column or in a box
void find_hidden_triple(Cell **p_cells, HiddenTriple *p_hidden_triples, int *p_counter)
{
    int hidden_triple_values[BOARD_SIZE];
    int counter = find_hidden_triple_values(p_cells, hidden_triple_values);
    for (int i = 0; i < counter - 2; i++)
    {
        for (int j = i + 1; j < counter - 1; j++)
        {
            for (int k = j + 1; k < counter; k++)
            {
                if (is_in_cell2(p_cells, hidden_triple_values[i], hidden_triple_values[j], hidden_triple_values[k]))
                {
                    for (int l = 0; l < BOARD_SIZE - 2; l++)
                    {
                        for (int m = l + 1; m < BOARD_SIZE - 1; m++)
                        {
                            for (int n = m + 1; n < BOARD_SIZE; n++)
                            {
                                int count_l = (p_cells[l]->candidates[hidden_triple_values[i] - 1] >= 1 ? 1 : 0) +
                              (p_cells[l]->candidates[hidden_triple_values[j] - 1] >= 1 ? 1 : 0) +
                              (p_cells[l]->candidates[hidden_triple_values[k] - 1] >= 1 ? 1 : 0);

                                int count_m = (p_cells[m]->candidates[hidden_triple_values[i] - 1] >= 1 ? 1 : 0) +
                              (p_cells[m]->candidates[hidden_triple_values[j] - 1] >= 1 ? 1 : 0) +
                              (p_cells[m]->candidates[hidden_triple_values[k] - 1] >= 1 ? 1 : 0);

                                int count_n = (p_cells[n]->candidates[hidden_triple_values[i] - 1] >= 1 ? 1 : 0) +
                              (p_cells[n]->candidates[hidden_triple_values[j] - 1] >= 1 ? 1 : 0) +
                              (p_cells[n]->candidates[hidden_triple_values[k] - 1] >= 1 ? 1 : 0);

                                if (count_l >= 2 && count_m >= 2 && count_n >= 2)
                                {
                                    p_hidden_triples[(*p_counter)++] = (HiddenTriple){p_cells[l], p_cells[m], p_cells[n], hidden_triple_values[i], hidden_triple_values[j], hidden_triple_values[k]};
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int hidden_triples(SudokuBoard *p_board) {
    int overlap = 0;
    int count = 0;
    HiddenTriple hidden_triples[BOARD_SIZE * BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {   
        find_hidden_triple(p_board->p_rows[i], hidden_triples, &count);
        find_hidden_triple(p_board->p_cols[i], hidden_triples, &count);
        find_hidden_triple(p_board->p_boxes[i], hidden_triples, &count);
    }

    for (int i = 0; i < count; i++)
    {   
        if ((hidden_triples[i].p_cell1->num_candidates == 2 || hidden_triples[i].p_cell1->num_candidates == 3) &&
            (hidden_triples[i].p_cell2->num_candidates == 2 || hidden_triples[i].p_cell2->num_candidates == 3) &&
            (hidden_triples[i].p_cell3->num_candidates == 2 || hidden_triples[i].p_cell3->num_candidates == 3))
        {
            overlap++;
        }
        int num1 = hidden_triples[i].p_cell1->num_candidates;
        int num2 = hidden_triples[i].p_cell2->num_candidates;
        int num3 = hidden_triples[i].p_cell3->num_candidates;
        int *candidates1 = get_candidates(hidden_triples[i].p_cell1);
        int *candidates2 = get_candidates(hidden_triples[i].p_cell2);
        int *candidates3 = get_candidates(hidden_triples[i].p_cell3);

        for (int j = 0; j < num1; j++)
        {
            if (candidates1[j] != hidden_triples[i].value1 && candidates1[j] != hidden_triples[i].value2 && candidates1[j] != hidden_triples[i].value3)
            {
                unset_candidate(hidden_triples[i].p_cell1, candidates1[j]);
            }
        }

        for (int j = 0; j < num2; j++)
        {
            if (candidates2[j] != hidden_triples[i].value1 && candidates2[j] != hidden_triples[i].value2 && candidates2[j] != hidden_triples[i].value3)
            {
                unset_candidate(hidden_triples[i].p_cell2, candidates2[j]);
            }
        }

        for (int j = 0; j < num3; j++)
        {
            if (candidates3[j] != hidden_triples[i].value1 && candidates3[j] != hidden_triples[i].value2 && candidates3[j] != hidden_triples[i].value3)
            {
                unset_candidate(hidden_triples[i].p_cell3, candidates3[j]);
            }
        }

        free(candidates1);
        free(candidates2);
        free(candidates3);
    }

    return count - overlap;
}