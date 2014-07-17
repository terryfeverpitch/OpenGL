#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define BK 4

int save[10];

int main()
{
	for(int i = 0 ; i < 10 ; i++)
		cout << save[i] << endl;
/*	int    bonusKinds[BK] = {1,   2,    -2,   4};		// bonus kinds
	double prob[BK]		  = {0.5, 0.25, 0.15, 0.1};	    // each probability
	double cprob[BK];									// cumulative probability
	double random;
	int i, index;
int irandom;

	srand(time(NULL));
	cprob[0] = prob[0];
	for(i = 1 ; i < BK ; i++)
		cprob[i] = cprob[i - 1] + prob[i];

	for(i = 0 ; i < 10 ; i++) 
	{
		irandom = rand();
        //random = ((double)irandom) / (RAND_MAX);
        random = (irandom % 1000)/999.;
        for(index = 0 ; index < BK ; index++)
		{
            if(random <= cprob[index]) 
				break;
		}

//cout << irandom << endl;
		cout << random << endl;
		cout << bonusKinds[index] << endl;

    }*/
    return 0;
}