#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

namespace tutorial {

    template <typename T, typename A = std::allocator<T>>
    class Vector {
    public:
        using size_type = typename A::size_type;
        using iterator = T*;

        explicit Vector(const A& = A());
        explicit Vector(size_type n, const T& val = T(), const A& = A());
        ~Vector();

        // copy operations
        Vector(const Vector& rhs);
        Vector& operator=(const Vector& rhs);

        // move operations
        Vector(Vector&& a);
        Vector& operator=(Vector&& a);

        size_type size() const;
        bool empty() const;

        void reserve(size_type n);
        iterator begin();
        iterator end();

    private:
        A alloc_;
        T* elem_;
        T* space_;
        T* last_;
    };

    template <typename T, typename A>
    typename Vector<T,A>::iterator Vector<T,A>::begin() {
        return elem_;
    }

    template <typename T, typename A>
    typename Vector<T,A>::iterator Vector<T,A>::end() {
        return space_;
    }

    template <typename T, typename A>
    Vector<T,A>::Vector(const A& a):
        alloc_{a}, elem_{nullptr}, space_{nullptr}, last_{nullptr}
    {}

    template <typename T, typename A>
    Vector<T,A>::Vector(size_type n, const T& val, const A& a)
    : alloc_{a}
    {
        elem_ = alloc_.allocate(n);

        try {
            // construct using default constr or supplied value
            std::uninitialized_fill(elem_, space_, val);
            space_ = last_ = elem_+n;
        }
        catch (...) {
            alloc_.deallocate(elem_, n);
            throw;
        }
    }

    template <typename T, typename A>
    Vector<T,A>::~Vector() {
        // call destructor 
        for (T* p = elem_; p != space_; ++p)
            alloc_.destroy(p);

        alloc_.deallocate(elem_, last_ - elem_);
    }
    
    template <typename T, typename A>
    Vector<T,A>::Vector(const Vector& rhs):
        alloc_{rhs.alloc_}
    {
        elem_ = alloc_.allocate(rhs.size());
        std::uninitialized_copy(rhs.elem_, rhs.elem_+rhs.size(), elem_);

        space_ = elem_+rhs.size();
        last_ = elem_+rhs.size();
    }

    template <typename T, typename A>
    Vector<T,A>& Vector<T,A>::operator=(const Vector& rhs) {
        if (&rhs == this) return *this;

        Vector<T,A> temp{rhs};
        for (T* p = elem_; p != space_; ++p)
            alloc_.destroy(p);

        alloc_.deallocate(elem_, last_ - elem_);

        elem_ = temp.elem_;
        space_ = temp.space_;
        last_ = temp.last_;

        return *this;
    }

    template <typename T, typename A>
    typename Vector<T,A>::size_type Vector<T,A>::size() const {
        return space_-elem_;
    }

    template <typename T, typename A>
    bool Vector<T,A>::empty() const {
        return elem_==space_;
    }

    template <typename T, typename A>
    void Vector<T,A>::reserve(size_type n) {
        if (n<=size()) return;

        T* p = alloc_.allocate(n);
        std::uninitialized_copy(elem_, elem_+size(), p);

        size_type old_size = size();
        // cleanup
        for (T* p = elem_; p != space_; ++p)
            alloc_.destroy(p);

        alloc_.deallocate(elem_, last_ - elem_);
        
        elem_ = p;
        space_ = elem_+old_size;
        last_ = elem_+n;
    }
     
    template <typename T, typename A>
    Vector<T,A>::Vector(Vector&& a):
        alloc_{a.alloc_}, elem_{a.elem_}, space_{a.space_}, last_{a.last_}
    {
        a.elem_ = nullptr;
        a.space_ = nullptr;
        a.last_ = nullptr;
    }

    template <typename T, typename A>
    Vector<T,A>& Vector<T,A>::operator=(Vector&& a) {
        alloc_ = a.alloc_;
        elem_ = a.elem_;
        space_ = a.space_;
        last_ = a.last_;
        
        a.elem_ = nullptr;
        a.space_ = nullptr;
        a.last_ = nullptr;

        return *this;
    }

}

#endif
