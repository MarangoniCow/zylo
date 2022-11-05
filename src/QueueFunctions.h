/***********************************************************
 *                      QUEUEFUNCTIONS
 * 
 * A set of functions designed to make working with queues simpler.
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "BoardPosition.h"


// EXTERNAL INCLUDES
#include <utility>
#include <queue>
#include <vector>


namespace QueueFunctions
{
    template <class T>
    std::vector<T> queueToVector(std::queue<T> queueToConvert);

    template <class T>
    std::queue<T> vectorToQueue(std::vector<T> vectorToConvert);

}


