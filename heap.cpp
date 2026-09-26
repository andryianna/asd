#include <fstream>
#include <iostream>
#include <climits>
#include <vector>

using namespace std;

class Heap {
    vector<int> heap;
    int heapSize;

    void swap(int &a, int &b) {
        int t = a;
        a = b;
        b = t;
    }
    void maxHeapify(vector<int>& A, int heapSz, int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest = i;

    if (l < heapSz && A[l] > A[largest])
        largest = l;
    if (r < heapSz && A[r] > A[largest])
        largest = r;

    if (largest != i) {
        swap(A[i], A[largest]);
        maxHeapify(A, heapSz, largest);
    }
}

void minHeapify(vector<int>& A, int heapSz, int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int smallest = i;

    if (l < heapSz && A[l] < A[smallest])
        smallest = l;
    if (r < heapSz && A[r] < A[smallest])
        smallest = r;

    if (smallest != i) {
        swap(A[i], A[smallest]);
        minHeapify(A, heapSz, smallest);
    }
}

void buildMinHeap(vector<int>& A, int heapSz) {
    for (int i = heapSz / 2 - 1; i >= 0; --i)
        minHeapify(A, heapSz, i);
}

void buildMaxHeap(vector<int>& A, int heapSz) {
    for (int i = heapSz / 2 - 1; i >= 0; --i)
        maxHeapify(A, heapSz, i);
}
public:
    Heap(const string &filename,bool min): heapSize(0) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << endl;
            return;
        }
        int el;
        while (file >> el)
            heapInsert(el,min);
        file.close();
    }

    int extractMin() {
        if (heapSize == 0) {
            cerr << "Heap is empty" << endl;
            return -1;
        }
        int min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapSize--;
        minHeapify(heap,heapSize,0);
        return min;
    }
    int extractMax() {
        if (heapSize == 0) {
            cerr << "Heap is empty" << endl;
            return -1;
        }
        int max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapSize--;
        maxHeapify(heap,heapSize,0);
        return max;
    }

    void printHeap(const string &filename, vector<int> &A, int heapsize) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << endl;
            return;
        }
        for (int i = 0; i < heapsize; i++)
            file << A[i] << " ";
        file << endl;
        file.close();
    }
    // Da usare su un max-heap.
void increaseKey(int i, int key) {
    if (i < 0 || i >= heapSize || key < heap[i])
        return;

    heap[i] = key;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (heap[parent] >= heap[i])
            break;

        swap(heap[i], heap[parent]);
        i = parent;
    }
}

// Da usare su un min-heap.
void decreaseKey(int i, int key) {
    if (i < 0 || i >= heapSize || key > heap[i])
        return;

    heap[i] = key;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (heap[parent] <= heap[i])
            break;

        swap(heap[i], heap[parent]);
        i = parent;
    }
}

void heapInsert(int key, bool min) {
    heap.push_back(min ? INT_MAX : INT_MIN);
    ++heapSize;

    if (min)
        decreaseKey(heapSize - 1, key);
    else
        increaseKey(heapSize - 1, key);
}
//ordine crescente usa maxHeap altrimenti usa minHeap
    vector<int> heapSort() {
        auto A = heap;
        int heapsize = A.size();
        buildMinHeap(A,heapsize);
        for (int i = heapsize - 1; i >= 0; i--) {
            swap(A[0], A[i]);
            minHeapify(A,i,0);
        }
        
        return A;
    }
};


int main(void){
	//Ho fatto una classe generica di Heap in cui un parametro del costruttore decide se é un min(true) o max(false) segue la firma nella riga successiva
	//Heap(const string& filename,bool min)
	//Nella prova ovviamente usate solo i metodi per min o max heap
	
	return 0;
}
