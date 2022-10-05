#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#define SIZE 10000

int main()
{
    long A[SIZE];
    long sum{0};
    std::mutex sum_access;
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = i + 1;
    }
    auto task = [A, &sum, &sum_access](int start, int end)
    {
        for (int i = start; i < end; i++)
        {
            sum_access.lock();
            sum += A[i];
            sum_access.unlock();
        }
    };
    auto task2 = [A, &sum](int start, int end)
    {
        for (int i = start; i < end; i++)
        {
            sum += A[i];
        }
    };
    std::thread thread1(task, 0, SIZE / 4);
    std::thread thread2(task, SIZE / 4, SIZE / 2);
    std::thread thread3(task, SIZE / 2, 3 * SIZE / 4);
    std::thread thread4(task, 3 * SIZE / 4, SIZE);
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    std::cout << "Final sum: " << sum << std::endl;

    sum = 0;
    std::thread thread5(task2, 0, SIZE / 4);
    std::thread thread6(task2, SIZE / 4, SIZE / 2);
    std::thread thread7(task2, SIZE / 2, 3 * SIZE / 4);
    std::thread thread8(task2, 3 * SIZE / 4, SIZE);
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    std::cout << "Final sum (wihout synchronization) 1: " << sum << std::endl;
    
    sum = 0;
    std::thread thread9(task2, 0, SIZE / 4);
    std::thread thread10(task2, SIZE / 4, SIZE / 2);
    std::thread thread11(task2, SIZE / 2, 3 * SIZE / 4);
    std::thread thread12(task2, 3 * SIZE / 4, SIZE);
    thread9.join();
    thread10.join();
    thread11.join();
    thread12.join();
    std::cout << "Final sum (wihout synchronization) 2: " << sum << std::endl;
    
}