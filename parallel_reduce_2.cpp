#include <iostream>
#include <vector>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <random>

#define SIZE 10
#define RANGE 100

using namespace std;

void print_vector(vector<double> &v)
{
    for (auto i : v)
    {
        cout << i << " ";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    vector<double> values(SIZE, 1.0);
    std::mt19937 seeder(411);
    std::uniform_real_distribution<double> random(-RANGE, RANGE);
    for (int i = 0; i < values.size(); ++i)
    {
        values[i] = random(seeder);
    }

    print_vector(values);

    int bigIdx = tbb::parallel_reduce(
        tbb::blocked_range<int>(0, values.size()),
        0,
        [&](tbb::blocked_range<int> r, int bigIdx) -> long
        {
            for (int i = r.begin(); i != r.end(); ++i)
            {
                bigIdx = abs(values[i]) > abs(values[bigIdx]) ? i : bigIdx;
            }
            return bigIdx;
        },
        [&](int x, int y)
        {
            return abs(values[x]) > abs(values[y]) ? x : y;
        });

    cout << "Biggest value is " << values[bigIdx] << " at index " << bigIdx << endl;

    return 0;
}