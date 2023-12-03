#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <iomanip> 

using namespace std;

struct ThreadData {
    int thread_id; // Additional initialization for easy debugging
    int L;         // Start index of sub array (left)
    int R;         // End index of sub array (right)
};

int array_size = 0;
int thread_cnt = 0;  // Initialize the number of threads

vector<int> A;

//Merge 2 sorted subarrays 
void merge(int L, int M, int R) {
    vector<int> merged(R - L + 1);
    int i = L, j = M + 1, k = 0;
    while (i <= M && j <= R) {
        merged[k] = (A[i] <= A[j]) ? A[i++] : A[j++];
        k++;
    }

    while (i <= M) {
        merged[k++] = A[i++];
    }

    while (j <= R) {
        merged[k++] = A[j++];
    }

    for (int m = 0; m < k; m++) {
        A[L + m] = merged[m];
    }
}

// Normal funcion Merge_Sort
void merge_sort(int L, int R) {
    if (L < R) {
        int M = L + (R - L) / 2;
        merge_sort(L, M);
        merge_sort(M + 1, R);
        merge(L, M, R);
    }
}

//  Multi Thread function Merge_Sort for a subarray
void merge_sort_threaded(const ThreadData& data) {
    merge_sort(data.L, data.R);
}

int main() {
    int n;

    // Get the size of the array from the user
    cout << "Enter size of array: ";

    // Validate input to ensure it's a positive integer
    while (!(cin >> n) || n <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Resize the array with user input
    A.resize(n);


    array_size = n;

    // Check the number of threads to use based on hardware concurrency
    thread_cnt = std::thread::hardware_concurrency();
    cout << "Using " << thread_cnt << " threads." << endl;

    // Initialize a random array
    for (int i = 0; i < array_size; i++) {
        A[i] = rand() % 100;
    }

    // Print the original array before sorting
    cout << "Array_input (Random) : " << endl;
    int cnt = 0;
    for (int num : A) {
        cnt++;
        cout << setw(4) << num << " ";
        if (cnt == 20) {
            cnt = 0;
            cout << endl;
        }
    }
    cout << endl;

    // Initialize threads and thread data
    vector<thread> threads;
    vector<ThreadData> thread_data;

    // Calculate the size of sub-arrays for each thread
    int subA_size = array_size / thread_cnt;

    // Create threads and assign sub-arrays to each thread
    for (int i = 0; i < thread_cnt; i++) {
        int L = i * subA_size;
        int R = (i == thread_cnt - 1) ? array_size - 1 : (i + 1) * subA_size - 1;
        thread_data.push_back({ i, L, R });
        threads.emplace_back(merge_sort_threaded, thread_data[i]);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Merge the sub-arrays sorted by the threads
    for (int i = 1; i < thread_cnt; i++) {
        int M = i * subA_size - 1;
        int R = (i == thread_cnt - 1) ? array_size - 1 : (i + 1) * subA_size - 1;
        merge(0, M, R);
    }

    // Print the sorted array
    cnt = 0;
    cout << "Array_output (Sorted) : " << endl;
    for (int num : A) {
        cnt++;
        cout << setw(4) << num << " ";
        if (cnt == 20) {
            cnt = 0;
            cout << endl;
        }
    }

    return 0;
}
