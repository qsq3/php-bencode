#ifndef BENCODE_HEADER_BLIST
#define BENCODE_HEADER_BLIST

extern "C" {
#include "php.h"
#include "zend_exceptions.h"
}

#include <string>
#include "bitem.h"

class blist : public bitem {
    public:
        HashTable *_data;

        blist();
        blist(const blist *that);
        ~blist();

        std::string get_type() const;
        zval * get(const size_t &key) const;
        bool has(const size_t &key) const;
        void add(zval *value);
        void set(const size_t &key, zval *value);
        bool del(const size_t &key);
        zval * get_path(const std::string &key, size_t &pt) const;
        size_t length() const;
        size_t count() const;

        static zval * parse(const std::string &ben, size_t &pt);
        std::string encode() const;
        zval * to_array(const bool include_meta) const;
};

#endif
