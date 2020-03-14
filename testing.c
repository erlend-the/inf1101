#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "set.h"

#define NUM_TRIES 5
#define NUM_ITEMS 15000
#define RESULT_DIR "/performancetest/"

int compare_ints(void *a, void *b) {
	/* Compare function from "assert_set.c. */
	int *ia = a;
	int *ib = b;

	return (*ia) - (*ib);
}

void insertItems(set_t *set, int numItems) {
	int i;
	for (i = 0; i < numItems; i++) {
		int *i = malloc(sizeof(int));
		*i = rand();
		set_add(set, i);
	}
}

set_t *setCreate(int numItems) {
	set_t *set = set_create(compare_ints);
	insertItems(set, numItems);
	return set;
}

void *setDestroy(set_t *set) {
	set_destroy(set);
}

double insertionTime(int numTries, int numItems) {
	clock_t startTime, endTime;
	double time = 0;
	int i;

	for (i = 0; i < numTries; i++) {
		set_t *set = setCreate(0);

		startTime = clock();
		insertItems(set, numItems);
		endTime = clock();

		time += (double)(endTime - startTime) / CLOCKS_PER_SEC;

		setDestroy(set);
	}

	return time / numTries;
}

double setUnionTime(int numTries, int numItems) {

	clock_t startTime, endTime;
	double time = 0;
	int i;

	for (i = 0; i < numTries; i++) {
		set_t *set1 = setCreate(numItems);
		set_t *set2 = setCreate(numItems);

		startTime = clock();
		set_t *unionSet = set_union(set1, set2);
		endTime = clock();
		
		time += (double)(endTime - startTime) / CLOCKS_PER_SEC;

		setDestroy(set1);
		setDestroy(set2);
		setDestroy(unionSet);
	}

	return time / numTries;
}

double setIntersectionTime(int numTries, int numItems) {
	clock_t startTime, endTime;
	double time = 0;
	int i;

	for (i = 0; i < numTries; i++) {
		set_t *set1 = setCreate(numItems);
		set_t *set2 = setCreate(numItems);


		startTime = clock();
		set_t *intersectionSet = set_intersection(set1, set2);
		endTime = clock();

		time += (double)(endTime - startTime) / CLOCKS_PER_SEC;

		setDestroy(set1);
		setDestroy(set2);
		setDestroy(intersectionSet);
	}

	return time / numTries;
}

double setDifferanceTime(int numTries, int numItems) {
	clock_t startTime, endTime;
	double time = 0;
	int i;

	for (i = 0; i < numTries; i++) {
		set_t *set1 = setCreate(numItems);
		set_t *set2 = setCreate(numItems);


		startTime = clock();
		set_t *differenceSet = set_difference(set1, set2);
		endTime = clock();

		time += (double)(endTime - startTime) / CLOCKS_PER_SEC;

		setDestroy(set1);
		setDestroy(set2);
		setDestroy(differenceSet);
	}

	return time / numTries;
}

double setCopyTime(int numTries, int numItems) {
	clock_t startTime, endTime, dt;
	double time = 0;
	int i;

	for (i = 0; i < numTries; i++) {
		set_t *set = setCreate(numItems);

		startTime = clock();
		set_t *copySet = set_copy(set);
		endTime = clock();

		time += (double)(endTime - startTime) / CLOCKS_PER_SEC;

		setDestroy(set);
		setDestroy(copySet);
	}

	return time / numTries;
}

double setIterationTime(int numTries, int numItems) {
	clock_t startTime, endTime, dt;
	double time = 0;
	int i;

	for (i = 0; i < numTries; i++) {
		set_t *set = setCreate(numItems);
		set_iter_t *iter = set_createiter(set);

		startTime = clock();
		while (set_hasnext(iter)) {
			set_next(iter);
		}
		endTime = clock();

		time += (double)(endTime - startTime) / CLOCKS_PER_SEC;

		setDestroy(set);
		set_destroyiter(iter);
	}

	return time / numTries;
}

int main (int argc, char **argv) {
	
	double insertTime = insertionTime(NUM_TRIES, NUM_ITEMS);
	double unionTime = setUnionTime(NUM_TRIES, NUM_ITEMS);
	double intersectionTime = setIntersectionTime(NUM_TRIES, NUM_ITEMS);
	double differenceTime = setDifferanceTime(NUM_TRIES, NUM_ITEMS);
	double copyTime = setCopyTime(NUM_TRIES, NUM_ITEMS);
	double iterationTime = setIterationTime(NUM_TRIES, NUM_ITEMS);

	FILE *fp = NULL;
	char dir[] = "performancetest/";
	char fileName[] = "arrayresult.csv";
	strcat(dir, fileName);

	fp = fopen(dir, "w");

	fprintf(fp, "LIST SET UNSORTED FULL RESULT\n\
	Tries: %d	Number of items: %d\n\n", NUM_TRIES, NUM_ITEMS);

	fprintf(fp, "Insert time: %f\n\
	Union time: %f\n\
	Intersection time: %f\n\
	Difference time: %f\n\
	Copy time: %f\n\
	Iteration time: %f\n",
	insertTime, unionTime, intersectionTime, differenceTime, copyTime, iterationTime);

	fclose(fp);

	return 0;
}
