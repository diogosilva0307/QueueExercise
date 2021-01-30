#include <gtest/gtest.h>
#include "../multithreadedQueue.hpp"

int maxSize = 3;

Queue<int> data_queue(maxSize);
TEST(ExampleTests, blocking_queue){
    //Generate a random seed
    srand(time(0));
    
    int maxTries = 5;
    int nElements = 10;
    int maxDelta = 3;
    
    for(int k = 0; k <= maxDelta; k++){
        for(int j = 0; j < maxTries; j++)
        {
            ASSERT_TRUE(data_queue.Empty()); 
            std::thread writer = std::thread([&](){
                for(int i=1; i<=nElements+k; i++){           
                    data_queue.push(i);
                    ASSERT_GE(maxSize, data_queue.Count());

                    std::this_thread::sleep_for(std::chrono::milliseconds((rand()%5+1)*5)); //40
                }
            });

            std::thread reader = std::thread([&](){
                for(int i=1; i<=nElements+k; i++){          
                    data_queue.pop();
                    ASSERT_LE(0,data_queue.Count());
                    std::this_thread::sleep_for(std::chrono::milliseconds((rand()%5+1)*10));  //60
                }
            });
            if (writer.joinable()){
                writer.join();
            }

            if (reader.joinable()){    
                reader.join();
            }
            
            ASSERT_TRUE(data_queue.Empty());
            EXPECT_EQ(0,data_queue.Count());
        }  
    } 
}

