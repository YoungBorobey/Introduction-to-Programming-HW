#include <utility>
#include <cstdlib>
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) {
        new(data) T(elem);
        defined = true;
    }
    Optional(T && elem) {
        new(data) T(std::move(elem));
        defined = true;
    }
    Optional(const Optional& other) {
        if (defined) {
            if (other.defined) {
                *(reinterpret_cast<T*>(data)) =
                *(reinterpret_cast<const T*>(other.data));
            } else {
                defined = false;
                reinterpret_cast<T*>(data)->~T();
            }
        } else {
            defined = other.defined;
            if (defined) {
                new(data) T(*(reinterpret_cast<const T*>(other.data)));
            }
        }
    }

    Optional& operator=(const Optional& other) {
        if (defined) {
            if (other.defined) {
                *(reinterpret_cast<T*>(data)) =
                *(reinterpret_cast<const T*>(other.data));
            } else {
                defined = false;
                reinterpret_cast<T*>(data)->~T();
            }
        } else {
            defined = other.defined;
            if (defined) {
                new(data) T(*(reinterpret_cast<const T*>(other.data)));
            }
        }
        return (*this);
    }
    Optional& operator=(const T& elem) {
        if (defined) {
            *(reinterpret_cast<T*>(data)) = elem;
        } else {
            defined = true;
            new(data) T(elem);
        }
        return (*this);
    }
    Optional& operator=(T && elem) {
        if (defined) {
            *(reinterpret_cast<T*>(data)) = std::move(elem);
        } else {
            defined = true;
            new(data) T(std::move(elem));
        }
        return (*this);
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return *(reinterpret_cast<T*>(data));
    }
    const T& operator*() const {
        return *(reinterpret_cast<const T*>(data));
    }

    T* operator->() {
        return reinterpret_cast<T*>(data);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    T& value() {
        if (defined) {
            return *(reinterpret_cast<T*>(data));
        }
        throw BadOptionalAccess();
    }
    const T& value() const {
        if (defined) {
            return *(reinterpret_cast<const T*>(data));
        }
        throw BadOptionalAccess();
    }

    void reset() {
        if (defined) {
            defined = false;
            reinterpret_cast<T*>(data)->~T();
        }
    }

    ~Optional() {
        if (defined) {
            reinterpret_cast<T*>(data)->~T();
        }
    }
};
