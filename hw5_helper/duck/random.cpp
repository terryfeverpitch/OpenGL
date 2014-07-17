#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BK 4

int save[15] = {1};

void generateRandom()
{
	int    bonusKinds[BK] = {1,   2,    -2,   4};		
	double prob[BK]		  = {0.5, 0.25, 0.15, 0.1};	
	double cprob[BK];									
	double random;
	int i, index;

	srand(time(NULL));

	cprob[0] = prob[0];
	for(i = 1 ; i < BK ; i++)
		cprob[i] = cprob[i - 1] + prob[i];

	for(i = 0 ; i < 15 ; i++) 
	{
        random = rand() / (RAND_MAX + 1.0);
        
        for(index = 0 ; index < BK ; index++)
		{
            if(random <= cprob[index]) 
				break;
		}
		save[i] = bonusKinds[index];
    }
}

int getRandom(int duckName)
{
	if (save[duckName] == NULL)
		return 1;

	return save[duckName];
}