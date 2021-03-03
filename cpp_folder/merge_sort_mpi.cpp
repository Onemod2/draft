#include <iostream>
#include <cstdlib>
//#include <mpi.h>
#include <algorithm>
#include <cmath>

using namespace std;


// Necessarily size of massive must be even
template <typename T>
void shuffle (T* mas, int n) {
    T* tmp = new T[n];

    int half_part = n / 2;

    for (int i = 0, j = 0; j < half_part; i+=2, ++j) {
        tmp[i] = mas[j];
        tmp[i + 1] = mas[j+half_part];
    }

    for (int i = 0; i < n; ++i) {
        mas[i] = tmp[i];
    }
}

template <typename T>
void unshuffle(T* mas, int n) {
    T* tmp = new T[n];

    int half_part = n / 2;

    for (int i = 0, j = 0; i < half_part; ++i, j+=2) {
        tmp[i] = mas[j];
        tmp[i + half_part] = mas[j+1];
    }

    for (int i = 0; i < n; ++i) {
        mas[i] = tmp[i];
    }

}

template <typename T>
void odd_even_merge_sort(T* mas, int n) {
    if (n == 1) {
        return;
    }
    if (n == 2) {
        if (mas[0] > mas[1]) {
            swap(mas[0], mas[1]);
        }
        return;
    }

    odd_even_merge_sort(mas, n/2);
    odd_even_merge_sort(mas+n/2, n/2);
    
    unshuffle(mas, n);

    odd_even_merge_sort(mas, n/2);
    odd_even_merge_sort(mas+n/2, n/2);
    
    shuffle(mas, n);
    
    for (int i = 1; i < n-1; i+=2) {
        if (mas[i] > mas[i+1]) {
            swap(mas[i], mas[i+1]);
        }
    }

}

int main() {

    srand(time(0));

    int rank, size;
    int inf = 2147483647;
    // MPI_Init(nullptr, nullptr);
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n, n_power_of_2;
    cin >> n;

    n_power_of_2 = pow(2.0, ceil(log2(n)));

    double* m = new double[n_power_of_2];
    for (int i = 0; i < n; ++i) {
        m[i] = (rand() % 10 / 1.0);
        cout << m[i] << ' ';
    }
    cout << endl;

    for (int i = n; i < n_power_of_2; ++i) {
        m[i] = inf;
    }
    
    odd_even_merge_sort(m, n_power_of_2);

    if (n_power_of_2 != n) {
        double* m_2 = new double[n];
        for (int i = 0; i < n; ++i) {
            m_2[i] = m[i];
        }
        delete[] m;
        
        m = m_2;
        m_2 = nullptr;
    }

    for (int i = 0; i < n; ++i) {
        cout << m[i] << ' ';
    }
    cout << endl;

    //MPI_Finalize();

    return 0;
}