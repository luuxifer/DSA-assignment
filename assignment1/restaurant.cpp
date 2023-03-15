#include "main.h"

/*===================================================================================*/
/*===================================================================================*/
// declare some global variable using during the assignment

table* after_merge = nullptr;
int start_merge_at = 0, end_merge_at = 0, size_merge = 0;
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
bool isMergeForm(restaurant* r) {
    table* temp_table = r->recentTable;
    int count = 0;
    for(int i = 1; i <= 15; i++) {
        if(abs(temp_table->ID - temp_table->next->ID) == 1 || abs(temp_table->ID - temp_table->next->ID) == 14) {
            count++;
        }
        temp_table = temp_table->next;
    }
    if(count == 15) return 0;
    return 1;

    temp_table = nullptr;
    delete temp_table;
}

int count_num_mergeForm(restaurant* r) {
    table* temp_table = r->recentTable;
    int count = 0;
    while(temp_table->ID != end_merge_at) temp_table = temp_table->next;
    
    for(int i = 1; i <= (16 - size_merge); i++) {
            if(temp_table->age >= 1 && temp_table->age <= 115) {
                count++;
            }
            temp_table = temp_table->next;
    }
    return count;

    temp_table = nullptr;
    delete temp_table;
}
// count the number of customers in queue
int count_num(restaurant* r) {
    int max_size = r->recentTable->ID;
    table* temp_table = r->recentTable->next;
    int count = 0;
    if(isMergeForm(r)) {
        count = count_num_mergeForm(r);
    }
    else {
        for(int i = 1; i <= max_size; i++) {
            if(temp_table->age >= 1 && temp_table->age <= 115) {
                count++;
            }
            temp_table = temp_table->next;
        }
    }
    return count;

    temp_table = nullptr;
    delete temp_table;
}



// check if the restaurant if full?
bool is_full(restaurant* r) {
    if(isMergeForm(r)) {
        if(count_num(r) == (16 - size_merge)) return 1;
    }
    if(count_num(r) == 15) return 1;
    return 0;
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

    temp = nullptr;
    delete temp;
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

    del = nullptr;
    delete del;
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

void REG(restaurant* r, restaurant* queue, restaurant* queueForPS, restaurant* queueForPQ, int ID, string NAME, int AGE, table* forPT) {
    if(isMergeForm(r)) {
        //if((dist < 0 && (ID < end_merge_at || ID > start_merge_at)) || (dist > 0 && (ID < end_merge_at && ID > start_merge_at))) {
            if(is_full(r)) {
                if(is_full(queue)) return;
                else {
                    enqueue(queue, NAME, AGE);
                    enqueue(queueForPQ, NAME, AGE);
                }
            }
            else {
                int start_id ;
                int dist = end_merge_at - start_merge_at;
                table* temp_table = r->recentTable->next;
                if(ID == 0) {
                    if(dist > 0) start_id = 1;
                    if(dist < 0) start_id = end_merge_at;
                    while(temp_table->ID != start_id) temp_table = temp_table->next;

                    while(temp_table->age != 0) temp_table = temp_table->next;
                    temp_table->name = NAME;
                    temp_table->age = AGE;
                }
                else {
                    if(dist > 0) {
                        if(ID < end_merge_at && ID > start_merge_at) start_id = end_merge_at;
                        else start_id = ID; 
                        while(temp_table->ID != start_id) temp_table = temp_table->next;

                        while(temp_table->age != 0) temp_table = temp_table->next;
                        temp_table->name = NAME;
                        temp_table->age = AGE;
                    }
                    else if(dist < 0) {
                        if(ID < end_merge_at || ID > start_merge_at) start_id = end_merge_at;
                        else start_id = ID;

                        while(temp_table->ID != start_id) temp_table = temp_table->next;

                        while(temp_table->age != 0) temp_table = temp_table->next;
                        temp_table->name = NAME;
                        temp_table->age = AGE;
                    }
                }
                forPT->name = NAME;
                forPT->age = temp_table->ID;
            }
            enqueue(queueForPS, NAME, AGE);
        
        //}
        
    }
    else {
        if(is_full(r)) { // if restaurant is full, add the customer to the queue
            if(is_full(queue)) return;
            else {
                enqueue(queue, NAME, AGE);
                enqueue(queueForPQ, NAME, AGE);
            }
        }
        else { // if not full, add them to the chosen ID
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
            forPT->name = NAME;
            forPT->age = temp_table->ID;
        }
    enqueue(queueForPS, NAME, AGE);
    }   
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
    int countID = 0;
    if(sum <= 15) {

        countID = (start + (length - NUM));
    }
    else if(sum > 15) {
        int len_first_half = 15 - start + 1;
        int len_second_half = length - len_first_half;

        if(len_second_half >= NUM) countID = 15;
        else countID = (start + (length - len_second_half));
    }
    return countID;
}

void REGM(restaurant* r, restaurant* queue, restaurant* queueForPS, string NAME, int AGE, int NUM, table* forPT) {
    if(isMergeForm(r) || (countMaxEmpty(r) < NUM)) return;
    int index = getTableID(r, NUM);
    table* temp_table = r->recentTable;
    while(temp_table->ID != index) temp_table = temp_table->next;
    temp_table->name = NAME;
    temp_table->age = AGE;

    forPT->name = NAME;
    forPT->age = temp_table->ID;
    start_merge_at = temp_table->ID;

    after_merge = temp_table->next;
    table* var_table = temp_table;
    for(int i = 1; i <= NUM; i++) var_table = var_table->next;

    temp_table->next = var_table;
    end_merge_at = var_table->ID;
    size_merge = NUM;

    enqueue(queueForPS, NAME, AGE);

    temp_table = nullptr;
    var_table = nullptr;
    delete temp_table;
    delete var_table;
}
/*===================================================================================*/
/*===================================================================================*/

/*===================================================================================*/
/*============================== 2.2.3. THIRD REQUEST ===============================*/
//CLE
void addAfterDel(restaurant* r, restaurant* queue) {
    if(count_num(queue) == 0) return;
    updateRes(r,queue);
}

void removeAt(restaurant* queue, string NAME, int AGE) {
    table* temp_table = queue->recentTable->next;
    for(int i = 1; i <= 30; i++) {
        if(temp_table->name == NAME && temp_table->age == AGE) break;
        temp_table = temp_table->next;
    }
    while(temp_table->next->age != 0) {
        temp_table->name = temp_table->next->name;
        temp_table->age = temp_table->next->age;
        temp_table = temp_table->next;
    }
    temp_table->name = "";
    temp_table->age = 0;
}

void pushTable(restaurant* r, restaurant* q, table* forPT) {
    int start_id = 0;
    if(isMergeForm(r)) {
        table* temp_table = r->recentTable->next;

        if(end_merge_at - start_merge_at > 0) start_id = 1;
        else if(end_merge_at - start_merge_at < 0) start_id = end_merge_at;

        while(temp_table->ID != start_id) temp_table  = temp_table->next;
        while(temp_table->age != 0) temp_table = temp_table->next;
        temp_table->name = q->recentTable->next->name;
        temp_table->age = q->recentTable->next->age;

        forPT->name = q->recentTable->next->name;
        forPT->age = temp_table->ID;
        dequeue(q);
        //cout << "ban moi duoc them khach: " << ID << endl;

    }
    else {
        table* temp_table = r->recentTable->next;
        while(temp_table->age != 0) temp_table = temp_table->next;
        temp_table->name = q->recentTable->next->name;
        temp_table->age = q->recentTable->next->age;

        forPT->name = q->recentTable->next->name;
        forPT->age = temp_table->ID;
        dequeue(q);
    }

}

void CLE(restaurant* r, restaurant* q, restaurant* queue, int ID, table* forPT) {
    string name;
    int age;
    table* temp_table = r->recentTable;
    
    if(!isMergeForm(r)) {
        while(temp_table->ID != ID) temp_table = temp_table->next;

        name = temp_table->name;
        age = temp_table->age;

        temp_table->name = "";
        temp_table->age = 0;

        removeAt(queue, name, age);
    }
    else {
        if(end_merge_at - start_merge_at < 0) {
            if(ID < end_merge_at || ID > start_merge_at) return;
        }
        if(end_merge_at - start_merge_at > 0) {
            if(ID < end_merge_at && ID > start_merge_at) return;
        }
        if(ID == start_merge_at) {
            while(temp_table->ID != start_merge_at) temp_table = temp_table->next;
            temp_table->name = "";
            temp_table->age = 0;
            temp_table->next = after_merge;
        }
        else {
            while(temp_table->ID != ID) temp_table = temp_table->next;

            name = temp_table->name;
            age = temp_table->age;

            temp_table->name = "";
            temp_table->age = 0;

            removeAt(queue, name, age);
        }
    }
    forPT->name = name;
    forPT->age = ID;
    while(!is_full(r) && count_num(q) > 0) {
        pushTable(r,q,forPT);
    }
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

void push_info(restaurant* queue) {
    table* temp_table = queue->recentTable->next;
    table* var_table = temp_table;
    while(var_table->age == 0) var_table = var_table->next;
    while(var_table->ID != 1) {
        temp_table->name = var_table->name;
        temp_table->age = var_table->age;

        var_table->name = "";
        var_table->age = 0;

        temp_table = temp_table->next;
        var_table = var_table->next;
    }
}
int count_queuePS(restaurant* queue) {
    table* temp_table = queue->recentTable->next;
    while(temp_table->name != "") temp_table = temp_table->next;
    return temp_table->ID - 1;

}
void PS(restaurant* queue, int NUM) {
    table* temp_table = queue->recentTable->next;
    int total = count_queuePS(queue);

    reverseCircularLinkedList(queue);
    push_info(queue);
    
    
    if(total == 0) cout << "Empty\n";
    else {
        if(NUM == 0 || (NUM > total && NUM <= 2*MAXSIZE)) {
            while(temp_table->age == 0) temp_table = temp_table->next;
            for(int i = 1; i <= total; i++) {
                cout << temp_table->name << "\n";
                temp_table = temp_table->next;
            }
        }
        else if(NUM <= total) {
            while(temp_table->age == 0) temp_table = temp_table->next;
            for(int i = 1; i <= NUM; i++) {
                cout << temp_table->name << "\n";
                temp_table = temp_table->next;
            }
        }
    }
    reverseCircularLinkedList(queue);
    push_info(queue);
}
/*===================================================================================*/
/*===================================================================================*/

/*===================================================================================*/
/*============================== 2.2.5. FIFTH REQUEST ===============================*/
void PQ(restaurant* queueForPQ, int NUM) {
    table* temp_table = queueForPQ->recentTable->next;
    if(NUM == 0) {
        for(int i = 1; i <= MAXSIZE; i++) {
            if(temp_table->age != 0) cout << temp_table->name << "\n";
            temp_table = temp_table->next;
        }
    }
    else {
        int count = count_num(queueForPQ);
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
/*============================== 2.2.6. SIXTH REQUEST ============================*/
void PT(restaurant* r, table* forPT) {
    table* temp_table = r->recentTable;
    while(temp_table->ID != forPT->age) temp_table = temp_table->next;
    for(int i = 1; i <= (16 - size_merge); i++) {
        if(temp_table->age != 0) cout << temp_table->ID << "-" << temp_table->name << endl;
        temp_table = temp_table->next;
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

void rewriteRes(restaurant* queue, restaurant* queueForPQ) {
    int num_in_queue = count_num(queue);
    int num_in_queueForPQ = count_num(queueForPQ);
    
    table* temp_queueForPQ = queueForPQ->recentTable->next;
    for(int i = 1; i <= num_in_queueForPQ; i++) {
        int count = 0;
        table* temp_queue = queue->recentTable->next;
        for(int i = 1; i <= num_in_queue; i++) {
            if((temp_queueForPQ->name == temp_queue->name) && (temp_queueForPQ->age == temp_queue->age)) count++;
            temp_queue = temp_queue->next;
        }
        if(count == 0) {
            temp_queueForPQ->name = "";
            temp_queueForPQ->age = 0;
        }
        temp_queueForPQ = temp_queueForPQ->next;
    }

    table* temp = queueForPQ->recentTable->next;
    for(int i = 1; i < num_in_queueForPQ; i++) {
        if(temp->age != 0) {
            temp = temp->next;
        }
        else {
            temp->name = temp->next->name;
            temp->age = temp->next->age;

            temp->next->name = "";
            temp->next->age = 0;

            temp = temp->next;
        }
    }


}

void SQ(restaurant* q, restaurant* queueForPQ, int NUM) {
    rewriteRes(q, queueForPQ);
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

    table* update_changes = new table(0, "", 0, nullptr);

    restaurant* queueForPQ = new restaurant();
    for (int i = 1; i <= MAXSIZE; i++)
    {
        queueForPQ->recentTable = queueForPQ->insert(queueForPQ->recentTable,i,"",0);
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
                    if(AGE >= 16 && AGE <= 115) {
                        REG(r, q, queueForPS, queueForPQ, 0, NAME, AGE, update_changes);
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
                    if(AGE >= 16 && AGE <= 115 && ID >=1 && ID <= MAXSIZE) {
                        REG(r, q, queueForPS, queueForPQ, ID, NAME, AGE, update_changes);
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
                    if(AGE >= 16 && AGE <= 115 && NUM >=1 && NUM <= MAXSIZE) {
                        REGM(r, q, queueForPS, NAME, AGE, NUM, update_changes);
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
                        CLE(r, q, queueForPS, ID, update_changes);
                    }
                }
            }
            else continue;
        }
        else if(command == "PS") {// check some necessary condition before implement request 
            if(length == 1) {
                PS(queueForPS, 0);
            }
            else if(length == 2) {
                string str_NUM = firstWord(removeFirst(newline));
                if(checkInt(str_NUM)) {
                    int NUM = stoi(str_NUM);
                    if(NUM >= 1 && NUM <= (2*MAXSIZE)) {
                        PS(queueForPS, NUM);
                    }
                }
            }
            else continue;
        }
        else if(command == "PQ") {// check some necessary condition before implement request 
            if(length == 1) {
                PQ(q, 0);
            }
            else if(length == 2) {
                string str_NUM = firstWord(removeFirst(newline));
                if(checkInt(str_NUM)) {
                    int NUM = stoi(str_NUM);
                    if(NUM >= 1 && NUM <= MAXSIZE) {
                        PQ(queueForPQ, NUM);
                    }
                }
            }
            else continue;
        }
        else if(command == "PT") {// check some necessary condition before implement request 
            PT(r, update_changes);
        }
        else if(command == "SQ") {// check some necessary condition before implement request 
            if(length == 2) {
                string str_NUM = firstWord(removeFirst(newline));
                if(checkInt(str_NUM)) {
                    int NUM = stoi(str_NUM);
                    if(NUM >= 1 && NUM <= MAXSIZE) {
                        SQ(q, queueForPQ, NUM);
                    }
                }
            }
            else continue;
        }
        else continue;
    }
    table* temp = r->recentTable->next;
    table* var = q->recentTable->next;
    for(int i = 1; i <= 15; i++) {
        cout << "NAME: " << temp->name << " " << "AGE: " << temp->age << " " << " ID: " << temp->ID << "\t\t||\t";
        cout << "NAME: " << var->name << " " << "AGE: " << var->age << " " << " ID: " << var->ID << endl;
        temp = temp->next;
        var = var->next;
    }
    cout << "ban moi thay doi gan day la: " << update_changes->age << " " << update_changes->name << endl;
    cout << count_num(r) <<  " "  << is_full(r) <<endl;
    cout << count_num(q) <<  " "  << is_full(q) <<endl;
    delete q;
    delete queueForPS;
    delete update_changes;
    delete queueForPQ;

}
