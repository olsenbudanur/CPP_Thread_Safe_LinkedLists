# Thread Safe Linked Lists in Modern C++

This project transcribes the thread safe linked lists implementations introduced in the 9th chapter of The Art of Multiprocessor Programming from Java to C++. Small tweaks were needed to acount for the lack of a garbage collection in C++ (Dynamic memory allocation, and smart pointers). 

Use g++ -std=c++17 -stdlib=libc++ CoarseList.cpp -o program to compile