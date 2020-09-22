#include <iostream>

template<typename T>
class SharedPtr {
private:
    T* data;
    size_t* count;

public:
    void inc() {
        if (data == nullptr)
            return;
        if (count == nullptr) {
            count = new size_t(0);
        }
        ++(*count);
    }

    void dec() {
        if (data == nullptr) {
            return;
        }
        if (*count == 1) {
            delete data;
            delete count;
            return;
        }
        --(*count);
    }

    SharedPtr() : data(nullptr), count(nullptr) {}

    SharedPtr(T* a) : data(a), count(nullptr) {
        inc();
    }

    SharedPtr(const SharedPtr& other) : data(other.data), count(other.count) {
        inc();
    }

    SharedPtr(SharedPtr&& other) : data(other.data), count(other.count) {
        other.data = nullptr;
        other.count = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (data != other.data) {
            dec();
            data = other.data;
            count = other.count;
            other.data = nullptr;
            other.count = nullptr;
        }
        return (*this);
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (data != other.data) {
            dec();
            data = other.data;
            count = other.count;
            inc();
        }
        return (*this);
    }

    ~SharedPtr() {
        dec();
    }

    T& operator*() {
        return *data;
    }

    const T& operator*() const {
        return *data;
    }

    const T* operator->() const {
        return data;
    }

    void reset(T* ptr) {
        if (data == ptr) {
            return;
        }
        dec();
        data = ptr;
        count = nullptr;
        inc();
    }

    void swap(SharedPtr& other) noexcept {
        if (data != other.data) {
            std::swap(data, other.data);
            std::swap(count, other.count);
        }
    }

    T* get() const {
        return data;
    }

    explicit operator bool() const noexcept {
        return count != nullptr;
    }
};
