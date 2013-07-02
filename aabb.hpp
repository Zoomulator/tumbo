#ifndef TUMBO_AABB_HPP
#define TUMBO_AABB_HPP

#include <vector>
#include <functional>
#include <tumbo/tumbo.hpp>
#include <algorithm>

using std::min;
using std::max;

namespace tumbo
    {

    template<typename T,size_t D> using
    aabb = tumbo::matrix<T,D,2>; // Two points, low and high corners


    typedef aabb<float,3>   faabb3;
    typedef aabb<float,2>   faabb2;

    typedef aabb<double,3>  daabb3;
    typedef aabb<double,2>  daabb2;

    typedef aabb<int,3>     iaabb3;
    typedef aabb<int,2>     iaabb2;

    template<typename T, size_t D> using
    aabb_list = std::vector<aabb<T,D>>;

    // Check if aabb contains point
    template<class T, size_t D> bool
    contains( aabb<T,D> a, vec<T,D> p )
        {
        for( size_t d=0; d < D; ++d )
            if( p[d] < a(d,0) || a(d,1) <= p[d] )
                return false;
        return true;
        }


    // Check if a contains b
    template<class T,size_t D> bool
    contains( aabb<T,D> a, aabb<T,D> b )
        {
        for( size_t d=0; d<D; ++d )
            if( b(d,0) < a(d,0) || b(d,0) > a(d,1) ||
                b(d,1) > a(d,1) || b(d,1) < a(d,0) )
                return false;
        return true;
        }


    template<class T,size_t D> bool
    overlaps( aabb<T,D> a, aabb<T,D> b )
        {
        for( size_t d=0; d < D; ++d )
            if( a(d,0) >= b(d,1) || b(d,0) >= a(d,1) )
                return false;
        return true;
        }


    /* Gives a vec with the width in each dimension. */
    template<class T, size_t D> vec<T,D>
    dimensions( aabb<T,D> a )
        {
        return (column(a,1)-column(a,0));
        }


    /* Get the center point of the aabb. */
    template<class T,size_t D> vec<T,D>
    center( aabb<T,D> a )
        {
        return column(a,0) + dimensions(a)/T(2);
        }


    /* Moves the aabb by delta amount. */
    template<class T,size_t D> aabb<T,D>
    translate( const aabb<T,D> a, const vec<T,D>& delta )
        {
        return weld( column(a,0)+delta, column(a,1)+delta );
        }


    /* Puts the center of the AABB at point p. */
    template<class T,size_t D> aabb<T,D>
    place( const aabb<T,D> a, const vec<T,D>& p )
        {
        return translate( a, p-center(a) );
        }


    /* Get the width of a specific dimension. */
    template<class T,size_t D> T
    width( const aabb<T,D> a, size_t d )
        {
        return a(d,1) - a(d,0);
        }


    template<class T,size_t D> T
    volume( const aabb<T,D> a )
        {
        T vol = 1;
        auto size = dimensions(a);
        for( size_t d=0; d < D; ++d )
            vol *= size[d];
        return vol;
        }


    template<class T,size_t D> std::vector<vec<T,D>>
    corners( const aabb<T,D>& a )
        {
        std::vector<vec<T,D>> result;
        /* Corner count is 2^D. */
        size_t count = (1<<D);
        result.reserve(count);
        for( size_t i=0; i < count; ++i )
            {
            vec<T,D> p;
            /* There's a low and high plane for each
                dimension that makes out the cube.
                A bit is given for each dimension,
                0 for low, 1 for high. This makes out
                the index of the corner. */
            for( size_t d=0; d<D; ++d )
                /* Check the dimension's bit for high or low*/
                p[d] = i&(1<<d) ? a(d,0) : a(d,1);

            result.push_back(p);
            }
        return result;
        }


    template<class T, size_t D> bool
    is_normalized( const aabb<T,D>& a )
        {
        for( size_t d=0; d<D; ++d )
            if( a(d,0) > a(d,1) ) return false;
        return true;
        }


    // Create an aabb from two points and normalizes it.
    template<class T,size_t D> aabb<T,D>
    make_aabb( const vec<T,D> p0, const vec<T,D> p1 )
        {
        vec<T,D> min_p, max_p;
        for( size_t d=0; d<D; ++d )
            {
            min_p[d] = min(p0[d],p1[d]);
            max_p[d] = max(p0[d],p1[d]);
            }

        auto R = weld( min_p, max_p );

        TUMBO_ASSERT( is_normalized(R) && "Non normalized aabb" );
        return R;
        }


    /* Splits an aabb into 2^D smaller aabbs. */
    template<class T, size_t D> aabb_list<T,D>
    split( const aabb<T,D>& a, const vec<T,D>& p )
        {
        aabb_list<T,D> result;
        if( !contains( a, p ) ) return result;
        for( auto c : corners(a) )
            result.push_back( make_aabb( c, p ) );
        }


    /* Generates a list of new aabbs which cover the
        difference area a\b. */
    template<class T,size_t D> aabb_list<T,D>
    difference( aabb_list<T,D>& a, const aabb<T,D>& b )
        {
        aabb_list<T,D> result;
        // if A is subset of B, results in empty set
        if( contains( b, a ) )
            return result;
        // A and B are disjoint, A is unaffected.
        if( !overlaps( a, b ) )
            {
            result.push_back(a);
            return result;
            }
        /* TODO: This isn't right! If more than one of Bs corner is in A */
        for( auto c : corners(b) )
            result.push_back( split( a, c ) );
        /* Remove the one aabb that overlaps b. */
        result.remove(
            std::remove_if( begin(result), end(result),
                std::bind(contains,b) ),
            end(result) );
        }


    template<class T, size_t D> aabb<T,D>
    intersect( const aabb<T,D>& a, const aabb<T,D>& b )
        {
        auto result = aabb<T,D>::uniform(0);
        if( !overlaps(a,b) )
            return result;

        for( int d = 0; d < D; ++d )
            {
            result(d,0) = max( a(d,0), b(d,0) );
            result(d,1) = min( a(d,1), b(d,1) );
            }
        return result;
        }


    } // namespace tumbo

#endif // TUMBO_AABB_HPP