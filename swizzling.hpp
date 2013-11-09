#ifndef TUMBO_SWIZZLING_HPP
#define TUMBO_SWIZZLING_HPP

#include <utility>
#include "matrix.hpp"

namespace tumbo
    {
    /* TODO: Direct assignment to vector of correct size. */

    template<class MatT, class T>
    class swiz_value {
        public:
            swiz_value( T val ) :
                val{val}
                {}

            typename MatT::scalar_t
            operator() (MatT&) const
                { return val; }
            T val;
        };


    template<class MatT>
    class swiz_value<MatT,components::cmp>
        {
        public:
            swiz_value( components::cmp c ) :
                c{c}
                {}

            typename MatT::scalar_t& operator()
            (MatT& mat) const
                { return mat[c]; }

        components::cmp c;
        };



    template<class T, size_t D, typename... Vs>
    class swizzle_ref;

    template<class T, size_t D, typename V, typename... Vs>
    class swizzle_ref<T,D,V,Vs...> : public swizzle_ref<T,D,Vs...>
        {
        public:
            typedef swizzle_ref<T,D,Vs...> base_t;
            typedef typename swizzle_ref<T,D,V>::mat_t mat_t;

            static constexpr size_t size()
                { return sizeof...(Vs)+1; }

            swizzle_ref( mat_t& mat, V v, Vs... vs ) :
                swizzle_ref<T,D,Vs...>( mat, vs... ),
                val{v}
                {}

            template<class TO, size_t DO, typename VO, typename... VOs>
            void
            assign( const swizzle_ref<TO,DO,VO,VOs...>&& other )
                {
                typedef swizzle_ref<TO,DO,VO,VOs...> other_t;
                val(*this->ref) = other.val(*other.ref);
                base_t::assign(
                    std::forward<const swizzle_ref<TO,DO,VOs...>>(other) );
                }

            template<class T1, size_t D1, typename... Vs1>
            swizzle_ref&
            operator = ( swizzle_ref<T1,D1,Vs1...>&& other )
                {
                assign(std::forward<swizzle_ref<T1,D1,Vs1...>>(other));
                return *this;
                }

            template<class T1, size_t D1> matrix<T1,D1,1>
            copy_to( matrix<T1,D1,1>& m, size_t i=0 ) const
                {
                m[i] = val(*this->ref);
                return base_t::template copy_to( m, i+1 );
                }

            operator matrix<T,size(),1>() const
                {
                matrix<T,size(),1> m{};
                return copy_to(m);
                }

            swiz_value<mat_t,V> val;
        };


    template<class T, size_t D, typename V>
    class swizzle_ref<T,D,V>
        {
        public:
            typedef matrix<T,D,1> mat_t;

            static constexpr size_t size()
                { return 1; }

            swizzle_ref( mat_t& mat, V v ) :
                ref{&mat}, val{v}
                {}

            template<class TO, size_t DO, typename VO>
            void
            assign( const swizzle_ref<TO,DO,VO>&& other )
                { val(*ref) = other.val(*other.ref); }

            template<class T1, size_t D1> matrix<T1,D1,1>&
            copy_to( matrix<T1,D1,1>& m, size_t i=0 ) const
                {
                m[i] = val(*ref);
                return m;
                }

            operator mat_t() const
                {
                mat_t m{};
                return copy_to(m);
                }

            mat_t* ref;
            swiz_value<mat_t,V> val;
        };


    template<class T, size_t D, typename... Vs> swizzle_ref<T,D,Vs...>
    swiz( matrix<T,D,1>& A, Vs... vs)
        {
        return {A, vs...};
        }


    } /* namespace tumbo */


#endif /* TUMBO_SWIZZLING_HPP */
