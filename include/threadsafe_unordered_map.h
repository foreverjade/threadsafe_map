/*
 * threadsafe_unordered_map.h
 *
 *  Created on: 2016-07-26
 *  Original Author: guyadong
 *  Original source: https://blog.csdn.net/10km/article/details/52072061?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522e14a5ad7004bb8aacb36be966ae1229f%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=e14a5ad7004bb8aacb36be966ae1229f&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~rank_v31_ecpm-6-52072061-null-null.nonecase&utm_term=RAII&spm=1018.2226.3001.4450
 *  Modified by: Haoran
 */

#ifndef COMMON_SOURCE_CPP_THREADSAFE_UNORDERED_MAP_H_
#define COMMON_SOURCE_CPP_THREADSAFE_UNORDERED_MAP_H_
#include <unordered_map>
#include <memory>
#include <utility>
#include "RWLock.h"
#include <functional>

namespace std {
/*
 * Based on std::unordered_map to implement thread-safe map
 * delete copy constructor
 * delete copy assignment operator
 * allow move constructor
 * delete move assignment operator
 * */
template<typename _Key, typename _Tp,
    typename _Hash = std::hash<_Key>,
    typename _Pred = std::equal_to<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class threadsafe_unordered_map{
private:
    std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc> map;
    mutable RWLock lock;
public:
    using map_type=std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>;
    using key_type=typename map_type::key_type;
    using mapped_type=typename map_type::mapped_type;
    using value_type=typename map_type::value_type;
    using hasher=typename map_type::hasher;
    using key_equal=typename map_type::key_equal;
    using allocator_type=typename map_type::allocator_type;
    using reference=typename map_type::reference;
    using const_reference=typename map_type::const_reference;
    using pointer=typename map_type::pointer;
    using const_pointer=typename map_type::const_pointer;
    using iterator=typename map_type::iterator;
    using const_iterator=typename map_type::const_iterator;
    using local_iterator=typename map_type::local_iterator;
    using const_local_iterator=typename map_type::const_local_iterator;
    using size_type=typename map_type::size_type;
    using difference_type=typename map_type::difference_type;


    threadsafe_unordered_map()=default;
    threadsafe_unordered_map(const threadsafe_unordered_map&)=delete;
    threadsafe_unordered_map(threadsafe_unordered_map&&)=default;
    threadsafe_unordered_map& operator=(const threadsafe_unordered_map&)=delete;
    threadsafe_unordered_map& operator=(threadsafe_unordered_map&&)=delete;
    explicit threadsafe_unordered_map(size_type __n,
                const hasher& __hf = hasher(),
                const key_equal& __eql = key_equal(),
                const allocator_type& __a = allocator_type()):map(__n,__hf,__eql,__a){}
    template<typename _InputIterator>
    threadsafe_unordered_map(_InputIterator __first, _InputIterator __last,
                  size_type __n = 0,
                  const hasher& __hf = hasher(),
                  const key_equal& __eql = key_equal(),
                  const allocator_type& __a = allocator_type()):map(__first,__last,__n,__hf,__eql,__a){}
    threadsafe_unordered_map(const map_type&v): map(v){}
    threadsafe_unordered_map(map_type&&rv):map(std::move(rv)){}
    explicit
    threadsafe_unordered_map(const allocator_type& __a):map(__a){}
    threadsafe_unordered_map(const map_type& __umap,
                const allocator_type& __a):map(__umap,__a){}
    threadsafe_unordered_map(map_type&& __umap,
                const allocator_type& __a):map(std::move(__umap),__a){}
    threadsafe_unordered_map(std::initializer_list<value_type> __l,
                size_type __n = 0,
                const hasher& __hf = hasher(),
                const key_equal& __eql = key_equal(),
                const allocator_type& __a = allocator_type()):map(__l,__n,__hf,__eql,__a){}
    threadsafe_unordered_map(size_type __n, const allocator_type& __a)
          : threadsafe_unordered_map(__n, hasher(), key_equal(), __a){}
    threadsafe_unordered_map(size_type __n, const hasher& __hf,
                const allocator_type& __a)
          : threadsafe_unordered_map(__n, __hf, key_equal(), __a){}
    template<typename _InputIterator>
    threadsafe_unordered_map(_InputIterator __first, _InputIterator __last,
                  size_type __n,
                  const allocator_type& __a):map(__first,__last,__n,__a){}
    template<typename _InputIterator>
    threadsafe_unordered_map(_InputIterator __first, _InputIterator __last,
                  size_type __n, const hasher& __hf,
                  const allocator_type& __a)
          : threadsafe_unordered_map(__first, __last, __n, __hf, key_equal(), __a){}
    threadsafe_unordered_map(std::initializer_list<value_type> __l,
                size_type __n,
                const allocator_type& __a)
          : threadsafe_unordered_map(__l, __n, hasher(), key_equal(), __a){}
    threadsafe_unordered_map(std::initializer_list<value_type> __l,
                size_type __n, const hasher& __hf,
                const allocator_type& __a)
          : threadsafe_unordered_map(__l, __n, __hf, key_equal(), __a){}
    bool  empty() const noexcept{
        auto guard=lock.read_guard();
        return map.empty();
    }
    size_type size() const noexcept{
        auto guard=lock.read_guard();
        return map.size();
    }
    size_type  max_size() const noexcept{
        auto guard=lock.read_guard();
        return map.max_size();
    }
     iterator begin() noexcept{
         auto guard=lock.write_guard();
         return map.begin();
     }
     const_iterator begin() const noexcept{
         auto guard=lock.read_guard();
         return map.begin();
     }
     const_iterator cbegin() const noexcept{
         auto guard=lock.read_guard();
        return map.cbegin();
     }
     iterator end() noexcept{
         auto guard=lock.write_guard();
         return map.end();
     }
     const_iterator end() const noexcept{
         auto guard=lock.read_guard();
         return map.end();
     }
     const_iterator cend() const noexcept{
         auto guard=lock.read_guard();
         return map.cend();
     }
     template<typename... _Args>
        std::pair<iterator, bool>
        emplace(_Args&&... __args){
         auto guard=lock.write_guard();
         return map.emplace(std::forward<_Args>(__args)...);
     }
     template<typename... _Args>
    iterator
    emplace_hint(const_iterator __pos, _Args&&... __args){
         auto guard=lock.write_guard();
         return map.emplace_hint(__pos, std::forward<_Args>(__args)...);
     }
     std::pair<iterator, bool> insert(const value_type& __x){
         auto guard=lock.write_guard();
         return map.insert(__x);
     }
     template<typename _Pair, typename = typename
               std::enable_if<std::is_constructible<value_type,
                                _Pair&&>::value>::type>
        std::pair<iterator, bool>
        insert(_Pair&& __x){
         auto guard=lock.write_guard();
         return map.insert(std::forward<_Pair>(__x));
     }
     iterator
     insert(const_iterator __hint, const value_type& __x) {
         auto guard=lock.write_guard();
         return map.insert(__hint, __x);
     }
     template<typename _Pair, typename = typename
               std::enable_if<std::is_constructible<value_type,
                                _Pair&&>::value>::type>
    iterator
    insert(const_iterator __hint, _Pair&& __x){
        auto guard=lock.write_guard();
        return map.insert(__hint, std::forward<_Pair>(__x));
    }
    template<typename _InputIterator>
    void
    insert(_InputIterator __first, _InputIterator __last){
        auto guard=lock.write_guard();
        map.insert(__first, __last);
    }
    void insert(std::initializer_list<value_type> __l){
        auto guard=lock.write_guard();
        map.insert(__l);
    }
    iterator  erase(const_iterator __position){
        auto guard=lock.write_guard();
        return map.erase(__position);
    }
    iterator erase(iterator __position){
        auto guard=lock.write_guard();
        return map.erase(__position);
    }
    size_type erase(const key_type& __x){
        auto guard=lock.write_guard();
        return map.erase(__x);
    }
    iterator erase(const_iterator __first, const_iterator __last){
        auto guard=lock.write_guard();
        return map.erase(__first, __last);
    }
    void clear() noexcept{
        auto guard=lock.write_guard();
        map.clear();
    }
    void swap(map_type& __x) noexcept( noexcept(map.swap(__x._M_h)) ){
        auto guard=lock.write_guard();
        map.swap(__x._M_h);
    }
    hasher hash_function() const{
        auto guard=lock.read_guard();
        return map.hash_function();
    }
    key_equal key_eq() const{
        auto guard=lock.read_guard();
        return map.key_eq();
    }
    iterator find(const key_type& __x){
        auto guard=lock.write_guard();
        return map.find(__x);
    }
    const_iterator find(const key_type& __x) const{
        auto guard=lock.read_guard();
        return map.find(__x);
    }
    size_type count(const key_type& __x) const {
        auto guard=lock.read_guard();
        return map.count(__x);
    }
    std::pair<iterator, iterator> equal_range(const key_type& __x){
        auto guard=lock.write_guard();
        return map.equal_range(__x);
    }
    std::pair<const_iterator, const_iterator>
    equal_range(const key_type& __x) const{
        auto guard=lock.read_guard();
        return map.equal_range(__x);
    }
    mapped_type& operator[](const key_type& __k){
        auto guard=lock.write_guard();
        return map[__k];
    }
    mapped_type& operator[](key_type&& __k){
        auto guard=lock.write_guard();
        return map[std::move(__k)];
    }
    mapped_type& at(const key_type& __k){
        auto guard=lock.write_guard();
        return map.at(__k);
    }
    const mapped_type& at(const key_type& __k) const{
        auto guard=lock.read_guard();
        return map.at(__k);
    }
    size_type bucket_count() const noexcept{
        auto guard=lock.read_guard();
        return map.bucket_count();
    }

    size_type max_bucket_count() const noexcept{
        auto guard=lock.read_guard();
        return map.max_bucket_count();
    }
    size_type bucket_size(size_type __n) const{
        auto guard=lock.read_guard();
        return map.bucket_size(__n);
    }
    size_type bucket(const key_type& __key) const{
        auto guard=lock.read_guard();
        return map.bucket(__key);
    }
    local_iterator  begin(size_type __n) {
        auto guard=lock.write_guard();
        return map.begin(__n);
    }
    const_local_iterator  begin(size_type __n) const {
        auto guard=lock.read_guard();
        return map.begin(__n);
    }
    const_local_iterator cbegin(size_type __n) const{
        auto guard=lock.read_guard();
        return map.cbegin(__n);
    }
    local_iterator end(size_type __n) {
        auto guard=lock.write_guard();
        return map.end(__n);
    }
    const_local_iterator end(size_type __n) const{
        auto guard=lock.read_guard();
        return map.end(__n);
    }
    const_local_iterator cend(size_type __n) const{
        auto guard=lock.read_guard();
        return map.cend(__n);
    }
    float load_factor() const noexcept{
        auto guard=lock.read_guard();
        return map.load_factor();
    }
    float max_load_factor() const noexcept{
        auto guard=lock.read_guard();
        return map.max_load_factor();
    }
    void max_load_factor(float __z){
        auto guard=lock.write_guard();
        map.max_load_factor(__z);
    }
    void rehash(size_type __n){
        auto guard=lock.write_guard();
        map.rehash(__n);
    }
    void reserve(size_type __n){
        auto guard=lock.write_guard();
        map.reserve(__n);
    }
    /*
     * New Function, bool value indicator if found
     * Return true, value is set to the found value
     * */
    bool find(const key_type& __x, mapped_type &value) const{
        auto guard=lock.read_guard();
        auto itor=map.find(__x);
        auto found=itor!=map.end();
        if(found)
            value=itor->second;
        return found;
    }
    bool key_exists(const key_type& __x) const{
        auto guard=lock.read_guard();
        return map.find(__x)!=map.end();
    }
    /*
     * New Function, return RAII object of read lock
     * Should be called before read operation on map
     * */
    gyd::raii read_guard()const noexcept{
        return lock.read_guard();
    }
    /*
     * New Function, return RAII object of write lock
     * Should be called before write operation on map
     * */
    gyd::raii write_guard()noexcept{
        return lock.write_guard();
    }
    /*
     * New Function
     * If the specified key does not exist, add key->value mapping
     * If the specified key exists, return the value of the key mapping, otherwise return value
     * */
    mapped_type insertIfAbsent(const key_type& key,const mapped_type &value){
        auto guard=lock.write_guard();
        auto itor=map.find(key);
        if (itor==map.end()){
            map.insert(value_type(key, value));
            return value;
        }
        return itor->second;
    }
    /*
     * New Function
     * If the specified key exists, use value to replace the value of the key mapping
     * If the specified key does not exist, return nullptr
     * */
    std::shared_ptr<mapped_type> replace(const key_type& key,const mapped_type &value){
        auto guard=lock.write_guard();
        if (map.find(key)!=map.end()){
            map.insert(value_type(key, value));
            return std::make_shared<mapped_type>(value);
        }
        return std::shared_ptr<mapped_type>();
    }
    /*
     * New Function
     * If there is a key->value mapping, use newValue to replace the value of the key mapping, return true
     * Otherwise return false
     * */
    bool replace(const key_type& key,const mapped_type &value,const mapped_type &newValue){
        auto guard=lock.write_guard();
        auto itor=map.find(key);
        if (itor!=map.end()&&itor->second==value){
            map.insert(value_type(key, newValue));
            return true;
        }
        return false;
    }
    /*
     * New Function
     * Safe read traversal interface
     * */
    void forEach(std::function<void(const _Key&, const _Tp&)> func) const {
        auto guard=lock.read_guard();
        for (const auto& pair : map) {
            func(pair.first, pair.second);
        }
    }
    template<typename _Key1, typename _Tp1, typename _Hash1, typename _Pred1,
           typename _Alloc1>
      friend bool
    operator==(const threadsafe_unordered_map<_Key1, _Tp1, _Hash1, _Pred1, _Alloc1>&,
         const threadsafe_unordered_map<_Key1, _Tp1, _Hash1, _Pred1, _Alloc1>&);
};
template<class _Key, class _Tp, class _Hash, class _Pred, class _Alloc>
inline bool
operator==(const threadsafe_unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __x,
           const threadsafe_unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __y)
{
    auto guardx=__x.lock.read_guard();
    auto guardy=__y.lock.read_guard();
    return __x.map._M_equal(__y.map);
}
template<class _Key, class _Tp, class _Hash, class _Pred, class _Alloc>
inline bool
operator!=(const threadsafe_unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __x,
           const threadsafe_unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __y)
{
    auto guardx=__x.lock.read_guard();
    auto guardy=__y.lock.read_guard();
    return !(__x == __y);
}
}/* namespace std */
#endif /* COMMON_SOURCE_CPP_THREADSAFE_UNORDERED_MAP_H_ */