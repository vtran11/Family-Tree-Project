#include "familytree.h"
#include "familyRunner.h"
#include "QuadraticProbing.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;


FamilyTree::FamilyTree(Family *families, int familyCount)//:hashTable(Person2(),familyCount*2) //familycount is the total number of families passed to the constructor via the *families array
{
    Person2 wrapper;
    wrapper.ID = 0;   //null
    
    hashTable= new QuadraticHashTable<Person2> (wrapper, familyCount* 2);
    
    for(int i = 0; i < familyCount; i++)
    {
        Family family = families[i];
        
        Person nullParent;
        nullParent.firstName[0] = '\0';
        nullParent.lastName[0] = '\0';
        nullParent.year = 0;
        nullParent.gender = '\0';
        
        wrapper.ID = createUniqueID(family.person);
        if(hashTable->find(family.person).ID <=  0)
        {
            wrapper.person = family.person;
            wrapper.parent[0] = nullParent;
            wrapper.parent[1] = nullParent;
            hashTable->insert(wrapper);
            
            people++;
        }
        
        wrapper.ID = createUniqueID(family.spouse) ;
        if(hashTable->find(family.person).ID <= 0 && family.spouse.year != -1)
        {
            wrapper.person = family.spouse;
            wrapper.parent[0] = nullParent;
            wrapper.parent[1] = nullParent;
            hashTable->insert( wrapper);
            
            people++;
        }
        
        for(int n = 0; n < family.childCount; n++) {
            wrapper.ID = createUniqueID(family.children[n]);
            wrapper.person = family.children[n];
            wrapper.parent[0] = family.person;
            wrapper.parent[1] = family.spouse;
            
            hashTable->insert(wrapper);
            
            people++;
        }
    }
}

void FamilyTree::runQueries(Query *queries, Person *answers, int queryCount)
{

    Person2 temp;
    temp.person.year = -1;
    
    for(int i = 0; i < queryCount; i++) {
        
        QuadraticHashTable<Person2> *ht = new QuadraticHashTable<Person2>(temp, 50000);
        Person2 pw = hashTable->find(queries[i].person1);
        
        // Get ancestors for person 1
        QuadraticHashTable<Person2> *ansc1 = findParentIndex(pw, ht);
        
        // Find youngest common ancestor
        pw = hashTable->find(queries[i].person2);
        temp.person.year = -1;
        answers[i] = compare().person;   ///need to create compare************
        
        delete(ht);
        //ht.makeEmpty();
    }
}  // runQueries()

int FamilyTree:: createUniqueID(Person p)
{
    int ID = 0;
    
    for (int i=0 ; i< strlen(p.lastName); i++)
    {
        ID = (ID*131) + p.lastName[i];
    }
    
    for (int i=0; i< strlen(p.firstName); i++)
    {
        ID = (ID * 131) + p.firstName[i];
    }
    
    ID = ID + p.year*37;
    ID = ID + p.gender*131;
    
    if(ID < 0)
        ID = - ID;
    
    return ID;
}

QuadraticHashTable<Person2>* FamilyTree::findParentIndex(Person2 p, QuadraticHashTable<Person2> *t)
{
    if ( p.ID <=0)
        return t;
    
    Person2 p1 = hashTable->find(p.parent[0]);
    Person2 p2 = hashTable->find(p.parent[1]);
    
    t->insert( p);
    findParentIndex(p1, t);
    findParentIndex(p2, t);
    
    return t;
}


Person2 FamilyTree::compare()
{
  
}