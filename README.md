# Multi-threaded element queue in C++

# Prerequisits

1 - Develop a class from scratch to queue a finite amount of elements.
- Reading thread pop elements while writing thread push elements.
- If the queue is empty, reading thread will block and wait for the next element.
- If the queue is full, writing thread will block and wait for another thread to remove an item.
- The reading/writing threads should optionally block for a certain period of time. If the action is successful within this time, true is returned, otherwise false.

2 - Develop unit tests for the Queue class with support of a framework

