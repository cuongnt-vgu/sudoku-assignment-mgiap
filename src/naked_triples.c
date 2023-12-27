#include "naked_triples.h"

int is_in_triples(int tripleValues[3], int targetValue)
{
    for (int idx = 0; idx < 3; idx++)
    {
        if (tripleValues[idx] == targetValue) return 1;
    }
    return 0;
}

int check_cell_in_triples(Cell* cell, int tripleValues[3])
{
    int* candidateValues = get_candidates(cell);
    for (int idx = 0; idx < cell->num_candidates; idx++)
    {
        if (!is_in_triples(tripleValues, candidateValues[idx]))
        {
            free(candidateValues);
            return 0;
        }
    }
    free(candidateValues);
    return 1;
}

int check_naked_triples(Cell **cells, int possibleTriples[], int *indices)
{
    int cellCounter = 0;
    for (int idx = 0; idx < BOARD_SIZE; idx++)
    {
        if (cells[idx]->num_candidates == 1 || cells[idx]->num_candidates > 3) continue;
        else if (check_cell_in_triples(cells[idx], possibleTriples)) indices[cellCounter++] = idx;
    }
    return cellCounter == 3;
}

void find_naked_triples(Cell **cells, NakedTriples *foundTriples, int *counter, int isBox)
{
    int candidateCount[BOARD_SIZE];
    for (int idx = 0; idx < BOARD_SIZE; idx++)
    {
        candidateCount[idx] = 0;
    }

    for (int idx = 0; idx < BOARD_SIZE; idx++)
    {
        int* candidateValues = get_candidates(cells[idx]);
        for (int j = 0; j < cells[idx]->num_candidates; j++)
        {
            candidateCount[candidateValues[j]-1] += 1;
        }
        free(candidateValues);
    }

    int possibleTriples[BOARD_SIZE];
    int possibleTriplesCount = 0;
    for (int idx = 0; idx < BOARD_SIZE; idx++)
    {
        if (candidateCount[idx] >= 2)
        {
            possibleTriples[possibleTriplesCount++] = idx+1;
        }
    }

    if (possibleTriplesCount < 3) return;

    for (int i = 0; i < possibleTriplesCount - 2; i++)
    {
        for (int j = i+1; j < possibleTriplesCount - 1; j++)
        {
            for (int k = j+1; k < possibleTriplesCount; k++)
            {
                int values[3] = {possibleTriples[i], possibleTriples[j], possibleTriples[k]};
                int indices[BOARD_SIZE];
                if (check_naked_triples(cells, values, indices))
                {
                    if (isBox == 1)
                    {
                        if (cells[indices[0]]->row_index == cells[indices[1]]->row_index && 
                            cells[indices[1]]->row_index == cells[indices[2]]->row_index) continue;
                        if (cells[indices[0]]->col_index == cells[indices[1]]->col_index && 
                            cells[indices[1]]->col_index == cells[indices[2]]->col_index) continue;
                    }
                    foundTriples[(*counter)++] = (NakedTriples){cells, {indices[0], indices[1], indices[2]}, 
                                                        {possibleTriples[i], possibleTriples[j], possibleTriples[k]}};
                }
            }
        }
    }
}

int naked_triples(SudokuBoard *board)
{
    int tripleCounter = 0;

    NakedTriples foundTriples[BOARD_SIZE * BOARD_SIZE];

    for (int idx = 0; idx < BOARD_SIZE; idx++)
    {
        find_naked_triples(board->p_rows[idx], foundTriples, &tripleCounter, 0);
        find_naked_triples(board->p_cols[idx], foundTriples, &tripleCounter, 0);
        find_naked_triples(board->p_boxes[idx], foundTriples, &tripleCounter, 1);
    }
    for (int idx = 0; idx < tripleCounter; idx++)
    {
        Cell** cellGroup = foundTriples[idx].p_cells;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (j != foundTriples[idx].indices[0] && j != foundTriples[idx].indices[1] && j != foundTriples[idx].indices[2])
            {
                for (int k = 0; k < 3; k++)
                {
                    if (is_candidate(cellGroup[j], foundTriples[idx].values[k]))
                    {
                        unset_candidate(cellGroup[j], foundTriples[idx].values[k]);
                    }
                }
            }
        }

    }
    return tripleCounter;
}