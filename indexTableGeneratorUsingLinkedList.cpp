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
        std::cout << p->stopWord << " ";
        std::cout << endl;
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
        std::cout << p->pageNumber << " ";
        std::cout << endl;
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
        std::cout << setw(14) << left << p->keyWord;
        std::cout << "- frequency: " << setw(5) << p->numberOfPages
                  << "At line: ";
        PageNode *cur = p->head;
        while (cur != NULL)
        {
            if (cur != p->head)
                std::cout << ", ";
            std::cout << cur->pageNumber;
            cur = cur->next;
        }
        std::cout << endl
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
    int choice1 = 0;
    StopWNode *headerOfStopWord = NULL;
    IndexNode *headerOfIndexList = NULL;
    string swFile;
    do
    {
        std::cout << "-----Nhap ten file stopWord-----" << endl;
        cin >> swFile;
        // Bulid Stop Word List
        ifstream file(swFile);
        string str;
        while (getline(file, str))
        {
            headerOfStopWord = addStopWNodeLast(str, headerOfStopWord);
        }
        // DONE READING STOPWORD FILE

        // Start building index table
        char ch;
        std::cout << "-----Nhap ten file van ban-----" << endl;
        string textFile;
        cin >> textFile;
        fstream fin(textFile, fstream::in);
        string newWord = "";
        int countLine = 1;
        do
        {
            fin >> noskipws >> ch;
            // Neu het file
            if (fin.eof())
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
                break;
            }
            // Neu doc duoc mot ki tu la chu
            if (isalpha(ch))
            {
                char chLower = tolower(ch);
                newWord.push_back(chLower);
            }
            // Neu doc duoc mot ki tu khong la chu
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
        } while (true);

        // Tao bang chi muc xong

        std::cout << "Bang chi muc da duoc tao thanh cong!" << endl;
        int choice = 0;
        do
        {
            std::cout << "Ban muon in ra console hay luu vao file" << endl;
            std::cout << "1. In ra console" << endl;
            std::cout << "2. Luu vao file" << endl;
            cin >> choice;
            if (choice == 1)
            {
                std::cout << "--------------------Index Table--------------------" << endl;
                sortIndexList(headerOfIndexList);
                printIndexList(headerOfIndexList);
            }
            else if (choice == 2)
            {
                std::cout << "Nhap ten file" << endl;
                string outFile;
                cin >> outFile;
                ofstream myfile(outFile);
                sortIndexList(headerOfIndexList);
                myfile << "--------------------Index Table--------------------" << endl;
                if (myfile.is_open())
                {
                    IndexNode *h = headerOfIndexList;
                    if (h == NULL)
                        cout << "Bang chi muc rong" << endl;
                    IndexNode *p = h;
                    while (p != NULL)
                    {
                        myfile << setw(14) << left << p->keyWord;
                        myfile << "- frequency: " << setw(5) << p->numberOfPages
                               << "At line: ";
                        PageNode *cur = p->head;
                        while (cur != NULL)
                        {
                            if (cur != p->head)
                                myfile << ", ";
                            myfile << cur->pageNumber;
                            cur = cur->next;
                        }
                        myfile << endl
                               << "-------------" << endl;
                        p = p->next;
                    }
                    myfile.close();
                }
                else
                    std::cout << "Khong mo duoc file" << endl;
            }

        } while (!(choice == 1 || choice == 2));
        std::cout << "Ban co muon tiep tuc khong?" << endl;
        std::cout << "1. Co" << endl;
        std::cout << "2. Khong" << endl;
        cin >> choice1;
        if (choice1 == 2)
            std::cout << "Goodbye!";
    } while (choice1 != 2);
    freeSTopWList(headerOfStopWord);
    freeIndexList(headerOfIndexList);

    return 0;
}