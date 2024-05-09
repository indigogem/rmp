#pragma once
#include "defs.h"
#include "eastl_defs.h"
#include "EASTL/vector.h"
#include "EASTL/fixed_vector.h"
#include "EASTL/array.h"
#include "EASTL/hash_map.h"

namespace kmp {
   
    template<typename T> using Vector = eastl::vector<T>;    
    template<typename T, size_t S> using InlineVector = eastl::fixed_vector<T, S, true>;
    template<typename T, size_t S> using Array = eastl::array<T, S>;

    using Blob = Vector<uint8_t>;

    template<typename K, typename V> using HashMap = eastl::hash_map<K, V, eastl::hash<K>, eastl::equal_to<K>>;

    template<typename K, typename V> using Pair = eastl::pair<K, V>;

    template<typename T> using Function = eastl::function<T>;

    //-------------------------------------------------------------------------
    // Simple utility functions to improve syntactic usage of container types
    //-------------------------------------------------------------------------

    // Find an element in a vector
    template<typename T>
    inline typename Vector<T>::const_iterator VectorFind(Vector<T> const& vector, T const& value) {
        return eastl::find(vector.begin(), vector.end(), value);
    }

    // Find an element in a vector
    // Usage: vectorFind( vector, value, [] ( T const& typeRef, V const& valueRef ) { ... } );
    template<typename T, typename V, typename Predicate>
    inline typename Vector<T>::const_iterator VectorFind(Vector<T> const& vector, V const& value, Predicate predicate) {
        return eastl::find(vector.begin(), vector.end(), value, eastl::forward<Predicate>(predicate));
    }

    // Find an element in a vector
    // Require non-const versions since we might want to modify the result
    template<typename T>
    inline typename Vector<T>::iterator VectorFind(Vector<T>& vector, T const& value) {
        return eastl::find(vector.begin(), vector.end(), value);
    }

    // Find an element in a vector
    // Usage: vectorFind( vector, value, [] ( T const& typeRef, V const& valueRef ) { ... } );
    // Require non-const versions since we might want to modify the result
    template<typename T, typename V, typename Predicate>
    inline typename Vector<T>::iterator VectorFind(Vector<T>& vector, V const& value, Predicate predicate) {
        return eastl::find(vector.begin(), vector.end(), value, eastl::forward<Predicate>(predicate));
    }

    template<typename T, typename V>
    inline bool VectorContains(Vector<T> const& vector, V const& value) {
        return eastl::find(vector.begin(), vector.end(), value) != vector.end();
    }

    // Usage: VectorContains( vector, value, [] ( T const& typeRef, V const& valueRef ) { ... } );
    template<typename T, typename V, typename Predicate>
    inline bool VectorContains(Vector<T> const& vector, V const& value, Predicate predicate) {
        return eastl::find(vector.begin(), vector.end(), value, eastl::forward<Predicate>(predicate)) != vector.end();
    }

    // Usage: VectorContains( vector, [] ( T const& typeRef ) { ... } );
    template<typename T, typename V, typename Predicate>
    inline bool VectorContains(Vector<T> const& vector, Predicate predicate) {
        return eastl::find_if(vector.begin(), vector.end(), eastl::forward<Predicate>(predicate)) != vector.end();
    }

    template<typename T>
    inline int32_t VectorFindIndex(Vector<T> const& vector, T const& value) {
        auto iter = eastl::find(vector.begin(), vector.end(), value);
        if (iter == vector.end()) {
            return InvalidIndex;
        } else {
            return (int32_t)(iter - vector.begin());
        }
    }

    // Usage: VectorFindIndex( vector, value, [] ( T const& typeRef, V const& valueRef ) { ... } );
    template<typename T, typename V, typename Predicate>
    inline int32_t VectorFindIndex(Vector<T> const& vector, V const& value, Predicate predicate) {
        auto iter = eastl::find(vector.begin(), vector.end(), value, predicate);
        if (iter == vector.end()) {
            return InvalidIndex;
        } else {
            return (int32_t)(iter - vector.begin());
        }
    }

    // Usage: VectorContains( vector, [] ( T const& typeRef ) { ... } );
    template<typename T, typename V, typename Predicate>
    inline int32_t VectorFindIndex(Vector<T> const& vector, Predicate predicate) {
        auto iter = eastl::find_if(vector.begin(), vector.end(), predicate);
        if (iter == vector.end()) {
            return InvalidIndex;
        } else {
            return (int32_t)(iter - vector.begin());
        }
    }

    //-------------------------------------------------------------------------

    template<typename T, typename V, eastl_size_t S>
    inline bool VectorContains(InlineVector<T, S> const& vector, V const& value) {
        return eastl::find(vector.begin(), vector.end(), value) != vector.end();
    }

    template<typename T, eastl_size_t S, typename V, typename Predicate>
    inline bool VectorContains(InlineVector<T, S> const& vector, V const& value, Predicate predicate) {
        return eastl::find(vector.begin(), vector.end(), value, eastl::forward<Predicate>(predicate)) != vector.end();
    }

    // Find an element in a vector
    template<typename T, typename V, eastl_size_t S>
    inline typename InlineVector<T, S>::const_iterator VectorFind(InlineVector<T, S> const& vector, V const& value) {
        return eastl::find(vector.begin(), vector.end(), value);
    }

    // Find an element in a vector
    template<typename T, typename V, eastl_size_t S, typename Predicate>
    inline typename InlineVector<T, S>::const_iterator VectorFind(InlineVector<T, S> const& vector, V const& value, Predicate predicate) {
        return eastl::find(vector.begin(), vector.end(), value, eastl::forward<Predicate>(predicate));
    }

    // Find an element in a vector
    // Require non-const versions since we might want to modify the result
    template<typename T, typename V, eastl_size_t S>
    inline typename InlineVector<T, S>::iterator VectorFind(InlineVector<T, S>& vector, V const& value) {
        return eastl::find(vector.begin(), vector.end(), value);
    }

    // Find an element in a vector
    // Require non-const versions since we might want to modify the result
    template<typename T, typename V, eastl_size_t S, typename Predicate>
    inline typename InlineVector<T, S>::iterator VectorFind(InlineVector<T, S>& vector, V const& value, Predicate predicate) {
        return eastl::find(vector.begin(), vector.end(), value, eastl::forward<Predicate>(predicate));
    }

    template<typename T, typename V, eastl_size_t S>
    inline int32_t VectorFindIndex(InlineVector<T, S> const& vector, V const& value) {
        auto iter = eastl::find(vector.begin(), vector.end(), value);
        if (iter == vector.end()) {
            return InvalidIndex;
        } else {
            return (int32_t)(iter - vector.begin());
        }
    }

    template<typename T, typename V, eastl_size_t S, typename Predicate>
    inline int32_t VectorFindIndex(InlineVector<T, S> const& vector, V const& value, Predicate predicate) {
        auto iter = eastl::find(vector.begin(), vector.end(), value, predicate);
        if (iter == vector.end()) {
            return InvalidIndex;
        } else {
            return (int32_t)(iter - vector.begin());
        }
    }

    //-------------------------------------------------------------------------

    template<typename T>
    inline void VectorEmplaceBackUnique(Vector<T>& vector, T&& item) {
        if (!VectorContains(vector, item)) {
            vector.emplace_back(item);
        }
    }

    template<typename T>
    inline void VectorEmplaceBackUnique(Vector<T>& vector, T const& item) {
        if (!VectorContains(vector, item)) {
            vector.emplace_back(item);
        }
    }

    template<typename T, eastl_size_t S>
    inline void VectorEmplaceBackUnique(InlineVector<T, S>& vector, T&& item) {
        if (!VectorContains(vector, item)) {
            vector.emplace_back(item);
        }
    }

    template<typename T, eastl_size_t S>
    inline void VectorEmplaceBackUnique(InlineVector<T, S>& vector, T const& item) {
        if (!VectorContains(vector, item)) {
            vector.emplace_back(item);
        }
    }

    

}


