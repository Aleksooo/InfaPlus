#include <iostream>

using i64 = int64_t;
using u64 = uint64_t;


class List;

struct Item {
    Item* prev;
    Item* next;
    i64 value;
    
    Item(): prev(nullptr), next(nullptr), value(0) {}
    Item(i64 v): prev(nullptr), next(nullptr), value(v) {}
};

class Iterator {
    public:
        List* lst;
        Item* itm;

    Iterator(List* l, Item* i) : lst(l), itm(i) {}

    Iterator& operator++() {
        itm = itm->next;

        return *this;
    }

    Iterator& operator--() {
        itm = itm->prev;

        return *this;
    }

    bool operator==(const Iterator& other) const {
        return (lst == other.lst) && (itm == other.itm);
    }

    bool operator!=(const Iterator& other) const {
        return !(other == *this);
    }

    i64* operator->() {
        return &(itm->value);
    }

    i64& operator*() {
        return itm->value;
    }

};


class List {
    public:
        Item* head;
        Item* tail;
        size_t size;

    List(): head(nullptr), tail(nullptr), size(0) {}
    List(List& other): head(nullptr), tail(nullptr), size(0) {
        for (size_t i = 0; i < other.size; i++) {
            (*this).PushBack(*other[i]);
        }
    }

    size_t Size() {
        return size;
    }

    void Fill(size_t N) {
        i64 v = 0;
        for (size_t i = 0; i < N; i++) {
            std::cin >> v;
            (*this).PushBack(v);
        }
    }

    void Print() {
        Iterator itr = (*this).begin();

        for (size_t i = 0; i < size; i++) {
            std::cout << *itr << " ";
            ++itr;
        }
        std::cout << "\n";
    }

    /*------------------------------------------------------------*/

    Iterator PushFront(i64 v) {
        Item* p = new Item(v);

        if (head == nullptr) {
            head = p;
            tail = p;
        } else {
            p->next = head;

            head->prev = p;
            head = p;
        }
        size++;

        return Iterator(this, head);
    }

    Iterator PushBack(i64 v) {
        Item* p = new Item(v);
        
        if (tail == nullptr) {
            head = p;
            tail = p;
        } else {
            p->prev = tail;

            tail->next = p;
            tail = p;
        }
        size++;

        return Iterator(this, tail);
    }

    /*------------------------------------------------------------*/

    Iterator PopFront() {
        if (head != tail) {
            head = head->next;
            delete head->prev;
            head->prev = nullptr;
        } else {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        size--;

        return Iterator(this, head);
    }

    Iterator PopBack() {
        if (head != tail) {
            tail = tail->prev;
            delete tail->next;
            tail->next = nullptr;
        } else {
            delete tail;
            head = nullptr;
            tail = nullptr;
        }
        size--;

        return Iterator(this, tail);
    }

    /*------------------------------------------------------------*/

    Iterator begin() {
        return Iterator(this, head);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    /*------------------------------------------------------------*/

    Iterator Remove(Iterator itr) {
        Item* p = itr.itm;
        Item* n = p->next;

        if (p->prev != nullptr) {
            p->prev->next = p->next;
        } else {
            head = p->next;
            head->prev = nullptr;
        }

        if (p->next != nullptr) {
            p->next->prev = p->prev;
        } else {
            tail = p->prev;
            tail->next = nullptr;
        }
        size--;
        delete p;

        return Iterator(this, n);
    }

    Iterator Insert(Iterator itr, i64 v) {
        Item* p = itr.itm;
        Item* a = new Item(v);

        a->prev = p;
        a->next = p->next;
        p->next = a;

        if (a->next != nullptr) {
            a->next->prev = a;
        } else {
            tail = a;
        }

        if (a->prev != nullptr) {
            a->prev->next = a;
        } else {
            head = a;
        }
        size++;

        return Iterator(this, a);
    }

    /*------------------------------------------------------------*/

    Iterator operator[] (size_t i) {
        Iterator itr(this, head);
        for (size_t j = 0; j < i; j++) {
            ++itr;
        }

        return itr;
    }

    List& operator= (List& other) {
        if (this == &other) {
            return *this;
        }

        size = 0;
        head = nullptr;
        tail = nullptr;
        for (size_t i = 0; i < other.size; i++) {
            (*this).PushBack(*other[i]);
        }

        return *this;
    }

    ~List() {
        Iterator itr = (*this).begin();
        Item* p;

        for (size_t i = 0; i < size; i++) {
            p = itr.itm;
            ++itr;
            delete p;
        }
    }
};


int main() {
    List lst1;
    lst1.Fill(5);

    List lst2(lst1);
    lst1.PopFront();
    lst1.PopBack();

    lst1.Remove(lst1[2]);
    lst1.Insert(lst1[0], 100);
    
    lst1.Print();
    lst2.Print();
}