#include <mutex>
#include <condition_variable>

using namespace std;
template <typename T>
class BlockingQueue {
private:
    struct Node{
        T data;
        unique_ptr<Node> next;
        template<typename... Args>
        Node(Args&&... args): data(forward<Args>(args)...), next(nullptr) {}
    };

    unique_ptr<Node> head;
    Node* tail;
    size_t sz;
    mutex m;
    condition_variable cv;
    
public:
    BlockingQueue(): head(nullptr), tail(nullptr), sz(0) {}

    ~BlockingQueue() {}

    void push(const T& val) {
        auto newNode = make_unique<Node>(val);
        Node* newTail = newNode.get();
        {
            lock_guard<mutex> lk(m);
            if(tail) {
                tail->next = move(newNode);
            } else {
                head = move(newNode);
            }

            tail = newTail;
            sz++;
        }
        cv.notify_one();
    }

    T pop() {
        unique_lock<mutex> lk(m);
        cv.wait(lk, [this]() {
            return sz!=0;
        });

        auto curr = move(head);
        head = move(curr->next);
        if(!head) {
            tail = nullptr;
        }
        sz--;
        return curr->data;
    }
};