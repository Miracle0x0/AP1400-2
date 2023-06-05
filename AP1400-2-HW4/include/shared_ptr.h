#ifndef SHARED_PTR
#define SHARED_PTR

#include <memory>
#include <string>

using std::make_shared;

template <typename T>
class SharedPtr {
   public:
    /*
     * Default Constructor
     */
    SharedPtr();

    /*
     * Constructor
     */
    SharedPtr(T* p);

    /*
     * make_shared
     */
    SharedPtr(std::shared_ptr<T> sp);

    /*
     * Copy Constructor
     */
    SharedPtr(SharedPtr& sp);

    /*
     * Operator=
     */
    SharedPtr& operator=(const SharedPtr& sp);

    /*
     * use_count
     *
     * todo:
     *      In SharedPtr we should have the ability to count the number of
     * instances pointing to a same place. To do this you have to define another
     * member variable for your SharedPtr class and keep track of this number.
     */
    size_t use_count() const;

    /*
     * get
     *
     * todo:
     *      The get() function should return the raw pointer stored in the
     * class.
     */
    T* get() const;

    /*
     * Operator*
     */
    T& operator*() const;

    /*
     * Operator->
     */
    T* operator->() const;

    /*
     * reset
     *
     * todo:
     *      The reset() function will delete the pointer and assigns nullptr to
     * it.
     */
    void reset();

    /*
     * reset
     *
     * todo:
     *      The reset() function can have a input and make a new pointer with it
     * after deleting the old pointer.
     */
    void reset(T* p);

    /*
     * operator bool()
     */
    operator bool() const;

    /*
     * Destructor
     */
    ~SharedPtr();

   private:
    size_t* _p_count = nullptr;
    T* _p = nullptr;
};

#include "shared_ptr.h"

template <typename T>
SharedPtr<T>::SharedPtr() : _p_count(new size_t(0)) {}

template <typename T>
SharedPtr<T>::SharedPtr(T* p) : _p(p), _p_count(new size_t{1}) {}

template <typename T>
SharedPtr<T>::SharedPtr(std::shared_ptr<T> sp)
    : _p(new T{*sp.get()}), _p_count(new size_t{1}) {}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr& sp) {
    (*sp._p_count)++;
    _p_count = sp._p_count;
    _p = sp._p;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& sp) {
    if (&sp == this) return *this;
    this->~SharedPtr();
    (*sp._p_count)++;
    if (_p_count == nullptr) _p_count = new size_t(*sp._p_count);
    _p = sp._p;
    return *this;
}

template <typename T>
size_t SharedPtr<T>::use_count() const {
    if (_p_count == nullptr) return 0;
    return *_p_count;
}

template <typename T>
T* SharedPtr<T>::get() const {
    return _p;
}

template <typename T>
T& SharedPtr<T>::operator*() const {
    return *_p;
}

template <typename T>
T* SharedPtr<T>::operator->() const {
    return &(this->operator*());
}

template <typename T>
void SharedPtr<T>::reset() {
    delete _p;
    _p = nullptr;
    *_p_count = 0;
}

template <typename T>
void SharedPtr<T>::reset(T* p) {
    if (p == _p) return;
    delete _p;
    _p = p;
    *_p_count = 1;
}

template <typename T>
SharedPtr<T>::operator bool() const {
    return _p == nullptr;
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    if (_p_count == nullptr) return;  // has been deleted
    if (*_p_count == 0) {             // points to nothing
        delete _p_count;
        return;
    }
    (*_p_count)--;
    if (*_p_count == 0) {
        delete _p;
        _p = nullptr;
        delete _p_count;
        _p_count = nullptr;
    }
}

#endif  // SHARED_PTR