#pragma once

namespace LC
{
    template <typename K, typename V>
    struct Pair
    {
        K* key;
        V* value;

        Pair() {};
        inline Pair(const Pair<K, V>& pr) { key = pr.key; value = pr.value; }
        inline Pair(const K& a, const V& b) { key = a; value = b; }
        ~Pair() {};

        operator==(const Pair<K, V>& pr)
        {
            if (pr.key == key && pr.value == value)
            {
                return true;
            }
            else
            {
                return false
            }
        }
        //can only compare by key not value
        operator==(const K& pr)
        {
            if (pr == key)
            {
                return true;
            }
            else
            {
                return false
            }
        }
    };
}