#include <stdlib.h> 
#include <stdio.h> 
#include "list.h"
#include "set.h" 
// #include "stdlib.h"
// #include "common.h"

#define MAXIMUM_ITEMS 10000

struct set
{
    void **array;
    cmpfunc_t cmpfunc;
    int num_items;
    int max_items;
    int sorted; 

};

struct set_iter
{
    set_t *set;
    int current;
};
/*
 * Creates a new set using the given comparison function
 * to compare elements of the set.
 */
set_t *set_create(cmpfunc_t cmpfunc)
{
    set_t *set = malloc(sizeof(set_t)); 
    if(set==NULL)
    {
        return NULL;
    }

    set->cmpfunc = cmpfunc;
    set->max_items = MAXIMUM_ITEMS;
    set->num_items = 0;
    set->array = calloc(set->max_items, sizeof(void*));
    
    return set;

}

/*
 * Destroys the given set.  Subsequently accessing the set
 * will lead to undefined behavior.
 */
void set_destroy(set_t *set)
{
    free(set->array);
    free(set); 


}

void set_sort(set_t *set)
{
    int i, j, min_index; 

    for (i=0; i < set->num_items - 1; i++)
    {
        min_index = i;
        
        for (j = i+1; j < set->num_items; j++)
            {
                if (set->cmpfunc(set->array[min_index],set->array[j]) > 0 )
                    min_index = j; 
            }

            void *tmp = set->array[i];
            set->array[i] = set->array[min_index];
            set->array[min_index] = tmp;

    }

}


/*
 * Returns the size (cardinality) of the given set.
 */
int set_size(set_t *set)
{
    return set->num_items;
}

/*
 * Adds the given element to the given set.
 */
void set_add(set_t *set, void *elem)
{
   
   
    if (set_contains(set, elem) == 1)
    {
        return;
    }
    

    if(set->num_items >= set->max_items)
    {
    
        set->array = realloc(set->array, sizeof(set_t)*set->max_items*2);
        set->max_items = set->max_items*2;   
    }
    
    set->array[set->num_items] = elem;
    set->num_items ++;    
}

/*
 * Returns 1 if the given element is contained in
 * the given set, 0 otherwise.
 */
int set_contains(set_t *set, void *elem)
{
    for(int i = 0; i < set->num_items; i++)
    {
        if(set->cmpfunc(set->array[i],elem)==0)
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
    set_t *union_set = set_create(a->cmpfunc);
    
    set_iter_t *iter_a = set_createiter(a);
    
    while(set_hasnext(iter_a))
    {
        void *item_a = set_next(iter_a);

        set_add(union_set, item_a);
    }
    
    set_iter_t *iter_b = set_createiter(b);
    
    while (set_hasnext(iter_b))
    {
        void *item_b = set_next(iter_b);

        set_add(union_set, item_b);
      
    }

    set_destroyiter(iter_a);
    
    set_destroyiter(iter_b); 


    return union_set;

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

   void *item_a = set_next(iter_a);


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
   
    void *item_a = set_next(iter_a);
   
    while (item_a != NULL)
    {
        
        if (set_contains(b,item_a) == 0)
        {
            set_add(difference_set, item_a);
        }

        item_a = set_next(iter_a); 
    
    }
    set_destroyiter(iter_a);
    return difference_set; 
}

/*
 * Returns a copy of the given set.
 */
set_t *set_copy(set_t *set)
{
    set_t *copied_set = set_create(set->cmpfunc);
   
    copied_set->sorted = set->sorted;
    
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
 * Creates a new set iterator for iterating over the given set.
 */
set_iter_t *set_createiter(set_t *set)
{
    set_iter_t *set_iter = malloc(sizeof(set_iter_t)); 
    
    if(set_iter == NULL)
    {
        return NULL;
    }
    
    set_iter ->set = set; 
    
    set_iter ->current = 0;
    
    return set_iter; 

}

/*
 * Destroys the given set iterator.
 */
void set_destroyiter(set_iter_t *iter)
{
    free (iter);
}


/*
 * Returns 0 if the given set iterator has reached the end of the
 * set, or 1 otherwise.
 */
int set_hasnext(set_iter_t *iter)
{
    int hasnext = iter->current < iter->set->num_items;
    return hasnext;
}

/*
 * Returns the next element in the sequence represented by the given
 * set iterator.
 */
void *set_next(set_iter_t *iter)
{
    if(!set_hasnext(iter))
    {
        return NULL;
    }

    void *elem = iter->set->array[iter->current];
    iter->current ++;
    return elem;
}

