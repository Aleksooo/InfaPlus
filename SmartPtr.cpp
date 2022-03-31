#include <iostream>

using i64 = int64_t;
using u64 = uint64_t;

struct T {
    i64 a, b;
    T(i64 a_, i64 b_): a(a_), b(b_) {}

    void Print() {
        std::cout << a << " " << b << "\n";
    }
};

/* Создание умного уникального указателя */
class UniquePtr {
    private:
        T* ptr;
    
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator= (const  UniquePtr&) = delete;

    UniquePtr(T* raw): ptr(raw) {}
    friend UniquePtr MakeUnique(T value);
    
    public:
    UniquePtr(UniquePtr&& rhs) {
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
    };
    UniquePtr(): ptr(nullptr) {}

    UniquePtr& operator= (UniquePtr&& rhs) {
        if (this == &rhs) {
            return *this;
        }

        UniquePtr t(std::move(rhs));
        std::swap(t.ptr, ptr);
        return *this;
    }

    T* operator->() { return ptr; }
    T& operator*() { return *ptr; }

    ~UniquePtr() { delete ptr; }
};

UniquePtr MakeUnique(T value) {
    T* p = new T(value);
    UniquePtr u(p);
    return u;
}


/*----------------------------------------------------------------------*/


struct RefCountBlock{
    size_t strong, weak;

    RefCountBlock(): strong(0), weak(0) {}
};

/* Создание Сильного указателя */
class SharedPtr {
    private:
        T* raw;

    SharedPtr(T* lhs): raw(lhs) {
        cnt = new RefCountBlock;
        cnt->strong = 1;
        cnt->weak = 0;
    }
    friend SharedPtr MakeShared(T value);

    public:
        RefCountBlock* cnt;

    SharedPtr(): raw(nullptr), cnt(nullptr) {}
    SharedPtr(SharedPtr&& rhs): raw(rhs.raw), cnt(rhs.cnt) {
        rhs.raw = nullptr;
        rhs.cnt = nullptr;
    }
    SharedPtr(const SharedPtr& lhs): raw(lhs.raw), cnt(lhs.cnt) {
        ++(cnt->strong);
    } 

    SharedPtr& operator= (SharedPtr&& rhs) {
        if (this == &rhs) {
            return *this;
        }

        SharedPtr t(std::move(rhs));
        std::swap(raw, t.raw);
        std::swap(cnt, t.cnt);
        return *this;
    }
    SharedPtr& operator= (const SharedPtr& lhs) {
        if (this == &lhs) {
            return *this;
        }

        SharedPtr t(lhs);
        std::swap(raw, t.raw);
        std::swap(cnt, t.cnt);
        return *this;
    }

    T* operator->() { return raw; }
    T& operator*() { return *raw; }

    ~SharedPtr() {
        if (cnt == nullptr) { return; }

        --(cnt->strong);
        if (cnt->strong != 0) { return; }
        delete raw;
        if (cnt->weak == 0) {
            delete cnt;
        }
    }
};

SharedPtr MakeShared(T value) {
    T* p = new T(value);
    SharedPtr s(p);
    return s;
}


/*----------------------------------------------------------------------*/


/* Создание Слабого указателя */
class WeakPtr {
    private:
        T* raw;

    WeakPtr(T* lhs): raw(lhs) {
        cnt = new RefCountBlock;
        cnt->strong = 0;
        cnt->weak = 1;
    }
    friend WeakPtr MakeWeak(T value);

    public: 
        RefCountBlock* cnt;

    WeakPtr(): raw(nullptr), cnt(nullptr) {}
    WeakPtr(WeakPtr&& rhs): raw(rhs.raw), cnt(rhs.cnt) {
        rhs.raw = nullptr;
        rhs.cnt = nullptr;
    }
    WeakPtr(const WeakPtr& lhs):  raw(lhs.raw), cnt(lhs.cnt) {
        ++(cnt->weak);
    }

    WeakPtr& operator= (WeakPtr&& rhs) {
        if (this == &rhs) {
            return *this;
        }

        WeakPtr t(std::move(rhs));
        std::swap(raw, t.raw);
        std::swap(cnt, t.cnt);
        return *this;
    }

    WeakPtr& operator= (const WeakPtr& lhs) {
        if (this == &lhs) {
            return *this;
        }

        WeakPtr t(lhs);
        std::swap(raw, t.raw);
        std::swap(cnt, t.cnt);
        return *this;
    }

    T* operator->() { return raw; }
    T& operator*() { return *raw; }

    ~WeakPtr() {
        if (cnt == nullptr) { return; }

        --(cnt->weak);
        if (cnt->weak == 0 && cnt->strong == 0) {
            delete cnt;
        }
    }
};

WeakPtr MakeWeak(T value) {
    T* p = new T(value);
    WeakPtr s(p);
    return s; 
}


int main() {
    return 0;
}