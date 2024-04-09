
#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

template <typename T>
class List
{
public:
    //! Hàm của thầy
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T &get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;

    // Defined some support functions
    virtual List<T> *subList(int start, int end) = 0;
};

template <typename T>
class Image : public List<T>
{
private:
    class Node
    {
    public:
        T data;
        Node *next;

    public:
        Node(T data, Node *next = nullptr) : data(data), next(next) {}
    };

private:
    Node *head;
    Node *tail;
    int size;

public:
    // Constructs an Image object with an empty linked list.
    Image()
    {
        head = tail = nullptr;
        size = 0;
    }

    /**
     * Destructor for the Image object.
     * Clears the linked list by removing all nodes.
     */
    ~Image()
    {
        this->clear();
    }

    // Inserts an element at the end of the linked list.
    void push_back(T value)
    {
        // Create a new node with the specified value.
        Node *newNode = new Node(value);

        if (!head)
        { // If the linked list is empty, set both the head and tail pointers to the new node.
            head = tail = newNode;
        }

        else
        { // If the linked list is not empty, append the new node to the end of the list and update the tail pointer.
            tail->next = newNode;
            tail = newNode;
        }
        // Increment the size of the linked list.
        size++;
    }

    // Inserts an element at the beginning of the linked list.
    void push_front(T value)
    {
        Node *newNode = new Node(value);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    // Inserts an element at a specified index in the linked list.
    void insert(int index, T value)
    {
        if (index < 0 || index > size)
        {
            return;
        }

        if (index == 0)
        {
            push_front(value);
        }
        else if (index == size)
        {
            push_back(value);
        }
        else
        {
            Node *newNode = new Node(value);
            Node *temp = head;

            for (int i = 0; i < index - 1; i++)
            {
                temp = temp->next;
            }

            newNode->next = temp->next;
            temp->next = newNode;
            size++;
        }
    }
    void remove(int index)
    {
        if (index < 0 || index >= size)
            return;

        Node *temp;
        if (index == 0)
        {
            temp = head;
            head = head->next;
            if (!head)
            {
                head = tail = nullptr;
            }
        }
        else
        {
            Node *current = head;
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }

            temp = current->next;
            current->next = temp->next;
            if (!current->next)
            {
                tail = current;
            }
        }

        delete temp;
        size--;
    }

    T &get(int index) const
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("get(): Out of range");

        // TODO: implement task 1
        Node *temp = head;
        for (int i = 0; i < index; i++)
        {
            temp = temp->next;
        }

        return temp->data;
    }

    int length() const
    {
        return size;
    }

    void clear()
    {
        // TODO: implement task 1
        while (head)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    // Prints the elements of the linked list.
    void print() const
    {
        Node *temp = head;
        for (int i = 0; i < this->size; i++)
        {
            if (i == this->size - 1)
                cout << temp->data; //<< endl
            else
                cout << temp->data << " ";
            temp = temp->next;
        }
    }

    // Reverses the order of nodes in the linked list.
    void reverse()
    {
        Node *prev = nullptr;
        Node *current = head;
        Node *nextNode = nullptr;

        tail = head;
        while (current)
        {
            nextNode = current->next;
            current->next = prev;
            prev = current;
            current = nextNode;
        }

        head = prev;
    }

    List<T> *subList(int start, int end)
    {

        // if (this->size <= start) // || start == end
        //     return nullptr;

        List<T> *result = new Image<T>();
        if (this->size <= start)
            return result;
        Node *temp = head;
        for (int i = 0; i < start; i++)
        {
            temp = temp->next;
        }
        if (end >= size)
        {
            while (temp)
            {
                result->push_back(temp->data);
                temp = temp->next;
            }
        }
        else
        {
            for (int i = start; i < end; i++)
            {
                result->push_back(temp->data);
                temp = temp->next;
            }
        }

        return result;
    }
};

// This function performs stable sorting of the 'label' array based on the corresponding values in the 'distance' array.
void sortArrays(double *distance, int *label, int length)
{
    bool check;
    for (int i = 0; i < length - 1; i++)
    {
        check = false;
        for (int j = 0; j < length - 1; j++)
        {
            if (distance[j] > distance[j + 1])
            {
                // swap distance
                double temp = distance[j];
                distance[j] = distance[j + 1];
                distance[j + 1] = temp;
                // swap của label
                temp = label[j];
                label[j] = label[j + 1];
                label[j + 1] = temp;
                check = true;
            }
        }
        if (!check)
            break;
    }
}

class Dataset
{
private:
    List<List<int> *> *data;
    List<string> *NameCol;
    // You may need to define more
public:
    //! Hàm của thầy
    Dataset()
    {
        data = new Image<List<int> *>();
        NameCol = new Image<string>();
    }
    ~Dataset()
    {
        // Perform cleanup (delete allocated memory, etc.)
        // Assuming List destructor takes care of freeing memory
        delete data;
        delete NameCol;
    }

    Dataset(List<List<int> *> *data, List<string> *NameCol) : data(nullptr), NameCol(nullptr)
    {
        // Deep copy the data (List<List<int> *>)
        if (data)
        {
            this->data = new Image<List<int> *>();
            for (int i = 0; i < data->length(); ++i)
            {
                List<int> *rowData = data->get(i);
                List<int> *copiedRow = rowData->subList(0, rowData->length()); // Assuming Image<int> handles deep copy
                this->data->push_back(copiedRow);
            }
        }

        // Deep copy the column names (List<string>)
        if (NameCol)
        {
            this->NameCol = NameCol->subList(0, NameCol->length()); // Assuming Image<string> handles deep copy
        }
    }

    Dataset(const Dataset &other)
    {
        // Deep copy the column names (List<string>)
        NameCol = other.NameCol->subList(0, other.NameCol->length());

        // Deep copy the data (List<List<int> *>)
        data = new Image<List<int> *>();
        for (int i = 0; i < other.data->length(); ++i)
        {
            List<int> *rowData = other.data->get(i);
            List<int> *copiedRow = rowData->subList(0, rowData->length());
            data->push_back(copiedRow);
        }

        // Deep copy the column names (List<string>)
        // NameCol = other.NameCol; // Deep copy the list of column names
    }

    // Assignment operator for deep copying
    Dataset &operator=(const Dataset &other)
    {
        if (this != &other)
        {
            data = new Image<List<int> *>();
            List<List<int> *> *otherData = other.data;
            for (int i = 0; i < otherData->length(); ++i)
            {
                // List<int> *rowData = otherData->get(i);
                List<int> *copiedRow = otherData->get(i)->subList(0, otherData->get(i)->length());
                data->push_back(copiedRow);
            }

            NameCol = other.NameCol->subList(0, other.NameCol->length());
        }
        return *this;
    }

    bool loadFromCSV(const char *fileName)
    {
        // ifstream myFile(fileName);
        // if (!myFile.is_open())
        // {
        //     return 0;
        // }

        // string headerLine;
        // getline(myFile, headerLine);
        // istringstream ColumnStream(headerLine);
        // string ColumnName;

        // while (getline(ColumnStream, ColumnName, ','))
        // {
        //     NameCol->push_back(ColumnName);
        // }

        // string row;
        // while (getline(myFile, row))
        // {
        //     istringstream rowStream(row);
        //     List<int> *rowData = new Image<int>();

        //     string val;
        //     while (getline(rowStream, val, ','))
        //     {
        //         int intValue = stoi(val);
        //         rowData->push_back(intValue);
        //     }

        //     data->push_back(rowData);
        // }

        // return 1;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ifstream myfile(fileName);
        //* kiểm tra mở file
        if (!myfile.is_open())
        {
            return false;
        }

        string str;
        int number;

        //* xử lý hàng đầu tiên chuyển , thành ' ' dùng thư viện stringstream
        myfile >> str;
        for (char &c : str)
        {
            if (c == ',')
            {
                c = ' ';
            }
        }
        stringstream ss(str);
        while (ss >> str)
            NameCol->push_back(str);

        //* xử lý các hàng còn lại , thành ' ' dùng thư viện stringstream
        while (myfile >> str)
        {
            for (char &c : str)
            {
                c = (c == ',') ? ' ' : c;
            }
            stringstream feature(str);
            List<int> *temp = new Image<int>();
            while (feature >> number)
                temp->push_back(number);
            data->push_back(temp);
        }

        return true;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    // Retrieves the shape of the dataset.
    void getShape(int &nRows, int &nCols) const
    {
        nRows = this->data->length();
        if (nRows == 0)
        {
            nCols = 0;
        }
        else
        {
            nCols = NameCol->length();
        }
    }

    // Prints all the names of columns(if have).
    void columns() const
    {
        if (NameCol->length() == 0)
            return;
        NameCol->print();
    }

    void printHead(int nRows = 5, int nCols = 5) const
    {
        if (nRows <= 0 || nCols <= 0)
        {
            return;
        }

        if (nRows >= data->length())
        {
            nRows = data->length();
        }
        if (nCols >= NameCol->length())
        {
            nCols = NameCol->length();
        }
        if (nCols == 0 && nCols == 0)
            return;
        List<string> *header = NameCol->subList(0, nCols);
        header->print();
        delete header;
        if (data->length() != 0)
        {
            cout << endl;
        }
        // cout << "aaaaaaaaa" << data->length() << "bbbb" << data->get(0)->length() << endl;
        // if (nRows == 0)
        //     return;
        for (int i = 0; i < nRows; i++)
        {
            List<int> *content = data->get(i)->subList(0, nCols);
            content->print();
            if (i != nRows - 1)
            {
                cout << endl;
            }

            delete content;
        }
    }

    void printTail(int nRows = 5, int nCols = 5) const
    {
        // Checks if the provided indices are valid and if the dataset is non-empty.
        if (nRows <= 0 || nCols <= 0)
        {
            return;
        }

        if (nRows >= data->length())
        {
            nRows = data->length();
        }
        if (nCols >= NameCol->length())
        {
            nCols = NameCol->length();
        }

        if (nCols == 0 && nCols == 0)
            return;

        List<string> *header = NameCol->subList(NameCol->length() - nCols, NameCol->length());
        header->print();
        delete header;
        if (data->length() != 0)
        {
            cout << endl;
        }
        for (int i = nRows; i > 0; i--)
        {
            List<int> *content = data->get(data->length() - i)->subList(NameCol->length() - nCols, NameCol->length());
            content->print();

            delete content;
            if (i != 1)
            {
                cout << endl;
            }
        }
    }

    bool drop(int axis = 0, int index = 0, std::string columns = "")
    {
        if (axis == 0 || axis == 1)
        {
            if (axis == 0)
            {
                if (index < 0 || index >= data->length())
                {
                    return false;
                }

                data->remove(index);
            }
            else
            {
                int delCol = -1;
                for (int i = 0; i < NameCol->length(); i++)
                {
                    if (columns == NameCol->get(i))
                        delCol = i;
                }

                if (delCol == -1)
                {
                    return false;
                }

                NameCol->remove(delCol);
                for (int j = data->length() - 1; j >= 0; j--)
                {
                    data->get(j)->remove(delCol);
                }
            }

            return true;
        }
        return false;
    }

    // Extracts a subset of the dataset.
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const
    {
        Dataset result;

        if ((endRow > -1 && startRow > endRow) || (endCol > -1 && startCol > endCol) || endRow < -1 || endCol < -1)
        {
            throw std::out_of_range("get(): Out of range");
        }
        /**
         * Checks if the provided indices are valid and if the dataset is non-empty.
         * If any of the indices are out of bounds or if the dataset is empty, an empty Dataset object is returned.
         */

        if (startRow < 0 || startCol < 0 || data->length() == 0 || endRow < -1 || endCol < -1)
        {
            return result;
        }

        /**
         * If endRow or endCol is set to -1, it represents the last row or column, respectively.
         */

        int end_data = 0, end_label = 0;
        if (endCol == -1 || endCol >= this->NameCol->length())
        {
            end_label = end_data = NameCol->length() - 1;
        }
        else
            end_label = end_data = endCol;

        if (endRow == -1 || endRow >= data->length())
        {
            endRow = data->length() - 1;
        }

        if (startRow <= endRow && startCol <= end_data)
        { // Data extraction starts when startRow is less than or equal to endRow and startCol is less than or equal to endCol.
            // Data is extracted by iterating over the rows and columns within the specified range.
            for (int i = startRow; i <= endRow; i++)
            {
                result.data->push_back(this->data->get(i)->subList(startCol, end_data + 1));
            }

            if (this->NameCol->length() != 0)
            {
                for (int i = startCol; i <= end_label; i++)
                { // If column names are available, they are also extracted and added to the resulting Dataset object.
                    result.NameCol->push_back(this->NameCol->get(i));
                }
            }
            else
            {
                result.NameCol->push_back("");
            }
        }
        return result;
    }

    // Retrieves the data stored in the object.
    List<List<int> *> *getData() const
    {
        return this->data;
    }

    // Calculates the Euclidean distance between two vectors represented as lists.
    double distanceEuclidean(const List<int> *x, const List<int> *y) const
    {
        double distance = 0.0;
        if (x->length() > y->length())
        {
            int i = 0;
            for (i = 0; i < y->length(); i++)
            {
                distance += pow(x->get(i) - y->get(i), 2);
            }
            for (int j = i; j < x->length(); j++)
            {
                distance += pow(x->get(j), 2);
            }
        }
        else
        {
            int i = 0;
            for (i = 0; i < x->length(); i++)
            {
                distance += pow(x->get(i) - y->get(i), 2);
            }
            for (int j = i; j < y->length(); j++)
            {
                distance += pow(y->get(j), 2);
            }
        }

        return sqrt(distance);
    }
    void swap(double &a, double &b)
    {
        double temp = a;
        a = b;
        b = temp;
    }

    void swap(int &a, int &b)
    {
        int temp = a;
        a = b;
        b = temp;
    }

    void bubbleSort(double arr[], int label[], int n)
    {
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                    swap(label[j], label[j + 1]);
                }
            }
        }
    }

    Dataset predict(const Dataset &X_train, const Dataset &Y_train, const int k) const
    {
        Dataset result;
        // Check if k is out of range
        if (k > X_train.data->length())
            throw std::out_of_range("get(): Out of range");
        /**
         * If any of the datasets are empty or if any necessary column names are missing, an empty Dataset object is returned.
         */
        if (this->data->length() == 0 || this->NameCol->length() == 0 || X_train.NameCol->length() == 0 || Y_train.NameCol->length() == 0)
        {
            return result;
        }

        // Apply k-nearest neighbors algorithm to predict labels for each row of the dataset

        // Allocate memory for arrays to store data
        List<int> **arrThis = new List<int> *[this->data->length()];
        List<int> **storeX = new List<int> *[X_train.data->length()];
        List<int> **storeY = new List<int> *[Y_train.data->length()];

        // Copy data from 'this' dataset to arrays
        for (int i = 0; i < this->data->length(); i++)
        {
            arrThis[i] = this->data->get(i);
        }

        // Copy data from X_train dataset to arrays
        for (int i = 0; i < X_train.data->length(); i++)
        {
            storeX[i] = X_train.data->get(i);
        }

        // Copy data from Y_train dataset to arrays
        for (int i = 0; i < Y_train.data->length(); i++)
        {
            storeY[i] = Y_train.data->get(i);
        }

        // Add the name of the label column to the result dataset
        result.NameCol->push_back(Y_train.NameCol->get(0));

        // Iterate over each row of 'this' dataset
        for (int row = 0; row < this->data->length(); row++)
        {
            // Allocate memory for arrays to store distances and labels
            double *distance = new double[X_train.data->length()];
            int *label = new int[Y_train.data->length()];

            // Calculate Euclidean distances between the current row and rows in X_train
            for (int i = 0; i < X_train.data->length(); i++)
            {
                distance[i] = this->distanceEuclidean(arrThis[row], storeX[i]);
                label[i] = storeY[i]->get(0);
            }

            // Sort arrays based on distances
            sortArrays(distance, label, X_train.data->length());

            // Count occurrences of labels among k-nearest neighbors
            int arr[10] = {0};
            int length = 0;
            if (k > Y_train.data->length())
            {
                length = Y_train.data->length();
            }
            else
            {
                length = k;
            }

            for (int i = 0; i < length; i++)
            {
                arr[label[i]] += 1; // Increment count for the corresponding label
            }

            // Find the label with the highest count
            int maxValue = 0;
            int maxIndex = 0;
            for (int i = 0; i < 10; i++)
            {
                if (maxValue < arr[i])
                {
                    maxValue = arr[i];
                    maxIndex = i;
                }
            }

            // Create a list to store the predicted label and add it to the result dataset
            List<int> *tempData = new Image<int>();
            tempData->push_back(maxIndex);
            result.data->push_back(tempData);
        }
        // Deallocate memory for arrays
        delete[] arrThis;
        delete[] storeX;
        delete[] storeY;
        return result;
    }

    double score(const Dataset &y_predict) const
    {
        if (y_predict.data->length() == 0 || this->data->length() == 0 || this->data->length() != y_predict.data->length())
        {
            return -1;
        }
        double result = 0;
        for (int i = 0; i < y_predict.data->length(); i++)
        {
            if (y_predict.data->get(i)->get(0) == this->data->get(i)->get(0))
            {
                result += 1;
            }
        }
        return result / y_predict.data->length();
    }
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset Y_train;
    // You may need to define more
public:
    kNN(int k = 5) : k(k){};
    void fit(const Dataset &X_train, const Dataset &y_train)
    {
        this->X_train = X_train;
        this->Y_train = y_train;
    }
    Dataset predict(const Dataset &X_test)
    {
        return X_test.predict(this->X_train, this->Y_train, this->k);
    }
    double score(const Dataset &y_test, const Dataset &y_pred)
    {
        return y_test.score(y_pred);
    }
};

void train_test_split(Dataset &X, Dataset &Y, double test_size,
                      Dataset &X_train, Dataset &X_test, Dataset &Y_train, Dataset &Y_test)
{
    if (X.getData()->length() != Y.getData()->length() || test_size >= 1 || test_size <= 0)
        return;

    double split = X.getData()->length() * (1 - test_size);

    if (abs(round(split) - split) < 1.0e-15 * X.getData()->length())
        split = round(split);

    X_train = X.extract(0, split - 1, 0, -1);
    Y_train = Y.extract(0, split - 1, 0, -1);

    X_test = X.extract(split, -1, 0, -1);
    Y_test = Y.extract(split, -1, 0, -1);
}

// Please add more or modify as needed