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
    IndexListNodeType *leftChild;
    IndexListNodeType *rightChild;
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
    newIndexNode->rightChild = NULL;
    newIndexNode->leftChild = NULL;
    return newIndexNode;
}

IndexNode *findIndexListNode(string v, IndexNode *root)
{
    if (root == NULL)
        return NULL;
    if (root->keyWord == v)
        return root;
    if (v < root->keyWord)
        return findIndexListNode(v, root->leftChild);
    else
        return findIndexListNode(v, root->rightChild);
}

IndexNode *insertIntoTree(string kw, int num,
                          PageNode *header, PageNode *tail, IndexNode *root, int line)
{
    if (root == NULL)
    {
        root = makeIndexListNode(kw, num, header, tail);
        return root;
    }
    if (root->keyWord == kw)
    {
        root->numberOfPages++;
        if (root->tail->pageNumber != line)
        {
            root->head = addPageToLast(line, root->head);
            root->tail = findLastPageNode(root->head);
        }
        return root;
    }

    if (kw > root->keyWord)
    {
        root->leftChild = insertIntoTree(kw, num, header, tail, root->leftChild, line);
    }
    else
    {
        root->rightChild = insertIntoTree(kw, num, header, tail, root->rightChild, line);
    }

    return root;
}

void printInorder(IndexNode *r)
{
    if (r == NULL)
        return;
    printInorder(r->leftChild);
    std::cout << setw(12) << left << r->keyWord << " - frequency: "
              << setw(5) << r->numberOfPages << "At line ";

    PageNode *cur = r->head;
    while (cur != NULL)
    {
        if (cur != r->head)
            std::cout << ", ";
        std::cout << cur->pageNumber;
        cur = cur->next;
    }
    std::cout << endl;
    std::cout << "---------------" << endl;
    printInorder(r->rightChild);
}

int countPageNode(PageNode *head)
{
    if (head == NULL)
        return 0;
    else
    {
        return 1 + countPageNode(head->next);
    }
}

void printInorderToFile(IndexNode *r, ofstream &filestream)
{
    if (r == NULL)
        return;
    printInorderToFile(r->leftChild, filestream);
    if (r->numberOfPages > countPageNode(r->head))
    {
        filestream << setw(12) << left << r->keyWord << " - frequency: "
                   << setw(5) << r->numberOfPages << "At line ";

        PageNode *cur = r->head;
        while (cur != NULL)
        {
            if (cur != r->head)
                filestream << ", ";
            filestream << cur->pageNumber;
            cur = cur->next;
        }
        filestream << endl;
        filestream << "---------------" << endl;
    }
    printInorderToFile(r->rightChild, filestream);
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
    IndexNode *root = NULL;
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
                        PageNode *pageH = NULL;
                        PageNode *pageT = NULL;
                        pageH = addPageToLast(countLine, pageH);
                        pageT = findLastPageNode(pageH);
                        root = insertIntoTree(newWord, 1, pageH, pageT, root, countLine);
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
                        PageNode *pageH = NULL;
                        PageNode *pageT = NULL;
                        pageH = addPageToLast(countLine, pageH);
                        pageT = findLastPageNode(pageH);
                        root = insertIntoTree(newWord, 1, pageH, pageT, root, countLine);
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
                printInorder(root);
            }
            else if (choice == 2)
            {
                std::cout << "Nhap ten file" << endl;
                string outFile;
                cin >> outFile;
                ofstream myfile(outFile);
                if (myfile.is_open())
                {

                    myfile << "--------------------Index Table--------------------" << endl;
                    printInorderToFile(root, myfile);
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
    return 0;
}