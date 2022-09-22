#include <thread>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
#define SIZE 400
#define THREADS 1

void init_arrays(int (&a)[SIZE][SIZE], int (&b)[SIZE][SIZE], int (&c)[SIZE][SIZE]) {
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            a[i][j] = 1;
            b[i][j] = 1;
            c[i][j] = 0;
        }
    }
}

int main(){
    int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    init_arrays(A, B, C);
    auto task = [A, B, &C](int start, int end){
                for(int i=start; i<end; i++){
                    for(int j=0; j<SIZE; j++){
                        for(int k=0; k<SIZE; k++){
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            };
    auto starttime = chrono::high_resolution_clock::now();
    vector<thread> threads;
    for(int i=0; i<THREADS; i++){
        threads.push_back(thread{task, i*SIZE/THREADS, (i+1)*SIZE/THREADS});
    }
    for(thread &t: threads){
        t.join();
    }
    auto endtime = chrono::high_resolution_clock::now();
    chrono::duration<double> time_span =
    chrono::duration_cast<chrono::duration<double>>(endtime - starttime);
    cout << "Total execution time with thread: " << time_span.count() << " seconds"
         << std::endl;

    init_arrays(A, B, C);
    starttime = chrono::high_resolution_clock::now();
    task(0, SIZE);
    endtime = chrono::high_resolution_clock::now();
    time_span = chrono::duration_cast<chrono::duration<double>>(endtime - starttime);
    cout << "Total execution time without thread: " << time_span.count() << " seconds"
         << std::endl;
}