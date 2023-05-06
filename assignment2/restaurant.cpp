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
    void deleteNode(K key) {
        int hashIndex = hashCode(key);
        // Delete and return the value of node
        while(arr[hashIndex] != nullptr) {
            if(arr[hashIndex]->key == key) {
                arr[hashIndex] = dummy;
                size--;
                //return temp->value;
            }
            hashIndex = (hashIndex+1)%capacity;
        }
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

// An AVL tree node
class AVLNode {
public:
	int key;
	string name;
    int ID;
    int num_Order;
	AVLNode *left;
	AVLNode *right;
	int height;
};

// A utility function to get height
// of the tree
int height(AVLNode* N) {
	if (N == NULL) return 0;
	return N->height;
}

// A utility function to get maximum
// of two integers
int max(int a, int b) {
	return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and NULLptr left and right pointers. */
AVLNode* newNode(int key,string name,int ID,int num_Order) {
	AVLNode* node = new AVLNode();

	node->key = key;
	node->name = name;
	node->ID = ID;
	node->num_Order = num_Order;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially // added at leaf

	return(node);
}

// A utility function to right rotate subtree rooted with y See the diagram given above.
AVLNode *rightRotate(AVLNode *y) {
	AVLNode *x = y->left;
	AVLNode *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left),height(y->right)) + 1;
	x->height = max(height(x->left),height(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x See the diagram given above.
AVLNode *leftRotate(AVLNode *x) {
	AVLNode *y = x->right;
	AVLNode *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int getBalance(AVLNode *N)
{
	if (N == NULL) return 0;
	return height(N->left) - height(N->right);
}

AVLNode* insert(AVLNode* node, int key,string name,int ID,int num_Order) {
	/* 1. Perform the normal BST rotation */
	if (node == NULL) return(newNode(key, name, ID, num_Order));

	if (key < node->key) 
		node->left = insert(node->left, key, name, ID, num_Order);
	else if (key > node->key)
		node->right = insert(node->right, key, name, ID, num_Order);
	else // Equal keys not allowed
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
						height(node->right));

	/* 3. Get the balance factor of this
		ancestor node to check whether
		this node became unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced,
	// then there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->key) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

/* Given a non-empty binary search tree,
return the node with minimum key value
found in that tree. Note that the entire
tree does not need to be searched. */
AVLNode * minValueNode(AVLNode* node) {
	AVLNode* current = node;
	/* loop down to find the leftmost leaf */
	while (current->left != NULL) current = current->left;
	return current;
}

// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
AVLNode* deleteNode(AVLNode* root, int key,string name) {
	// STEP 1: PERFORM STANDARD BST DELETE
	if (root == NULL) return root;

	// If the key to be deleted is smaller than the root's key, then it lies in left subtree
	if ( key < root->key )
		root->left = deleteNode(root->left, key, name);

	// If the key to be deleted is greater than the root's key, then it lies in right subtree
	else if( key > root->key )
		root->right = deleteNode(root->right, key, name);

	// if key is same as root's key, then
	// This is the node to be deleted
	else {
		// node with only one child or no child
		if( (root->left == NULL) || (root->right == NULL) ) {
			AVLNode *temp = root->left ? root->left : root->right;

			// No child case
			if (temp == NULL) {
				temp = root;
				root = NULL;
			}
			else // One child case
			*root = *temp; // Copy the contents of the non-empty child
			free(temp);
		}
		else {
			// node with two children: Get the inorder successor (smallest in the right subtree)
			AVLNode* temp = minValueNode(root->right);

			// Copy the inorder successor's
			// data to this node
			root->key = temp->key;
			root->name = temp->name;
			root->ID = temp->ID;
			root->num_Order = temp->num_Order;

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp->key, temp->name);
		}
	}

	// If the tree had only one node
	// then return
	if (root == NULL) return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + max(height(root->left), height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
	int balance = getBalance(root);

	// If this node becomes unbalanced, then there are 4 cases
	// Left Left Case
	if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);

	// Left Right Case
	if (balance > 1 && getBalance(root->left) < 0) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);

	// Right Left Case
	if (balance < -1 && getBalance(root->right) > 0) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void preOrder(AVLNode *root) {
	if(root != NULL) {
		cout << root->ID << "-" << root->key << "-" << root->num_Order << endl;
		preOrder(root->left);
		preOrder(root->right);
	}
}

int count(AVLNode* root) {
        if(root == nullptr) return 0;
        int lh = count(root->left);
        int rh = count(root->right);

        return 1 + lh + rh;
}

bool find_AVLNode(AVLNode* root , int result, string name){
    AVLNode* temp = root;
    while(temp != nullptr) {
        if(temp->key == result && temp->name == name) return 1;
        if(result < temp->key ) temp=temp->left;//|| (result == temp->result && compareStrings(root->name, name) > 0)
        else temp=temp->right;
    }
    return 0;
}

void searchDuplicateOrder(AVLNode* root, int result, string name) {
        AVLNode* temp = root;
        while(temp != nullptr) {
            if(temp->key == result && temp->name == name) temp->num_Order++;
            if(result < temp->key ) temp=temp->left;//|| (result == temp->result && compareStrings(root->name, name) > 0)
            else temp=temp->right;
        }
    }
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
            table* first = recentTable->next;
            int getID = result%MAXSIZE + 1;
            while(first->ID != getID) first = first->next;
            while(first->name != "") first = first->next;

            first->name = name;
            first->result = result;

            return first->ID;
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

    int getIDbyname(string name) {
        table* first = recentTable->next;
        for(int i = 1; i <= MAXSIZE; i++) {
            if(first->name == name) {
                return first->ID;
            }
            first = first->next;
        }
        return 0;
    }
    // USE FOR REG FUNCTION
    void update_OPT_2_store(string old_name, int new_result, string new_name) {
        table* first = recentTable->next;
        while(first->name != old_name) first = first->next;
        first->name = new_name;
        first->result = new_result;
    }
    // USE FOR REG FUNCTION
    void update_OPT_2_FIFO(string old_name, int new_result, string new_name) {
        table* first = recentTable->next;
        while(first->name != old_name) first = first->next;
        while(first->ID != MAXSIZE) {
            first->name =first->next->name;
            first->num_Order = first->next->num_Order;

            first = first->next;
        }
        first->name = new_name;
        first->result = new_result;
        first->num_Order = 1;
    }

    //USE FOR CLE FUNCTION
    // find if the table with specified ID has a customer
    bool find_by_ID(int NUM) {
        table* temp = recentTable->next;
        while(temp->ID != NUM) temp = temp->next;
        if(temp->name != "") return 1;
        return 0;
    }

    string find_name_by_ID(int NUM) {
        table* temp = recentTable->next;
        while(temp->ID != NUM) temp = temp->next;
        return temp->name;
    }

    int find_result_by_ID(int NUM) {
        table* temp = recentTable->next;
        while(temp->ID != NUM) temp = temp->next;
        return temp->result;
    }

    int find_result_by_name(string name) {
        table* temp = recentTable->next;
        while(temp->name != name) temp = temp->next;
        return temp->result;
    }

    void store_queue_CLE(int NUM) {
        table* temp = recentTable->next;
        while(temp->ID != NUM) temp = temp->next;
        temp->name = "";
        temp->result = 0;
    }

    void store_queue_CLE(string name) {
        table* temp = recentTable->next;
        while(temp->name != name) temp = temp->next;
        temp->name = "";
        temp->result = 0;
    }

    void FIFO_queue_CLE(string name) {
        table* temp = recentTable->next;
        while(temp->name != name) temp = temp->next;
        temp->name = "";
        temp->num_Order = 0;
        while(temp->ID != MAXSIZE) {
            temp->name = temp->next->name;
            temp->num_Order = temp->next->num_Order;

            temp = temp->next;
        }
    }

    // Use for cle all queue
    string top_name() {
        return recentTable->next->name;
    }

    int top_result() {
        return recentTable->next->result;
    }

    void pop_first() {
        table* temp = recentTable->next;
        while(temp->ID != 32) {
            
        }
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



/*==================THIS IS LRCO QUEUE FOT OPT == 1==============================================*/
class LRCOtable {
public:
    int result;
    string name;
    int not_order_n_times;
    LRCOtable* next;

    LRCOtable(int result, string name, int not_order_n_times, LRCOtable* next) {
        this->name = name;
        this->result = result;
        this->not_order_n_times = not_order_n_times;
        this->next = next;
    } 

};

class LRCOqueue {
public:
    LRCOtable *recentTable;
    LRCOqueue() {
        recentTable = nullptr;
    }
    ~LRCOqueue() {
        delete recentTable;
    }

    LRCOtable* finsert(LRCOtable *last, int result, string name, int not_order_n_times)
    {
        if (last != nullptr)
            return last;
        LRCOtable *temp = new LRCOtable(result, name, not_order_n_times, last);

        last = temp;
        last->next = last;
        return last;
    }
    LRCOtable *insert(LRCOtable *last, int result, string name, int not_order_n_times)
    {
        if (last == NULL)
            return finsert(last, result, name, not_order_n_times);
        LRCOtable *temp = new LRCOtable (result, name, not_order_n_times, last->next);
        last -> next = temp;
        last = temp;
        return last;
    }

    void printList() {
        LRCOtable* first = recentTable->next;
        for(int i = 0; i < 32; i++) {
            cout << "Name " << first->name << " result " << first->result << " not order: " << first->not_order_n_times << endl;
            first=first->next;
        }
    }

    bool search(string name) {
        LRCOtable* temp = recentTable->next;
        for(int i = 1; i <= MAXSIZE; i++) {
            if(temp->name == name) return 1;
            temp = temp->next;
        }
        return 0;
    }

    void add_and_update(string name, int result) {
        LRCOtable* temp = recentTable->next;
        if(search(name)) {
            for(int i = 1; i <= MAXSIZE; i++) {
                if(temp->name != "") {
                    if(temp->name == name) temp->not_order_n_times = 0;
                    else temp->not_order_n_times++;
                    temp = temp->next;
                }
            }
        }
        else {
            while(temp->name != "") {
                temp->not_order_n_times++;
                temp = temp->next;
            }
            temp->name = name;
            temp->result = result;
            temp->not_order_n_times = 0;
        }
    }

    int get_result_by_name(string name) {
        LRCOtable* temp = recentTable->next;
        while(temp->name != name ) temp = temp->next;
        return temp->result;
    }

    string check_LRCO() {
        LRCOtable* temp = recentTable->next;
        int least_recently = 0;
        string kick_name = "";
        for(int i = 1; i <= MAXSIZE; i++) {
            if(temp->not_order_n_times > least_recently) {
                least_recently = temp->not_order_n_times;
                kick_name = temp->name;
            }
            temp = temp->next;
        }
        return kick_name;
    }

    void update_LRCO_queue(string old_name, int new_result, string new_name) {
        LRCOtable* temp = recentTable->next;
        while(temp->name != old_name) temp = temp->next;
        while(temp != recentTable) {
            temp->name = temp->next->name;
            temp->result = temp->next->result;
            temp->not_order_n_times = temp->next->not_order_n_times;

            temp = temp->next;
        }
        temp->name = new_name;
        temp->result = new_result;
        temp->not_order_n_times = 0;
    }

    // use for cle function
    void lrco_queue_CLE(string name) {
        LRCOtable* temp = recentTable->next;
        while(temp->name != name) temp = temp->next;
        while(temp != recentTable) {
            temp->name = temp->next->name;
            temp->result = temp->next->result;
            temp->not_order_n_times = temp->next->not_order_n_times;

            temp = temp->next;
        }
    }
};

int update_Other_queue(restaurant* storeQueue, restaurant* FIFOqueue, LRCOqueue* LRqueue, string name, int result){
    string kick_name = LRqueue->check_LRCO();
    LRqueue->update_LRCO_queue(kick_name, result, name);

    table* temp = FIFOqueue->recentTable->next;
    while(temp->name != name) temp = temp->next;
    while(temp != FIFOqueue->recentTable) {
        temp->name = temp->next->name;
        temp->num_Order = temp->next->num_Order;

        temp = temp->next;
    } 
    temp->name = name;
    temp->num_Order = 1;

    table* flag = storeQueue->recentTable->next;
    while(flag->name != name) flag = flag->next;
    flag->name = name;
    flag->result = result;

    return flag->ID;
}

/*=================================================THIS IS LFCO QUEUE FOR OPT == 2 ==============*/
class LFCOtable {
public:
    int rank;
    int result;
    string name;
    int ID;
    int num_Orders;
    

    LFCOtable(int rank, int result, string name, int ID,int num_Orders): rank(rank), result(result), name(name), ID(ID),num_Orders(num_Orders) {}
};

class MinHeap {
private:
    vector<LFCOtable> heap;
    unsigned int maxSize;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    bool hasLeftChild(int i) { return unsigned(leftChild(i)) < heap.size(); }
    bool hasRightChild(int i) { return unsigned(rightChild(i)) < heap.size(); }

    void heapifyUp(int i) {
        if (i && (heap[parent(i)].num_Orders > heap[i].num_Orders || (heap[parent(i)].num_Orders == heap[i].num_Orders && heap[parent(i)].rank > heap[i].rank))) {
            swap(heap[i], heap[parent(i)]);
            heapifyUp(parent(i));
        }
    }

    void heapifyDown(int i) {
        int minIndex = i;
        int l = leftChild(i);
        int r = rightChild(i);
        if (hasLeftChild(i) && (heap[l].num_Orders < heap[minIndex].num_Orders || (heap[l].num_Orders == heap[minIndex].num_Orders && heap[l].rank < heap[minIndex].rank))) {
            minIndex = l;
        }
        if (hasRightChild(i) && (heap[r].num_Orders < heap[minIndex].num_Orders || ( heap[r].num_Orders == heap[minIndex].num_Orders && heap[r].rank < heap[minIndex].rank))) {
            minIndex = r;
        }
        if (minIndex != i) {
            swap(heap[i], heap[minIndex]);
            heapifyDown(minIndex);
        }
    }

public:
    MinHeap(int maxSize) : maxSize(maxSize) {}

    int getRank() {return heap.size()+1;}

    void insert_if_not_full(LFCOtable value) {
        if (heap.size() != maxSize) {
            heap.push_back(value);
            heapifyUp(heap.size() - 1);
        }
    }
    //use to find and increase the num_order by 1 if its a duplicate order
    void insert_if_duplicate(string name) {
        int index = find(name);
        int rank = heap[index].rank;
        int result = heap[index].result;
        string name_update = heap[index].name;
        int ID = heap[index].ID;
        int num = heap[index].num_Orders;
        remove_for_replace(name);
        insert_if_not_full(LFCOtable(rank, result, name_update, ID,++num));
    }

    //use to find and increase the num_order by 1 if its a duplicate order
    void remove_for_replace(string name) {
        int index = find(name);
        if (index != -1) {
            heap[index] = heap[heap.size() - 1];
            heap.pop_back();
            if (unsigned(index) < heap.size()) {
                heapifyDown(index);
                heapifyUp(index);
            }
        }
    }

    int find(string name) {
        for (unsigned int i = 0; i < heap.size(); i++) {
            if (heap[i].name == name) {
                return i;
            }
        }
        return -1;
    }

    // solve conflict in case of OPT== 0 || == 1
    void updateRank(int index) {
        for(unsigned int i = 0; i < heap.size(); i++) {
            if(heap[i].rank > index) heap[i].rank--;
        }
    }

    void solve_conflict(string name) {
        int index = find(name);
        if (index != -1) {
            heap[index] = heap[heap.size() - 1];
            heap.pop_back();
            if (unsigned(index) < heap.size()) {
                heapifyDown(index);
                heapifyUp(index);
            }
        }
        updateRank(index);
    }
    // solve conflict in case of OPT==2
    LFCOtable pop() {
        if (heap.size() == 0) {
            throw "Heap is empty";
        }
        LFCOtable min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);

        updateRank(min.rank);
        return min;
    }

    //USE FOR CLE FUNCTION
    void lfco_queue_CLE(string name) {
        solve_conflict(name);
    }

    void print() {
        for (auto elem : heap) {
            cout << "Rank: " << elem.rank << " " << elem.name  << ", NUM: " << elem.num_Orders << ", RESULT: " << elem.result << endl;
        }
    }

    void preorder(int i = 0) {
        if (unsigned(i) >= heap.size()) return;

        cout << heap[i].ID << "-" << heap[i].num_Orders << endl;

        preorder(leftChild(i));
        preorder(rightChild(i));
    }
};
/*===============================================================================================*/
/*===============================================================================================*/

//=======================================================DEMO ONLY================================
/*===============================================================================================*/
/*=======================1. FIRST REQUEST - IMPLEMENT REG COMMAND================================*/
// get ID for ID infomation 
// A Tree node
class HuffNode {
public:
    char ch;
    int pri;
    int freq;
    HuffNode* left;
    HuffNode* right;
};

// Function to create a new node
HuffNode* newNode(char ch, int freq, HuffNode* left, HuffNode* right) {
    HuffNode* node = new HuffNode;
    node->ch = ch;
    node->pri = 0;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

// Comparison object to be used to order the heap
struct compare {
    bool operator()(HuffNode* l, HuffNode* r) {
        if(l->freq == r -> freq)
        {
            if(l->ch == r ->ch) return l->pri > r -> pri;
            return l->ch > r->ch;
        }
        return l->freq > r->freq;
    }
};

// Function to build the Huffman tree and return the root node
HuffNode* buildHuffmanTree(map<char, int>& freqMap) {
    priority_queue<HuffNode*, vector<HuffNode*>, compare> pq;
    int prio = 0;
    // Create a leaf node for each character and add it to the priority queue
    for (auto& pair : freqMap) {
        pq.push(newNode(pair.first, pair.second, nullptr, nullptr));
    }
    if(pq.size() == 1)
    {
        HuffNode* right = pq.top();
        pq.pop();
        HuffNode* left = nullptr;
        HuffNode* parent = newNode('}', right->freq, left, right);
        pq.push(parent);
        prio++;
    }
    // Repeat until there is only one node left in the priority queue
    while (pq.size() > 1) {
        // Remove the two nodes of highest priority
        HuffNode* left = pq.top();
        pq.pop();
        HuffNode* right = pq.top(); 
        pq.pop();

        // Create a new internal node with the sum of the two highest frequencies
        // Make the two removed nodes as left and right children of the new node
        HuffNode* parent = newNode('}', left->freq + right->freq, left, right);
        parent->pri = prio;
        prio++;

        // Add the new node to the priority queue
        pq.push(parent);
    }

    // The remaining node is the root node of the Huffman tree
    return pq.top();
}

// Function to traverse the Huffman tree and generate the codes
void generateCodes(HuffNode* root, string code, map<char, string>& codeMap) {
    if (root == nullptr) {
        return;
    }
    // Leaf node
    if (root->left == nullptr && root->right == nullptr) {
        codeMap[root->ch] = code;
    }

    // Traverse left subtree
    generateCodes(root->left, code + "0", codeMap);

    // Traverse right subtree
    generateCodes(root->right, code + "1", codeMap);
}

// Function to encode the input string using the Huffman codes
string buildHuffmanTree(string name) {
    map<char, int> freqMap;
    for (char ch : name) {
        freqMap[ch]++;
    }

    // Build the Huffman tree and get the root node
    HuffNode* root = buildHuffmanTree(freqMap);

    // Generate the Huffman codes for each character
    map<char, string> codeMap;
    generateCodes(root, "", codeMap);

    // Encode the input string using the Huffman codes
    string encodedStr = "";
    for (char ch : name) {
        encodedStr += codeMap[ch];
    }
	if(encodedStr.length() > 15)
	{
		encodedStr = encodedStr.substr(encodedStr.length() - 15, 15);
	}
    return encodedStr;
}

int bin_to_dec(string nameEncode) {
	int num = 0;
    int len = nameEncode.length();

    for (int i = 0; i < len; i++) {
        if (nameEncode[i] == '1') {
            num += pow(2, len - 1 - i);
        }
    }
    return num;
}


int getIDCustomer(int result) {
    return (result%MAXSIZE + 1);
}
int getOPT(int result) {
    return result%3;
}

void REG(string name, hashMap<int, string, int, int>* seaView, AVLNode* &mountainView, restaurant* storeQueue, restaurant* FIFOqueue, LRCOqueue* lrco_queue, MinHeap &lfco_queue) {
    int result = bin_to_dec(buildHuffmanTree(name));
    //int ID = result % MAXSIZE + 1;
    int checkOrder = storeQueue->search(name);
    // if a customer is already sut in the restaurant and order another food, findthem and increase there num_orders by 1
    if(storeQueue->totalCustomer(storeQueue->recentTable) != 0 && checkOrder != 0) {
        if(seaView->search(result, name) != -1) seaView->arr[seaView->search(result, name)]->num_Order += 1;
        if(find_AVLNode( mountainView,result, name)) {
            searchDuplicateOrder( mountainView,result, name);
        }

        // incre the num_order of each customer with specified name by 1
        FIFOqueue->addFiFo(name);
        lrco_queue->add_and_update(name, result);
        lfco_queue.insert_if_duplicate(name);

        
    }
    // If this is the first time the customer go to the restaurant, add them into the restaurant follow the rule
    else {
        // This brach use when reult % 2 == 1 mean they will seat in seaView queue and divide this case into four subcase 
        if(result % 2 != 0) {
            // if the seaView queue is not full, add them to the seaViewQueue
            if(!seaView->isFull()) {
                // update the storeQueue after add a new customer
                int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                seaView->insertNode(result, name, realID, 1);
                
                // add the customer to the FIFO queue
                FIFOqueue->addFiFo(name);
                // add the customer to the LRCOqueue
                lrco_queue->add_and_update(name, result);
                // add the customer to the LFCOqueue
                lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, realID,1));
            }
            else {
                // if the seaView queue is full but the mountainView isnt, add them to to mountainView queue
                if(count(mountainView) != 0) {
                    int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                    mountainView = insert(mountainView,result, name, realID, 1);

                    // add the customer to the FIFO queue
                    FIFOqueue->addFiFo(name);
                    // add the customer to the LRCOqueue
                    lrco_queue->add_and_update(name, result);
                    // add the customer to the LFCOqueue
                    lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, realID, 1));
                    
                }
                else {
                    int OPT = result % 3;
                    if(OPT == 0) {
                        // get ID for the new customer 
                        string kick_name = storeQueue->getFirstName();
                        int kick_result = storeQueue->getFirstResult();

                        // get earliest customer from FIFO queue to update other queue
                        //1,2
                        int getID = updateStoreQueue(storeQueue->recentTable, FIFOqueue->recentTable, name, result);

                        int search_in_Hash = seaView->search(kick_result, kick_name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = getID;
                        }

                        if(find_AVLNode(mountainView, kick_result, kick_name)) {
                            deleteNode(mountainView, kick_result, kick_name);
                            mountainView = insert(mountainView, result, name, getID, 1);
                        }
                        //3
                        lrco_queue->update_LRCO_queue(kick_name, result, name);
                        //4
                        // update minheap-queue
                        lfco_queue.solve_conflict(kick_name);
                        lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, getID, 1));
                    }
                    else if(OPT == 1) {
                        string kick_name = lrco_queue->check_LRCO();
                        int kick_result = lrco_queue->get_result_by_name(kick_name);

                        // update 3 queues simutaneously
                        //1,2,3
                        int getID = update_Other_queue(storeQueue, FIFOqueue, lrco_queue, name, result);

                        int search_in_Hash = seaView->search(kick_result, kick_name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = getID;
                        }
                        if(find_AVLNode(mountainView, kick_result, kick_name)) {
                            mountainView = deleteNode(mountainView, kick_result, kick_name);
                            mountainView = insert(mountainView, result, name, getID, 1);
                        }
                        //4
                        // update minheap-queue
                        lfco_queue.solve_conflict(kick_name);
                        lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, getID, 1));
                    }
                    else {
                        //1
                        LFCOtable kick_customer = lfco_queue.pop();
                        int kick_ID = storeQueue->getIDbyname(kick_customer.name);

                        int search_in_Hash = seaView->search(kick_customer.result, kick_customer.name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = kick_ID;
                        }
                        if(find_AVLNode(mountainView, kick_customer.result, kick_customer.name)) {
                            mountainView = deleteNode(mountainView, kick_customer.result, kick_customer.name);
                            mountainView = insert(mountainView, result, name, kick_ID, 1);
                        }
                        //2,3,4
                        storeQueue->update_OPT_2_store(kick_customer.name, result, name);
                        FIFOqueue->update_OPT_2_FIFO(kick_customer.name, result, name);
                        lrco_queue->update_LRCO_queue(kick_customer.name, result, name);
                    }
                }
            }
        }
        else {
            if(count(mountainView) != (MAXSIZE/2)) {
                //1
                int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                mountainView = insert(mountainView, result, name, realID, 1);

                //2,3,4
                // add the customer to the FIFO queue
                FIFOqueue->addFiFo(name);
                // add the customer to the LRCOqueue
                lrco_queue->add_and_update(name, result);
                // add the customer to the LFCOqueue
                lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, realID,1));
        
            }
            else {
                if(!seaView->isFull()) {
                    //1
                    int realID = storeQueue->addNewCustomer(storeQueue->recentTable, result, name);
                    seaView->insertNode(result, name, realID, 1);

                    //2,3,4
                    // add the customer to the FIFO queue
                    FIFOqueue->addFiFo(name);
                    // add the customer to the LRCOqueue
                    lrco_queue->add_and_update(name, result);
                    // add the customer to the LFCOqueue
                    lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, realID, 1));
                    
                }
                else {
                    int OPT = result % 3;
                    if(OPT == 0) {
                        // get ID for the new customer 
                        string kick_name = storeQueue->getFirstName();
                        int kick_result = storeQueue->getFirstResult();

                        // get earliest customer from FIFO queue to update other queue
                        //1,2
                        int getID = updateStoreQueue(storeQueue->recentTable, FIFOqueue->recentTable, name, result);

                        int search_in_Hash = seaView->search(kick_result, kick_name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = getID;
                        }

                        if(find_AVLNode(mountainView,kick_result, kick_name)) {
                            mountainView = deleteNode(mountainView, kick_result, kick_name);
                            mountainView = insert(mountainView, result, name, getID, 1);
                        }
                        //3
                        lrco_queue->update_LRCO_queue(kick_name, result, name);
                        //4
                        // update minheap-queue
                        lfco_queue.solve_conflict(kick_name);
                        lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, getID, 1));
                    }
                    else if(OPT == 1) {
                        string kick_name = lrco_queue->check_LRCO();
                        int kick_result = lrco_queue->get_result_by_name(kick_name);

                        // update 3 queues simutaneously
                        //1,2,3
                        int getID = update_Other_queue(storeQueue, FIFOqueue, lrco_queue, name, result);

                        int search_in_Hash = seaView->search(kick_result, kick_name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = getID;
                        }
                        if(find_AVLNode(mountainView, kick_result, kick_name)) {
                            mountainView = deleteNode(mountainView, kick_result, kick_name);
                            mountainView = insert(mountainView, result, name, getID, 1);
                        }
                        //4
                        // update minheap-queue
                        lfco_queue.solve_conflict(kick_name);
                        lfco_queue.insert_if_not_full(LFCOtable(lfco_queue.getRank(), result, name, getID, 1));
                    }
                    else {
                        //1
                        LFCOtable kick_customer = lfco_queue.pop();
                        int kick_ID = storeQueue->getIDbyname(kick_customer.name);

                        int search_in_Hash = seaView->search(kick_customer.result, kick_customer.name);
                        if(search_in_Hash != -1) {
                            seaView->arr[search_in_Hash]->value = name;
                            seaView->arr[search_in_Hash]->key = result;
                            seaView->arr[search_in_Hash]->num_Order = 1;
                            seaView->arr[search_in_Hash]->ID = kick_ID;
                        }
                        if(find_AVLNode(mountainView, kick_customer.result, kick_customer.name)) {
                            mountainView = deleteNode(mountainView, kick_customer.result, kick_customer.name);
                            mountainView = insert(mountainView,result, name, kick_ID, 1);
                        }
                        //2,3,4
                        storeQueue->update_OPT_2_store(kick_customer.name, result, name);
                        FIFOqueue->update_OPT_2_FIFO(kick_customer.name, result, name);
                        lrco_queue->update_LRCO_queue(kick_customer.name, result, name);
                    }
                }
            }
        }
    }
    
}
/*================================================================================================================*/
/*================================================================================================================*/

/*================================================================================================================*/
/*============================================REQUEST 2: CLE======================================================*/
void CLE(int NUM, hashMap<int, string, int, int>* seaView, AVLNode* &mountainView, restaurant* storeQueue, restaurant* FIFOqueue, LRCOqueue* lrco_queue, MinHeap &lfco_queue) {
    if(NUM < 1) {
        int temp = 1;
        while(seaView->getSize() > 0) {
            string CLE_name = FIFOqueue->find_name_by_ID(temp);
            int CLE_result = storeQueue->find_result_by_name(CLE_name);
            if(seaView->search(CLE_result, CLE_name) != -1) {
                storeQueue->store_queue_CLE(CLE_name);
                FIFOqueue->FIFO_queue_CLE(CLE_name);
                lrco_queue->lrco_queue_CLE(CLE_name);
                lfco_queue.lfco_queue_CLE(CLE_name);
                seaView->deleteNode(CLE_result);
            }
            else temp++;
        }
    }
    else if(NUM > MAXSIZE) {
        int temp = 1;
        while(count(mountainView) > 0) {

            string CLE_name = FIFOqueue->find_name_by_ID(temp);
            int CLE_result = storeQueue->find_result_by_name(CLE_name);
            cout << CLE_result << find_AVLNode(mountainView,CLE_result, CLE_name) << endl;
            
            if(find_AVLNode(mountainView,CLE_result, CLE_name)) {
                storeQueue->store_queue_CLE(CLE_name);
                FIFOqueue->FIFO_queue_CLE(CLE_name);
                lrco_queue->lrco_queue_CLE(CLE_name);
                lfco_queue.lfco_queue_CLE(CLE_name);
                mountainView = deleteNode(mountainView, CLE_result, CLE_name);
            }
            else {
                temp++;
            }
        }
    }
    else {
        if(storeQueue->find_by_ID(NUM) == 1) {
            string CLE_name = storeQueue->find_name_by_ID(NUM);
            int CLE_result = storeQueue->find_result_by_ID(NUM);

            storeQueue->store_queue_CLE(NUM);
            FIFOqueue->FIFO_queue_CLE(CLE_name);
            lrco_queue->lrco_queue_CLE(CLE_name);
            lfco_queue.lfco_queue_CLE(CLE_name);
            int find = seaView->search(CLE_result, CLE_name);
            if(find != -1) {
                seaView->deleteNode(CLE_result);
            }
            if(find_AVLNode(mountainView, CLE_result, CLE_name)) {
                mountainView = deleteNode(mountainView, CLE_result, CLE_name);
            }

        } 
    }
}
/*================================================================================================================*/
/*================================================================================================================*/

/*================================================================================================================*/
/*============================================REQUEST 3: PRINTHT==================================================*/
//this function mean we need to print all the customer in the seaView-queue with the following form: "ID-Result-NUM/n"
void PRINTHT(hashMap<int, string, int, int>* seaView) {
    for(int i = 0; i < seaView->capacity; i++) {
        if(seaView->arr[i] != nullptr && seaView->arr[i]->key != -1) {
            cout << seaView->arr[i]->ID << "-" << seaView->arr[i]->key << "-" << seaView->arr[i]->num_Order << endl;
        }
    }
}
/*================================================================================================================*/
/*================================================================================================================*/

/*=========================================================================================================================*/
/*================================================REQUEST 4: PRINTAVL======================================================*/
// this function mean we need to print all the customer in the mountainView-queue with the following form: "ID-Result-NUM/n"
void PRINTAVL(AVLNode* &mountainView) {
    preOrder(mountainView);
}
/*=========================================================================================================================*/
/*=========================================================================================================================*/

/*=========================================================================================================================*/
/*================================================REQUEST 4: PRINTMH======================================================*/
// this function mean we need to print all the customer in the mountainView-queue with the following form: "ID-Result-NUM/n"
void PRINTMH(MinHeap &lfco_queue ) {
    lfco_queue.preorder(0);
}
/*=========================================================================================================================*/
/*=========================================================================================================================*/


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
    AVLNode* mountainView = nullptr;
    
    LRCOqueue* lrco_queue = new LRCOqueue();
    for (int i = 1; i <= 32; i++)
    {
        lrco_queue->recentTable = lrco_queue->insert(lrco_queue->recentTable,0,"",0);
    }

    MinHeap lfco_queue(32);

	ifstream input_file("test2.txt");
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
                REG(NAME, seaView, mountainView, storeQueue, FIFOqueue, lrco_queue,lfco_queue);
            }
            else continue;
        }
        else if(command == "CLE") {// check some necessary condition before implement request 
            if(length == 2) {
                string str_ID = firstWord(removeFirst(newline));
                if(checkInt(str_ID)) {
                    int ID = stoi(str_ID);
                    CLE(ID, seaView, mountainView, storeQueue, FIFOqueue, lrco_queue,lfco_queue);
                }
            }
            else continue;
        }
        else if(command == "PrintHT") {// check some necessary condition before implement request 
            PRINTHT(seaView);
        }
        else if(command == "PrintAVL") {// check some necessary condition before implement request 
            PRINTAVL(mountainView);
        }
        else if(command == "PrintMH") {// check some necessary condition before implement request 
            PRINTMH(lfco_queue);
        }
        else continue;
    }

    delete storeQueue;
    delete FIFOqueue;
    delete lrco_queue;

}