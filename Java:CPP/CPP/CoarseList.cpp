//
// Used for hashing
#include <functional>
//
// Used for printing
#include <iostream>
//
// Used for locks
#include <mutex>

using namespace std;

template<typename T> class CoarseList {
    private: 
        /**
         * Inner nested node class.
         */
        class Node{
            public:
                //
                // Item being stored
                T item; 

                //
                // Hash of the item.
                size_t key;

                //
                // Next node in the chain.
                Node *next;

                /**
                 * Regular Node constructor 
                 */
                Node(T item) {
                    this->item = item;
                    //
                    // We initiate a new hasher every time which is bad... 
                    // Should use a static hasher. #TODO
                    this->key = hash<T>{}(item);
                    this->next = nullptr;
                }

                /**
                 * Constructor for sentinal nodes
                 */
                Node(size_t key, bool sentinal){
                    this->key = key;
                    this->next = nullptr;
                }
        };

        //
        // The head and tail of the singly linked list implementation of CoarseList.
        Node head;
        Node tail;

        //
        // Lock for the coarse grained implementation.
        std::mutex lock;

        //
        // The std hashing object, so we don't need to initate it multiple times.
        hash<T> hasher;

    public: 
        /**
         * The constructor for the CoarseList. It initiates the head and tail.
         */
        CoarseList() : head(0, true), tail(std::numeric_limits<std::size_t>::max(), true){
            cout << "Constructor is called \n";
            head.next = &tail;
        }

        /**
         * Add an element.
         * @param item element to add
         * @return true iff element was not there already
         */
        bool add(T item) {
            //
            // Get the hash of the item we are trying to insert.
            size_t key = hasher(item);
            Node* curr;
            Node* prev;

            //
            // Acquire the only lock. No one can do anything now..
            lock.lock();
            try {
                //
                // Find the spot we need to add this item to.
                prev = &head;
                curr = prev->next;
                while (curr->key < key){
                    prev = curr;
                    curr = curr->next;
                }

                //
                // If the item already exists in the list, return false.
                if (key == curr->key){
                    lock.unlock();
                    return false;
                }
                
                //
                // Insert the node.
                Node* newNode = (Node*) malloc(sizeof(Node));

                newNode->item = item;
                newNode->key = key;

                newNode->next = curr;
                prev->next = newNode;

                lock.unlock();
                return true;
                
            } catch (...) {
                lock.unlock();
                cout << "Something went wrong during add(). \n";
                return false;
            }
        }

        /**
         * Remove an element.
         * @param item element to remove
         * @return true if element was present
         */
        bool remove(T item) {
            //
            // Get the hash of the item we are trying to insert.
            size_t key = hasher(item);
            Node* curr;
            Node* prev;

            //
            // Acquire the only lock. No one can do anything now..
            lock.lock();
            try {
                //
                // Find the spot we need to add this item to.
                prev = &head;
                curr = prev->next;
                while (curr->key < key){
                    prev = curr;
                    curr = curr->next;
                }

                //
                // If the item does not exist in the list, return false.
                if (key != curr->key){
                    lock.unlock();
                    return false;
                }

                //
                // Remove the node
                prev->next = curr->next;
                free(curr);

                lock.unlock();
                return true;
                
            } catch (...) {
                lock.unlock();
                cout << "Something went wrong during contains(). \n";
                return false;
            }
        }

        /**
         * Test whether element is present
         * @param item element to test
         * @return true iff element is present
         */
        bool contains(T item) {
            //
            // Get the hash of the item we are trying to insert.
            size_t key = hasher(item);
            Node* curr;
            Node* prev;

            //
            // Acquire the only lock. No one can do anything now..
            lock.lock();
            try {
                //
                // Find the spot we need to add this item to.
                prev = &head;
                curr = prev->next;
                while (curr->key < key){
                    prev = curr;
                    curr = curr->next;
                }

                //
                // If the item  exists in the list, return true.
                if (key == curr->key){
                    lock.unlock();
                    return true;
                }
                else {
                    lock.unlock();
                    return false;
                }

            } catch (...) {
                lock.unlock();
                cout << "Something went wrong during contains(). \n";
                return false;
            }
        }
};




int main()
{
    CoarseList<int> list;
    list.add(1);
    bool a = list.contains(1);
    cout << a << "\n";
    bool remove = list.remove(0);
    cout << remove << "\n";
    return 0;
}