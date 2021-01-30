#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <condition_variable>
#include <mutex>
#include <iostream>
#include <queue>
#include <thread> 
#include <chrono> 
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define DEBUG_MODE 0    //Variable to compile with (1) or without (0) debug 
#define MAXSIZE 10
template<typename T>
class Queue
{
public:
    Queue(int size){
#if DEBUG_MODE        
        std::lock_guard<std::mutex> lock(cout_mut);
        std::cout << "Max Size of the queue is " << size << std::endl;
#endif
        if(size <= 0)
        {
            max_size = MAXSIZE;
        }else{
            max_size = size;
        }
    }

    void push(T new_value)
    {
        std::unique_lock<std::mutex> lk(mut);

        condNotFull.wait_for(lk,std :: chrono :: milliseconds ( 50 ),[this]{
            if ((_count == max_size)) {
#if DEBUG_MODE                
                std::lock_guard<std::mutex> lock(cout_mut);
                std::cout << "Full Queue " << std::endl;
#endif                
                return false;
            }else{
                return true;
            }           
        });

        queue.push(new_value);
#if DEBUG_MODE         
        std::lock_guard<std::mutex> lock(cout_mut);
        std::cout << "Pushing " << new_value << std::endl;
#endif        
        _count++;
        condNotEmpty.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lk(mut);

        condNotEmpty.wait_for(lk, std :: chrono :: milliseconds ( 85 ),[this]{   
            if(queue.empty()){
#if DEBUG_MODE
                std::lock_guard<std::mutex> lock(cout_mut);
                std::cout << "Empty queue" << std::endl;
#endif
                return false;
            }else{
                return true;
            }
        });

        auto value=queue.front();
        queue.pop();
#if DEBUG_MODE        
        std::lock_guard<std::mutex> lock(cout_mut);
        std::cout << "Popping " << value << std::endl;
#endif        
        _count--;
        condNotFull.notify_one();
        
        return value;
    }

    int Count(){
        std::lock_guard<std::mutex> lk(mut);
        return queue.size();
    }

    int Size(){
        return max_size;
    }

    bool Empty(){
        std::lock_guard<std::mutex> lk(mut);
        return queue.empty();
    }

private:
    std::mutex mut;
#if DEBUG_MODE
    std::mutex cout_mut;
#endif
    std::queue<T> queue;
    std::condition_variable condNotEmpty;
    std::condition_variable condNotFull;

    int _count;
    int max_size;    
};

#endif EXAMPLE_HPP