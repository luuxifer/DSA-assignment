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
void printNSpace(int n) {
    for (int i = 0; i < n - 1; i++) cout << " ";
}

void printInteger(int &n) {
    cout << n << " ";
}

int compareStrings(string s1, string s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] > s2[i]) {
            return 1;
        } else if (s1[i] < s2[i]) {
            return -1;
        }
        i++;
    }
    if (s1[i] == '\0' && s2[i] == '\0') {
        return 0;
    } else if (s1[i] == '\0') {
        return -1;
    } else {
        return 1;
    }
}

class AVLNode {
public:
    int result;
    string name;
    int ID;
    int num_Order;
    int height;
    AVLNode* left;
    AVLNode* right;
    
    AVLNode(int val, string name, int ID, int num_Order) : result(val), name(name), ID(ID), num_Order(num_Order), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;
    
    int getHeight(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }
    
    int getBalance(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }
    
    void updateHeight(AVLNode* node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }
    
    AVLNode* rotateRight(AVLNode* node) {
        AVLNode* newRoot = node->left;
        AVLNode* rightChild = newRoot->right;
        newRoot->right = node;
        node->left = rightChild;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }
    
    AVLNode* rotateLeft(AVLNode* node) {
        AVLNode* newRoot = node->right;
        AVLNode* leftChild = newRoot->left;
        newRoot->left = node;
        node->right = leftChild;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }
    
    AVLNode* balance(AVLNode* node) {
        int balanceFactor = getBalance(node);
        if (balanceFactor > 1) { // left-heavy
            if (getBalance(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        } else if (balanceFactor < -1) { // right-heavy
            if (getBalance(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        } else { // balanced
            return node;
        }
    }
    
    AVLNode* insertHelper(AVLNode* node, int result, string name, int ID, int num_Orders) {
        if (node == nullptr) {
            return new AVLNode(result, name, ID, num_Orders);
        }
        if (result < node->result || (result == node->result && compareStrings(root->name, name) > 0)) {
            node->left = insertHelper(node->left, result, name ,ID, num_Orders);
        } else { // equal values go to the right
            node->right = insertHelper(node->right, result, name , ID, num_Orders);
        }
        updateHeight(node);
        return balance(node);
    }
    
    void inorderTraversalHelper(AVLNode* node) {
        if (node != nullptr) {
            inorderTraversalHelper(node->left);
            std::cout << node->result << " ";
            inorderTraversalHelper(node->right);
        }
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    void insert(int result, string name, int ID, int num_Orders) {
        root = insertHelper(root, result, name, ID, num_Orders);
    }
    
    AVLNode* getRoot() {
        return root;
    }
    
    void inorderTraversal() {
        inorderTraversalHelper(root);
        std::cout << std::endl;
    }

    void printTreeStructure() {
        int height = this->getHeight(root);
        if (this->root == NULL) {
            cout << "NULL\n";
            return;
        }
        queue<AVLNode *> q;
        q.push(root);
        AVLNode *temp;
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
                // temp->data<<" " <<
                cout <<temp->result;//<< " "<<temp->name;
                q.push(temp->left);
                q.push(temp->right);
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

    AVLNode* remove(AVLNode* node, int result, string name) {
        if (node == nullptr) {
            return nullptr;
        }
        if (result < node->result || (result == node->result && compareStrings(node->name, name) > 0)) {
            node->left = remove(node->left, result, name);
        } else if (result > node->result || (result == node->result && compareStrings(node->name, name) < 0)) {
            node->right = remove(node->right, result, name);
        } else {
            AVLNode* temp;
            if (node->left == nullptr || node->right == nullptr) {
                temp = (node->left) ? node->left : node->right;
                if (temp == NULL) {
                    temp = node;
                    node = NULL;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                temp = findMinNode(node->right);
                node->name = temp->name;
                node->ID = temp->ID;
                node->result = temp->result;
                node->num_Order = temp->num_Order;

                node->right = remove(node->right, temp->result,temp->name);
            }
        }
        if (node == NULL) return nullptr;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    AVLNode* findMinNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left != NULL) {
            current = current->left;
        }
        return current;
    }

    // Some helpful function as count number of customers, queue is full?,...
    int count(AVLNode* root) {
        if(root == nullptr) return 0;
        int lh = count(root->left);
        int rh = count(root->right);

        return 1 + lh + rh;
    }

    bool find(int result, string name){
        AVLNode* temp = root;
        while(temp != nullptr) {
            if(temp->result == result && temp->name == name) return 1;
            if(result < temp->result || (result == temp->result && compareStrings(root->name, name) > 0)) temp=temp->left;
            else temp=temp->right;
        }
        return 0;
    }

    void searchDuplicateOrder(int result, string name) {
        AVLNode* temp = root;
        while(temp != nullptr) {
            if(temp->result == result && temp->name == name) temp->num_Order++;
            if(result < temp->result || (result == temp->result && compareStrings(root->name, name) > 0)) temp=temp->left;
            else temp=temp->right;
        }
    }
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
    int num_Order;
    table* next;
    table(int ID, string name, int result, int num_Order,table* next)
    {
        this->ID = ID;
        this->name = name;
        this->result = result;
        this->num_Order = num_Order;
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
    table* finsert(table *last, int ID, string name, int result, int num_Order)
    {
        if (last != nullptr)
            return last;
        table *temp = new table(ID, "", result, num_Order, last);

        last = temp;
        last->next = last;
        return last;
    }
    table *insert(table *last, int ID, string name, int result, int num_Order)
    {
        if (last == NULL)
            return finsert(last, ID, name, result, num_Order);
        table *temp = new table (ID, name, result, num_Order, last->next);
        last -> next = temp;
        last = temp;
        return last;
    }

    void printList(table *recentTable) {
        table* first = recentTable->next;
        for(int i = 0; i < MAXSIZE; i++) {
            cout << "Name " << first->name << " result " << first->result << " ID " << first->ID << " num_Order: " << first->num_Order << endl;
            first=first->next;
        }
    }

    // Check if the restautant is full? take ID and lead them into the suitable table
    bool isFull(table *recentTable) {
        table* first = recentTable->next;
        int count = 0;
        for(int i = 0; i < MAXSIZE; i++) {
            if(first->name != "") count++;
            first=first->next;
        }
        return (count==MAXSIZE);
    }

    int totalCustomer(table* recentTable) {
        table* first = recentTable->next;
        int count = 0;
        for(int i = 1; i <= MAXSIZE; i++) {
            if(first->name != "") count++;
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

    // method to add in FIFO queue
    void addFiFo(string name) {
        if(search(name) != 0) {
            table* first = recentTable->next;
            for(int i = 1; i <= MAXSIZE; i++) {
                if(first->name == name) first->num_Order++;
                first = first->next;
            }
        }
        else {
            table* temp = recentTable->next;
            while(temp->name != "") temp = temp->next;
            temp->name = name;
            temp->num_Order = 1;
        }
    }

    string getFirstName() {
        table* temp = recentTable->next;
        return temp->name;
    }

    int getFirstResult() {
        table* temp = recentTable->next;
        return temp->result;
    }
};

////////////////////////////////////////////////////////////////////////////////////
    string updateFIFOqueue(table* store, string new_customer) {
        table* temp = store->next;
        string name = temp->name;
        for(int i = 1; i < MAXSIZE; i++) {
            temp->name = temp->next->name;
            temp->num_Order = temp->next->num_Order;

            temp = temp->next;
        }
        temp->name = new_customer;
        temp->num_Order = 1;

        return name;
    }

    int updateStoreQueue(table* store, table* FIFOqueue, string new_name, int result) {
        string kick = updateFIFOqueue(store, new_name);
        table* temp = FIFOqueue->next;
        while(temp->name != kick) temp = temp->next;

        temp->name = new_name;
        temp->result = result;
        return temp->ID;
    }


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

void REG(string name, hashMap<int, string, int, int>* seaView, AVLTree* mountainView, restaurant* storeQueue, restaurant* FIFOqueue) {
    int result = bin_to_dec(buildHuffmanTree(name));
    int ID = result % MAXSIZE + 1;
    int checkOrder = storeQueue->search(name);
    if(storeQueue->totalCustomer(storeQueue->recentTable) != 0 && checkOrder != 0) {
        if(seaView->search(result, name) != -1) seaView->arr[seaView->search(result, name)]->num_Order += 1;
        if(mountainView->find(result, name)) {
            mountainView->searchDuplicateOrder(result, name);
        }

        FIFOqueue->addFiFo(name);
    }
    else {
        // This brach use when reult % 2 == 1 mean they will seat in seaView queue and divide this case into four subcase 
        if(result % 2 != 0) {
            if(!storeQueue->isFull(storeQueue->recentTable)) {
                int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                seaView->insertNode(result, name, realID, 1);

                FIFOqueue->addFiFo(name);
            }
            else {
                if(mountainView->count(mountainView->getRoot()) != 0) {
                    storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                    mountainView->insert(result, name, ID, 1);

                    FIFOqueue->addFiFo(name);
                }
                else {
                    int OPT = result % 3;
                    if(OPT == 0) {
                        // get ID for the new customer 
                        string kick_name = storeQueue->getFirstName();
                        int kick_result = storeQueue->getFirstResult();

                        int getID = updateStoreQueue(storeQueue->recentTable, FIFOqueue->recentTable, name, result);
                        // get the name of the ealiest customer went to the restaurant 
                        int result_kick_name = bin_to_dec(buildHuffmanTree(kick_name));
                        int search_in_Hash = seaView->search(result_kick_name, kick_name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result_kick_name;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = getID;
                        }

                        if(mountainView->find(kick_result, kick_name)) {
                            mountainView->remove(mountainView->getRoot(), kick_result, kick_name);
                            mountainView->insert(result, name, getID, 1);
                        }
                    }
                    else if(OPT == 1) {

                    }
                    else {

                    }
                }
            }
        }
        else {
            if(mountainView->count(mountainView->getRoot()) != (MAXSIZE/2)) {
                storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                mountainView->insert(result, name, ID, 1);

                FIFOqueue->addFiFo(name);
            }
            else {
                if(!storeQueue->isFull(storeQueue->recentTable)) {
                    int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                    seaView->insertNode(result, name, realID, 1);

                    FIFOqueue->addFiFo(name);
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
        storeQueue->recentTable = storeQueue->insert(storeQueue->recentTable,i,"",0,0);
    }

    // create queue to deal with the fully occupied situation
    restaurant* FIFOqueue = new restaurant();
    for (int i = 1; i <= MAXSIZE; i++)
    {
        FIFOqueue->recentTable = FIFOqueue->insert(FIFOqueue->recentTable,i,"",0,0);
    }
    // Initialize some important data structure to use throughout the assignment2
    hashMap<int, string, int, int>* seaView = new hashMap<int, string, int, int>;
    AVLTree* mountainView = new AVLTree;
    
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
    REG("tELYXTj",seaView,mountainView,storeQueue,FIFOqueue);
    REG("tELYXTj",seaView,mountainView,storeQueue,FIFOqueue);
    REG("tELYXTj",seaView,mountainView,storeQueue,FIFOqueue);
    REG("tELYXTj",seaView,mountainView,storeQueue,FIFOqueue);
    REG("tELYXaaabb",seaView,mountainView,storeQueue,FIFOqueue);
    REG("tELYXT",seaView,mountainView,storeQueue,FIFOqueue);
    REG("ETPtkkkkt",seaView,mountainView,storeQueue,FIFOqueue);
    REG("ETPtkttt",seaView,mountainView,storeQueue,FIFOqueue);

    mountainView->remove(mountainView->getRoot(), 2802, "tELYXTj");
    // REG("ETPtkttt",seaView,mountainView,storeQueue,FIFOqueue);
    // REG("ETPtkttt",seaView,mountainView,storeQueue,FIFOqueue);
    cout << storeQueue->search("tELYXTj") << endl;
    storeQueue->printList(storeQueue->recentTable);
    cout << "=================seaview===============================================\n";
    seaView->insertNode(33,"first",2,1);
    seaView->display();
    cout << "=================mountainview==========================================\n";

    mountainView->printTreeStructure();
    cout << "=================fifoqueue==========================================\n";
    FIFOqueue->printList(FIFOqueue->recentTable);


    // cout << mountainView->count(mountainView->root);
    // cout << mountainView->count(mountainView->root);
    // storeQueue->printList(storeQueue->recentTable);
    // cout << storeQueue->totalCustomer(storeQueue->recentTable)<<endl;
    // cout<< seaView->search(bin_to_dec(buildHuffmanTree("tELYXTj")),"tELYXTj")<<endl;
    // FIFOqueue->addFiFo("1");
    // FIFOqueue->addFiFo("2");
    // FIFOqueue->addFiFo("3");
    // FIFOqueue->addFiFo("4");
    // FIFOqueue->addFiFo("5");
    // FIFOqueue->addFiFo("6");
    // FIFOqueue->addFiFo("7");
    // FIFOqueue->addFiFo("8");
    // FIFOqueue->addFiFo("9");
    // FIFOqueue->addFiFo("10");
    // FIFOqueue->addFiFo("11");
    // FIFOqueue->addFiFo("13");
    // FIFOqueue->addFiFo("14");
    // FIFOqueue->addFiFo("15");
    // FIFOqueue->addFiFo("16");
    // FIFOqueue->addFiFo("17");
    // FIFOqueue->addFiFo("18");
    // FIFOqueue->addFiFo("19");
    // FIFOqueue->addFiFo("20");
    // FIFOqueue->addFiFo("21");
    // FIFOqueue->addFiFo("22");
    // FIFOqueue->addFiFo("23");
    // FIFOqueue->addFiFo("24");
    // FIFOqueue->addFiFo("25");
    // FIFOqueue->addFiFo("26");
    // FIFOqueue->addFiFo("27");
    // FIFOqueue->addFiFo("28");
    // FIFOqueue->addFiFo("29");
    // FIFOqueue->addFiFo("30");
    // FIFOqueue->addFiFo("31");
    // FIFOqueue->addFiFo("32");
    // FIFOqueue->addFiFo("33");
    // cout << FIFOqueue->totalCustomer(FIFOqueue->recentTable) << " " << FIFOqueue->isFull(FIFOqueue->recentTable);
}