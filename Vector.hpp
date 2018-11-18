#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

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
        void destroy();

        void reserve(size_type n);
        iterator begin();
        iterator end();

    private:
        VectorBase<T,A> base_;
    };

    template <typename T, typename A>
    typename Vector<T,A>::iterator Vector<T,A>::begin() {
        return base_.elem;
    }

    template <typename T, typename A>
    typename Vector<T,A>::iterator Vector<T,A>::end() {
        return base_.space;
    }

    template <typename T, typename A>
    Vector<T,A>::Vector(const A& a):
        base_{a, 0}
    {}

    template <typename T, typename A>
    Vector<T,A>::Vector(size_type n, const T& val, const A& a)
    : base_{a, n}
    {
        // construct using default constr or supplied value
        std::uninitialized_fill(base_.elem, base_.space, val);
    }

    template <typename T, typename A>
    Vector<T,A>::~Vector() {
        // call destructor 
        destroy();
    }
    
    template <typename T, typename A>
    Vector<T,A>::Vector(const Vector& rhs):
        base_{rhs.base_, rhs.size()}
    {
        std::uninitialized_copy(rhs.base_.elem, rhs.base_.elem+rhs.size(), base_.elem);
    }

    template <typename T, typename A>
    void Vector<T,A>::destroy() {
        for (T* p = base_.elem; p!=base_.space; ++p)
            base_.alloc.destroy(p);
    }

    template <typename T, typename A>
    Vector<T,A>& Vector<T,A>::operator=(const Vector& rhs) {
        if (&rhs == this) return *this;

        VectorBase<T,A> tmp{rhs.base_, rhs.size()};

        // destroy elements
        destroy();

        // swap representations
        swap(base_, tmp);

        return *this;
    }

    template <typename T, typename A>
    typename Vector<T,A>::size_type Vector<T,A>::size() const {
        return base_.space-base_.elem;
    }

    template <typename T, typename A>
    bool Vector<T,A>::empty() const {
        return base_.elem==base_.space;
    }

    template <typename T, typename A>
    void Vector<T,A>::reserve(size_type n) {
        if (n<=size()) return;

        VectorBase<T,A> p{base_.alloc, n};
        std::uninitialized_copy(base_.elem, base_.elem+size(), p.elem);
        p.space = p.elem + size();

        // swap representations
        swap(base_, p);
    }
     
    template <typename T, typename A>
    Vector<T,A>::Vector(Vector&& a):
        base_{a.base_}
    {}

    template <typename T, typename A>
    Vector<T,A>& Vector<T,A>::operator=(Vector&& a) {
        swap(base_, a.base_);
        return *this;
    }

}

#endif
