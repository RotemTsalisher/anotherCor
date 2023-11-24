#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

#define MAX_MAT_SIZE 1000

typedef struct {
	int mat[MAX_MAT_SIZE];
	int n, m;
}s_mat;

void initMask(s_mat* testMat,int k);
void initMask_xor(s_mat* mask, int k);
void printMat(s_mat* mat);
void printRealIdx(s_mat* mat, int k);
void initTestMat(s_mat* testMat);
void padMat(s_mat* mat, int xpad, int ypad);
void unpadMat(s_mat* mat, int xpad, int ypad);
int wSum(int* m1, int* m2, int n_m1, int m_m1, int n_m2, int m_m2);
void corr2D(s_mat* corr, s_mat* mat, s_mat* mask);
void occupiedSpots(s_mat* original, s_mat* corr);
int findMax(s_mat* mat);

void main()
{
	s_mat m1, m2, m3;
	int max;
	printf("m1 address = %p, m2 address = %p, m3 address = %p\n", &m1, &m2, &m3);
	initMask_xor(&m1,2);
	initTestMat(&m2);
	corr2D(&m3, &m2, &m1);
	printf("mask\n:"); printMat(&m1);
	printf("testMat\n:"); printMat(&m2);
	printf("corr:\n"); printMat(&m3);
	occupiedSpots(&m2, &m3);
	printf("after occupied spots:\n"); printMat(&m3);
	max = findMax(&m3);
	printf("max corr value  = %d\n", max);
}
void initMask_xor(s_mat* mask, int k)
{
	int i, j, offset;
	mask->n = (k << 1) + 1;
	mask->m = (k << 1) + 1;

	offset = k;
	for (i = 0; i < mask->n; i++)
	{
		for (j = 0; j < mask->m; j++)
		{
			if (
				(((i - offset) < 0  && (j - offset) < 0) && (i + j - (offset << 1) >= -k)) ^
				(((i - offset) < 0) && ((j - i <= k)))                                     ^
				(((j - offset) < 0) && (i - j <= k))                                       ^
				((i - offset) >= 0 && (j - offset) >= 0) && (i + j - (offset << 1) <= k)  )
			{
				*(mask->mat + mask->m * i + j) = 1;
			}
			else
			{
				*(mask->mat + mask->m * i + j) = 0;
			}
		}
	}
}
void printMat(s_mat* mat)
{
	int i, j;

	printf("PRINTING %dx%d MAT:\n", mat->n, mat->m);
	for (i = 0; i < mat->n; i++)
	{
		for (j = 0; j < mat->m; j++)
		{
			printf("| %d |", *(mat->mat + mat->m * i + j));
		}
		printf("\n");
	}
}
void printRealIdx(s_mat* mat, int k)
{
	int i, j;

	for (i = 0; i < mat->n; i++)
	{
		for (j = 0; j < mat->m; j++)
		{
			printf("| (%2d,%2d) |", (i - k) , (j - k));
		}
		printf("\n");
	}
}
void initMask(s_mat* mask, int k)
{
	int i, j, offset;
	mask->n = (k << 1) + 1;
	mask->m = (k << 1) + 1;

	offset = k;
	for (i = 0; i < mask->n; i++)
	{
		for (j = 0; j < mask->m; j++)
		{
			if ((i - offset) < 0 && (j - offset) < 0)
			{
				if (i + j - (offset << 1) >= -k)
				{
					*(mask->mat + mask->m * i + j) = 1;
				}
				else
				{
					*(mask->mat + mask->m * i + j) = 0;
				}
			}
			else if ((i - offset) < 0)
			{
				if (j - i <= k)
				{
					*(mask->mat + mask->m * i + j) = 1;
				}
				else
				{
					*(mask->mat + mask->m * i + j) = 0;
				}
			}
			else if ((j - offset) < 0)
			{
				if (i - j <= k)
				{
					*(mask->mat + mask->m * i + j) = 1;
				}
				else
				{
					*(mask->mat + mask->m * i + j) = 0;
				}
			}
			else
			{
				if (i + j - (offset << 1) <= k)
				{
					*(mask->mat + mask->m * i + j) = 1;
				}
				else
				{
					*(mask->mat + mask->m * i + j) = 0;
				}
			}
		}
	}
}
void initTestMat(s_mat* testMat)
{
	testMat->n = 3;
	testMat->m = 4;

	*(testMat->mat     ) = 1;
	*(testMat->mat +  1) = 0;
	*(testMat->mat +  2) = 0;
	*(testMat->mat +  3) = 0;
	*(testMat->mat +  4) = 0;
	*(testMat->mat +  5) = 1;
	*(testMat->mat +  6) = 1;
	*(testMat->mat +  7) = 0;
	*(testMat->mat +  8) = 0;
	*(testMat->mat +  9) = 0;
	*(testMat->mat + 10) = 0;
	*(testMat->mat + 11) = 1;
}
void padMat(s_mat* mat, int xpad, int ypad)
{
	int i,j, new_n, new_m;

	new_n = mat->n + (ypad<<1);
	new_m = mat->m + (xpad<<1);

	for (i = (new_n - 1); i > -1; i--)
	{
		for (j = (new_m - 1); j > -1; j--)
		{
			if ((i > (mat->n-1) + ypad) || (j > (mat->m-1) + xpad) || (i < ypad) || (j < xpad))
			{
				*(mat->mat + new_m * i + j) = 0;
			}
			else
			{
				*(mat->mat + new_m * i + j) = *(mat->mat + mat->m * (i - ypad) + (j - xpad));
			}
		}
	}
	mat->n = new_n;
	mat->m = new_m;
}
void unpadMat(s_mat* mat, int xpad, int ypad)
{
	int i, j, new_n, new_m;

	new_n = mat->n - (ypad << 1);
	new_m = mat->m - (xpad << 1);

	for (i = 0; i < mat->n; i++)
	{
		for (j = 0; j < mat->m; j++)
		{
			if ((i < new_n) || (j < new_m))
			{
				*(mat->mat + new_m * i + j) = *(mat->mat + mat->m * (i + ypad) + (j + xpad));
			}
			else
			{
				*(mat->mat + new_m * i + j) = 0;
			}
		}
	}
	mat->m = new_m;
	mat->n = new_n;
}
int wSum(int* m1, int* m2, int n_m1, int m_m1, int n_m2, int m_m2)
{
	int i, j, sum, xdim, ydim;

	ydim = (n_m1 < n_m2) ? n_m1 : n_m2;
	xdim = (m_m1 < m_m2) ? m_m1 : m_m2;

	sum = 0;
	for (i = 0; i < ydim; i++)
	{
		for (j = 0; j < xdim; j++)
		{
			sum += ((*(m1 + m_m1 * i + j)) * (*(m2 + m_m2 * i + j)));
		}
	}
	return sum;
}
void corr2D(s_mat* corr, s_mat* mat, s_mat* mask)
{
	int i, j, xpad, ypad;


	corr->n = mat->n;
	corr->m = mat->m;
	xpad = (mask->m - 1) >> 1;
	ypad = (mask->n - 1) >> 1;

	padMat(mat, xpad, ypad);

	for (i = 0; i < corr->n; i++)
	{
		for (j = 0; j < corr->m; j++)
		{
			*(corr->mat + corr->m * i + j) = wSum((mat->mat + mat->m * i + j), mask->mat, mat->n, mat->m, mask->n, mask->m);
		}
	}
	unpadMat(mat, xpad, ypad);
}
void occupiedSpots(s_mat* original, s_mat* corr)
{
	int i, j;

	for (i = 0; i < original->n; i++)
	{
		for (j = 0; j < original->m; j++)
		{
			if ( (*(original->mat + original->m * i + j)) & 0x01)
			{
				*(corr->mat + corr->m * i + j) &= 0x00;
			}
		}
	}
}
int findMax(s_mat* mat)
{
	int i, j, max;

	max = *(mat->mat);

	for (i = 0; i < mat->n; i++)
	{
		for (j = 0; j < mat->m; j++)
		{
			if (*(mat->mat + mat->m * i + j) > max)
			{
				max = *(mat->mat + mat->m * i + j);
			}
		}
	}
	return max;
}