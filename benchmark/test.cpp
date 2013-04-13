#include <cstdio>
#include <ctime>
#define _SCL_SECURE_NO_WARNINGS
#define BOOST_DISABLE_ASSERTS
#include <boost/multi_array.hpp>
#include "Array/array.hpp"

int main(int, char* [])
{
    const int X_SIZE = 600;
    const int Y_SIZE = 600;
    const int ITERATIONS = 500;
    clock_t startTime;
    clock_t endTime;

    // Create Array
    marray::Array<double, 2> MyArray(X_SIZE,Y_SIZE);

    // Create the boost array
    typedef boost::multi_array<double, 2> ImageArrayType;
    ImageArrayType boostMatrix(boost::extents[X_SIZE][Y_SIZE]);

    // Create the native array
    double *nativeMatrix = new double [X_SIZE * Y_SIZE];

    //------------------Measure boost----------------------------------------------
    startTime = clock();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        for (int x = 0; x < X_SIZE; ++x)
        {
            for (int y = 0; y < Y_SIZE; ++y)
            {
                boostMatrix[x][y] = 2.345;
            }
        }
    }
    endTime = clock();
    printf("[Boost] Elapsed time: %6.3f seconds\n", double(endTime - startTime) / CLOCKS_PER_SEC);

    //------------------Measure Array----------------------------------------------
    startTime = clock();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        for (int x = 0; x < X_SIZE; ++x)
        {
            for (int y = 0; y < Y_SIZE; ++y)
            {
                MyArray(x,y) = 2.345;
            }
        }
    }
    endTime = clock();
    printf("[Array] Elapsed time: %6.3f seconds\n", double(endTime - startTime) / CLOCKS_PER_SEC);    


    //------------------Measure native-----------------------------------------------
    startTime = clock();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        for (int x = 0; x < X_SIZE; ++x)
        {
            for (int y = 0; y < Y_SIZE; ++y)
            {
                nativeMatrix[x*Y_SIZE + y] = 2.345;
            }
        }
    }
    endTime = clock();
    printf("[Native]Elapsed time: %6.3f seconds\n", double(endTime - startTime) / CLOCKS_PER_SEC);



    if (nativeMatrix)
      delete nativeMatrix;
    return 0;
}
