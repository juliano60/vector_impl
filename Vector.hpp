#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <algorithm>

namespace tutorial {

    template <typename T, typename A = std::allocator<T>>
    struct VectorBase {
        VectorBase(const A& a, typename A::size_type n):
            alloc{a}
        {
            elem = alloc.allocate(n);
            space = last = elem+n;
        }

        ~VectorBase() {
            alloc.deallocate(elem, last-elem);
        }

        VectorBase(const VectorBase&) =delete;
        VectorBase& operator=(const VectorBase&) =delete;

        VectorBase(VectorBase&& a):
            alloc{a.alloc},
            elem{a.elem},
            space{a.space},
            last{a.last}
        {
            a.elem = nullptr;
            a.space = nullptr;
            a.last = nullptr;
        }

        VectorBase& operator=(VectorBase&& a) {
            alloc = a.alloc;
            elem = a.elem;
            space = a.space;
            last = a.last;

            a.elem = nullptr;
            a.space = nullptr;
            a.last = nullptr;

            return *this;
        }
            
        A alloc;
        T* elem;
        T* space;
        T* last;
    };
 

    template <typename T, typename A = std::allocator<T>>
    class Vector {
    public:
        using size_type = typename A::size_type;
        using iterator = T*;
        using const_iterator = const T*;

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
        size_type capacity() const;

        bool empty() const;
        void destroy();

        void reserve(size_type n);
        void resize(size_type n, const T& val = T());
        void clear();
        void push_back(const T&);

        iterator begin();
        const_iterator begin() const;

        iterator end();
        const_iterator end() const;

    private:
        VectorBase<T,A> base_;
    };

    template <typename T, typename A>
    Vector<T,A>::Vector(const A& a):
        base_{a, 0}
    {}

    template <typename T, typename A>
    Vector<T,A>::Vector(size_type n, const T& val, const A& a):
        base_{a, n}
    {
        // constructs objects
        std::uninitialized_fill(base_.elem, base_.elem+n, val);
    }

    template <typename T, typename A>
    Vector<T,A>::Vector(const Vector& rhs):
        base_{rhs.base_.alloc, rhs.size()}
    {
       // now copy construct elements
       std::uninitialized_copy(rhs.base_.elem, rhs.base_.elem+rhs.size(), base_.elem);
    }

    template <typename T, typename A>
    Vector<T,A>& Vector<T,A>::operator=(const Vector& rhs) {
        if (&rhs == this) return *this;

        if (capacity() < rhs.size()) {
            Vector<T,A> tmp{rhs};
        
            // now swap representations
            swap(*this, tmp);

            return *this;
        }

        size_type sz = size();
        size_type asz = rhs.size();

        if (asz <= sz) {
            std::copy(rhs.begin(), rhs.begin()+asz, base_.elem);

            // delete extra elements
            for (T* p = base_.elem+asz; p!= base_.elem+sz; ++p)
                p->~T();
        }
        else {
            std::copy(rhs.begin(), rhs.begin()+sz, base_.elem);

            // construct extra elements
            std::uninitialized_copy(rhs.begin()+sz, rhs.begin()+asz, base_.elem);
        }

        return *this;
    }

    template <typename T, typename A>
    Vector<T,A>::Vector(Vector&& a):
        base_{a.base_}
    {}

    template <typename T, typename A>
    Vector<T,A>& Vector<T,A>::operator=(Vector&& a) {
        // swap representations
        swap(base_, a.base_);

        return *this;
    }

    template <typename T, typename A>
    Vector<T,A>::~Vector() {
        destroy();
    }

    template <typename T, typename A>
    void Vector<T,A>::destroy() {
        for (T* p = base_.elem; p!= base_.space; ++p)
            p->~T();
        base_.space = base_.elem;
    }

    template <typename T, typename A>
    bool Vector<T,A>::empty() const {
        return base_.elem==base_.space;
    }

    template <typename T, typename A>
    typename Vector<T,A>::size_type Vector<T,A>::size() const {
        return base_.space-base_.elem;
    }

    template <typename T, typename A>
    typename Vector<T,A>::size_type Vector<T,A>::capacity() const {
        return base_.last-base_.elem;
    }

    template <typename T, typename A>
    typename Vector<T,A>::iterator Vector<T,A>::begin() {
        return base_.elem;
    }

    template <typename T, typename A>
    typename Vector<T,A>::const_iterator Vector<T,A>::begin() const {
        return base_.elem;
    }

    template <typename T, typename A>
    typename Vector<T,A>::iterator Vector<T,A>::end() {
        return base_.space;
    }
    
    template <typename T, typename A>
    typename Vector<T,A>::const_iterator Vector<T,A>::end() const {
        return base_.space;
    }
    
    template <typename T, typename A>
    void Vector<T,A>::reserve(size_type n) {
        if (n<=capacity()) return;

        VectorBase<T,A> tmp{base_.alloc, n};
        size_type sz = size();
        
        // move elements
        T* q = base_.elem;
        for (T* p = base_.elem; p!=base_.elem+sz; ++p, ++q) {
            new (static_cast<void*>(&*q)) T{std::move(*p)};
            p->~T();
        }
        tmp.space = tmp.elem+sz;

        swap(base_, tmp);
    }
 
    template <typename T, typename A>
    void Vector<T,A>::clear() {
        resize(0);
    }

    template <typename T, typename A>
    void Vector<T,A>::resize(size_type n, const T& val) {
        reserve(n);

        size_type sz = size();

        if (n <= sz) {
            // destroy extra elements
            for (T* p = base_.elem+n; p!=base_.elem+sz; ++p)
                p->~T();
        }
        else {
            // construct extra elements
            std::uninitialized_fill(base_.elem+sz, base_.elem+n, val);
        }
        base_.space = base_.elem+n;
    }

    template <typename T, typename A>
    void Vector<T,A>::push_back(const T& val) {
        if (size()==capacity()) {
            // grow vector
            reserve(std::min(size_type(8), size()*2));
        }

        new(static_cast<void*>(&*base_.space)) T{val};
        ++base_.space;
    }
}

#endif

    

