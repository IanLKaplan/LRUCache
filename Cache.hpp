//
// Created by iank on 5/20/21.
//

#ifndef CACHE_CACHE_HPP
#define CACHE_CACHE_HPP

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

template<typename K, typename V>
class Cache {
public:
    explicit Cache(int cacheSize) { maxElements = cacheSize; }
    ~Cache() = default;

    void put(K &key, V &value) {
        auto search = hashMap.find(key);
        if (search == hashMap.end()) { // the key was not found in the map, so add it
            if (hashMap.size() == maxSize()) {
                // remove from the last (oldest) element in the list
                K lastKey = doubleList.deleteLastElem();
                // remove from the hash map.
                hashMap.erase(lastKey);
            }
            auto *newElem = new ListElem(key, value);
            doubleList.addToFront(newElem);
            hashMap.insert({key, newElem});
        } else { // The key was found
            // if element is not at the start of the list, move it to the start
            if (doubleList.getHead()->key != key) {
                ListElem *foundElemPtr = search->second;
                doubleList.unlinkElem(foundElemPtr);
                doubleList.addToFront(foundElemPtr);
            }
        }
    }

    V &get(K &key) {
        auto search = hashMap.find(key);
        if (search != hashMap.end) {
            return search->first->value;
        } else {
            return nullptr;
        }
    }

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

    int maxSize() { return maxElements; }
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

        ListElem *getHead() { return head; }

    private:
        ListElem *head = nullptr;
        ListElem *tail = nullptr;
    };

    int maxElements;
    unordered_map<K, ListElem *> hashMap;
    DoubleList doubleList;
};

#endif //CACHE_CACHE_HPP
