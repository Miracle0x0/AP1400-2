#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <memory>
#include <string>

using std::make_unique;

template <typename T>
class UniquePtr {
   public:
    /*
     * Default Constructor
     *
     * todo:
     *      Assign nullptr to _p.
     */
    UniquePtr();

    /*
     * Constructor
     */
    UniquePtr(T* p);

    /*
     * make_unique
     */
    UniquePtr(std::unique_ptr<T> up);

    /*
     * Copy Constructor (DELETED)
     */
    UniquePtr(const UniquePtr& up) = delete;

    /*
     * Operator= (DELETED)
     */
    UniquePtr& operator=(const UniquePtr& up) = delete;

    /*
     * get
     *
     * todo:
     *      Return the raw pointer stored in the class.
     */
    T* get() const;

    /*
     * Operator*
     *
     * todo:
     *      Smart pointers should be able to be dereferenced exactly like raw
     * pointers.
     */
    T& operator*() const;

    /*
     * Operator->
     *
     * todo:
     *      Smart pointers can use the arrow operator like normal pointers.
     */
    T* operator->() const;

    /*
     * reset
     *
     * todo:
     *      Delete the pointer and assign nullptr to it.
     */
    void reset();

    /*
     * reset
     *
     * todo:
     *      The reset() function can have a input and make a new pointer with it
     * deleting the old pointer.
     */
    void reset(T* p);

    /*
     * release
     *
     * todo:
     *      The release() function returns the stored pointer in the class (like
     * get) with two differences: The UniquePtr class won't store the pointer
     * anymore and also deleting the pointer should not be done by UniquePtr
     * class after calling release().
     */
    T* release();

    /*
     * Operator bool()
     */
    operator bool() const;

    /*
     * Destructor
     */
    ~UniquePtr();

   private:
    T* _p = nullptr;
};

template <typename T>
UniquePtr<T>::UniquePtr() : _p(nullptr) {}

template <typename T>
UniquePtr<T>::UniquePtr(T* p) : _p(p) {}

template <typename T>
UniquePtr<T>::UniquePtr(std::unique_ptr<T> up) : _p(up.release()) {}

template <typename T>
T* UniquePtr<T>::get() const {
    return _p;
}

template <typename T>
T& UniquePtr<T>::operator*() const {
    return *_p;
}

template <typename T>
T* UniquePtr<T>::operator->() const {
    return &(this->operator*());
}

template <typename T>
void UniquePtr<T>::reset() {
    delete _p;
    _p = nullptr;
}

template <typename T>
void UniquePtr<T>::reset(T* p) {
    delete _p;
    _p = p;
}

template <typename T>
T* UniquePtr<T>::release() {
    T* p = _p;
    _p = nullptr;
    return p;
}

template <typename T>
UniquePtr<T>::operator bool() const {
    return _p != nullptr;
}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete _p;
    _p = nullptr;
}

#endif  // UNIQUE_PTR