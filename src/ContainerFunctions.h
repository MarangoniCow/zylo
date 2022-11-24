/***********************************************************
 *                      ContainerFunctions
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

template <typename T>
bool elementInVector(const T& itemToMatch, const std::vector<T>& vectorToSearch)
{
    for(auto it:vectorToSearch)
        if(itemToMatch == it)
            return true;
    return false;
}

template <typename T>
std::vector<T> queueToVector(std::queue<T> queueToConvert)
{
    std::vector<T> returnVector;
    while(!queueToConvert.empty())
    {
        returnVector.push_back(queueToConvert.front());
        queueToConvert.pop();
    }
    return returnVector;
}

template <typename T>
std::queue<T> vectorToQueue(std::vector<T> vectorToConvert)
{
    std::queue<T> returnQueue;
    for(auto it = vectorToConvert.begin(); it != vectorToConvert.end(); it++)
        returnQueue.push(*it);
    return returnQueue;
}

template <typename T>
std::queue<T> appendToQueue(std::queue<T> queueToAppend, std::queue<T> appendQueue)
{
    while(!appendQueue.empty())
    {
        queueToAppend.push(appendQueue.front());
        appendQueue.pop();
    }
    return queueToAppend;
}

template <typename T>
void emptyQueue(std::queue<T>* queueToEmpty)
{
    std::queue<T> emptyQueue;
    *queueToEmpty = emptyQueue;
}

template <typename T>
bool elementInQueue(const T& searchQuery, std::queue<T> queueToSearch)
{
    while(!queueToSearch.empty())
    {
        if(queueToSearch.front() == searchQuery)
            return 1;
        queueToSearch.pop();
    }
    return 0;
}

template <typename T, typename Y>
std::queue<T> applyFunctionToQueue(std::queue<T> queueToActOn, Y (*func)(T))
{
    std::queue<Y> returnQueue;
    while(!queueToActOn.empty())
    {
        returnQueue.push(func(queueToActOn.front()));
        queueToActOn.pop();
    }
    return returnQueue;
}

template <typename T>
std::queue<T> returnMatchingElements(std::queue<T> queueToSearch, std::queue<T> queueToMatch)
{
    std::vector<T> vectorToMatch = queueToVector(queueToMatch);
    std::queue<T> matchedElementsQueue;

    while(!queueToSearch.empty())
    {
        for(auto it = vectorToMatch.begin(); it != vectorToMatch.end(); it++)
        {
            if(*it == queueToSearch.front())
            {
                // Assume no repeats
                matchedElementsQueue.push(queueToSearch.front());
                vectorToMatch.erase(it);
                break;
            }
        }
        queueToSearch.pop();
    }
    return matchedElementsQueue;
}
