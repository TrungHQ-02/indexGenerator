#include <iostream>
#include <fstream>
#include <iomanip>
#define MAX 1000

using namespace std;

struct StopWordNodeType
{
    string stopWord;
    StopWordNodeType *next;
};

typedef StopWordNodeType StopWNode;

struct PageNodeType
{
    int pageNumber;
    PageNodeType *next;
};

typedef PageNodeType PageNode;

struct IndexListNodeType
{
    string keyWord;
    int numberOfPages;
    PageNode *head;
    PageNode *tail;
    IndexListNodeType *next;
};

typedef IndexListNodeType IndexNode;

//  FOR STOP WORD LIST
StopWNode *makeStopWNode(string x)
{
    StopWNode *newNode = new StopWNode;
    newNode->stopWord = x;
    newNode->next = NULL;
    return newNode;
}

StopWNode *findLastNode(StopWNode *header)
{
    if (header == NULL)
        return NULL;
    StopWNode *p = header;
    while (p != NULL)
    {
        if (p->next == NULL)
            return p;
        p = p->next;
    }
    return NULL;
}

void printStopWordList(StopWNode *header)
{
    StopWNode *p = header;
    while (p != NULL)
    {
        cout << p->stopWord << " ";
        cout << endl;
        p = p->next;
    }
}

StopWNode *addStopWNodeLast(string val, StopWNode *header)
{
    StopWNode *q = makeStopWNode(val);
    if (header == NULL)
        return q;
    StopWNode *p = findLastNode(header);
    p->next = q;
    q->next = NULL;
    return header;
}

void freeSTopWList(StopWNode *h)
{
    if (h == NULL)
        return;
    freeSTopWList(h->next);
    delete h;
}

// END OF STOP WORD LIST

// FOR PAGE NODE TYPE
PageNode *makePageNode(int pageValue)
{
    PageNode *page = new PageNode;
    page->pageNumber = pageValue;
    page->next = NULL;
    return page;
}

void printPageWordList(PageNode *h)
{
    if (h == NULL)
        return;
    PageNode *p = h;
    while (p != NULL)
    {
        cout << p->pageNumber << " ";
        cout << endl;
        p = p->next;
    }
}
PageNode *findLastPageNode(PageNode *header)
{
    if (header == NULL)
        return NULL;
    PageNode *p = header;
    while (p != NULL)
    {
        if (p->next == NULL)
            return p;
        p = p->next;
    }
    return NULL;
}

PageNode *addPageToLast(int v, PageNode *header)
{
    PageNode *q = makePageNode(v);
    if (header == NULL)
        return q;
    PageNode *p = findLastPageNode(header);
    p->next = q;
    q->next = NULL;
    return header;
}

void freePageList(PageNode *h)
{
    if (h == NULL)
        return;
    freePageList(h->next);
    delete h;
}

// END OF PAGE NODE TYPE

// FOR INDEX LIST

IndexNode *makeIndexListNode(string keyWord, int numberOfPages, PageNode *head, PageNode *tail)
{
    IndexNode *newIndexNode = new IndexNode;
    newIndexNode->keyWord = keyWord;
    newIndexNode->numberOfPages = numberOfPages;
    newIndexNode->head = head;
    newIndexNode->tail = tail;
    newIndexNode->next = NULL;
    return newIndexNode;
}

void sortIndexList(IndexNode *h)
{
    int swapped;
    do
    {
        swapped = 0;
        IndexNode *p = h;
        for (; p->next != NULL; p = p->next)
        {
            if (p->keyWord > p->next->keyWord)
            {
                PageNode *temp = p->head;
                p->head = p->next->head;
                p->next->head = temp;

                string swapKeyWord = p->keyWord;
                p->keyWord = p->next->keyWord;
                p->next->keyWord = swapKeyWord;

                PageNode *tempTail = p->tail;
                p->tail = p->next->tail;
                p->next->tail = tempTail;

                int tempPage = p->numberOfPages;
                p->numberOfPages = p->next->numberOfPages;
                p->next->numberOfPages = tempPage;

                swapped = 1;
            }
        }
    } while (swapped == 1);
}

void printIndexList(IndexNode *h)
{
    if (h == NULL)
        return;
    IndexNode *p = h;
    while (p != NULL)
    {
        cout << setw(14) << left << p->keyWord;
        cout << "- frequency: " << setw(5) << p->numberOfPages
             << "At line: ";
        PageNode *cur = p->head;
        while (cur != NULL)
        {
            if (cur != p->head)
                cout << ", ";
            cout << cur->pageNumber;
            cur = cur->next;
        }
        cout << endl
             << "-------------" << endl;
        p = p->next;
    }
}

IndexNode *findLastIndexNode(IndexNode *head)
{
    if (head == NULL)
        return NULL;
    IndexNode *p = head;
    while (p != NULL)
    {
        if (p->next == NULL)
            return p;
        p = p->next;
    }
    return NULL;
}

IndexNode *addIndexNodeLast(string kw, int num, PageNode *header, PageNode *tail, IndexNode *head)
{
    IndexNode *q = makeIndexListNode(kw, num, header, tail);
    if (head == NULL)
        return q;
    IndexNode *p = findLastIndexNode(head);
    p->next = q;
    q->next = NULL;
    return head;
}

void freeIndexList(IndexNode *h)
{
    if (h == NULL)
        return;
    freeIndexList(h->next);
    freePageList(h->head);
    delete h;
}

int checkStopWord(string c, StopWNode *headerOfStopW)
{
    StopWNode *p = headerOfStopW;
    while (p != NULL)
    {
        if (p->stopWord == c)
            return 0;
        p = p->next;
    }
    return 1;
}

// END INDEX LIST

int main()
{
    // Bulid Stop Word List
    StopWNode *headerOfStopWord = NULL;
    ifstream file("stopWord.txt");
    string str;
    string file_contents;
    while (getline(file, str))
    {
        headerOfStopWord = addStopWNodeLast(str, headerOfStopWord);
    }
    // printStopWordList(headerOfStopWord);
    // DONE READING STOPWORD FILE

    // Start building index table
    char ch;
    fstream fin("VanBan.txt", fstream::in);
    IndexNode *headerOfIndexList = NULL;

    string newWord = "";
    int countLine = 1;
    while (fin >> noskipws >> ch)
    {
        if (ch == '*')
        {
            // gap ki tu * thi dung doc van ban
            countLine--;
            break;
        }
        if (isalpha(ch))
        {
            char chLower = tolower(ch);
            newWord.push_back(chLower);
        }

        else
        {
            if (newWord != "")
            {
                if (checkStopWord(newWord, headerOfStopWord))
                {
                    // process when keyword is new
                    IndexNode *cursorNode = headerOfIndexList;
                    if (cursorNode == NULL)
                    {
                        PageNode *pageH = NULL;
                        PageNode *pageT = NULL;
                        pageH = addPageToLast(countLine, pageH);
                        pageT = findLastPageNode(pageH);
                        headerOfIndexList = addIndexNodeLast(newWord, 1, pageH, pageT, headerOfIndexList);
                    }
                    else if (cursorNode->next == NULL)
                    {
                        if (cursorNode->keyWord == newWord)
                        {
                            cursorNode->numberOfPages++;
                            int pageCheck = cursorNode->tail->pageNumber == countLine;
                            if (pageCheck != 1)
                            {
                                cursorNode->head = addPageToLast(countLine, cursorNode->head);
                                cursorNode->tail = findLastPageNode(cursorNode->head);
                            }
                        }
                        else
                        {
                            PageNode *pageH = NULL;
                            PageNode *pageT = NULL;
                            pageH = addPageToLast(countLine, pageH);
                            pageT = findLastPageNode(pageH);
                            headerOfIndexList = addIndexNodeLast(newWord, 1, pageH, pageT, headerOfIndexList);
                        }
                    }
                    else
                    {
                        while (cursorNode->next != NULL)
                        {
                            if (cursorNode->keyWord == newWord)
                            {
                                break;
                            }
                            cursorNode = cursorNode->next;
                        }

                        if (cursorNode->next == NULL)
                        {
                            if (cursorNode->keyWord == newWord)
                            {
                                cursorNode->numberOfPages++;
                                int pageCheck = cursorNode->tail->pageNumber == countLine;
                                if (pageCheck != 1)
                                {
                                    cursorNode->head = addPageToLast(countLine, cursorNode->head);
                                    cursorNode->tail = findLastPageNode(cursorNode->head);
                                }
                            }
                            else
                            {
                                PageNode *pageH = NULL;
                                PageNode *pageT = NULL;
                                pageH = addPageToLast(countLine, pageH);
                                pageT = findLastPageNode(pageH);
                                headerOfIndexList = addIndexNodeLast(newWord, 1, pageH, pageT, headerOfIndexList);
                            }
                        }
                        else
                        {
                            cursorNode->numberOfPages++;
                            int pageCheck = cursorNode->tail->pageNumber == countLine;
                            if (pageCheck != 1)
                            {
                                cursorNode->head = addPageToLast(countLine, cursorNode->head);
                                cursorNode->tail = findLastPageNode(cursorNode->head);
                            }
                        }
                    }
                }
            }

            newWord = "";
            if (ch == '\n')
            {
                countLine++;
                continue;
            }
        }
    }
    cout << "Number Of Lines in VanBan.txt: " << countLine << endl;
    cout << "--------------------Index Table--------------------" << endl;
    sortIndexList(headerOfIndexList);
    printIndexList(headerOfIndexList);

    freeSTopWList(headerOfStopWord);
    freeIndexList(headerOfIndexList);

    return 0;
}