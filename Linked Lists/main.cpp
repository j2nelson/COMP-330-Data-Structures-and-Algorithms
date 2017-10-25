#include <iostream>
#include <string>
#include<fstream>
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <time.h>

using namespace std;

//linked list element for next words
class NextWordElement
{
public:
        NextWordElement(string);
        string word2;
        int count;
        NextWordElement* next2;     //next element
};

NextWordElement::NextWordElement(string w)
{
        word2= w;
        next2=NULL;      //make it a last element by default
}

//linked list for next words
class NextWordList
{
public:
        NextWordList();
        void add(string keyword2);
        void printlist();
        NextWordElement* first2;
        NextWordElement* findWord(string targ);
        void foundNextWord(string nextword);
        string getRandomWord();
};

//linked list element
class KeyWordElement
{
public:
        KeyWordElement(string);
        string word;
        NextWordList* nextwordlist;
        KeyWordElement* next;     //next element
};

KeyWordElement::KeyWordElement(string initialvalue)
{
        word = initialvalue;
        nextwordlist = new NextWordList();
        next = NULL;      //make it a last element by default
}

//linked list
class KeyWordList
{
public:
        KeyWordList();
        void foundNextWord(string keyword, string nextword);
        string getRandomNextWord(string keyword);
        string getRandomKeyword();
        void add(string keyword);
        void printlist();
        void addUnique(string keyword);
        KeyWordElement* findWord(string target);
        KeyWordElement* first;
};

//makes an empty linked list
KeyWordList::KeyWordList()
{
    first=NULL;
}

//adds a word element to the list
void KeyWordList::add(string keyword)
{
    //encapsulate keyword as an element
    KeyWordElement* newelement = new KeyWordElement(keyword);
    newelement->next=first;
    first = newelement;
}

//prints out every list element on its own line
void KeyWordList::printlist()
{
    KeyWordElement* currentValue = first;

    while(currentValue!=NULL)
    {
        cout << currentValue->word << " - " << endl;
        currentValue->nextwordlist->printlist();
        cout << endl;
        currentValue=currentValue->next;
    }
}

//searches the list for a word and returns its element if it finds it or NULL if it does not
KeyWordElement* KeyWordList::findWord(string target)
{
    KeyWordElement* current=first;
    while(current!=NULL)
    {
        if(current->word == target)
            return current;
        current=current->next;
    }
    return current;
}

//first calls findWord and then only calls add if findWord returned NULL
void KeyWordList::addUnique(string keyword)
{
    if(findWord(keyword)== NULL)
    {
        add(keyword);
    }
}


void KeyWordList::foundNextWord(string keyword, string nextword)
{
    addUnique(keyword);
    KeyWordElement* e = findWord(keyword);
    e->nextwordlist->foundNextWord(nextword);
}

string KeyWordList::getRandomKeyword()
{
    //figure out # words
    int keytotal=0;
    KeyWordElement* c = first;
    while(c!=NULL)
    {
        keytotal++;
        c = c->next;
    }
    int choice = rand()%keytotal;
    keytotal = 0;
    c = first;
    while (c!=NULL)
    {
        keytotal++;
        if(keytotal == choice)
        {
            return c->word;
        }
        c = c->next;
    }
    return c->word;
}

string KeyWordList::getRandomNextWord(string keyword)
{
    KeyWordElement* k = findWord(keyword);
    return k->nextwordlist->getRandomWord();
}

//makes an empty linked list
NextWordList::NextWordList()
{
    first2=NULL;
}

//adds a word element to the list
void NextWordList::add(string keyword2)
{
    //encapsulate keyword as an element
    NextWordElement* newelement2 = new NextWordElement(keyword2);
    newelement2->next2=first2;
    first2 = newelement2;
    newelement2->count = 1;
}

//prints out every list element on its own line
void NextWordList::printlist()
{
    NextWordElement* currentValue = first2;

    while(currentValue!=NULL)
    {
        cout << currentValue->word2 << ": ";
        cout << currentValue->count << endl;
        currentValue=currentValue->next2;
    }
}

//searches the list for a word and returns its element if it finds it or NULL if it does not
NextWordElement* NextWordList::findWord(string targ)
{
    NextWordElement* current=first2;
    while(current!=NULL)
    {
        if(current->word2 == targ)
            return current;
        current=current->next2;
    }
    return current;
}

//will search the list for nextword,
//if it doesn't find it, it will add it and set its count to 1,
//if it does find it, it will increase its count
void NextWordList::foundNextWord(string nextword)
{
    NextWordElement* value = findWord(nextword);
    if(value == NULL)
    {
        add(nextword);
    }
    else
    {
       value->count++;
    }
}

string NextWordList::getRandomWord()
{
    //figure out # words
    int total=0;
    NextWordElement* c = first2;
    while(c!=NULL)
    {
        total++;
        c = c->next2;
    }
    int choice = rand()%total;
    int runningcount = 0;
    c = first2;
    while (c!=NULL)
    {
        runningcount= runningcount+c->count;
        if(runningcount >= choice)
        {
            return c->word2;
        }
        c = c->next2;
    }
    return c->word2;
}


string preprocess(string word)
{
    int i=0;
    int y = word.length();
    string newword="";
    for(i=0; i < y; i++)
    {
        if (isupper(word[i])&&isalpha(word[i]))
        {
            newword+= (char)tolower(word[i]);
        }
        else if (islower(word[i])&&isalpha(word[i]))
        {
            newword+=word[i];
        }
    }
    return newword;
}

int main()
{
    srand (time(NULL));
    KeyWordList* keywordlist = new KeyWordList();
    string nextword,startword,word1, word2, keyword;
    int order;

    ifstream book("wizardofoz.txt");


    cout << "What order would you like? ";
    cin >> order;
    string word;

    int i=0;
    for(int e = 0; e< order; e++)
    {
        book >> word[e];
    }
    word = preprocess(word);

    //book >> word1;
    //book >> word2;

    //word1=preprocess(word1);
    //word2 = preprocess(word2);
    while(!book.eof())
    {
        i=0;
        if (order == 1)
        {
            keyword = word;
        }
        else
        {
            keyword = word[i];
        }
        for(i=1; i<order; i++)
        {
            keyword+= " " + word[i];
        }
        //keyword = word1 + " " + word2;
        book >> nextword;
        nextword=preprocess(nextword);
        keywordlist->foundNextWord(keyword,nextword);//keyword = word[i], keyword+= " " + word[i];
        if (order == 1)
        {
            word = nextword;
        }
        else
        {
            word = word[1];
            for (i=2; i<order; i++)
            {
                word += word[i];
            }
            word += nextword;
        }

    }

    for(int p=0; p < 100; p++)
    {
        startword = keywordlist->getRandomKeyword();
        nextword = keywordlist->getRandomNextWord(startword);
        cout << nextword << " ";
        startword.substr(startword.find(" ")+1,startword.size());
        startword = startword + " " + nextword;
    }
    cout << endl;
    book.close();
    return 0;
}
