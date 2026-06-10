#include <bits/stdc++.h>
#define present(container, element) (container.find(element) != container.end())
#define all(c) c.begin(), c.end()
#define lli long long int
#define vi vector<int>
#define cpresent(container, element) (find(all(container),element) != container.end())
#define tr(container, it) for(auto it = container.begin(); it != container.end(); it++)
using namespace std;

template<typename T>
class Queue{
    struct Node{
        T data;
        Node* next;

        template<typename... Args>
        Node(Args... args): data(args), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    Node* freeList;
    size_t reuseCount;

private:
    template<typename... Args>
    Node* getNode(Args&&... args) {
        if(freeList != nullptr) {
            Node* node = freeList;
            freeList = freeList->next;

            node->data.~T();
            new (&node->data) T(forward<Args>(args)...);

            node->next = nullptr;

            reuseCount++;
            return node;
        }

        return new Node(forward<Args>(args)...);
    }

    void recycleNode(Node* node) {
        node->next = freeList;
        freeList = node;
    }
public:
    ~Queue() {
        while(head != nullptr) {
            Node* curr = head;
            head = head->next;
            delete curr;
        }

        while(freeList != nullptr) {
            Node* curr = freeList;
            freeList = freeList->next;
            delete curr;
        }
    }

    bool empty() const {
        return head==nullptr;
    }


    void push(const T& value) {
        Node* newNode = getNode(value);

        if(tail == nullptr) {
            tail = head = newNode;
        } else {
            tail->next = newNode;
            tail = tail->next;
        }
    }

    template<typename... Args>
    void push(Args... args) {
        Node* newNode = getNode(forward<Args>(args)...);
        if(tail == nullptr) {
            tail = head = newNode;
        } else {
            tail->next = newNode;
            tail = tail->next;
        }
    }

    void pop() {
        if(head == nullptr) return;
        Node* oldHead = head;

        head = head->next;

        if (head == nullptr)
            tail = nullptr;

        recycleNode(oldHead);
    }

    T& front() {
        return head->data;
    }

    T& back() {
        return tail->data;
    }

    std::size_t reusedNodes() const {
        return reuseCount;
    }

};

struct Msg {
    uint8_t id;
    uint64_t timestamp;

    Msg(uint8_t i, uint64_t t)
        : id(i), timestamp(t) {}
};

int main() {

    Queue<Msg> q;

    // Initial allocations
    for (int i = 0; i < 1000; ++i) {
        q.emplace(i, i);
    }

    // Recycle all nodes
    for (int i = 0; i < 1000; ++i) {
        q.pop();
    }

    // These should reuse nodes
    for (int i = 0; i < 1000; ++i) {
        q.emplace(i, i);
    }

    std::cout << "Reused nodes: "
              << q.reusedNodes()
              << "\n";
}