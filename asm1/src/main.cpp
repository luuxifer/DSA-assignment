#include "kNN.hpp"

void tc1158()
{
    int nRows, nCols;
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    Dataset dataset_extract = dataset.extract(0, -2, 0, -1);
    Dataset dataset_extract2 = Dataset(dataset_extract);
    dataset_extract2.getShape(nRows, nCols);
    cout << "Dataset extract shape: " << nRows << "x" << nCols << endl;
}

int main()
{
    tc1158();
    return 0;
}