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
#include <stdexcept>


template <typename T>
unsigned int vectorMaxPosition(const std::vector<T>& vectorToSearch)
{

    if(vectorToSearch.size() == 0)
        throw std::invalid_argument( "received empty vector" );

    unsigned int idx = 0;
    T maxItem = vectorToSearch[idx];

    for(int i = 1; i < vectorToSearch.size(); i++)
    {
        if(maxItem < vectorToSearch[i])
        {
            idx = i;
            maxItem = vectorToSearch[i];
        }
    }
    return idx;
}

template <typename T>
unsigned int vectorMinPosition(const std::vector<T>& vectorToSearch)
{

    if(vectorToSearch.size() == 0)
        throw std::invalid_argument( "received empty vector" );

    unsigned int idx = 0;
    T minItem = vectorToSearch[idx];

    for(int i = 1; i < vectorToSearch.size(); i++)
    {
        if(minItem > vectorToSearch[i])
        {
            idx = i;
            minItem = vectorToSearch[i];
        }
            
    }
    return idx;
}

template <typename T>
bool elementInVector(T& itemToMatch, const std::vector<T>& vectorToSearch)
{
    for(auto it:vectorToSearch)
        if(itemToMatch == it)
            return true;
    return false;
}

template <typename T>
void appendToVector(std::vector<T>& vectorToAddTo, std::vector<T>& vectorOfNewItems)
{
    for(auto it:vectorOfNewItems)
        vectorToAddTo.push_back(it);
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
std::vector<T> returnMatchingElements(const std::vector<T>& vectorToSearch, const std::vector<T>& vectorToMatch)
{

    std::vector<T> matchedElements;

    for(auto searchItem:vectorToSearch)
    {
        for(auto matchedIT = vectorToMatch.begin(); matchedIT != vectorToMatch.end(); matchedIT++)
        {
            if(searchItem == *matchedIT)
            {
                // Assume no repeats
                matchedElements.push_back(searchItem);
                break;
            }
        }
    }
    return matchedElements;
}
