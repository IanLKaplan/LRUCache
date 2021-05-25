//
// Created by Ian L Kaplan, iank@bearcave.com, www.topstonesoftware.com, on 5/20/21.
//

#ifndef CACHE_CACHE_HPP
#define CACHE_CACHE_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;

/**
 *
 * @tparam K The type of the key for the LRU cache. This type must support a hash function for the unordered
 * cache that the keys and element pointers are stored in.
 * @tparam V The type of the LRU cache value associated with the key (e.g., a pair {K, V}).
 */
template<typename K, typename V>
class Cache {
public:
    /**
     * The Cache constructor
     * @param cacheSize the maximum number of elements to store in the cache.
     */
    explicit Cache(int cacheSize) : maxElements(cacheSize) {}
    ~Cache() = default;

    /**
     * Add an element to the LRU cache. If the cache size has been reached, the least recently
     * used element will be removed from the cache when this element is added.
     *
     * @param key the lookup key for the element stored in the cache.
     * @param value the value associated with the key.
     */
    void put(K key, V &value) {
        auto search = hashMap.find(key);
        if (search == hashMap.end()) { // the key was not found in the map, so add it
            if (hashMap.size() == maxSize()) {
                // remove from the last (oldest) element in the list
                K lastKey = doubleList.deleteLastElem();
                // remove from the hash map.
                hashMap.erase(lastKey);
            }
        } else { // The key was found
            // Update the value.  We don't want to require that the value object implements the == operator, so
            // the value is always updated.
            ListElem* currentElem = search->second;
            // remove the element from the LRU list
            doubleList.unlinkElem( currentElem );
            delete currentElem;
        }
        auto *newElem = new ListElem(key, value);
        doubleList.addToFront( newElem );
        hashMap[ key ] = newElem;
    }

    /**
     * If the key exists in the cache, return the key. The element has been referenced, so move to the
     * start of the LRU list (the start of the list being the most recently referenced item).
     *
     * @param key the key for the cache value
     * @return a pointer to the value stored in the cache, or nullptr if the {key, value} pair don't exist.
     */
    V* get(K key) {
        V* rsltPtr = nullptr;
        auto search = hashMap.find(key);
        if (search != hashMap.end()) {
            rsltPtr = &(search->second->value);
            ListElem* elemPtr = search->second;
            doubleList.moveToFront( elemPtr );
        }
        return rsltPtr;
    }

    /**
     * @return a vector of {key, value} pairs from the map. The vector is returned as
     * a unique_ptr() "smart pointer" reference.
     */
    unique_ptr<vector<pair<K,V>>> mapVector() {
        auto* mapValsPtr = new vector<pair<K,V>>();
        unique_ptr<vector<pair<K,V>>> mapVals(mapValsPtr );
        for (auto& mapElem : hashMap) {
            auto mapKey = mapElem.second->key;
            auto mapValue = mapElem.second->value;
            pair<K,V> mapPair = make_pair(mapKey, mapValue);
            mapValsPtr->push_back(mapPair);
        }
        return mapVals;
    }

    /**
     *
     * @return the maximum number of items that can be stored in the cache.
     */
    int maxSize() { return maxElements; }
    /**
     *
     * @return the number of elements currently stored in the cache
     */
    int size() { return hashMap.size(); }

private:
    struct ListElem {
        ListElem(K &keyVal, V &val) : key(keyVal), value(val) {}

        ~ListElem() = default;

        K key;
        V value;
        ListElem *prev = nullptr;
        ListElem *next = nullptr;
    };

    class DoubleList {
    public:
        DoubleList() = default;

        ~DoubleList() {
            // delete the doubly linked list
            while (tail != nullptr) {
                ListElem *prev = tail->prev;
                delete tail;
                tail = prev;
            }
        }

        /**
         * Remove a ListElem from the doubly linked list.
         * @param elem a pointer to the element to be removed.
         */
        void unlinkElem(ListElem *elem) {
            ListElem *prev = elem->prev;
            ListElem *next = elem->next;
            prev->next = next;
            if (next != nullptr) {
                next->prev = prev;
            }
            if (tail == elem) {
                tail = prev;
            }
            if (head == elem) {
                head = next;
            }
        }

        /**
         * Add a list element to the front of the list. This is the most recently element in the list.
         *
         * @param the list element to be added to the front of the list.
         */
        void addToFront(ListElem *elem) {
            if (head == nullptr) {
                tail = elem;
            } else {
                head->prev = elem;
            }
            elem->next = head;
            elem->prev = nullptr;
            head = elem;
        }

        void moveToFront(ListElem *elem) {
            if (head != elem) {
                unlinkElem( elem );
                addToFront( elem );
            }
        }

        /**
         * Remove an element from the end of the list.
         * @return the key value for the last element in the list.
         */
        K deleteLastElem() {
            ListElem *lastElem = tail;
            tail = tail->prev;
            tail->next = nullptr;
            lastElem->next = nullptr;
            lastElem->prev = nullptr;
            auto key = lastElem->key;
            delete lastElem;
            return key;
        }

    private:
        ListElem *head = nullptr;
        ListElem *tail = nullptr;
    };

    const int maxElements;
    unordered_map<K, ListElem *> hashMap;
    DoubleList doubleList;
};

#endif //CACHE_CACHE_HPP
