#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>


#include "set.h"

#define NUMBER_OF_TRIES 5
#define NUMBER_OF_ITEMS 15000
#define RESULT_DIRECTORY "/performancetest/"



int cmpint(void *a, void *b) {
	/* Compare function from "assert_set.c. */
	int *inta = a;
	int *intb = b;
	return (*inta) - (*intb);
}

void insert_items(set_t *set, int number_of_items) {
	int i;
	for (i = 0; i < number_of_items; i++) {
		int *i = malloc(sizeof(int));
		*i = rand();
		set_add(set, i);
	}
}

set_t *setCreate(int number_of_items) {
	set_t *set = set_create(cmpint);
	insert_items(set, number_of_items);
	return set;
}

void *set_destruct (set_t *set) {
	set_destroy(set);
}

double insertionTime(int number_of_tries, int number_of_items) {
	clock_t start, stop;
	double sumoftime = 0;
	int i;

	for (i = 0; i < number_of_tries; i++) {
		set_t *set = setCreate(0);

		start = clock();
		insert_items(set, number_of_items);
		stop = clock();

		sumoftime += (double)(stop - start) / CLOCKS_PER_SEC;

		set_destruct (set);
	}

	return sumoftime / number_of_tries;
}

double setUnionTime(int number_of_tries, int number_of_items) {

	clock_t start, stop;
	double time = 0;
	int i;

	for (i = 0; i < number_of_tries; i++) {
		set_t *set1 = setCreate(number_of_items);
		set_t *set2 = setCreate(number_of_items);

		start = clock();
		set_t *unionSet = set_union(set1, set2);
		stop = clock();
		
		time += (double)(stop - start) / CLOCKS_PER_SEC;

		set_destruct (set1);
		set_destruct (set2);
		set_destruct (unionSet);
	}

	return time / number_of_tries;
}

double setIntersectionTime(int number_of_tries, int number_of_items) {
	clock_t start, stop;
	double time = 0;
	int i;

	for (i = 0; i < number_of_tries; i++) {
		set_t *set1 = setCreate(number_of_items);
		set_t *set2 = setCreate(number_of_items);


		start = clock();
		set_t *intersectionSet = set_intersection(set1, set2);
		stop = clock();

		time += (double)(stop - start) / CLOCKS_PER_SEC;

		set_destruct (set1);
		set_destruct (set2);
		set_destruct (intersectionSet);
	}

	return time / number_of_tries;
}

double setDifferanceTime(int number_of_tries, int number_of_items) {
	clock_t start, stop;
	double time = 0;
	int i;

	for (i = 0; i < number_of_tries; i++) {
		set_t *set1 = setCreate(number_of_items);
		set_t *set2 = setCreate(number_of_items);


		start = clock();
		set_t *differenceSet = set_difference(set1, set2);
		stop = clock();

		time += (double)(stop - start) / CLOCKS_PER_SEC;

		set_destruct (set1);
		set_destruct (set2);
		set_destruct (differenceSet);
	}

	return time / number_of_tries;
}

double setCopyTime(int number_of_tries, int number_of_items) {
	clock_t start, stop, dt;
	double time = 0;
	int i;

	for (i = 0; i < number_of_tries; i++) {
		set_t *set = setCreate(number_of_items);

		start = clock();
		set_t *copySet = set_copy(set);
		stop = clock();

		time += (double)(stop - start) / CLOCKS_PER_SEC;

		set_destruct (set);
		set_destruct (copySet);
	}

	return time / number_of_tries;
}

double setIterationTime(int number_of_tries, int number_of_items) {
	clock_t start, stop, dt;
	double time = 0;
	int i;

	for (i = 0; i < number_of_tries; i++) {
		set_t *set = setCreate(number_of_items);
		set_iter_t *iter = set_createiter(set);

		start = clock();
		while (set_hasnext(iter)) {
			set_next(iter);
		}
		stop = clock();

		time += (double)(stop - start) / CLOCKS_PER_SEC;

		set_destruct (set);
		set_destroyiter(iter);
	}

	return time / number_of_tries;
}

int main (int argc, char **argv) {

	if(argc != 2) {
        printf("USAGE: performancetest {target file}");
        return 0;
    }

	double insertTime = insertionTime(NUMBER_OF_TRIES, NUMBER_OF_ITEMS);
	double unionTime = setUnionTime(NUMBER_OF_TRIES, NUMBER_OF_ITEMS);
	double intersectionTime = setIntersectionTime(NUMBER_OF_TRIES, NUMBER_OF_ITEMS);
	double differenceTime = setDifferanceTime(NUMBER_OF_TRIES, NUMBER_OF_ITEMS);
	double copyTime = setCopyTime(NUMBER_OF_TRIES, NUMBER_OF_ITEMS);
	double iterationTime = setIterationTime(NUMBER_OF_TRIES, NUMBER_OF_ITEMS);

	FILE *fp = NULL;
	char directory[] = "/performancetest/";
	char fileName[] = "array.csv";
	strcat(directory, fileName);

	fp = fopen(directory, "w");

	fprintf(fp, "LIST SET UNSORTED FULL RESULT\n\
	Tries: %d	Number of items: %d\n\n", NUMBER_OF_TRIES, NUMBER_OF_ITEMS);

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
