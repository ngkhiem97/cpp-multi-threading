#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <random>
#include <limits>
#include <mutex>

#define RANGE 100
#define SIZE 100

void print_vector(std::vector<double> &v)
{
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    std::mt19937 seeder(411);
    std::uniform_real_distribution<double> random(-RANGE, RANGE);
    auto values = std::vector<double>(SIZE);
    
    tbb::parallel_for(0, SIZE,
                        [&](int i) {
                            values[i] = random(seeder);
                        }
                     );

    // print_vector(values);

    std::mutex m;

    auto total = tbb::parallel_reduce(tbb::blocked_range<int>(0, values.size()), 0.0,
                                        [&](tbb::blocked_range<int> r, double running_total)
                                        {
                                            for (int i=r.begin(); i<r.end(); ++i)
                                            {
                                                running_total += values[i];
                                            }
                                            m.lock();
                                            std::cout << "Total now = " << running_total
                                                    << " from iterations " << r.begin()
                                                    << " to " << r.end() << std::endl;
                                            m.unlock();
                                            return running_total;
                                        },
                                        [](double x, double y){
                                            std::cout << "Reducing " << x << " and " << y << std::endl;
                                            return x + y;
                                        });

    std::cout << total << std::endl;

    return 0;
}