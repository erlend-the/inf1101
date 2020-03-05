#include <stdlib.h> 
#include <stdio.h> 
#include "list.h"
#include "set.h" 


/*
 * The type of sets.
 */

struct set 
{
    list_t *list;
    cmpfunc_t *cmpfunc;
    int issorted;
};

/*
 * Creates a new set using the given comparison function
 * to compare elements of the set.
 */
set_t *set_create(cmpfunc_t cmpfunc)
{
    set_t *set = malloc(sizeof(set_t));
    if(set == NULL)
    
        return NULL;
    
    int issorted = 0;
   
    set->cmpfunc = cmpfunc; 

    set->list = list_create(cmpfunc);
    
    if(set->list == NULL)
    {
        return NULL;
    }
    
    return set; 


}


/*
 * Destroys the given set.  Subsequently accessing the set
 * will lead to undefined behavior.
 */
void set_destroy(set_t *set)
    {
        list_destroy(set->list);

        free(set); 
    } 

void set_sort(set_t * set)
{
    set -> issorted = 1; 
    list_sort(set->list);
}

/*
 * Returns the size (cardinality) of the given set.
 */
int set_size(set_t *set)
{
    return list_size(set->list);
}

/*
 * Adds the given element to the given set.
 */
void set_add(set_t *set, void *elem)
{
    if (set_contains(set,elem) == 1)
    {
        return;
    }

    list_addlast(set->list, elem);
}

/*
 * Returns 1 if the given element is contained in
 * the given set, 0 otherwise.
 */
int set_contains(set_t *set, void *elem)
{
    if (list_contains(set->list, elem) == 1)
    {
        return 1;
    }

    return 0; 
    
}   




/*
 * Returns the union of the two given sets; the returned
 * set contains all elements that are contained in either
 * a or b.
 */
set_t *set_union(set_t *a, set_t *b)
{
    set_t *unionset = set_create(a->cmpfunc);
    set_iter_t *iter_a = set_createiter(a);
    set_iter_t *iter_b = set_createiter(b); 
    while(set_hasnext(iter_a))
    {
        void *item_a = set_next(iter_a);

        set_add(unionset, item_a);
    }

    while (set_hasnext(iter_b))
    {
        void *item_b = set_next(iter_b);

        set_add(unionset, item_b);
      
    }

    set_destroyiter(iter_a);
    set_destroyiter(iter_b); 


    return unionset;

}

/*
 * Returns the intersection of the two given sets; the
 * returned set contains all elements that are contained
 * in both a and b.
 */
set_t *set_intersection(set_t *a, set_t *b)
{
   set_t *intersection_set = set_create(a->cmpfunc);
   
   set_iter_t *iter_a = set_createiter(a);

   void *item_a = set_next(item_a);


    while(item_a != NULL)
    {
        if (set_contains(b, item_a) == 1)
        {
            set_add(intersection_set, item_a);
        }
        
        item_a = set_next(iter_a); 

    }
    
    set_destroyiter(iter_a);
    return intersection_set; 
}


/*
 * Returns the set difference of the two given sets; the
 * returned set contains all elements that are contained
 * in a and not in b.
 */
set_t *set_difference(set_t *a, set_t *b)
{
    set_t *difference_set = set_create(a->cmpfunc);

    set_t *iter_a = set_createiter(a);
    set_t *iter_b = set_createiter(b);
    void *item_a = set_next(iter_a);

    while (item_a != NULL)
    {
        if (set_contains(b,item_a) == 0)
        {
            set_add(difference_set, item_a);
        }
    
        item_a  = set_next(iter_b);
    }
    set_destroyiter;
    return difference_set; 
}

/*
 * Returns a copy of the given set.
 */
set_t *set_copy(set_t *set)
{
    set_t *copied_set = set_create(set->cmpfunc);
    copied_set->issorted = set->issorted;
    set_t *iter = set_createiter(set);
    void *item = set_next(item);

    while (item != NULL)
    {
        set_add(copied_set, item);
        item = set_next(iter);
    }

    return copied_set; 
}

/*
 * The type of set iterators.
 */
struct set_iter
{
    list_iter_t *iter;
};


/*
 * Creates a new set iterator for iterating over the given set.
    Should look like list iter, except it is ordered set so it should be ascending order 
 */
set_iter_t *set_createiter(set_t *set)
{
    set_sort(set);
    set_iter_t *iterset = malloc(sizeof(set_iter_t));

    if (iterset == NULL)
    {
	    return NULL;
    }

    list_iter_t *iterlist = list_createiter(set->list);
    if (iterlist == NULL)
    {
        free(iterset);
        return NULL;

    }
    
    iterset->iter = iterlist;
    return iterset;
    
}

/*
 * Destroys the given set iterator.
 */
void set_destroyiter(set_iter_t *iter)
{
    list_destroy(iter->iter);
    free(iter);
}

/*
 * Returns 0 if the given set iterator has reached the end of the
 * set, or 1 otherwise.
 */
int set_hasnext(set_iter_t *iter)
{
    return list_hasnext(iter->iter);
}

/*
 * Returns the next element in the sequence represented by the given
 * set iterator.
 */
void *set_next(set_iter_t *iter)
{
    return list_next(iter->iter);
}


