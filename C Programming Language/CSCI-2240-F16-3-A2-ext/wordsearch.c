/*
 @Author: Huong Truong
 @Description: Assignment 2
 @Course: CSCI-2240-F16-3-A2
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MATRIX_SIZE 128
#define BUFFER_SIZE MAX_MATRIX_SIZE * 2 + 2

void removespace (char numrow [], int ct);
int checkdown (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkdnright (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkright (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkrightup (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkup (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkupleft (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkleft (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);
int checkleftdown (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl);

int main ()
{
    char buf [BUFFER_SIZE] = {0};
    char box [MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
    int count = 0;
    int i = 0; 
    int j = 0;
    int length = 0;
    int place = 0;
    int sol, a, b, ck;
    char solution [MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

    fgets (buf, MAX_MATRIX_SIZE, stdin);
    for (i = 0; buf[i] != '\0'; i++)
    {
        if (!isspace (buf [i]))
            count ++;
    }
    removespace (buf, count);
    for (i = 0; i < count ; i ++)
    {
        for (j = 0; j < count; j ++)
        {
            box [i][j] = buf [j];
        }
    }
    for (i = 0; i < count; i ++)
    {
        for (j = 0; j < count; j ++)
        {
            solution [i][j] = ' ';
        }
    }
    for (i = 1; i < count; i ++)
    {
        fgets (buf, sizeof buf, stdin);
        removespace (buf, count);

        for (j = 0; j < count; j ++)
        {
            box [i][j] = buf [j];
        }
    }
    while (fgets (buf, sizeof buf, stdin) != NULL)
    {
        place = 0;
        length = 0;
        for (i = 0; i < strlen (buf) - 1; i ++)
        {
            length ++;
        }
        for (i = 0; i < count; i ++)
        {
            for (j = 0; j < count; j ++)
            {
                if (box [i][j] == buf [place])
                {
                    if (box [i + length - 1][j] == buf [length - 1])
                    {
                        sol = checkdown (box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = i + length - 1;
                            for (a = i; a <= ck; a ++)
                            {
                                solution [a][j] = box [a][j];
                            }
                        }
                    }
                    if (box [i + length - 1][j + length - 1] == buf [length - 1])
                    {
                        sol = checkdnright (box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = i + length - 1;
                            b = j;
                            for (a = i; a <= ck; a ++)
                            {
                                solution [a][b] = box [a][b];
                                b++;
                            }
                        }
                    }
                    if (box [i][j + length - 1] == buf [length - 1])
                    {
                        sol = checkright (box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = j + length - 1;
                            a = i;
                            for (b = j; b <= ck; b ++)
                            {
                                solution [a][b] = box [a][b];
                            }
                        }
                    }
                    if (box [i - length + 1][j + length - 1] == buf [length - 1])
                    {
                        sol = checkrightup (box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = j + length - 1;
                            a = i;


                            for (b = j; b <= ck; b ++)
                            {
                                solution [a][b] = box [a][b];
                                a --;
                            }
                        }
                    }
                    if (box [i - length + 1][j] == buf [length - 1])
                    {
                        sol = checkup (box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = i - length + 1;
                            b = j;
                            for (a = i; a >= ck; a --)
                            {
                                solution [a][b] = box [a][b];
                            }
                        }
                    }
                    if (box [i - length + 1][j - length + 1] == buf [length - 1])
                    {
                        sol = checkupleft(box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = i - length + 1;
                            b = j;
                            for (a = i; a >= ck; a --)
                            {
                                solution [a][b] = box [a][b];
                                b --;
                            }
                        }
                    }
                    if (box [i][j - length + 1] == buf [length - 1])
                    {
                        sol = checkleft (box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = j - length + 1;
                            a = i;
                            for (b = j; b >= ck; b --)
                            {
                                solution [a][b] = box [a][b];
                            }
                        }
                    }
                    if (box [i + length - 1][j - length + 1] == buf [length - 1])
                    {
                        sol = checkleftdown(box, buf, i, j, length);
                        if (sol == 1)
                        {
                            ck = i + length - 1;
                            b = j;
                            for (a = i; a <= ck; a ++)
                            {
                                solution [a][b] = box [a][b];
                                b --;
                            }
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < count; i ++)
    {
        for (j = 0; j < count; j ++)
        {
            printf ("%c", solution [i][j]);
            printf (" ");
        }
        printf ("\n");
    }
    return 0;
}

void removespace (char numrow [], int counts)
{
    int i;
    int j;
    int maxi = (counts * 2) - 1;
    int maxj = (counts * 2) - 1;
    for (i = 0; i < maxi; i ++)
    {
        if (numrow [i] == ' ')
        {
            for (j = i; j < maxj; j ++)
            {
                numrow [j] = numrow [j + 1];
            }
            maxj --;
        }
    }
}

int checkdown (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int c = column;
    int solve;
    int w = 0;
    int max = row + maxl - 1;
    for (; row <= max; row ++)
    {
        if (b [row][c] == word [w])
        {
            solve = 1;
            w ++;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}

int checkdnright (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;
    int w = 0;
    int max = row + maxl - 1;
    for (; row <= max; row ++)
    {
        if (row > MAX_MATRIX_SIZE - 1 || column > MAX_MATRIX_SIZE - 1)
        {
            solve = 0;
            break;
        }
        else if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
            column ++;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}


int checkright (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;
    int w = 0;
    int max = column + maxl - 1;
    for (; column <= max; column ++)
    {
        if (column > MAX_MATRIX_SIZE - 1)
        {
            solve = 0;
            break;
        }
        else if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}

int checkrightup (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;
    int w = 0;
    int max = column + maxl - 1;
    for (; column <= max; column ++)
    {
        if (column > MAX_MATRIX_SIZE - 1 || row < 0)
        {
            solve = 0;
            break;
        }
        else if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
            row --;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}


int checkup (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;
    int w = 0;
    int max = row - maxl + 1;
    for (; row >= max; row --)
    {
        if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}


int checkupleft (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;  
    int w = 0;  
    int max = row - maxl + 1;
    for (; row >= max; row --)
    {
        if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
            column --;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}

int checkleft (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;
    int w = 0;
    int max = column - maxl + 1;
    for (; column >= max; column --)
    {
        if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}


int checkleftdown (char b [][MAX_MATRIX_SIZE], char word [], int row, int column, int maxl)
{
    int solve;
    int w = 0;
    int max = row + maxl - 1;
    for (; row <= max; row ++)
    {
        if (b [row][column] == word [w])
        {
            solve = 1;
            w ++;
            column --;
        }
        else
        {
            solve = 0;
            break;
        }
    }
    return solve;
}

