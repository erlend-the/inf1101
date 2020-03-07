/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include "list.h"
#include "set.h"
#include "common.h"

/*
 * Case-insensitive comparison function for strings.
 */
static int compare_words(void *a, void *b)
{
    return strcasecmp(a, b);
}

/*
 * Returns the set of (unique) words found in the given file.
 */
static set_t *tokenize(char *filename)
{
	set_t *wordset = set_create(compare_words);
	list_t *wordlist = list_create(compare_words);
	list_iter_t *it;
	FILE *f;
	
	f = fopen(filename, "r");
	if (f == NULL) {
		perror("fopen");
		fatal_error("fopen() failed");
	}
	tokenize_file(f, wordlist);
	
	it = list_createiter(wordlist);
	while (list_hasnext(it)) {
		void* tmp = list_next(it); 
		set_add(wordset, list_next(it));		
	}
	list_destroyiter(it);
	list_destroy(wordlist);
	return wordset;
}

/*
 * Prints a set of words.
 */
static void printwords(char *prefix, set_t *words)
{
	set_iter_t *it;
	
	it = set_createiter(words);
	printf("%s: ", prefix);
	while (set_hasnext(it)) {
		printf(" %s", set_next(it));
	}
	printf("\n");
	set_destroyiter(it);
}



/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
	char *spamdir, *nonspamdir, *maildir;
	
	if (argc != 4) {
		fprintf(stderr, "usage: %s <spamdir> <nonspamdir> <maildir>\n",
				argv[0]);
		return 1;
	}
	spamdir = argv[1];
	nonspamdir = argv[2];
	maildir = argv[3];
	
	list_t *list = find_files("spam");
	list_iter_t *iter = list_createiter(list); 
	set_t *spamwords = NULL;
	while(list_hasnext(iter))
	{
		char *f =(char *)list_next(iter);
		set_t *set = tokenize(f);
		if(spawnwords == NULL) 
		{
			spamwords = set;
			continue;
		}
		set_t *new = set_intersection(spamwords, tokenize(f));

		set_destroy(spamwords);
		spamwords = new;
	}

	list_destroyiter(iter);
	list_destroy(list);

	list_t *nonspamlist = find_files("nonspam");
	list_iter_t *iter = list_createiter(nonspamlist);
	set_t *nonspam = NULL;
	while(list_hasnext(iter))
	{
		char *f = (char *) list_next(iter);
		set_t *nset = tokenize(f);
		if(nonspam == NULL)
		{
			nonspam = nset;
			continue; 
		}
		set_t *unin = set_union(nonspam, nset);
		set_destroy(nonspam);
		nonspam = unin;
	}
	
	list_destroyiter(iter);
	list_destroy(nonspamlist);

	set_t *triggerwords = set_difference(spawnwords, nonspam);

	list_t *mailfiles = find_files("mail");
	list_iter_t *mailfileiter = list_createiter(mailfiles);
	while(list_hasnext(mailfileiter))
	{
		char *file = (char*) list_next(mailfileiter); 
		set_t *file words = tokenize(file);
		set_t *intersect = set_intersection(file_words,triggerwords);
		printf("%s CONTAINS %d SPAMWORDS(s) AND IS THEREFO", file, set_size(intersect));
		if(set_size(intersect) > 0)
		{
			printf("SPAM");

		}
		else
		{
			printf("NOT SPAM");
		}
		printf("\n");
	}

    return 0;
}
