#include <iostream>

#define TABLE_SIZE 1000

template <typename K, typename V>
class HashNode {
private:

    // key-value pair
    K key;
    V value;

    // next bucket with the same key
    HashNode *next;

public:
    HashNode(const K& key, const V& value): key(key), value(value), next(NULL) { }

    K getKey() const {
        return key;
    }

    V getValue() const {
        return value;
    }

    void setValue(V value) {
        HashNode::value = value;
    }

    HashNode* getNext() const {
        return next;
    }

    void setNext(HashNode* next) {
        HashNode::next = next;
    }
};

template <typename K>
struct KeyHash {
    unsigned long operator()(const K& key) const
    {
        return static_cast<unsigned long>(key) % TABLE_SIZE;
    }
};

template <typename K, typename V, typename F = KeyHash<K>>
class HashMap {
private:
    HashNode<K, V>** table;
    F hashFunc;

public:
    HashMap() {
        // construct zero initialized hash table of size
        table = new HashNode<K, V>*[TABLE_SIZE]();
    }

    ~HashMap() {
        // destroy all buckets one by one
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode<K, V> *entry = table[i];
            while (entry != NULL) {
                HashNode<K, V> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
        // destroy the hash table
        delete [] table;
    }

    bool get(const K &key, V &value) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL) {
            if (entry->getKey() == key) {
                value = entry->getValue();
                return true;
            }
            entry = entry->getNext();
        }
        return false;
    }

    void put(const K &key, const V &value) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = NULL;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            entry = new HashNode<K, V>(key, value);
            if (prev == NULL) {
                // insert as first bucket
                table[hashValue] = entry;
            } else {
                prev->setNext(entry);
            }
        } else {
            // just update the value
            entry->setValue(value);
        }
    }

    void remove(const K &key) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = NULL;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            // key not found
            return;
        }
        else {
            if (prev == NULL) {
                // remove first bucket of the list
                table[hashValue] = entry->getNext();
            } else {
                prev->setNext(entry->getNext());
            }
            delete entry;
        }
    }
};

struct MyKeyHash {
    unsigned long operator()(const int& k) const
    {
        return k % 10;
    }
};

int main(int argc, char* argv[]) {

    // HashMap<int, std::string, MyKeyHash> hmap;
    HashMap<int, std::string> hmap;
    hmap.put(1, "val1");
    hmap.put(2, "val2");
    hmap.put(3, "val3");

    std::string value;
    hmap.get(2, value);
    std::cout << value << std::endl;
    
    return 0;
}