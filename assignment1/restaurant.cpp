#include "main.h"

/*===================================================================================*/
/*===================================================================================*/
// declare some global variable using during the assignment
table* update_changes = new table(0, "", 0, nullptr);
table* after_merge = nullptr;
/*===================================================================================*/
/*===================================================================================*/

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
/*============================== 2.2.1. FIRST REQUEST ===============================*/
// count the number of customers in queue
int count_num(restaurant* r) {
    int max_size = r->recentTable->ID;
    table* temp_table = r->recentTable->next;
    int count = 0;
    for(int i = 1; i <= max_size; i++) {
        if(temp_table->age >= 1 && temp_table->age <= 115) {
            count++;
        }
        temp_table = temp_table->next;
    }
    return count;
    delete temp_table;
}

// check if the restaurant if full?
bool is_full(restaurant* r) {
    if(count_num(r) == 15) return 1;
    else return 0;
}

// update a new customer to the queue
void enqueue(restaurant* r, string NAME, int AGE) {
    int max_size = r->recentTable->ID;
    table* temp = r->recentTable->next;
    for(int i = 1; i <= max_size; i++) {
        if(temp->age == 0) {
            temp->name = NAME;
            temp->age = AGE;
            break;
        }
        temp = temp->next;
    }
}

void dequeue(restaurant* queue) {
    table* del = queue->recentTable->next;

    for(int i = 1; i < 15; i++) {
        del->name = del->next->name;
        del->age = del->next->age;
        del = del->next;
    }
    del->name = "";
    del->age = 0;
}
// pop the first customer from the queue
void updateRes(restaurant* r, restaurant* queue) {
    while(count_num(r) < 15 && count_num(queue) > 0) {
        table* del = queue->recentTable->next;
        table* addInfo = r->recentTable->next;
        while(addInfo->age != 0) addInfo = addInfo->next;
        addInfo->name = del->name;
        addInfo->age = del->age;
        dequeue(queue);
    }
}

void REG(restaurant* r, restaurant* queue, restaurant* queueForPS, int ID, string NAME, int AGE) {
    if(is_full(r)) { // if restaurant is full, add the customer to the queue
        if(is_full(queue)) return;
        else {
            enqueue(queue, NAME, AGE);
        }
    }
    else { // if not full, add them to the chosen ID
        if(count_num(queue) >= 1 && count_num(queue) <= 15) {
            updateRes(r, queue);
            enqueue(queue, NAME, AGE);
        }
        else if(count_num(queue) == 0) {
            table* temp_table = r->recentTable->next;
            if(ID == 0) { // arbitrarily arrange the customer
                while(temp_table->age != 0) temp_table = temp_table->next;
                temp_table->name = NAME;
                temp_table->age = AGE;
            }
            else if(ID != 0) {
                while(temp_table->ID != ID) temp_table = temp_table->next;
                if(temp_table->age == 0) {
                    temp_table->name = NAME;
                    temp_table->age = AGE;
                }
                else {
                    while(temp_table->age != 0) temp_table = temp_table->next;
                    temp_table->name = NAME;
                    temp_table->age = AGE;
                }
            }
        }
    }
    enqueue(queueForPS, NAME, AGE);
}
/*===================================================================================*/
/*===================================================================================*/

/*===================================================================================*/
/*============================== 2.2.1. SECOND REQUEST ===============================*/
int getFirstNotEmpty(restaurant* r) {
    if(count_num(r) > 0) {
        table* temp_table = r->recentTable;
        while(temp_table->age == 0) temp_table = temp_table->next;
        return temp_table->ID;
    }
    else return -1;
    
}

//int countMaxEmpty(table* start) {
int countMaxEmpty(restaurant* r) {
    if(getFirstNotEmpty(r) == -1) return 15;

    table* start = r->recentTable;
    while(start->ID != getFirstNotEmpty(r)) start = start->next;
    int max_len = 0, cur_len = 0;
    table* temp_table = start;

    for(int i = 1; i <= 15; i++) {
        if (temp_table->age != 0) {
            cur_len = 0;
            temp_table = temp_table->next;
        } else {
            cur_len++;
            temp_table = temp_table->next;
        }

        if (cur_len > max_len) {
            max_len = cur_len;
        }
    }

    return max_len;
}

int countEmptyChain(restaurant* r, int NUM) {
    table* start = r->recentTable;
    while(start->ID != getFirstNotEmpty(r)) start = start->next;
    int max_len = 0, cur_len = 0, count_chain = 0;
    table* temp_table = start;

    for(int i = 1; i <= 15; i++) {
        if (temp_table->age != 0) {
            if(cur_len)
            cur_len = 0;
            temp_table = temp_table->next;
        } else {
            cur_len++;
            temp_table = temp_table->next;
        }

        if (cur_len > max_len) {
            max_len = cur_len;
        }
    }

    return max_len;
}
// điền chuỗi số âm vào age , tìm ra node bắt đầu thỏa mãn yêu cầu hàm regm
void fillBlankTable(restaurant* r) {
    table* start = r->recentTable->next;
    if(getFirstNotEmpty(r) == -1) return;
    while(start->age == 0) start = start->next;
    int value = 0;
    
    do
    {
        /* code */
        if (start->age != 0) {
            value = 0;
        } 
        else if(start->age == 0) {
            start->age = value; 
        }
        value--;
        start = start->next;
    } while (start->age >= 0);
    table* current = r->recentTable->next;
    for(int i = 1; i <= 15; i++) {
        if(current->age < -15) current->age = current->age + 15;
        current = current->next;
    }
}

bool checkAge(restaurant* r, int NUM) {

}
int findIndex(restaurant* r, int NUM) {
    if(count_num(r) == 0) return 15;
    else {
        fillBlankTable(r);
        int index = 0, max_index = 0;
        table* temp_table = r->recentTable;
        for(int i = 1; i <= 16; i++) {
            if(temp_table->age < 0 && temp_table->next->age > 0 && abs(temp_table->age) >= NUM) {
                if(temp_table->ID - NUM < 0) {
                    max_index = 16 + (temp_table->ID + temp_table->age);
                    break;
                }
                else {
                    index = 1 + (temp_table->ID + temp_table->age);
                    if(index > max_index) max_index = index;
                }
            }
            temp_table = temp_table->next;
        }
        return max_index;
        temp_table = nullptr;
        delete temp_table;
    }
}



void deleteStupidAge(restaurant* r) {
    table* backup = r->recentTable;
        for(int i = 1; i <= 16; i++) {
            if(backup->age < 0) backup->age = 0;
            backup = backup->next;
        }
    backup = nullptr;
    delete backup;
}
int emptyChainLength(restaurant* r, int NUM) {
    int start = findIndex(r, NUM);
    deleteStupidAge(r);
    int count = 0;
    table* temp_table = r->recentTable;
    while(temp_table->ID != start) temp_table = temp_table->next;
    while(temp_table->age == 0) {
        count++;
        temp_table = temp_table->next;
    }
    return count; 
    temp_table = nullptr;
    delete temp_table;  
}

int getTableID(restaurant* r, int NUM) {
    int start = findIndex(r, NUM);
    int length = emptyChainLength(r, NUM);
    int sum = (start + length - 1);
    if(sum <= 15) {
        return (start + (length - NUM));
    }
    else if(sum > 15) {
        int len_first_half = 15 - start + 1;
        int len_second_half = length - len_first_half;
        if(len_second_half >= NUM) return 15;
        else return (start + (length - len_second_half));
    }
}

bool isMergeForm(restaurant* r) {
    table* temp_table = r->recentTable;
    for(int i = 1; i <= 16; i++) {
        if(abs(temp_table->ID - temp_table->next->ID) != 1) {
            return 1;
            break;
        }
    }
    return 0;
}

void REGM(restaurant* r, restaurant* queue, restaurant* queueForPS, string NAME, int AGE, int NUM, table* merge) {
    if(isMergeForm(r) && (countMaxEmpty(r) < NUM)) return;
    int index = getTableID(r, NUM);
    table* temp_table = r->recentTable;
    while(temp_table->ID != index) temp_table = temp_table->next;
    merge = temp_table;
    table* var_table = temp_table;
    for(int i = 1; i <= NUM; i++) var_table = var_table->next;
    merge->next = var_table;
    enqueue(queueForPS, NAME, AGE);

    // temp_table = nullptr;
    // var_table = nullptr;
    // delete temp_table;
    // delete var_table;
}
/*===================================================================================*/
/*===================================================================================*/


/*===================================================================================*/
/*============================== 2.2.4. FOURTH REQUEST ===============================*/
// function to reverse the list to print the latest order
void reverseCircularLinkedList(restaurant* queue){
    int max_size = queue->recentTable->ID;

    // temporarily transform circular->singly list before reverse
    table* tailNode = queue->recentTable;
    queue->recentTable= queue->recentTable->next;
    table* headNode = queue->recentTable;
    tailNode->next = nullptr;

    // initialize some helpful TABLE to reverse
    table *current = queue->recentTable;
    table *prev = NULL;

    table *next = NULL;
    // traversal through the temporary singly list
    while (current != NULL) {
        // assign NEXT table after CURRENT table
        next = current->next;

        // assign next of the CURRENT table to the PREV table
        current->next = prev;

        // increment the loop
        prev = current;
        current = next;
    }

    // turn back to circular list and assign the LAST table be recentable in queue
    queue->recentTable->next = tailNode;
    queue->recentTable= headNode;
    
    // table* temp = prev;
    // // cout << prev->ID << " ";
    // for( int i = 1;  i <= 30; i++) {
    //     cout << temp->ID << " " ;
    //     temp=temp->next;
    // }
    table* temp_table = tailNode;
    for(int i = 1; i <= max_size; i++) {
        temp_table->ID = i;
        temp_table = temp_table->next;
    }
}

// main function to implement fourth request
void PS(restaurant* queue, int NUM) {
    reverseCircularLinkedList(queue);

    table* temp_table = queue->recentTable->next;
    int total = count_num(queue);
    // if total of customer in the meantime = 0 print the notice "empty"
    if(total == 0) cout << "Empty\n";
    else { // if total > 0 
        // if NUM argument is ambuguos or (> total && < 30) print all the queue
        if(NUM == 0 || (NUM > total && NUM <= 2*MAXSIZE)) {
            while(temp_table->age == 0) temp_table = temp_table->next;
            for(int i = 1; i <= total; i++) {
                cout << temp_table->name << "\n";
                temp_table = temp_table->next;
            }
        }
        // if < total print the NUM latest customer went to the restaurant
        else if(NUM <= total) {
            while(temp_table->age == 0) temp_table = temp_table->next;
            for(int i = 1; i <= NUM; i++) {
                cout << temp_table->name << "\n";
                temp_table = temp_table->next;
            }
        }
    }
}
/*===================================================================================*/
/*===================================================================================*/

/*===================================================================================*/
/*============================== 2.2.5. FIFTH REQUEST ===============================*/
void PQ(restaurant* queue, int NUM) {
    table* temp_table = queue->recentTable->next;
    if(NUM == 0) {
        for(int i = 1; i <= MAXSIZE; i++) {
            if(temp_table->age != 0) cout << temp_table->name << "\n";
            temp_table = temp_table->next;
        }
    }
    else {
        int count = count_num(queue);
        if(count == 0) cout << "Empty\n";
        else {
            if(NUM <= count) {
                for(int i = 1; i <= NUM; i++) {
                    cout << temp_table->name << "\n";
                    temp_table = temp_table->next;
                }
            }
            else if(NUM > count && NUM <= MAXSIZE) {
                for(int i = 1; i <= MAXSIZE; i++) {
                    if(temp_table->age != 0) cout << temp_table->name << "\n";
                    temp_table = temp_table->next;
                }
            }
        }
    }
}

/*===================================================================================*/
/*===================================================================================*/

/*===================================================================================*/
/*============================== 2.2.7. SENVENTH REQUEST ============================*/
void swapInfo(table* p, table* q) {
    string NAME = p->name;
    int AGE = p->age;

    p->name = q->name;
    p->age = q->age;

    q->name = NAME;
    q->age = AGE;
}

void SQ(restaurant* q, int NUM) {
    int waiting = count_num(q);
    // case 1: no customer in queue -> print line "empty"
    if(waiting == 0) cout << "Empty\n";
    // case 2: only need to sort fisrt NUM oldest customers
    else if(NUM <= waiting) {
        table* flag = q->recentTable->next;   
        while(flag->ID <= NUM) {
            table* var = flag;
            table* current = flag->next; 
            while(current->ID <= waiting) {
                if(current->age > var->age) var = current;
                current = current->next;
            }
            swapInfo(flag, var);
            flag = flag->next;
        }
    }
    //case 3: sort all the customer in queue 
    else if(NUM > waiting && NUM <= MAXSIZE) {
        table* flag = q->recentTable->next;   
        while(flag->ID <= waiting) {
            table* var = flag;
            table* current = flag->next; 
            while(current->ID <= waiting) {
                if(current->age > var->age) var = current;
                current = current->next;
            }
            swapInfo(flag, var);
            flag = flag->next;
        }
    }
    table* print = q->recentTable->next;
    for(int i = 1; i <= waiting; i++) {
        cout << print->name << "\n";
        print = print->next;
    }
}
/*===================================================================================*/
/*===================================================================================*/
void simulate(string filename, restaurant* r)
{
    //  check if the input file is read
    ifstream input_file("test1.txt");
    if (!input_file.is_open()) {
        return;
    }

    // innitialize a waiting queue
    // QUEUE LENGTH = MAXSIZE 
    restaurant* q = new restaurant();
    for (int i = 1; i <= MAXSIZE; i++)
    {
        q->recentTable = q->insert(q->recentTable,i,"",0);
    }

    restaurant* queueForPS = new restaurant();
    for (int i = 1; i <= 2*MAXSIZE; i++)
    {
        queueForPS->recentTable = queueForPS->insert(queueForPS->recentTable,i,"",0);
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
            if(length == 3) {
                string NAME = firstWord(removeFirst(newline));
                string str_AGE = firstWord(removeFirst(removeFirst(newline)));
                if(checkInt(str_AGE)) {
                    int AGE = stoi(str_AGE);
                    if(AGE >= 1 && AGE <= 115) {
                        REG(r, q, queueForPS, 0, NAME, AGE);
                    }
                }
            }
            else if(length == 4) {
                string str_ID = firstWord(removeFirst(newline));
                string NAME = firstWord(removeFirst(removeFirst(newline)));
                string str_AGE = firstWord(removeFirst(removeFirst(removeFirst(newline))));
                if(checkInt(str_AGE) && checkInt(str_ID)) {
                    int ID = stoi(str_ID);
                    int AGE = stoi(str_AGE);
                    if(AGE >= 1 && AGE <= 115 && ID >=1 && ID <= MAXSIZE) {
                        REG(r, q, queueForPS, ID, NAME, AGE);
                    }
                }
            }
            else continue;
        }
        else if(command == "REGM") {// check some necessary condition before implement request 
            if(length == 4) {
                string NAME = firstWord(removeFirst(newline));
                string str_AGE = firstWord(removeFirst(removeFirst(newline)));
                string str_NUM = firstWord(removeFirst(removeFirst(removeFirst(newline))));
                if(checkInt(str_AGE) && checkInt(str_NUM)) {
                    int AGE = stoi(str_AGE);
                    int NUM = stoi(str_NUM);
                    if(AGE >= 1 && AGE <= 115 && NUM >=1 && NUM <= MAXSIZE) {
                        cout << "regm" << " " << NAME << " " << AGE << " " << NUM << endl;
                    }
                }
            }
            else continue;
        }
        else if(command == "CLE") {// check some necessary condition before implement request 
            if(length == 2) {
                string str_ID = firstWord(removeFirst(newline));
                if(checkInt(str_ID)) {
                    int ID = stoi(str_ID);
                    if(ID >= 1 && ID <= MAXSIZE) {
                        cout << "cle" << " " << ID << endl;
                    }
                }
            }
            else continue;
        }
        else if(command == "PS") {// check some necessary condition before implement request 
            if(length == 1) {
                cout << "ps" << " " << 0 << endl;
            }
            else if(length == 2) {
                string str_NUM = firstWord(removeFirst(newline));
                if(checkInt(str_NUM)) {
                    int NUM = stoi(str_NUM);
                    if(NUM >= 1 && NUM <= (2*MAXSIZE)) {
                        cout << "ps" << " " << NUM << endl;
                    }
                }
            }
            else continue;
        }
        else if(command == "PQ") {// check some necessary condition before implement request 
            if(length == 1) {
                cout << "pq" << " " << 0 << endl;
            }
            else if(length == 2) {
                string str_NUM = firstWord(removeFirst(newline));
                if(checkInt(str_NUM)) {
                    int NUM = stoi(str_NUM);
                    if(NUM >= 1 && NUM <= MAXSIZE) {
                        cout << "pq" << " " << NUM << endl;
                    }
                }
            }
            else continue;
        }
        else if(command == "SQ") {// check some necessary condition before implement request 
            if(length == 2) {
                string str_NUM = firstWord(removeFirst(newline));
                if(checkInt(str_NUM)) {
                    int NUM = stoi(str_NUM);
                    if(NUM >= 1 && NUM <= MAXSIZE) {
                        cout << "sq" << " " << NUM << endl;
                    }
                }
            }
            else continue;
        }
        else continue;
    }
    table* temp = r->recentTable->next;
    for (int i = 1; i <= MAXSIZE; i++)
    {
        cout << temp->ID << " " << temp->name << " " << temp->age << " " << endl;
        temp = temp->next;
    }
    cout << count_num(r) << " " << is_full(r);
    delete q;
    delete queueForPS;
    
}




















// // IMPLEMENT THE SQ FUNCTION FOR SORTING THE QUEUE  
// // swap info of two table
// void swapInfo(table* guest1, table* guest2) {
//         string temp_name = guest1->name;
//         int temp_age = guest1->age;

//         guest1->name = guest2->name;
//         guest1->age = guest2->age;

//         guest2->name = temp_name;
//         guest2->age = temp_age;
// }

