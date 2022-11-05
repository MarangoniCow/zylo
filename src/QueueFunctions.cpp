/***********************************************************
 *                  QUEUEFUNCTIONS IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "QueueFunctions.h"

// EXTERNAL INCLUDES
#include <utility>
#include <queue>
#include <vector>

template <class T>
std::vector<T> QueueFunctions::queueToVector(std::queue<T> queueToConvert)
{
    std::vector<T> returnVector;
    while(!queueToConvert.empty())
    {
        returnVector.push_back(queueToConvert.front());
        queueToConvert.pop();
    }
    return returnVector;
}

template <class T>
std::queue<T> QueueFunctions::vectorToQueue(std::vector<T> vectorToConvert)
{
    std::queue<T> returnQueue;
    for(auto it = vectorToConvert.begin(); it != vectorToConvert.end(); it++)
        returnQueue.push(*it);
    return returnQueue;
}


