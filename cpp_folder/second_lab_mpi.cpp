#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cmath>
#include <ctime>

using namespace std;

#define N 3
#define TAG 10
#define EPS 0.00001

MPI_Status status;

double compute_AE(double* v1, double* v2, int n) {
	double sum_error = 0;
	for (int i = 0; i < n; ++i) {
		sum_error += abs(v1[i] - v2[i]);
	}
	return sum_error;
}

void print_v(double* v, int n) {
	cout << "Vector ";
	for (int i = 0; i < n; ++i) {
		cout << v[i] << ' ';
	}
	cout << endl;
}


int main(int argc, char** argv) {
	
	srand(time(0));
	
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if (rank == 0) {

		/*
			Form of equations is:
				a11 * x1 + a12 * x2 + a13 * x3 - b1 = 0
				a21 * x1 + a22 * x2 + a23 * x3 - b2 = 0
				a31 * x1 + a32 * x2 + a33 * x3 - b3 = 0
		*/

		double main_mas[][N+1] = {{10.0, 1.0, -1.0, -11.0},
							      {1.0, 10.0, -1.0, -10.0},
							      {-1.0, 1.0, 10.0, -10.0}};

		double** m = new double*[N];

		for (int i = 0; i < N; ++i) {
			m[i] = new double[N+1];
		}
		
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N+1; ++j) {
				m[i][j] = main_mas[i][j];	
				cout << m[i][j] << ' ';
			}
			cout << endl;
		}
		cout << endl;
		
		
		int count = 0;
		if (size != 1) {
			count = N / (size-1);
		}
		else {
			exit(1);
		}
		
		int last_count = (N - count*(size-2));

		// Send count of row for every PC
		for (int i = 1; i < size-1; ++i) {
			MPI_Send(&count, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
		}
		MPI_Send(&last_count, 1, MPI_INT, size-1, TAG, MPI_COMM_WORLD);
		
		// Send rows for every PC
		for (int i = 1; i < size-1; ++i) {
			int first_row = (i-1) * count;
			MPI_Send(&first_row, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
			for (int j = 0; j < count; ++j) {
				MPI_Send(m[((i-1)*count) + j], N+1, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
			}
		}
		
		// Send number of first row for last PC
		// Send rows for last PC (if number of rows not divide by num of PC)
		int first_row = N-last_count;
		MPI_Send(&first_row, 1, MPI_INT, size-1, TAG, MPI_COMM_WORLD);

		for (int j = 0; j < last_count; ++j) {
			MPI_Send(m[(N-last_count) + j], N+1, MPI_DOUBLE, size-1, TAG, MPI_COMM_WORLD);
		}		
		
		// Answer vector and previous vector for AE 
		double* answer_vector = new double[N+1];
		double* pr_vector = new double[N+1];
		double AE = 1.0;
			
		for (int i = 0; i < N; ++i) {
			answer_vector[i] = 0.5;
			pr_vector[i] = 0.5;
		}
		answer_vector[N] = 1.0;
		pr_vector[N] = 1.0;
		
        bool condition = true;

		// While AE > EPS compute iteration (send answer vector and get new answer vector from PC users)
		while (AE > EPS) {

			for (int i = 1; i < size; ++i) {
                MPI_Send(&condition, 1, MPI_C_BOOL, i, TAG, MPI_COMM_WORLD);
            	MPI_Send(answer_vector, N+1, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
			}
			
			for (int i = 0; i < N; ++i) {
				int from = ceil((i+1) / double(count));
				from = min(from, size-1);
				
				MPI_Recv(&(answer_vector[i]), 1, MPI_DOUBLE, from, TAG, MPI_COMM_WORLD, &status);
			}
			
			AE = compute_AE(answer_vector, pr_vector, N+1);
			
			for (int i = 0; i < N+1; ++i) {
				pr_vector[i] = answer_vector[i];
			}
			cout << "Mean error " << AE << endl;
		}

        condition = false;
        for (int i = 1; i < size; ++i) {
            MPI_Send(&condition, 1, MPI_C_BOOL, i, TAG, MPI_COMM_WORLD);
        }
		
		for (int i = 0; i < N+1; ++i) {
			cout << answer_vector[i] << ' ';
		}
		
		for (int i = 0; i < N; ++i) {
			delete[] m[i];
		}
		delete[] m;
		
		delete[] answer_vector;
		delete[] pr_vector;
	
		cout << "\nEND_ITERATION\n";
	}
	else {
	
		int count;
		
		// Get count of rows for current PC
		MPI_Recv(&count, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);

		double** m = new double*[count];
		for (int i = 0; i < count; ++i) {
			m[i] = new double[N+1];
		}
		
        // Answer vector - solution vector, first_row - start position for current PC
        // Condition - it is condition for next iteration
		double* answer_vector = new double[N+1];
		int first_row;
		bool condition;

		// Get number of first row for current PC
		MPI_Recv(&first_row, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
		
		// Get rows by one, for current PC
		for (int i = 0; i < count; ++i) {
			MPI_Recv(m[i], N+1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
		}
		
		// Get answer_vector
		// Compute new answer for rows and send it 
		while (true) {
            MPI_Recv(&condition, 1, MPI_C_BOOL, 0, TAG, MPI_COMM_WORLD, &status);
            if (!condition) {
                break;
            }
			MPI_Recv(answer_vector, N+1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

			for (int i = 0; i < count; ++i) {
				double ans = 0.0;
				for (int j = 0; j < N+1; ++j) {
					if (j != (i+first_row)) {
						ans += m[i][j] * answer_vector[j];
					}
				}
				ans = (-ans) / m[i][i+first_row];
				MPI_Send(&ans, 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
			}
		}
		
		for (int i = 0; i < count; ++i) {
			delete[] m[i];
		}
		delete[] m;
		delete[] answer_vector;
	}
	
	MPI_Finalize();
	
	return 0;
}