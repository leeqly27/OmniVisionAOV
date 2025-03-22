#pragma once

#include "Includes2.h"

namespace Cheat {
	void CheatAttach();
	
	void *CheatGetImageByName(const char *image);
	void *CheatGetClassType(const char *image, const char *namespaze, const char *clazz);
	
	void CheatGetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output);
	void CheatSetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value);
	
	void *CheatGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount);
	size_t CheatGetFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name);

}

template<typename T> struct CheatArray {
    void *klass;
    void *monitor;
    void *bounds;
    int max_length;
    T m_Items[65535];

    int getLength() {
        return max_length;
    }

    T *getPointer() {
        return (T *)m_Items;
    }

    T &operator[](int i) {
        return m_Items[i];
    }

    T &operator[](int i) const {
        return m_Items[i];
    }
};

template<typename T>
using ArrayCt = CheatArray<T>;

struct CheatString {
    void *klass;
    void *monitor;
    int32_t length;
    uint16_t start_char;

    const char *CString();

    const wchar_t *WCString();

    static CheatString *Create(const char *s);
    static CheatString *Create(const wchar_t *s, int len);

    int getLength() {
        return length;
    }
};

typedef CheatString StringCt;

template<typename T> struct CheatList {
    void *klass;
    void *unk1;
    CheatArray<T> *items;
    int size;
    int version;

    T *getItems() {
        return items->getPointer();
    }

    int getSize() {
        return size;
    }

    int getVersion() {
        return version;
    }

    T &operator[](int i) {
        return items->m_Items[i];
    }

    T &operator[](int i) const {
        return items->m_Items[i];
    }
};

template<typename T>
using ListCt = CheatList<T>;

template<typename K, typename V> struct CheatDictionary {
    void *klass;
    void *unk1;
    CheatArray<int **> *table;
    CheatArray<void **> *linkSlots;
    CheatArray<K> *keys;
    CheatArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K *getKeys() {
        return keys->getPointer();
    }

    V *getValues() {
        return values->getPointer();
    }

    int getNumKeys() {
        return keys->getLength();
    }

    int getNumValues() {
        return values->getLength();
    }

    int getSize() {
        return size;
    }
};

template<typename K, typename V>
using DictionaryCt = CheatDictionary<K, V>;
