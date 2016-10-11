#include "royals.h"
#include "RunRoyals.h"
#include "QuadraticProbing.h"
#include <cstring>
#include <iostream>

using namespace std;

/* Weiss quicksort (pg 286) */
template <typename Object>
void quicksort(vector<Object> & a, int left, int right )
{
    if(left + 10 <= right )
    {
        int center = (left + right) / 2;
        if(a[center] < a[left])
            swap(a[left],a[center]);
        if(a[right] < a[left])
            swap(a[left],a[right]);
        if(a[right] < a[center])
            swap(a[center],a[right]);
        
        swap(a[center],a[right-1]);
        Object pivot = a[right-1];
        
        int i = left, j = right - 1;
        for( ;; )
        {
            while(a[++i] < pivot ) { }
            while(pivot < a[--j] ) { }
            if(i < j)
                swap( a[i], a[j] );
            else
                break;
        }
        
        swap( a[i], a[ right - 1] ); //Restore pivot
        
        quicksort( a, left, i - 1 ); //Sort small elements
        quicksort( a, i + 1, right ); //Sort large elements
    }
    else
        insertionSort( a, left, right );
}

/* Weiss insertionSort * (pg 282) */
template <typename Object>
void insertionSort(vector<Object> &a, int left, int right)
{
    int j;
    
    for(int p = left; p <= right; p++)
    {
        Object tmp = a[p];
        for( j = p; j > 0 && tmp < a[ j - 1]; j--)
            a[ j ] = a[ j-1 ];
        a[ j ] = tmp;
    }
}

Royal::Royal()
{
    name[0] = '\0';
    birthYear = -1;
}

Royal::Royal(const Person &x)
{
    strncpy(name,x.name,100);
    birthYear = x.birthYear;
    spouseCount = x.spouseCount;
    n_child = 0;
    n_parent = 0;
    n_ancest = -1;
}

Royals::Royals(const Person *people, int count) : hashTable(count*2)
{
    int i = 0;
    insertDriver(people,NULL,i,count);
}  // Royals()

void Royals::insertDriver(const Person *people, const Person *parent, int &i, int count)
{
    Royal * r_parent = NULL;
    if(parent != NULL)
    {
        r_parent = hashTable.array[hashTable.findObject(parent->name,parent->birthYear)].element;
    }
    Person root = people[i]; //root to recurse from
    bool quit = 0;
    while(!quit && i < count)
    {
        int length_i = strlen(people[i].ID);
        int length_r = strlen(root.ID);
        if((length_i-1) > length_r) //new root (-1 in case 9->10)
        {
            insertDriver(people, &people[i-1], i, count); //people[i-1] is parent
        }
        else if(length_i >= length_r) //siblings or equal root
        {
            if(hashTable.findObject(people[i].name,people[i].birthYear) == -1) //not found
            {
                hashTable.insert(new Royal(people[i]));
                Royal * child = hashTable.array[hashTable.findObject(people[i].name,people[i].birthYear)].element;
                if(r_parent != NULL) //if not root
                {
                    r_parent->children[r_parent->n_child] = child;
                    r_parent->n_child++;
                    child->parents[0] = r_parent; //parents = 0 if empty
                    child->n_parent++;
                }
            }
            else //parents > 0
            {
                Royal * child = hashTable.array[hashTable.findObject(people[i].name,people[i].birthYear)].element;
                if((child->parents[0] != r_parent) && (child->n_parent != 2)) //not duplicate
                {
                    r_parent->children[r_parent->n_child] = child;
                    r_parent->n_child++;
                    child->parents[1] = r_parent;
                    child->n_parent++;
                }
            }
            i++;
        }
        else //less, recurse back
        {
            quit = 1;
        }
    }
}


void Royals::getAncestor(const char *descendentName1, int descendentBirthYear1,
                         const char *descendentName2, int descendentBirthYear2,
                         const char **ancestorName, int *ancestorBirthYear)
{
    int pos_d1 = hashTable.findObject(descendentName1,descendentBirthYear1);
    Royal * d1 = hashTable.array[pos_d1].element;
    int pos_d2 = hashTable.findObject(descendentName2,descendentBirthYear2);
    Royal * d2 = hashTable.array[pos_d2].element;
    //vector<Royal *> list1; //list of ancestors
    //vector<Royal *> list2;
    int i1 = 0, i2 = 0;
    if(d1->n_ancest == -1) //not instalized
    {
        getAncestorDriver(d1,&d1->ancestors,i1,1);
        quicksort(d1->ancestors,0,i1-1);
        d1->n_ancest = i1;
    }
    else
        i1 = d1->n_ancest;
    if(d2->n_ancest == -1)
    {
        getAncestorDriver(d2,&d2->ancestors,i2,1);
        quicksort(d2->ancestors,0,i2-1);
        d2->n_ancest = i2;
    }
    else
        i2 = d2->n_ancest;
    
    *ancestorBirthYear = 0;
    
    int m1 = 0, m2 = 0;
    while((m1 != i1) && (m2 != i2))
    {
        Royal * tmp1 = d1->ancestors[m1];
        Royal * tmp2 = d2->ancestors[m2];
        
        if(tmp1 == tmp2)
        {
            if(tmp1->birthYear > *ancestorBirthYear)
            {
                *ancestorBirthYear = tmp1->birthYear;
                *ancestorName = tmp1->name;
            }
            
            m1++;
            m2++;
        }
        else if(tmp1 < tmp2)
        {
            m1++;
        }
        else
        {
            m2++;
        }
    }
    
} // getAncestor()

void Royals::getAncestorDriver(Royal * d, vector<Royal *> *a, int & index, bool first)
{
    if(d->n_parent == 2)
    {
        getAncestorDriver(d->parents[0],a,index,0);
        getAncestorDriver(d->parents[1],a,index,0);
    }
    else if(d->n_parent == 1)
    {
        getAncestorDriver(d->parents[0],a,index,0);
    }
    if(first != 1) //don't insert yourself
    {
        (*a)[index]=d;
        index++;
    }
}

int Royals::getChildren(const char *name, int birthYear)
{
    int tmp = hashTable.findObject(name, birthYear);
    return hashTable.array[tmp].element->n_child;
} // getSiblings()


void Royals::getDescendent(const char *ancestorName, int ancestorBirthYear,
                           const char **descendentName, int *descendentBirthYear)
{
    int pos_a = hashTable.findObject(ancestorName,ancestorBirthYear);
    Royal * a = hashTable.array[pos_a].element;
    int n_c = a->n_child;
    *descendentBirthYear = 0; //zero out
    if(n_c == 0)
    {
        *descendentName = a->name;
        *descendentBirthYear = a->birthYear;
    }
    else
    {
        for(int i = 0; i < n_c; i++) // we always have descendents
        {
            getDescendentDriver(a->children[i],descendentName,descendentBirthYear);
        }
    }
} //getDescedent()

void Royals::getDescendentDriver(Royal * ancestor, const char ** descendentName, int * descendentBirthYear)
{
    int n_c = ancestor->n_child;
    if(ancestor->birthYear > *descendentBirthYear)
    {
        *descendentName = ancestor->name;
        *descendentBirthYear = ancestor->birthYear;
    }
    
    if(n_c != 0)
    {
        for(int i = 0; i < n_c; i++) // more descendents to check
        {
            getDescendentDriver(ancestor->children[i],descendentName,descendentBirthYear);
        }
    }
}
