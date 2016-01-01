#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php_bencode.h"
#include "binit.h"

/**** BITEM *****/
PHP_METHOD(bitem, __construct)
{
    // block instantiating bitem
    // TODO: find a programmatic way to do this
    RETURN_NULL();
}
PHP_METHOD(bitem, parse)
{
    char *ben;
    size_t ben_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    std::string ben_str(ben);
    RETURN_ZVAL(bitem::parse(ben_str), 1, 1);
}
PHP_METHOD(bitem, load)
{
    char *file_path;
    size_t file_path_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &file_path_len) == FAILURE) {
        RETURN_NULL();
    }
    std::string file_path_str(file_path);
    RETURN_ZVAL(bitem::load(file_path_str), 1, 1);
}
PHP_METHOD(bitem, save)
{
    char *file_path;
    size_t file_path_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &file_path_len) == FAILURE) {
        RETURN_NULL();
    }
    std::string file_path_str(file_path);
    std::string class_name = zend_container::bnode_object_get_class_name(getThis());
    if (class_name == "bdict") {
        bdict_object *intern = Z_BDICT_OBJ_P(getThis());
        intern->bdict_data->save(file_path_str);
        RETURN_TRUE;
    } else if (class_name == "blist") {
        blist_object *intern = Z_BLIST_OBJ_P(getThis());
        intern->blist_data->save(file_path_str);
        RETURN_TRUE;
    } else if (class_name == "bstr") {
        bstr_object *intern = Z_BSTR_OBJ_P(getThis());
        intern->bstr_data->save(file_path_str);
        RETURN_TRUE;
    } else if (class_name == "bint") {
        bint_object *intern = Z_BINT_OBJ_P(getThis());
        intern->bint_data->save(file_path_str);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}
static zend_function_entry bitem_methods[] = {
    PHP_ME(bitem, __construct,          NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bitem, parse,                NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bitem, load,                 NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bitem, save,                 NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** BDICT *****/
PHP_METHOD(bdict, __construct)
{
    bdict *bnode = NULL;
    bnode = new bdict();
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    intern->bdict_data = bnode;
}
PHP_METHOD(bdict, get_type)
{
    std::string result;
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    result = intern->bdict_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bdict, get)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
            RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    RETURN_ZVAL(intern->bdict_data->get(_key), 1, 0);
}
PHP_METHOD(bdict, get_copy)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
            RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    RETURN_OBJ(zend_container::bnode_object_clone(intern->bdict_data->get(_key)));
}
PHP_METHOD(bdict, set)
{
    char *key;
    size_t key_len = 0;
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so", &key, &key_len, &zv) == FAILURE) {
            RETURN_FALSE;
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    intern->bdict_data->set(_key, zv);
    RETURN_TRUE;
}
PHP_METHOD(bdict, has)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
            RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    RETURN_BOOL(intern->bdict_data->has(_key));
}
PHP_METHOD(bdict, del)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
            RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    RETURN_BOOL(intern->bdict_data->del(_key));
}
PHP_METHOD(bdict, length)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    size_t result = intern->bdict_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bdict, count)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    size_t result = intern->bdict_data->count();
    RETURN_LONG(result);
}
PHP_METHOD(bdict, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len > 0) RETURN_NULL();
    std::string tmp(ben);
    size_t pt = 0;
    RETURN_ZVAL(bdict::parse(tmp, pt), 1, 1);
}
PHP_METHOD(bdict, encode)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string result = intern->bdict_data->encode();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bdict, to_array)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    RETURN_ZVAL(intern->bdict_data->to_array(false), 1, 1);
}
PHP_METHOD(bdict, to_meta_array)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    RETURN_ZVAL(intern->bdict_data->to_array(true), 1, 1);
}
PHP_METHOD(bdict, __toString)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string result = intern->bdict_data->encode();
    RETURN_STRING(result.c_str());
}
static zend_function_entry bdict_methods[] = {
    PHP_ME(bdict, __construct,          NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bdict, get_type,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get_copy,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, set,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, has,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, del,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, length,               NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, count,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, parse,                NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bdict, encode,               NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, to_array,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, to_meta_array,        NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, __toString,           NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** BLIST *****/
PHP_METHOD(blist, __construct)
{
    blist *bnode = NULL;
    bnode = new blist();
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    intern->blist_data = bnode;
}
PHP_METHOD(blist, get_type)
{
    std::string result;
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    result = intern->blist_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(blist, get)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
            RETURN_NULL();
    }
    if (key < 0) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_ZVAL(intern->blist_data->get(key), 1, 0);
}
PHP_METHOD(blist, get_copy)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
            RETURN_NULL();
    }
    if (key < 0) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_OBJ(zend_container::bnode_object_clone(intern->blist_data->get(key)));
}
PHP_METHOD(blist, add)
{
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &zv) == FAILURE) {
            RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    intern->blist_data->add(zv);
    RETURN_TRUE;
}
PHP_METHOD(blist, set)
{
    long key;
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lo", &key, &zv) == FAILURE) {
            RETURN_FALSE;
    }
    if (key < 0) {
        RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    intern->blist_data->set(key, zv);
    RETURN_TRUE;
}
PHP_METHOD(blist, has)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
            RETURN_NULL();
    }
    if (key < 0) {
        RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_BOOL(intern->blist_data->has(key));
}
PHP_METHOD(blist, del)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
            RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_BOOL(intern->blist_data->del(key));
}
PHP_METHOD(blist, length)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    size_t result = intern->blist_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(blist, count)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    size_t result = intern->blist_data->count();
    RETURN_LONG(result);
}
PHP_METHOD(blist, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len > 0) RETURN_NULL();
    std::string tmp(ben);
    size_t pt = 0;
    RETURN_ZVAL(blist::parse(tmp, pt), 1, 1);
}
PHP_METHOD(blist, encode)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string result = intern->blist_data->encode();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(blist, to_array)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_ZVAL(intern->blist_data->to_array(false), 1, 1);
}
PHP_METHOD(blist, to_meta_array)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_ZVAL(intern->blist_data->to_array(true), 1, 1);
}
PHP_METHOD(blist, __toString)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string result = intern->blist_data->encode();
    RETURN_STRING(result.c_str());
}
static zend_function_entry blist_methods[] = {
    PHP_ME(blist, __construct,          NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(blist, get_type,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, get,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, get_copy,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, add,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, set,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, has,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, del,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, length,               NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, count,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, parse,                NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(blist, encode,               NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, to_array,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, to_meta_array,        NULL, ZEND_ACC_PUBLIC)
    PHP_ME(blist, __toString,           NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** BSTR *****/
PHP_METHOD(bstr, __construct)
{
    char *value;
    size_t value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &value, &value_len) == FAILURE) {
            RETURN_NULL();
    }
    bstr *bnode = NULL;
    if (value_len > 0) {
        std::string tmp(value);
        bnode = new bstr(tmp);
    } else {
        bnode = new bstr();
    }
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    intern->bstr_data = bnode;
}
PHP_METHOD(bstr, get_type)
{
    std::string result;
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    result = intern->bstr_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bstr, get)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bstr_data->get();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bstr, set)
{
    char *value;
    size_t value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    std::string _value(value);
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    intern->bstr_data->set(_value);
    RETURN_TRUE;
}
PHP_METHOD(bstr, length)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    size_t result = intern->bstr_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bstr, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len > 0) RETURN_NULL();
    std::string tmp(ben);
    size_t pt = 0;
    RETURN_ZVAL(bstr::parse(tmp, pt), 1, 1);
}
PHP_METHOD(bstr, encode)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bstr_data->encode();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bstr, to_array)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    RETURN_ZVAL(intern->bstr_data->to_array(false), 1, 1);
}
PHP_METHOD(bstr, to_meta_array)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    RETURN_ZVAL(intern->bstr_data->to_array(true), 1, 1);
}
PHP_METHOD(bstr, __toString)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bstr_data->encode();
    RETURN_STRING(result.c_str());
}
static zend_function_entry bstr_methods[] = {
    PHP_ME(bstr, __construct,           NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bstr, get_type,              NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, get,                   NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, set,                   NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, length,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, parse,                 NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bstr, encode,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, to_array,              NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, to_meta_array,         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, __toString,            NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** BINT *****/
PHP_METHOD(bint, __construct)
{
    long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &value) == FAILURE) {
            RETURN_NULL();
    }
    bint *bnode = NULL;
    if (ZEND_NUM_ARGS()) {
        bnode = new bint(value);
    } else {
        bnode = new bint();
    }
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    intern->bint_data = bnode;
}
PHP_METHOD(bint, get_type)
{
    std::string result;
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    result = intern->bint_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bint, get)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    long result = intern->bint_data->get();
    RETURN_LONG(result);
}
PHP_METHOD(bint, set)
{
    long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &value) == FAILURE) {
        RETURN_FALSE;
    }
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    intern->bint_data->set(value);
    RETURN_TRUE;
}
PHP_METHOD(bint, length)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    size_t result = intern->bint_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bint, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len > 0) RETURN_NULL();
    std::string tmp(ben);
    size_t pt = 0;
    RETURN_ZVAL(bint::parse(tmp, pt), 1, 1);
}
PHP_METHOD(bint, encode)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    std::string result = intern->bint_data->encode();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bint, to_array)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    RETURN_ZVAL(intern->bint_data->to_array(false), 1, 1);
}
PHP_METHOD(bint, to_meta_array)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    RETURN_ZVAL(intern->bint_data->to_array(true), 1, 1);
}
PHP_METHOD(bint, __toString)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    std::string result = intern->bint_data->encode();
    RETURN_STRING(result.c_str());
}
static zend_function_entry bint_methods[] = {
    PHP_ME(bint, __construct,           NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bint, get_type,              NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, get,                   NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, set,                   NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, length,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, parse,                 NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bint, encode,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, to_array,              NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, to_meta_array,         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bint, __toString,            NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** PHP ****/
PHP_MINIT_FUNCTION(bencode)
{
    do {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "bitem", bitem_methods);
        ce.ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
        ce.ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
        zend_container::bitem_ce = zend_register_internal_class(&ce TSRMLS_CC);
        memcpy(&zend_container::bitem_object_handlers,
                zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        zend_container::bitem_object_handlers.offset = XtOffsetOf(bitem_object, std);
    } while (0);
    BI_MINIT(bdict)
    BI_MINIT(blist)
    BI_MINIT(bstr)
    BI_MINIT(bint)
    return SUCCESS;
}

static zend_function_entry bencode_functions[] = {
    PHP_FE(bencode_hello, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry bencode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_BENCODE_EXTNAME,
    bencode_functions,          //FUNCTIONS
    PHP_MINIT(bencode),         //PHP_MINIT
    NULL,                       //PHP_MSHUTDOWN(bencode),
    NULL,                       //PHP_RINIT(bencode),
    NULL,                       //PHP_RSHUTDOWN
    NULL,                       //PHP_MINFO
#if ZEND_MODULE_API_NO >= 20010901
    PHP_BENCODE_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BENCODE
extern "C" {
ZEND_GET_MODULE(bencode)
}
#endif

PHP_FUNCTION(bencode_hello)
{
    php_printf("Hello Bencode!\n");
}
