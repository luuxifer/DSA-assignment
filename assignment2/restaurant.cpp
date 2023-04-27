#include "main.h"
/*===================================================================================*/
/*===================================================================================*/
// Some support function to read file
// count how many words in this command
int countWords(const string& str) {
    int count = 0;
    bool inWord = false;
    for (char c : str) {
        if (isspace(c)) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            count++;
        }
    }
    return count;
}

// read first word of line to determine which type of command should be executed
string firstWord(string str) {
    string word;
    bool inWord = false;
    for (char c : str) {
        if (isspace(c)) {
            if (inWord) {
                break;
            }
        } else {
            word += c;
            inWord = true;
        }
    }
    return word;
}

// check if this string can convert into integer
bool checkInt(string s) {
    if (s.find_first_not_of("0123456789") == string::npos) {
        return 1;
    } else {
        return 0;
    }
}

// remove first word to get next info about the command
string removeFirst(string str) {
    string word;
    size_t pos = str.find(" ");
    if (pos != string::npos) {
        word = str.substr(pos + 1);
    }
    return word;
}

// check if between any 2 words has duplicated whitespace 
bool checkDuplicate(string str) {
    size_t pos = 0;
    while (pos != string::npos) {
        // find first whitespace after first word 
        size_t space_pos = str.find(' ', pos);
        // if don't, break the loop
        if (space_pos == string::npos) break;
        // find first character of next word 
        size_t next_word_pos = str.find_first_not_of(' ', space_pos);
        // if don't, break the loop
        if (next_word_pos == string::npos) break;
        // if the number of whitespace between any two words >= 2 -> return true
        if (next_word_pos - space_pos > 1) {
            return 1;
            break;
        }
    }
    return 0;
}
/*===================================================================================*/
/*===================================================================================*/

/*===================================================================================*/
/*========================FIRST AREA--SEA VIEW=======================================*/
/*===========CREATE SEAVIEW-CHAIN-TABLE AS A HASH TABLE FOR THE CUSTOMERS============*/
// template for generic type
template<typename K, typename V, typename I, typename N>
class hashNode{
public:
    //initialize some attribute and method of each node
    V value;
    K key;
    I ID;
    N num_Order;
    // Constructor
    hashNode(K key, V value, I ID, N num_Order) {
        this->key = key;
        this->value = value;
        this->ID = ID;
        this->num_Order = num_Order;
    }
};

template<typename K, typename V, typename I, typename N>
class hashMap{
public:
    hashNode<K, V, I, N>** arr;
    //stupid node
    hashNode<K, V, I, N>* dummy;
    // max num of customers can get in
    int capacity;
    // current num of customers
    int size;

    //Constructor
    hashMap() {
        capacity = MAXSIZE/2;
        size=0;
        arr = new hashNode<K, V, I, N>*[capacity];
 
        for (int i = 0; i < capacity; i++)
            arr[i] = nullptr;

        dummy = new hashNode<K, V, I, N>(-1, "", 0, 0);
    }
    // Return the index depend on input Key
    int hashCode(K key) {
        return key%capacity;
    }
    //Method to insert a Node
    void insertNode(K key, V value, I ID, N num_Order) {
        hashNode<K, V, I, N>* temp = new hashNode<K, V, I, N> (key, value, ID, num_Order);
        int hashIndex = hashCode(key);
        // go straight if any node meet the conflict or occupied
        while(arr[hashIndex] != nullptr && arr[hashIndex]->key != key && arr[hashIndex]->key != -1 && arr[hashIndex]->ID != 0) {
            hashIndex++;
            hashIndex %= capacity;
        }
        // insert node
        if(arr[hashIndex] == nullptr || arr[hashIndex]->key == -1) {
            ++size;
        }  
        arr[hashIndex] = temp;
    }
    // Method to delete a Node 
    V deleteNode(K key) {
        int hashIndex = hashCode(key);
        // Delete and return the value of node
        while(arr[hashIndex] != nullptr) {
            if(arr[hashIndex]->key == key) {
                hashNode<K, V, I, N>* temp = arr[hashIndex];
                arr[hashIndex] = dummy;
                size--;
                return temp->value;
            }
            hashIndex = (hashIndex+1)%capacity;
        }
        return nullptr;
    }

    // Method to get the value of Node if exist or return nullptr if none
    V get(int key) {
        int hashIndex = hashCode(key);
        int count = 0;

        while(arr[hashIndex] != nullptr) {
            if(++count > capacity) return nullptr;

            if(arr[hashIndex]->key == key) {
                return arr[hashIndex]->value;
            }
            hashIndex = (hashIndex+1)%capacity;
        }
        return nullptr;
    }

    // Search if exist the customer we need to find or not
    int search(int key, string name) {
        
        int hashIndex = hashCode(key);
        int count = 0;

        while(arr[hashIndex] != nullptr) {
            if(++count > capacity) return -1;

            if(arr[hashIndex]->value == name) {
                return hashIndex;
            }
            hashIndex = (hashIndex+1)%capacity;
        }
        return -1;
    }


    // Return current size
    int getSize() {
        return size;
    }
    // Check current state is empty or not
    bool isEmpty() {
        return (size==0);
    }
    // Check current state is full or not
    bool isFull() {
        return (size==capacity);
    }
    // dislay the hashmap
    void display() {
        for(int i = 0; i < capacity; i++) {
            if(arr[i] != nullptr && arr[i]->key != -1) {
                cout << i << " key = " << arr[i]->key << " value = " << arr[i]->value << " ID= " << arr[i]->ID << " Order " << arr[i]->num_Order << " times " << endl;
            }
        }
    }
};
/*===============================================================================================*/
/*===============================================================================================*/
/*===============================================================================================*/

/*===============================================================================================*/
/*================================SECOND AREA--MOUNTAIN VIEW=====================================*/
/*===============CREATE MOUNTAINVIEW-CHAIN-TABLE AS AN AVL TREE FOR THE CUSTOMERS================*/

enum BalanceValue {
    LH = -1,
    EH = 0,
    RH = 1
};

class Node {
public:
    int data;
    int ID;
    string name;
    int num_Order;
    Node *pLeft, *pRight;
    BalanceValue balance;

    Node(int value, int IDtable, string name, int num_Order) : data(value), ID(IDtable), name(name), num_Order(num_Order),pLeft(NULL), pRight(NULL), balance(EH) {}
    ~Node() {}
};

void printNSpace(int n) {
    for (int i = 0; i < n - 1; i++) cout << " ";
}

void printInteger(int &n) {
    cout << n << " ";
}

template<class T>
class AVLTree
{
public:
    class Node;
    Node *root;
protected:
    int getHeightRec(Node *node) {
        if (node == NULL)
            return 0;
        int lh = this->getHeightRec(node->pLeft);
        int rh = this->getHeightRec(node->pRight);
        return (lh > rh ? lh : rh) + 1;
    }
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree(){}
    int getHeight() {
        return this->getHeightRec(this->root);
    }
    void printTreeStructure() {
        int height = this->getHeight();
        if (this->root == NULL) {
            cout << "NULL\n";
            return;
        }
        queue<Node *> q;
        q.push(root);
        Node *temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = pow(2, height);
        printNSpace(space / 2);
        while (!q.empty()) {
            temp = q.front();
            q.pop();
            if (temp == NULL) {
                cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else {
                cout << temp->data<<" " <<temp->num_Order;
                q.push(temp->pLeft);
                q.push(temp->pRight);
            }
            printNSpace(space);
            count++;
            if (count == maxNode) {
                cout << endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height) return;
        }
    }

/*####################################################################################################*/
    // ADD a new customer with result and ID into AVL tree
    int getIDforNode(int result) {
        return result%32+1;
    }
    // left or right rotate depend on which kind of unbalanced tree
    Node* rotateRight(Node* root) {
        Node* temp=root->pLeft;
        // right rotate
        root->pLeft=temp->pRight;
        temp->pRight=root;

        return temp;
    }
    Node* rotateLeft(Node* root) {
        Node* temp=root->pRight;
        // left rotate
        root->pRight=temp->pLeft;
        temp->pLeft=root;

        return temp;
    }

    // update balance state after each rotate 
    Node* leftbalance(Node* root, bool &taller) {
        Node* ltree=root->pLeft;
        if (ltree->balance==LH) {
            root=rotateRight(root);
            root->balance=EH;
            root->pRight->balance=EH;
            taller=false;
        }
        else {
            Node* rtree=ltree->pRight;
            if (rtree->balance==LH) {
                root->balance=RH;
                ltree->balance=EH;
            }
            else if (rtree->balance==EH) {
                ltree->balance=EH;
                root->balance=EH;
            }
            else {
                root->balance=EH;
                ltree->balance=LH;
            }
            rtree->balance=EH;
            root->pLeft=rotateLeft(ltree);
            root=rotateRight(root);
            taller=false;
        }
        return root;
    }

    Node* rightbalance(Node* root, bool &taller) {
        Node* rtree=root->pRight;
        if (rtree->balance==RH) {
            root=rotateLeft(root);
            root->balance=EH;
            root->pLeft->balance=EH;
            taller=false;
        }
        else {
            Node* ltree=rtree->pLeft;
            if(ltree->balance==RH) {
                root->balance=LH;
                rtree->balance=EH;
            }
            else if (ltree->balance==EH) {
                rtree->balance=EH;
                root->balance=EH;
            }
            else {
                root->balance=EH;
                rtree->balance=RH;
            }
            ltree->balance=EH;
            root->pRight=rotateRight(rtree);
            root=rotateLeft(root);
            taller=false;
        }
        return root;
    }

    Node* insertAVL(Node*root, int value, int ID, string name, bool& taller) {
       if(root==NULL) {
            Node* temp=new Node(value, ID, name, 1);
            root=temp;
            taller=true;
            root->balance=EH;
        
            return root;
        }
        if (root->data>value) {
            root->pLeft = insertAVL(root->pLeft, value, ID, name, taller);
            if (taller) {
                if (root->balance==LH) root=leftbalance(root, taller);
                else if (root->balance==EH) root->balance=LH;
                else {
                    root->balance=EH;
                    taller=false;
                }
            }
        }
        else {
            root->pRight=insertAVL(root->pRight, value, ID, name, taller);
            if (taller) {
                if (root->balance==RH) root=rightbalance(root, taller);
                else if (root->balance==EH) root->balance=RH;
                else {
                    root->balance=EH;
                    taller=false;
                }
            }
        }
        return root;
    }

    void insert(const T &value, T ID, string name) {
        bool taller=false;
        root=insertAVL(root,value,ID,name,taller);
    }
/*####################################################################################################*/

/*####################################################################################################*/
    // delete any customer while only know his/her ID table

/*####################################################################################################*/
    // Seach, print the AVL tree follow the rule
    void printInorder(Node*temp) {
        if(temp == nullptr) return;
        else {
            printInorder(temp->pLeft);
            cout<<temp->data<<" ";
            printInorder(temp->pRight);
        }
    }

    void printInorder() {
        Node*temp=root;
        if(temp == nullptr) return;
        else {
            printInorder(temp);
        }
    }

    bool find(const T &value, string name){
        Node*temp=root;
        while(temp != nullptr) {
            if(temp->data==value) return 1;
            if(value<temp->data) temp=temp->pLeft;
            else temp=temp->pRight;
        }
        return 0;
    }

    void search(T result, string name) {
        Node* temp = root;
        while(temp != nullptr) {
            if(temp->data==result) {
                temp->num_Order++;
                return;
            }
            if(result < temp->data) temp=temp->pLeft;
            else temp=temp->pRight;
        }
        return;
    }

/*####################################################################################################*/
    // Some helpful function as count number of customers, queue is full?,...
    int count(Node* root) {
        if(root == nullptr) return 0;
        int lh = count(root->pLeft);
        int rh = count(root->pRight);

        return 1 + lh + rh;
    }

    
/*####################################################################################################*/
    class Node
    {
    public:
        T data;
        T ID;
        string name;
        T num_Order;
        Node *pLeft, *pRight;
        BalanceValue balance;
        friend class AVLTree<T>;

    
        Node(T value, T IDtable, string name, T num_Order) : data(value), ID(IDtable), name(name), num_Order(num_Order),pLeft(NULL), pRight(NULL), balance(EH) {}
        ~Node() {}
    };
};
/*===============================================================================================*/
/*===============================================================================================*/
/*===============================================================================================*/

/*===============================================================================================*/
/*=====A LINKED LIST TO STORE AND GET ID WHENEVER A NEW CUSTOMER COME IN THE RESTAURANT==========*/
class table
{
public:
    int ID;
    string name;
    // result is the number we will have after huffman encoding
    int result;
    table* next;
    table(int ID, string name, int result, table* next)
    {
        this->ID = ID;
        this->name = name;
        this->result = result;
        this->next = next;
    }
};

class restaurant
{
public:
    table *recentTable;
    restaurant()
    {
        recentTable = nullptr;
    }
    ~restaurant()
    {
        delete recentTable;
    }
    table* finsert(table *last, int ID, string name, int result)
    {
        if (last != nullptr)
            return last;
        table *temp = new table(ID, "", result, last);

        last = temp;
        last->next = last;
        return last;
    }
    table *insert(table *last, int ID, string name, int result)
    {
        if (last == NULL)
            return finsert(last, ID, name, result);
        table *temp = new table (ID, name, result, last->next);
        last -> next = temp;
        last = temp;
        return last;
    }

    void printList(table *recentTable) {
        table* first = recentTable->next;
        for(int i = 0; i < MAXSIZE; i++) {
            cout << "Name " << first->name << " result " << first->result << " ID " << first->ID << endl;
            first=first->next;
        }
    }

    // Check if the restautant is full? take ID and lead them into the suitable table
    bool isFull(table *recentTable) {
        table* first = recentTable->next;
        int count = 0;
        for(int i = 0; i < MAXSIZE; i++) {
            if(first->result != 0) count++;
            first=first->next;
        }
        return (count==MAXSIZE);
    }

    int totalCustomer(table* recentTable) {
        table* first = recentTable->next;
        int count = 0;
        for(int i = 1; i <= MAXSIZE; i++) {
            if(first->result != 0) count++;
            first=first->next;
        }
        return count;
    }

    int addNewCustomer(table* recentable, int result, string name) {
        if(isFull(recentable) == 0) {
            table* first = recentTable->next;
            int ID = result%MAXSIZE + 1;
            while(first->ID != ID) first = first->next;
            while(first->result != 0 && first->name != "") first = first->next;
            first->name = name;
            first->result = result;
            return first->ID;
        }
        else return 0;
    }

    int search(string name) {
        table* first = recentTable->next;
        for(int i = 1; i <= MAXSIZE; i++) {
            if(first->name == name) {
                return i;
            }
            first = first->next;
        }
        return 0;
    }
};
/*===============================================================================================*/
/*===============================================================================================*/

//=======================================================DEMO ONLY================================
/*===============================================================================================*/
/*=======================1. FIRST REQUEST - IMPLEMENT REG COMMAND================================*/
// get ID for ID infomation 
// A Tree node
class huffNode{
public:
	char ch;
	int freq;
	huffNode *left, *right;
};

// Function to allocate a new tree node
huffNode* getNode(char ch, int freq, huffNode* left, huffNode* right)
{
	huffNode* node = new huffNode();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Comparison object to be used to order the heap
class comp{
public:
	bool operator()(huffNode* l, huffNode* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(huffNode* root, string str,
			unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}


// Builds Huffman Tree and decode given input text
string buildHuffmanTree(string text)
{
	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<huffNode*, vector<huffNode*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		huffNode *left = pq.top(); pq.pop();
		huffNode *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	huffNode* root = pq.top();

	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);
	// print encoded string
	string str = "";
	for (char ch: text) {
		str += huffmanCode[ch];
	}
    string str1= str.substr(str.size() - 15, 15);

	return str1;
}
int bin_to_dec(string binaryStr) {
    int decimalNum = 0;

    // duyệt từng ký tự trong chuỗi
    for (int i = 0; i < binaryStr.length(); i++) {
        // tính giá trị của ký tự tại vị trí i
        int digit = binaryStr[i] - '0';
        // cộng vào giá trị thập phân
        decimalNum += digit * pow(2, binaryStr.length() - i - 1);
    }
    return decimalNum;
}

int getIDCustomer(int result) {
    return (result%MAXSIZE + 1);
}
int getOPT(int result) {
    return result%3;
}

void REG(string name, hashMap<int, string, int, int>* seaView, AVLTree<int>* mountainView, restaurant* storeQueue) {
    int result = bin_to_dec(buildHuffmanTree(name));
    int ID = result % MAXSIZE + 1;
    int checkOrder = storeQueue->search(name);
    if(storeQueue->totalCustomer(storeQueue->recentTable) != 0 && checkOrder != 0) {
        if(seaView->search(result, name) != -1) seaView->arr[seaView->search(result, name)]->num_Order += 1;
        if(mountainView->find(result, name)) {
            mountainView->search(result, name);
        }
    }
    else {
        // This brach use when reult % 2 == 1 mean they will seat in seaView queue and divide this case into four subcase 
        if(result % 2 != 0) {
            if(!storeQueue->isFull(storeQueue->recentTable)) {
                int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                seaView->insertNode(result, name, realID, 1);
            }
            else {
                if(mountainView->count(mountainView->root) != 0) {
                    storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                    mountainView->insert(result, ID, name);
                }
                else {
                    int OPT = result % 3;
                    if(OPT == 0) {

                    }
                    else if(OPT == 1) {

                    }
                    else {

                    }
                }
            }
        }
        else {
            if(mountainView->count(mountainView->root) != (MAXSIZE/2)) {
                storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                mountainView->insert(result, ID, name);
            }
            else {
                if(!storeQueue->isFull(storeQueue->recentTable)) {
                    int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                    seaView->insertNode(result, name, realID, 1);
                }
                else {

                }
            }
        }
    }
    
}
/*===============================================================================================*/
/*===============================================================================================*/
void simulate(string filename)
{
    restaurant* storeQueue = new restaurant();
    for (int i = 1; i <= MAXSIZE; i++)
    {
        storeQueue->recentTable = storeQueue->insert(storeQueue->recentTable,i,"",0);
    }

    // create queue to deal with the fully occupied situation
    restaurant* FIFOqueue = new restaurant();
    for (int i = 1; i <= MAXSIZE; i++)
    {
        storeQueue->recentTable = storeQueue->insert(storeQueue->recentTable,i,"",0);
    }
    // Initialize some important data structure to use throughout the assignment2
    hashMap<int, string, int, int>* seaView = new hashMap<int, string, int, int>;
    AVLTree<int>* mountainView = new AVLTree<int>;
    
	ifstream input_file("test.txt");
    if (!input_file.is_open()) {
        return;
    }
	// initialize a word to read file
    string line;
    while(!input_file.eof()) {
        // initialize some var for string parsing
        getline(input_file, line);
        string newline = line;
        string command = firstWord(newline);
        int length = countWords(newline);
        
        if(command == "REG") {// check some necessary condition before implement request 
            if(length == 2) {
                string NAME = firstWord(removeFirst(newline));
                string str_AGE = firstWord(removeFirst(removeFirst(newline)));
                cout << NAME << " " << str_AGE << endl;
            }
            else continue;
        }
        else if(command == "CLE") {// check some necessary condition before implement request 
            if(length == 2) {
                string str_ID = firstWord(removeFirst(newline));
                if(checkInt(str_ID)) {
                    int ID = stoi(str_ID);
                    if(ID >= 1 && ID <= MAXSIZE) {
                        cout << "cle" << ID << endl;
                    }
                }
            }
            else continue;
        }
        else if(command == "PrintHT") {// check some necessary condition before implement request 
            cout << "printht\n";
        }
        else if(command == "PrintAVL") {// check some necessary condition before implement request 
            cout << "printavl\n";
        }
        else if(command == "PrintMH") {// check some necessary condition before implement request 
            cout << "printmh\n";
        }
        else continue;
    }

    // cout << storeQueue->addNewCustomer(storeQueue->recentTable,33,"hihi") << endl;
    // storeQueue->printList(storeQueue->recentTable);
    REG("tELYXTj",seaView,mountainView,storeQueue);
REG("tELYXTj",seaView,mountainView,storeQueue);
    REG("tELYXaaabb",seaView,mountainView,storeQueue);
    REG("tELYXT",seaView,mountainView,storeQueue);
    cout << storeQueue->search("tELYXTj") << endl;
    storeQueue->printList(storeQueue->recentTable);
    cout << "==================================================================\n";
    seaView->insertNode(33,"first",2,1);
    seaView->display();
    cout << "==================================================================\n";
    mountainView->printTreeStructure();
    cout << mountainView->count(mountainView->root);
    cout << mountainView->count(mountainView->root);
    // storeQueue->printList(storeQueue->recentTable);
    // cout << storeQueue->totalCustomer(storeQueue->recentTable)<<endl;
    // cout<< seaView->search(bin_to_dec(buildHuffmanTree("tELYXTj")),"tELYXTj")<<endl;
    
}