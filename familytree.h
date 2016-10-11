
#ifndef FAMILYTREE_H
#define	FAMILYTREE_H

#include "familyRunner.h"
#include "QuadraticProbing.h"
#include "QueueAr.h"


class FamilyTree
{
public:
    QuadraticHashTable<Person2> *hashTable; //need to give it a a const &notfound  and an int size. person2 has a default constructor
    int createUniqueID(Person);
    int people;
    Person2 compare(); //not finish this
    QuadraticHashTable<Person2>* findParentIndex(Person2, QuadraticHashTable<Person2>*);
    
    FamilyTree(Family *families, int familyCount);
    void runQueries(Query *queries, Person *answers, int queryCount);
};

/*
class Array1
{
public:
    Array1 *Q1 = new Array1[100000];
    int front;
    int back;
    //front=back=0;
};

class Array2
{
public:
    Array2 *Q2 = new Array2[100000];
    int front;
    int back;
    //front=back=0;
 
};
*/


#endif	/* FAMILYTREE_H */
