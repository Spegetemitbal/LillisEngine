#pragma once
#include <string>
#include "LCList.h"
#include "LCPair.h"
#include <optional>

namespace LC
{
#define DEFAULT_CAPACITY 10
#define MAX_LOAD_FACTOR 0.7

    //Supports strings only as key value.
    template <typename V>
    class HashTable {

    public:
        // Initialize the array with a starting capacity
        HashTable(int cap = DEFAULT_CAPACITY) {
            if (cap < 1) { cap = 1; } // cannot have < 1 capacity
            resize(cap);
        }

        // Erase the array
        ~HashTable() {
            for (LinkedList<Pair<std::string, V>> list : backingStore)
            {
                Uint32 size = list.getQuantity();
                for (; size > 1; size--)
                {
                    LinkedList<Pair<std::string, V>>* iter = list;
                    for (Uint32 i = 1; i < size - 1; i++)
                    {
                        iter = iter->GetNext();
                    }
                    delete iter->GetNext();
                }
            }
            
            delete[] backingStore;
        }

        void put(const std::string key, const V value) {

            optional<V> temp = get(key);
            size_t index = hashKey(key);
            LinkedList<Pair<std::string, V>>& located_index = backingStore[index];

            if (temp)
            {
                //replace item
                auto hashed = new Pair<std::string, V>(key, value);
                LinkedList<Pair<std::string, V>>& current = located_index;
                LinkedList<Pair<std::string, V>>& located = nullptr;
                while (current.GetNext() != nullptr)
                {
                    current = current.GetNext();
                    Pair<std::string, V> replace = current.GetItem();
                    if (replace.key == key)
                    {
                        current.SetItem(hashed);
                        break;
                    }
                }
            }
            else
            {
                auto hashed = new Pair<std::string, V>(key, value);
                if (backingStore[index] == nullptr)
                {
                    //add new value to empty slot
                    backingStore[index] = new LinkedList<Pair<std::string, V>>(hashed, nullptr);
                }
                else
                {
                    //add new value to full slot
                    auto hashed = new Pair<std::string, V>(key, value);
                    LinkedList<Pair<std::string, V>>* current = located_index;
                    while (current->GetNext() != nullptr)
                    {
                        current = current->GetNext();
                    }
                    LinkedList<Pair<std::string, V>>* insert = new LinkedList<Pair<std::string, V>>(hashed, nullptr);
                    current->SetNext(insert);
                }
                count++;
            }

            if (getLoadFactor() > MAX_LOAD_FACTOR)
            {
                resize(capacity * growthFactor);
            }
        }

        optional<V> get(const std::string& key) {
            size_t index = hashKey(key);
            LinkedList<Pair<std::string, V>>& located_index = backingStore[index];

            if (located_index != nullptr)
            {
                LinkedList<Pair<std::string, V>>* current = located_index;
                while (current->GetNext() != nullptr)
                {
                    current = current->GetNext();
                    Pair<std::string, V> iter = current->GetItem();
                    if (iter.key == key)
                    {
                        return iter;
                    }
                }
            }

            return {};
        }

        void remove(const std::string& key)
        {
            optional<V> temp = get(key);

            if (temp)
            {
                size_t index = hashKey(key);
                LinkedList<Pair<std::string, V>>& located_index = backingStore[index];
                Pair<std::string, V> hashed(key, *temp);

                LinkedList<Pair<std::string, V>>* current = located_index;
                LinkedList<Pair<std::string, V>>* previous = nullptr;
                while (current->GetNext() != nullptr)
                {
                    previous = current;
                    current = current->GetNext();
                    Pair<std::string, V> iter = current->GetItem();
                    if (iter.key == key)
                    {
                        previous->SetNext(current->GetNext());
                        delete current;
                        break;
                    }
                }
                
                count--;
            }
        }

        // Calculate and return the load factor
        float getLoadFactor() {
            return ((float)count) / ((float)capacity);
        }

        // Get the count
        int getCount() {
            return count;
        }

        // Get the capacity
        int getCapacity() {
            return capacity;
        }

    private:
        Uint32 capacity = 0;
        Uint32 growthFactor = 2;
        Uint32 count = 0;
        LinkedList<Pair<std::string, V>>* backingStore = nullptr;

        // Shift all of the items in backingStore into a
        // new backing store of size cap, or create
        // the backingStore for the first time
        void resize(int cap)
        {

            if (cap == 0)
            {
                backingStore = new LinkedList<Pair<std::string, V>>[DEFAULT_CAPACITY];
                capacity = DEFAULT_CAPACITY;
            }
            else
            {
                LinkedList<Pair<std::string, V>>* auxilliaryArray = new LinkedList<Pair<std::string, V>>[cap];

                for (int i = 0; i < capacity; i++)
                {
                    if (backingStore[i].size() > 0)
                    {
                        for (auto& p : backingStore[i]) {
                            size_t index = hashKey(p.key) % cap;
                            auxilliaryArray[index].push_back(p);
                        }
                    }
                    //move each list to a new location.
                }
                capacity = cap;
                backingStore = auxilliaryArray;
            }
        }

        size_t hashKey(const std::string& key) {
            int sum = 0;
            for (int k = 0; k < key.length(); k++)
                sum = sum + int(key[k]);
            return  sum % capacity;
        }
};
}