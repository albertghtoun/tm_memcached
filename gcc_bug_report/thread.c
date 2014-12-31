/*compile with:

gcc -std=gnu99 -DHAVE_CONFIG_H -I.  -DNDEBUG   -g -O2 -pthread -fgnu-tm -pthread -Wall -Werror -pedantic -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -MT memcached-thread.o -MD -MP -MF .deps/memcached-thread.Tpo -c -o memcached-thread2.o thread.c

*/





/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

/** \file
 * The main memcached header holding commonly used data
 * structures and function prototypes.
 */
typedef int bool;

#ifdef HAVE_CONFIG_H
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Set to nonzero if you want to include DTRACE */
/* #undef ENABLE_DTRACE */

/* Set to nonzero if you want to include SASL */
/* #undef ENABLE_SASL */

/* Set to nonzero if you want to enable a SASL pwdb */
/* #undef ENABLE_SASL_PWDB */

/* machine is bigendian */
/* #undef ENDIAN_BIG */

/* machine is littleendian */
#define ENDIAN_LITTLE 1

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1

/* Define this if you have an implementation of drop_privileges() */
/* #undef HAVE_DROP_PRIVILEGES */

/* GCC Atomics available */
#define HAVE_GCC_ATOMICS 1

/* Define to 1 if you have the `getpagesizes' function. */
/* #undef HAVE_GETPAGESIZES */

/* Have ntohll */
/* #undef HAVE_HTONLL */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `memcntl' function. */
/* #undef HAVE_MEMCNTL */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mlockall' function. */
#define HAVE_MLOCKALL 1

/* we have sasl_callback_ft */
/* #undef HAVE_SASL_CALLBACK_FT */

/* Set to nonzero if your SASL implementation supports SASL_CB_GETCONF */
/* #undef HAVE_SASL_CB_GETCONF */
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <event.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

/*
 * Copyright (c) <2008>, Sun Microsystems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the  nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SUN MICROSYSTEMS, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SUN MICROSYSTEMS, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Summary: Constants used by to implement the binary protocol.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Trond Norbye <trond.norbye@sun.com>
 */

#ifndef PROTOCOL_BINARY_H
#define PROTOCOL_BINARY_H

/**
 * This file contains definitions of the constants and packet formats
 * defined in the binary specification. Please note that you _MUST_ remember
 * to convert each multibyte field to / from network byte order to / from
 * host order.
 */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Definition of the legal "magic" values used in a packet.
     * See section 3.1 Magic byte
     */
    typedef enum {
        PROTOCOL_BINARY_REQ = 0x80,
        PROTOCOL_BINARY_RES = 0x81
    } protocol_binary_magic;

    /**
     * Definition of the valid response status numbers.
     * See section 3.2 Response Status
     */
    typedef enum {
        PROTOCOL_BINARY_RESPONSE_SUCCESS = 0x00,
        PROTOCOL_BINARY_RESPONSE_KEY_ENOENT = 0x01,
        PROTOCOL_BINARY_RESPONSE_KEY_EEXISTS = 0x02,
        PROTOCOL_BINARY_RESPONSE_E2BIG = 0x03,
        PROTOCOL_BINARY_RESPONSE_EINVAL = 0x04,
        PROTOCOL_BINARY_RESPONSE_NOT_STORED = 0x05,
        PROTOCOL_BINARY_RESPONSE_DELTA_BADVAL = 0x06,
        PROTOCOL_BINARY_RESPONSE_AUTH_ERROR = 0x20,
        PROTOCOL_BINARY_RESPONSE_AUTH_CONTINUE = 0x21,
        PROTOCOL_BINARY_RESPONSE_UNKNOWN_COMMAND = 0x81,
        PROTOCOL_BINARY_RESPONSE_ENOMEM = 0x82
    } protocol_binary_response_status;

    /**
     * Defintion of the different command opcodes.
     * See section 3.3 Command Opcodes
     */
    typedef enum {
        PROTOCOL_BINARY_CMD_GET = 0x00,
        PROTOCOL_BINARY_CMD_SET = 0x01,
        PROTOCOL_BINARY_CMD_ADD = 0x02,
        PROTOCOL_BINARY_CMD_REPLACE = 0x03,
        PROTOCOL_BINARY_CMD_DELETE = 0x04,
        PROTOCOL_BINARY_CMD_INCREMENT = 0x05,
        PROTOCOL_BINARY_CMD_DECREMENT = 0x06,
        PROTOCOL_BINARY_CMD_QUIT = 0x07,
        PROTOCOL_BINARY_CMD_FLUSH = 0x08,
        PROTOCOL_BINARY_CMD_GETQ = 0x09,
        PROTOCOL_BINARY_CMD_NOOP = 0x0a,
        PROTOCOL_BINARY_CMD_VERSION = 0x0b,
        PROTOCOL_BINARY_CMD_GETK = 0x0c,
        PROTOCOL_BINARY_CMD_GETKQ = 0x0d,
        PROTOCOL_BINARY_CMD_APPEND = 0x0e,
        PROTOCOL_BINARY_CMD_PREPEND = 0x0f,
        PROTOCOL_BINARY_CMD_STAT = 0x10,
        PROTOCOL_BINARY_CMD_SETQ = 0x11,
        PROTOCOL_BINARY_CMD_ADDQ = 0x12,
        PROTOCOL_BINARY_CMD_REPLACEQ = 0x13,
        PROTOCOL_BINARY_CMD_DELETEQ = 0x14,
        PROTOCOL_BINARY_CMD_INCREMENTQ = 0x15,
        PROTOCOL_BINARY_CMD_DECREMENTQ = 0x16,
        PROTOCOL_BINARY_CMD_QUITQ = 0x17,
        PROTOCOL_BINARY_CMD_FLUSHQ = 0x18,
        PROTOCOL_BINARY_CMD_APPENDQ = 0x19,
        PROTOCOL_BINARY_CMD_PREPENDQ = 0x1a,
        PROTOCOL_BINARY_CMD_TOUCH = 0x1c,
        PROTOCOL_BINARY_CMD_GAT = 0x1d,
        PROTOCOL_BINARY_CMD_GATQ = 0x1e,
        PROTOCOL_BINARY_CMD_GATK = 0x23,
        PROTOCOL_BINARY_CMD_GATKQ = 0x24,

        PROTOCOL_BINARY_CMD_SASL_LIST_MECHS = 0x20,
        PROTOCOL_BINARY_CMD_SASL_AUTH = 0x21,
        PROTOCOL_BINARY_CMD_SASL_STEP = 0x22,

        /* These commands are used for range operations and exist within
         * this header for use in other projects.  Range operations are
         * not expected to be implemented in the memcached server itself.
         */
        PROTOCOL_BINARY_CMD_RGET      = 0x30,
        PROTOCOL_BINARY_CMD_RSET      = 0x31,
        PROTOCOL_BINARY_CMD_RSETQ     = 0x32,
        PROTOCOL_BINARY_CMD_RAPPEND   = 0x33,
        PROTOCOL_BINARY_CMD_RAPPENDQ  = 0x34,
        PROTOCOL_BINARY_CMD_RPREPEND  = 0x35,
        PROTOCOL_BINARY_CMD_RPREPENDQ = 0x36,
        PROTOCOL_BINARY_CMD_RDELETE   = 0x37,
        PROTOCOL_BINARY_CMD_RDELETEQ  = 0x38,
        PROTOCOL_BINARY_CMD_RINCR     = 0x39,
        PROTOCOL_BINARY_CMD_RINCRQ    = 0x3a,
        PROTOCOL_BINARY_CMD_RDECR     = 0x3b,
        PROTOCOL_BINARY_CMD_RDECRQ    = 0x3c
        /* End Range operations */

    } protocol_binary_command;

    /**
     * Definition of the data types in the packet
     * See section 3.4 Data Types
     */
    typedef enum {
        PROTOCOL_BINARY_RAW_BYTES = 0x00
    } protocol_binary_datatypes;

    /**
     * Definition of the header structure for a request packet.
     * See section 2
     */
    typedef union {
        struct {
            uint8_t magic;
            uint8_t opcode;
            uint16_t keylen;
            uint8_t extlen;
            uint8_t datatype;
            uint16_t reserved;
            uint32_t bodylen;
            uint32_t opaque;
            uint64_t cas;
        } request;
        uint8_t bytes[24];
    } protocol_binary_request_header;

    /**
     * Definition of the header structure for a response packet.
     * See section 2
     */
    typedef union {
        struct {
            uint8_t magic;
            uint8_t opcode;
            uint16_t keylen;
            uint8_t extlen;
            uint8_t datatype;
            uint16_t status;
            uint32_t bodylen;
            uint32_t opaque;
            uint64_t cas;
        } response;
        uint8_t bytes[24];
    } protocol_binary_response_header;

    /**
     * Definition of a request-packet containing no extras
     */
    typedef union {
        struct {
            protocol_binary_request_header header;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header)];
    } protocol_binary_request_no_extras;

    /**
     * Definition of a response-packet containing no extras
     */
    typedef union {
        struct {
            protocol_binary_response_header header;
        } message;
        uint8_t bytes[sizeof(protocol_binary_response_header)];
    } protocol_binary_response_no_extras;

    /**
     * Definition of the packet used by the get, getq, getk and getkq command.
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_get;
    typedef protocol_binary_request_no_extras protocol_binary_request_getq;
    typedef protocol_binary_request_no_extras protocol_binary_request_getk;
    typedef protocol_binary_request_no_extras protocol_binary_request_getkq;

    /**
     * Definition of the packet returned from a successful get, getq, getk and
     * getkq.
     * See section 4
     */
    typedef union {
        struct {
            protocol_binary_response_header header;
            struct {
                uint32_t flags;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_response_header) + 4];
    } protocol_binary_response_get;

    typedef protocol_binary_response_get protocol_binary_response_getq;
    typedef protocol_binary_response_get protocol_binary_response_getk;
    typedef protocol_binary_response_get protocol_binary_response_getkq;

    /**
     * Definition of the packet used by the delete command
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_delete;

    /**
     * Definition of the packet returned by the delete command
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_delete;

    /**
     * Definition of the packet used by the flush command
     * See section 4
     * Please note that the expiration field is optional, so remember to see
     * check the header.bodysize to see if it is present.
     */
    typedef union {
        struct {
            protocol_binary_request_header header;
            struct {
                uint32_t expiration;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header) + 4];
    } protocol_binary_request_flush;

    /**
     * Definition of the packet returned by the flush command
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_flush;

    /**
     * Definition of the packet used by set, add and replace
     * See section 4
     */
    typedef union {
        struct {
            protocol_binary_request_header header;
            struct {
                uint32_t flags;
                uint32_t expiration;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header) + 8];
    } protocol_binary_request_set;
    typedef protocol_binary_request_set protocol_binary_request_add;
    typedef protocol_binary_request_set protocol_binary_request_replace;

    /**
     * Definition of the packet returned by set, add and replace
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_set;
    typedef protocol_binary_response_no_extras protocol_binary_response_add;
    typedef protocol_binary_response_no_extras protocol_binary_response_replace;

    /**
     * Definition of the noop packet
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_noop;

    /**
     * Definition of the packet returned by the noop command
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_noop;

    /**
     * Definition of the structure used by the increment and decrement
     * command.
     * See section 4
     */
    typedef union {
        struct {
            protocol_binary_request_header header;
            struct {
                uint64_t delta;
                uint64_t initial;
                uint32_t expiration;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header) + 20];
    } protocol_binary_request_incr;
    typedef protocol_binary_request_incr protocol_binary_request_decr;

    /**
     * Definition of the response from an incr or decr command
     * command.
     * See section 4
     */
    typedef union {
        struct {
            protocol_binary_response_header header;
            struct {
                uint64_t value;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_response_header) + 8];
    } protocol_binary_response_incr;
    typedef protocol_binary_response_incr protocol_binary_response_decr;

    /**
     * Definition of the quit
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_quit;

    /**
     * Definition of the packet returned by the quit command
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_quit;

    /**
     * Definition of the packet used by append and prepend command
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_append;
    typedef protocol_binary_request_no_extras protocol_binary_request_prepend;

    /**
     * Definition of the packet returned from a successful append or prepend
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_append;
    typedef protocol_binary_response_no_extras protocol_binary_response_prepend;

    /**
     * Definition of the packet used by the version command
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_version;

    /**
     * Definition of the packet returned from a successful version command
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_version;


    /**
     * Definition of the packet used by the stats command.
     * See section 4
     */
    typedef protocol_binary_request_no_extras protocol_binary_request_stats;

    /**
     * Definition of the packet returned from a successful stats command
     * See section 4
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_stats;

    /**
     * Definition of the packet used by the touch command.
     */
    typedef union {
        struct {
            protocol_binary_request_header header;
            struct {
                uint32_t expiration;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header) + 4];
    } protocol_binary_request_touch;

    /**
     * Definition of the packet returned from the touch command
     */
    typedef protocol_binary_response_no_extras protocol_binary_response_touch;

    /**
     * Definition of the packet used by the GAT(Q) command.
     */
    typedef union {
        struct {
            protocol_binary_request_header header;
            struct {
                uint32_t expiration;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header) + 4];
    } protocol_binary_request_gat;

    typedef protocol_binary_request_gat protocol_binary_request_gatq;
    typedef protocol_binary_request_gat protocol_binary_request_gatk;
    typedef protocol_binary_request_gat protocol_binary_request_gatkq;

    /**
     * Definition of the packet returned from the GAT(Q)
     */
    typedef protocol_binary_response_get protocol_binary_response_gat;
    typedef protocol_binary_response_get protocol_binary_response_gatq;
    typedef protocol_binary_response_get protocol_binary_response_gatk;
    typedef protocol_binary_response_get protocol_binary_response_gatkq;

    /**
     * Definition of a request for a range operation.
     * See http://code.google.com/p/memcached/wiki/RangeOps
     *
     * These types are used for range operations and exist within
     * this header for use in other projects.  Range operations are
     * not expected to be implemented in the memcached server itself.
     */
    typedef union {
        struct {
            protocol_binary_response_header header;
            struct {
                uint16_t size;
                uint8_t  reserved;
                uint8_t  flags;
                uint32_t max_results;
            } body;
        } message;
        uint8_t bytes[sizeof(protocol_binary_request_header) + 4];
    } protocol_binary_request_rangeop;

    typedef protocol_binary_request_rangeop protocol_binary_request_rget;
    typedef protocol_binary_request_rangeop protocol_binary_request_rset;
    typedef protocol_binary_request_rangeop protocol_binary_request_rsetq;
    typedef protocol_binary_request_rangeop protocol_binary_request_rappend;
    typedef protocol_binary_request_rangeop protocol_binary_request_rappendq;
    typedef protocol_binary_request_rangeop protocol_binary_request_rprepend;
    typedef protocol_binary_request_rangeop protocol_binary_request_rprependq;
    typedef protocol_binary_request_rangeop protocol_binary_request_rdelete;
    typedef protocol_binary_request_rangeop protocol_binary_request_rdeleteq;
    typedef protocol_binary_request_rangeop protocol_binary_request_rincr;
    typedef protocol_binary_request_rangeop protocol_binary_request_rincrq;
    typedef protocol_binary_request_rangeop protocol_binary_request_rdecr;
    typedef protocol_binary_request_rangeop protocol_binary_request_rdecrq;

#ifdef __cplusplus
}
#endif
#endif /* PROTOCOL_BINARY_H */

/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#ifndef CACHE_H
#define CACHE_H
#include <pthread.h>

#ifdef HAVE_UMEM_H
#include <umem.h>
#define cache_t umem_cache_t
#define cache_alloc(a) umem_cache_alloc(a, UMEM_DEFAULT)
#define cache_free(a, b) umem_cache_free(a, b)
#define cache_create(a,b,c,d,e) umem_cache_create((char*)a, b, c, d, e, NULL, NULL, NULL, 0)
#define cache_destroy(a) umem_cache_destroy(a);

#else

#ifndef NDEBUG
/* may be used for debug purposes */
extern int cache_error;
#endif

/**
 * Constructor used to initialize allocated objects
 *
 * @param obj pointer to the object to initialized.
 * @param notused1 This parameter is currently not used.
 * @param notused2 This parameter is currently not used.
 * @return you should return 0, but currently this is not checked
 */
typedef int cache_constructor_t(void* obj, void* notused1, int notused2);
/**
 * Destructor used to clean up allocated objects before they are
 * returned to the operating system.
 *
 * @param obj pointer to the object to initialized.
 * @param notused1 This parameter is currently not used.
 * @param notused2 This parameter is currently not used.
 * @return you should return 0, but currently this is not checked
 */
typedef void cache_destructor_t(void* obj, void* notused);

/**
 * Definition of the structure to keep track of the internal details of
 * the cache allocator. Touching any of these variables results in
 * undefined behavior.
 */
typedef struct {
    /** Mutex to protect access to the structure */
    pthread_mutex_t mutex;
    /** Name of the cache objects in this cache (provided by the caller) */
    char *name;
    /** List of pointers to available buffers in this cache */
    void **ptr;
    /** The size of each element in this cache */
    size_t bufsize;
    /** The capacity of the list of elements */
    int freetotal;
    /** The current number of free elements */
    int freecurr;
    /** The constructor to be called each time we allocate more memory */
    cache_constructor_t* constructor;
    /** The destructor to be called each time before we release memory */
    cache_destructor_t* destructor;
} cache_t;

/**
 * Create an object cache.
 *
 * The object cache will let you allocate objects of the same size. It is fully
 * MT safe, so you may allocate objects from multiple threads without having to
 * do any syncrhonization in the application code.
 *
 * @param name the name of the object cache. This name may be used for debug purposes
 *             and may help you track down what kind of object you have problems with
 *             (buffer overruns, leakage etc)
 * @param bufsize the size of each object in the cache
 * @param align the alignment requirements of the objects in the cache.
 * @param constructor the function to be called to initialize memory when we need
 *                    to allocate more memory from the os.
 * @param destructor the function to be called before we release the memory back
 *                   to the os.
 * @return a handle to an object cache if successful, NULL otherwise.
 */
cache_t* cache_create(const char* name, size_t bufsize, size_t align,
                      cache_constructor_t* constructor,
                      cache_destructor_t* destructor);
/**
 * Destroy an object cache.
 *
 * Destroy and invalidate an object cache. You should return all buffers allocated
 * with cache_alloc by using cache_free before calling this function. Not doing
 * so results in undefined behavior (the buffers may or may not be invalidated)
 *
 * @param handle the handle to the object cache to destroy.
 */
void cache_destroy(cache_t* handle);
/**
 * Allocate an object from the cache.
 *
 * @param handle the handle to the object cache to allocate from
 * @return a pointer to an initialized object from the cache, or NULL if
 *         the allocation cannot be satisfied.
 */
void* cache_alloc(cache_t* handle);
/**
 * Return an object back to the cache.
 *
 * The caller should return the object in an initialized state so that
 * the object may be returned in an expected state from cache_alloc.
 *
 * @param handle handle to the object cache to return the object to
 * @param ptr pointer to the object to return.
 */
void cache_free(cache_t* handle, void* ptr);
#endif

#endif

#ifndef SASL_DEFS_H
#define SASL_DEFS_H 1

// Longest one I could find was ``9798-U-RSA-SHA1-ENC''
#define MAX_SASL_MECH_LEN 32

#if defined(HAVE_SASL_SASL_H) && defined(ENABLE_SASL)

#include <sasl/sasl.h>
void init_sasl(void);

extern char my_sasl_hostname[1025];

#else /* End of SASL support */

typedef void* sasl_conn_t;

#define init_sasl() {}
#define sasl_dispose(x) {}
#define sasl_server_new(a, b, c, d, e, f, g, h) 1
#define sasl_listmech(a, b, c, d, e, f, g, h) 1
#define sasl_server_start(a, b, c, d, e, f) 1
#define sasl_server_step(a, b, c, d, e) 1
#define sasl_getprop(a, b, c) {}

#define SASL_OK 0
#define SASL_CONTINUE -1

#endif /* sasl compat */

#endif /* SASL_DEFS_H */

// [branch 012] Include errno, so we can access it in our new perror code
#include <errno.h>

// [branch 008] Support for safe assertions.  Note that the evaluation of the
// expression occurs within the context of the transaction, but we don't
// commit before we call the safe_assert_internal code.
#if defined(NDEBUG)
#define tm_assert(e)   ((void)0)
#else
#include <stdlib.h>
#define tm_assert(e) ((e) ? (void)0 : tm_assert_internal(__FILE__, __LINE__, __func__, #e))
__attribute__((transaction_pure))
void tm_assert_internal(const char *filename, int linenum, const char *funcname, const char *sourceline);
#endif /* NDEBUG */

// [branch 008] This is our 'safe' printf-and-abort function
__attribute__((transaction_pure))
void tm_msg_and_die(const char* msg);

// [branch 009] Provide safe versions of memcmp, memcpy, strlen, strncmp,
//              strncpy, and realloc
__attribute__((transaction_safe))
int tm_memcmp(const void *s1, const void *s2, size_t n);
__attribute__((transaction_safe))
void *tm_memcpy(void *dst, const void *src, size_t len);
__attribute__((transaction_safe))
size_t tm_strlen(const char *s);
__attribute__((transaction_safe))
int tm_strncmp(const char *s1, const char *s2, size_t n);
__attribute__((transaction_safe))
char *tm_strncpy(char *dst, const char *src, size_t n);
__attribute__((transaction_safe))
void *tm_realloc(void *ptr, size_t size, size_t old_size);

// [branch 009b] a custom strncpy that reads via TM, and writes directly to a
//               location presumed to be thread-private (e.g., on the stack)
__attribute__((transaction_safe))
char *tm_strncpy_to_local(char *local_dst, const char *src, size_t n);

// [branch 009b] Provide safe versions of strtol, atoi, strtol, strchr, and
//               isspace
__attribute__((transaction_safe))
long int tm_strtol(const char *nptr, char **endptr, int base);
__attribute__((transaction_safe))
int tm_atoi(const char *nptr);
__attribute__((transaction_safe))
unsigned long long int tm_strtoull(const char *nptr, char **endptr, int base);
__attribute__((transaction_safe))
char *tm_strchr(const char *s, int c);
// [branch 009b] This can just be marked pure
__attribute__((transaction_pure))
int tm_isspace(int c);

// [branch 012] Provide support for oncommit handlers
__attribute__((transaction_pure))
void registerOnCommitHandler(void (*func)(void*), void *param);
void delayed_perror(int error_number, char *message);

/** Maximum length of a key. */
#define KEY_MAX_LENGTH 250

/** Size of an incr buf. */
#define INCR_MAX_STORAGE_LEN 24

#define DATA_BUFFER_SIZE 2048
#define UDP_READ_BUFFER_SIZE 65536
#define UDP_MAX_PAYLOAD_SIZE 1400
#define UDP_HEADER_SIZE 8
#define MAX_SENDBUF_SIZE (256 * 1024 * 1024)
/* I'm told the max length of a 64-bit num converted to string is 20 bytes.
 * Plus a few for spaces, \r\n, \0 */
#define SUFFIX_SIZE 24

/** Initial size of list of items being returned by "get". */
#define ITEM_LIST_INITIAL 200

/** Initial size of list of CAS suffixes appended to "gets" lines. */
#define SUFFIX_LIST_INITIAL 20

/** Initial size of the sendmsg() scatter/gather array. */
#define IOV_LIST_INITIAL 400

/** Initial number of sendmsg() argument structures to allocate. */
#define MSG_LIST_INITIAL 10

/** High water marks for buffer shrinking */
#define READ_BUFFER_HIGHWAT 8192
#define ITEM_LIST_HIGHWAT 400
#define IOV_LIST_HIGHWAT 600
#define MSG_LIST_HIGHWAT 100

/* Binary protocol stuff */
#define MIN_BIN_PKT_LENGTH 16
#define BIN_PKT_HDR_WORDS (MIN_BIN_PKT_LENGTH/sizeof(uint32_t))

/* Initial power multiplier for the hash table */
#define HASHPOWER_DEFAULT 16

/* unistd.h is here */
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

/* Slab sizing definitions. */
#define POWER_SMALLEST 1
#define POWER_LARGEST  200
#define CHUNK_ALIGN_BYTES 8
#define MAX_NUMBER_OF_SLAB_CLASSES (POWER_LARGEST + 1)

/** How long an object can reasonably be assumed to be locked before
    harvesting it on a low memory condition. */
#define TAIL_REPAIR_TIME (3 * 3600)

/* warning: don't use these macros with a function, as it evals its arg twice */
#define ITEM_get_cas(i) (((i)->it_flags & ITEM_CAS) ? \
        (i)->data->cas : (uint64_t)0)

#define ITEM_set_cas(i,v) { \
    if ((i)->it_flags & ITEM_CAS) { \
        (i)->data->cas = v; \
    } \
}

#define ITEM_key(item) (((char*)&((item)->data)) \
         + (((item)->it_flags & ITEM_CAS) ? sizeof(uint64_t) : 0))

#define ITEM_suffix(item) ((char*) &((item)->data) + (item)->nkey + 1 \
         + (((item)->it_flags & ITEM_CAS) ? sizeof(uint64_t) : 0))

#define ITEM_data(item) ((char*) &((item)->data) + (item)->nkey + 1 \
         + (item)->nsuffix \
         + (((item)->it_flags & ITEM_CAS) ? sizeof(uint64_t) : 0))

#define ITEM_ntotal(item) (sizeof(struct _stritem) + (item)->nkey + 1 \
         + (item)->nsuffix + (item)->nbytes \
         + (((item)->it_flags & ITEM_CAS) ? sizeof(uint64_t) : 0))

#define STAT_KEY_LEN 128
#define STAT_VAL_LEN 128

/** Append a simple stat with a stat name, value format and value */
#define APPEND_STAT(name, fmt, val) \
    append_stat(name, add_stats, c, fmt, val);

// [branch 011] Clone append_stat, use safe version
#define APPEND_STAT_LLU(name, fmt, val) \
    append_stat_llu(name, add_stats, c, fmt, val);
#define APPEND_STAT_U(name, fmt, val) \
    append_stat_u(name, add_stats, c, fmt, val);
#define APPEND_STAT_D(name, fmt, val) \
    append_stat_d(name, add_stats, c, fmt, val);
#define APPEND_STAT_LU(name, fmt, val) \
    append_stat_lu(name, add_stats, c, fmt, val);
#define APPEND_STAT_LD(name, fmt, val) \
    append_stat_ld(name, add_stats, c, fmt, val);
#define APPEND_STAT_S(name, fmt, val) \
    append_stat_s(name, add_stats, c, fmt, val);

/** Append an indexed stat with a stat name (with format), value format
    and value */
#define APPEND_NUM_FMT_STAT(name_fmt, num, name, fmt, val)          \
    klen = snprintf(key_str, STAT_KEY_LEN, name_fmt, num, name);    \
    vlen = snprintf(val_str, STAT_VAL_LEN, fmt, val);               \
    add_stats(key_str, klen, val_str, vlen, c);

// [branch 011] Clone APPEND_NUM_FMT_STAT to make it safe
#define APPEND_NUM_FMT_STAT_U(name_fmt, num, name, fmt, val)          \
    klen = tm_snprintf_ds(key_str, STAT_KEY_LEN, name_fmt, num, name);    \
    vlen = tm_snprintf_u(val_str, STAT_VAL_LEN, fmt, val);               \
    add_stats(key_str, klen, val_str, vlen, c);
#define APPEND_NUM_FMT_STAT_LLU(name_fmt, num, name, fmt, val)          \
    klen = tm_snprintf_ds(key_str, STAT_KEY_LEN, name_fmt, num, name);    \
    vlen = tm_snprintf_llu(val_str, STAT_VAL_LEN, fmt, val);               \
    add_stats(key_str, klen, val_str, vlen, c);

/** Common APPEND_NUM_FMT_STAT format. */
#define APPEND_NUM_STAT(num, name, fmt, val) \
    APPEND_NUM_FMT_STAT("%d:%s", num, name, fmt, val)
// [branch 011] Clone APPEND_NUM_STAT to make it safe
#define APPEND_NUM_STAT_U(num, name, fmt, val) \
    APPEND_NUM_FMT_STAT_U("%d:%s", num, name, fmt, val)
#define APPEND_NUM_STAT_LLU(num, name, fmt, val) \
    APPEND_NUM_FMT_STAT_LLU("%d:%s", num, name, fmt, val)

/**
 * Callback for any function producing stats.
 *
 * @param key the stat's key
 * @param klen length of the key
 * @param val the stat's value in an ascii form (e.g. text form of a number)
 * @param vlen length of the value
 * @parm cookie magic callback cookie
 */
// [branch 005] Force all instances of this function pointer to be safe to
//              call from atomic transactions
typedef void (*ADD_STAT)(const char *key, const uint16_t klen,
                         const char *val, const uint32_t vlen,
                         const void *cookie) __attribute__((transaction_safe));

/*
 * NOTE: If you modify this table you _MUST_ update the function state_text
 */
/**
 * Possible states of a connection.
 */
enum conn_states {
    conn_listening,  /**< the socket which listens for connections */
    conn_new_cmd,    /**< Prepare connection for next command */
    conn_waiting,    /**< waiting for a readable socket */
    conn_read,       /**< reading in a command line */
    conn_parse_cmd,  /**< try to parse a command from the input buffer */
    conn_write,      /**< writing out a simple response */
    conn_nread,      /**< reading in a fixed number of bytes */
    conn_swallow,    /**< swallowing unnecessary bytes w/o storing */
    conn_closing,    /**< closing this connection */
    conn_mwrite,     /**< writing out many items sequentially */
    conn_max_state   /**< Max state value (used for assertion) */
};

enum bin_substates {
    bin_no_state,
    bin_reading_set_header,
    bin_reading_cas_header,
    bin_read_set_value,
    bin_reading_get_key,
    bin_reading_stat,
    bin_reading_del_header,
    bin_reading_incr_header,
    bin_read_flush_exptime,
    bin_reading_sasl_auth,
    bin_reading_sasl_auth_data,
    bin_reading_touch_key,
};

enum protocol {
    ascii_prot = 3, /* arbitrary value. */
    binary_prot,
    negotiating_prot /* Discovering the protocol */
};

enum network_transport {
    local_transport, /* Unix sockets*/
    tcp_transport,
    udp_transport
};

enum item_lock_types {
    ITEM_LOCK_GRANULAR = 0,
    ITEM_LOCK_GLOBAL
};

#define IS_UDP(x) (x == udp_transport)

#define NREAD_ADD 1
#define NREAD_SET 2
#define NREAD_REPLACE 3
#define NREAD_APPEND 4
#define NREAD_PREPEND 5
#define NREAD_CAS 6

enum store_item_type {
    NOT_STORED=0, STORED, EXISTS, NOT_FOUND
};

enum delta_result_type {
    OK, NON_NUMERIC, EOM, DELTA_ITEM_NOT_FOUND, DELTA_ITEM_CAS_MISMATCH
};

/** Time relative to server start. Smaller than time_t on 64-bit systems. */
typedef unsigned int rel_time_t;

/** Stats stored per slab (and per thread). */
struct slab_stats {
    uint64_t  set_cmds;
    uint64_t  get_hits;
    uint64_t  touch_hits;
    uint64_t  delete_hits;
    uint64_t  cas_hits;
    uint64_t  cas_badval;
    uint64_t  incr_hits;
    uint64_t  decr_hits;
};

/**
 * Stats stored per-thread.
 */
struct thread_stats {
    // [branch 010] Removed per-thread stats mutexes; keeping object in
    //              struct for alignment
    pthread_mutex_t   defunct_mutex;
    uint64_t          get_cmds;
    uint64_t          get_misses;
    uint64_t          touch_cmds;
    uint64_t          touch_misses;
    uint64_t          delete_misses;
    uint64_t          incr_misses;
    uint64_t          decr_misses;
    uint64_t          cas_misses;
    uint64_t          bytes_read;
    uint64_t          bytes_written;
    uint64_t          flush_cmds;
    uint64_t          conn_yields; /* # of yields for connections (-R option)*/
    uint64_t          auth_cmds;
    uint64_t          auth_errors;
    struct slab_stats slab_stats[MAX_NUMBER_OF_SLAB_CLASSES];
};

/**
 * Global stats.
 */
struct stats {
    pthread_mutex_t mutex;
    unsigned int  curr_items;
    unsigned int  total_items;
    uint64_t      curr_bytes;
    unsigned int  curr_conns;
    unsigned int  total_conns;
    uint64_t      rejected_conns;
    unsigned int  reserved_fds;
    unsigned int  conn_structs;
    uint64_t      get_cmds;
    uint64_t      set_cmds;
    uint64_t      touch_cmds;
    uint64_t      get_hits;
    uint64_t      get_misses;
    uint64_t      touch_hits;
    uint64_t      touch_misses;
    uint64_t      evictions;
    uint64_t      reclaimed;
    time_t        started;          /* when the process was started */
    bool          accepting_conns;  /* whether we are currently accepting */
    uint64_t      listen_disabled_num;
    unsigned int  hash_power_level; /* Better hope it's not over 9000 */
    uint64_t      hash_bytes;       /* size used for hash tables */
    bool          hash_is_expanding; /* If the hash table is being expanded */
    uint64_t      expired_unfetched; /* items reclaimed but never touched */
    uint64_t      evicted_unfetched; /* items evicted but never touched */
    bool          slab_reassign_running; /* slab reassign in progress */
    uint64_t      slabs_moved;       /* times slabs were moved around */
};

#define MAX_VERBOSITY_LEVEL 2

/* When adding a setting, be sure to update process_stat_settings */
/**
 * Globally accessible settings as derived from the commandline.
 */
struct settings {
    size_t maxbytes;
    int maxconns;
    int port;
    int udpport;
    char *inter;
    int verbose;
    rel_time_t oldest_live; /* ignore existing items older than this */
    int evict_to_free;
    char *socketpath;   /* path to unix socket if using local socket */
    int access;  /* access mask (a la chmod) for unix domain socket */
    double factor;          /* chunk size growth factor */
    int chunk_size;
    int num_threads;        /* number of worker (without dispatcher) libevent threads to run */
    int num_threads_per_udp; /* number of worker threads serving each udp socket */
    char prefix_delimiter;  /* character that marks a key prefix (for stats) */
    int detail_enabled;     /* nonzero if we're collecting detailed stats */
    int reqs_per_event;     /* Maximum number of io to process on each
                               io-event. */
    bool use_cas;
    enum protocol binding_protocol;
    int backlog;
    int item_size_max;        /* Maximum item size, and upper end for slabs */
    bool sasl;              /* SASL on/off */
    bool maxconns_fast;     /* Whether or not to early close connections */
    bool slab_reassign;     /* Whether or not slab reassignment is allowed */
    int slab_automove;     /* Whether or not to automatically move slabs */
    int hashpower_init;     /* Starting hash power level */
};

extern struct stats stats;
extern time_t process_started;
extern struct settings settings;

#define ITEM_LINKED 1
#define ITEM_CAS 2

/* temp */
#define ITEM_SLABBED 4

#define ITEM_FETCHED 8

/**
 * Structure for storing items within memcached.
 */
typedef struct _stritem {
    struct _stritem *next;
    struct _stritem *prev;
    struct _stritem *h_next;    /* hash chain next */
    rel_time_t      time;       /* least recent access */
    rel_time_t      exptime;    /* expire time */
    int             nbytes;     /* size of data */
    // [branch 007] This field should not be accessed nontransactionally
    unsigned short  tm_refcount;
    uint8_t         nsuffix;    /* length of flags-and-length string */
    uint8_t         it_flags;   /* ITEM_* above */
    uint8_t         slabs_clsid;/* which slab class we're in */
    uint8_t         nkey;       /* key length, w/terminating null and padding */
    /* this odd type prevents type-punning issues when we do
     * the little shuffle to save space when not using CAS. */
    union {
        uint64_t cas;
        char end;
    } data[];
    /* if it_flags & ITEM_CAS we have 8 bytes CAS */
    /* then null-terminated key */
    /* then " flags length\r\n" (no terminating null) */
    /* then data with terminating \r\n (no terminating null; it's binary!) */
} item;

typedef struct {
    pthread_t thread_id;        /* unique ID of this thread */
    struct event_base *base;    /* libevent handle this thread uses */
    struct event notify_event;  /* listen event for notify pipe */
    int notify_receive_fd;      /* receiving end of notify pipe */
    int notify_send_fd;         /* sending end of notify pipe */
    struct thread_stats stats;  /* Stats generated by this thread */
    struct conn_queue *new_conn_queue; /* queue of new connections to handle */
    cache_t *suffix_cache;      /* suffix cache */
    uint8_t item_lock_type;     /* use fine-grained or global item lock */
} LIBEVENT_THREAD;

typedef struct {
    pthread_t thread_id;        /* unique ID of this thread */
    struct event_base *base;    /* libevent handle this thread uses */
} LIBEVENT_DISPATCHER_THREAD;

/**
 * The structure representing a connection into memcached.
 */
typedef struct conn conn;
struct conn {
    int    sfd;
    sasl_conn_t *sasl_conn;
    enum conn_states  state;
    enum bin_substates substate;
    struct event event;
    short  ev_flags;
    short  which;   /** which events were just triggered */

    char   *rbuf;   /** buffer to read commands into */
    char   *rcurr;  /** but if we parsed some already, this is where we stopped */
    int    rsize;   /** total allocated size of rbuf */
    int    rbytes;  /** how much data, starting from rcur, do we have unparsed */

    char   *wbuf;
    char   *wcurr;
    int    wsize;
    int    wbytes;
    /** which state to go into after finishing current write */
    enum conn_states  write_and_go;
    void   *write_and_free; /** free this memory after finishing writing */

    char   *ritem;  /** when we read in an item's value, it goes here */
    int    rlbytes;

    /* data for the nread state */

    /**
     * item is used to hold an item structure created after reading the command
     * line of set/add/replace commands, but before we finished reading the actual
     * data. The data is read into ITEM_data(item) to avoid extra copying.
     */

    void   *item;     /* for commands set/add/replace  */

    /* data for the swallow state */
    int    sbytes;    /* how many bytes to swallow */

    /* data for the mwrite state */
    struct iovec *iov;
    int    iovsize;   /* number of elements allocated in iov[] */
    int    iovused;   /* number of elements used in iov[] */

    struct msghdr *msglist;
    int    msgsize;   /* number of elements allocated in msglist[] */
    int    msgused;   /* number of elements used in msglist[] */
    int    msgcurr;   /* element in msglist[] being transmitted now */
    int    msgbytes;  /* number of bytes in current msg */

    item   **ilist;   /* list of items to write out */
    int    isize;
    item   **icurr;
    int    ileft;

    char   **suffixlist;
    int    suffixsize;
    char   **suffixcurr;
    int    suffixleft;

    enum protocol protocol;   /* which protocol this connection speaks */
    enum network_transport transport; /* what transport is used by this connection */

    /* data for UDP clients */
    int    request_id; /* Incoming UDP request ID, if this is a UDP "connection" */
    struct sockaddr request_addr; /* Who sent the most recent request */
    socklen_t request_addr_size;
    unsigned char *hdrbuf; /* udp packet headers */
    int    hdrsize;   /* number of headers' worth of space is allocated */

    bool   noreply;   /* True if the reply should not be sent. */
    /* current stats command */
    struct {
        char *buffer;
        size_t size;
        size_t offset;
    } stats;

    /* Binary protocol stuff */
    /* This is where the binary header goes */
    protocol_binary_request_header binary_header;
    uint64_t cas; /* the cas to return */
    short cmd; /* current command being processed */
    int opaque;
    int keylen;
    conn   *next;     /* Used for generating a list of conn structures */
    LIBEVENT_THREAD *thread; /* Pointer to the thread object serving this connection */
};


/* current time of day (updated periodically) */
// [branch 006] Replace volatile variable with "transactional" variable
extern rel_time_t tm_current_time;

/* TODO: Move to slabs.h? */
// [branch 006] Replace volatile variable with "transactional" variable
extern int tm_slab_rebalance_signal;

struct slab_rebalance {
    void *slab_start;
    void *slab_end;
    void *slab_pos;
    int s_clsid;
    int d_clsid;
    int busy_items;
    uint8_t done;
};

extern struct slab_rebalance slab_rebal;

/*
 * Functions
 */
void do_accept_new_conns(const bool do_accept);
// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
enum delta_result_type do_add_delta(conn *c, const char *key,
                                    const size_t nkey, const bool incr,
                                    const int64_t delta, char *buf,
                                    uint64_t *cas, const uint32_t hv);
// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
enum store_item_type do_store_item(item *item, int comm, conn* c, const uint32_t hv);
conn *conn_new(const int sfd, const enum conn_states init_state, const int event_flags, const int read_buffer_size, enum network_transport transport, struct event_base *base);
extern int daemonize(int nochdir, int noclose);

static inline int mutex_lock(pthread_mutex_t *mutex)
{
    while (pthread_mutex_trylock(mutex));
    return 0;
}

#define mutex_unlock(x) pthread_mutex_unlock(x)

/* stats */
void stats_prefix_init(void);
// [branch 004] This function is called from a relaxed transaction
// [branch 005] This function is actually transaction safe
__attribute__((transaction_safe))
void stats_prefix_clear(void);
void stats_prefix_record_get(const char *key, const size_t nkey, const bool is_hit);
void stats_prefix_record_delete(const char *key, const size_t nkey);
void stats_prefix_record_set(const char *key, const size_t nkey);
/*@null@*/
char *stats_prefix_dump(int *length);

/* slabs memory allocation */
#ifndef SLABS_H
#define SLABS_H

/** Init the subsystem. 1st argument is the limit on no. of bytes to allocate,
    0 if no limit. 2nd argument is the growth factor; each slab will use a chunk
    size equal to the previous slab's chunk size times this factor.
    3rd argument specifies if the slab allocator should allocate all memory
    up front (if true), or allocate memory in chunks as it is needed (if false)
*/
void slabs_init(const size_t limit, const double factor, const bool prealloc);


/**
 * Given object size, return id to use when allocating/freeing memory for object
 * 0 means error: can't store such a large object
 */

// [branch 004b] This function is called from a relaxed transaction
// [branch 005b] This function is actually transaction_safe
__attribute__((transaction_safe))
unsigned int slabs_clsid(const size_t size);

/** Allocate object of given length. 0 on error */ /*@null@*/
// [branch 004] This function is called from a relaxed transaction
// [branch 009] safe realloc lets this become safe
__attribute__((transaction_safe))
void *slabs_alloc(const size_t size, unsigned int id);

/** Free previously allocated object */
// [branch 004] This function is called from a relaxed transaction
// [branch 008] With safe asserts, this becomes safe
__attribute__((transaction_safe))
void slabs_free(void *ptr, size_t size, unsigned int id);

/** Adjust the stats for memory requested */
// [branch 004] This function is called from a relaxed transaction
// [branch 008] Safe tm_msg_and_die lets this become atomic
__attribute__((transaction_safe))
void slabs_adjust_mem_requested(unsigned int id, size_t old, size_t ntotal);

/** Return a datum for stats in binary protocol */
bool get_stats(const char *stat_type, int nkey, ADD_STAT add_stats, void *c);

/** Fill buffer with stats */ /*@null@*/
void slabs_stats(ADD_STAT add_stats, void *c);

int start_slab_maintenance_thread(void);
void stop_slab_maintenance_thread(void);

enum reassign_result_type {
    REASSIGN_OK=0, REASSIGN_RUNNING, REASSIGN_BADCLASS, REASSIGN_NOSPARE,
    REASSIGN_SRC_DST_SAME
};

// [branch 004] This function is called from a relaxed transaction
// [branch 012] This becomes safe once we have oncommit
__attribute__((transaction_safe))
enum reassign_result_type slabs_reassign(int src, int dst);

void slabs_rebalancer_pause(void);
void slabs_rebalancer_resume(void);

#endif

/* associative array */
void assoc_init(const int hashpower_init);
// [branch 004b] This function is called from a relaxed transaction
// [branch 009b] With safe memcmp, this becomes safe
__attribute__((transaction_safe))
item *assoc_find(const char *key, const size_t nkey, const uint32_t hv);
// [branch 004] This function is called from a relaxed transaction
// [branch 012] With oncommit, this becomes safe
__attribute__((transaction_safe))
int assoc_insert(item *item, const uint32_t hv);
// [branch 004] This function is called from a relaxed transaction
// [branch 009] With safe memcmp, this becomes safe
__attribute__((transaction_safe))
void assoc_delete(const char *key, const size_t nkey, const uint32_t hv);
void do_assoc_move_next_bucket(void);
int start_assoc_maintenance_thread(void);
void stop_assoc_maintenance_thread(void);
extern unsigned int hashpower;

/* See items.c */
// [branch 004] This function is called from a relaxed transaction
// [branch 005] This function is actually transaction safe
__attribute__((transaction_safe))
uint64_t get_cas_id(void);

/*@null@*/
// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
item *do_item_alloc(char *key, const size_t nkey, const int flags, const rel_time_t exptime, const int nbytes, const uint32_t cur_hv);
// [branch 004] This function is called from a relaxed transaction
// [branch 008] With safe asserts, this becomes safe
__attribute__((transaction_safe))
void item_free(item *it);
bool item_size_ok(const size_t nkey, const int flags, const int nbytes);

// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
int  do_item_link(item *it, const uint32_t hv);     /** may fail if transgresses limits */
// [branch 004b] This function is called from a relaxed transaction
// [branch 009b] This becomes safe with safe memcmp
__attribute__((transaction_safe))
void do_item_unlink(item *it, const uint32_t hv);
// [branch 004] This function is called from a relaxed transaction
// [branch 009] With safe memcmp, this becomes safe
__attribute__((transaction_safe))
void do_item_unlink_nolock(item *it, const uint32_t hv);
// [branch 004] This function is called from a relaxed transaction
// [branch 008] With safe asserts, this becomes safe
__attribute__((transaction_safe))
void do_item_remove(item *it);
// [branch 004b] This function is called from a relaxed transaction
// [branch 008b] With safe asserts, this becomes safe
__attribute__((transaction_safe))
void do_item_update(item *it);   /** update LRU time to current and reposition */
// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
int  do_item_replace(item *it, item *new_it, const uint32_t hv);

/*@null@*/
// [branch 004] This function is called from a relaxed transaction
// [branch 011] With safe snprintf, this becomes safe
__attribute__((transaction_safe))
char *do_item_cachedump(const unsigned int slabs_clsid, const unsigned int limit, unsigned int *bytes);
// [branch 004] This function is called from a relaxed transaction
// [branch 011] With safe stats, this becomes safe
__attribute__((transaction_safe))
void do_item_stats(ADD_STAT add_stats, void *c);
// [branch 004] This function is called from a relaxed transaction
// [branch 011] With safe stats, this becomes safe
__attribute__((transaction_safe))
void do_item_stats_totals(ADD_STAT add_stats, void *c);
/*@null@*/
// [branch 004] This function is called from a relaxed transaction
// [branch 011] With safe stats, this becomes safe
__attribute__((transaction_safe))
void do_item_stats_sizes(ADD_STAT add_stats, void *c);
// [branch 004] This function is called from a relaxed transaction
// [branch 009] With safe memcmp, this becomes safe
__attribute__((transaction_safe))
void do_item_flush_expired(void);

// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
item *do_item_get(const char *key, const size_t nkey, const uint32_t hv);
// [branch 004b] This function is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
item *do_item_touch(const char *key, const size_t nkey, uint32_t exptime, const uint32_t hv);
void item_stats_reset(void);
// [branch 002] Removed declaration of cache lock
void item_stats_evictions(uint64_t *evicted);

#ifndef TRACE_H
#define TRACE_H

#define MEMCACHED_ASSOC_DELETE(arg0, arg1, arg2)
#define MEMCACHED_ASSOC_DELETE_ENABLED() (0)
#define MEMCACHED_ASSOC_FIND(arg0, arg1, arg2)
#define MEMCACHED_ASSOC_FIND_ENABLED() (0)
#define MEMCACHED_ASSOC_INSERT(arg0, arg1, arg2)
#define MEMCACHED_ASSOC_INSERT_ENABLED() (0)
#define MEMCACHED_COMMAND_ADD(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_ADD_ENABLED() (0)
#define MEMCACHED_COMMAND_APPEND(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_APPEND_ENABLED() (0)
#define MEMCACHED_COMMAND_CAS(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_CAS_ENABLED() (0)
#define MEMCACHED_COMMAND_DECR(arg0, arg1, arg2, arg3)
#define MEMCACHED_COMMAND_DECR_ENABLED() (0)
#define MEMCACHED_COMMAND_DELETE(arg0, arg1, arg2)
#define MEMCACHED_COMMAND_DELETE_ENABLED() (0)
#define MEMCACHED_COMMAND_GET(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_GET_ENABLED() (0)
#define MEMCACHED_COMMAND_TOUCH(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_TOUCH_ENABLED() (0)
#define MEMCACHED_COMMAND_INCR(arg0, arg1, arg2, arg3)
#define MEMCACHED_COMMAND_INCR_ENABLED() (0)
#define MEMCACHED_COMMAND_PREPEND(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_PREPEND_ENABLED() (0)
#define MEMCACHED_COMMAND_REPLACE(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_REPLACE_ENABLED() (0)
#define MEMCACHED_COMMAND_SET(arg0, arg1, arg2, arg3, arg4)
#define MEMCACHED_COMMAND_SET_ENABLED() (0)
#define MEMCACHED_CONN_ALLOCATE(arg0)
#define MEMCACHED_CONN_ALLOCATE_ENABLED() (0)
#define MEMCACHED_CONN_CREATE(arg0)
#define MEMCACHED_CONN_CREATE_ENABLED() (0)
#define MEMCACHED_CONN_DESTROY(arg0)
#define MEMCACHED_CONN_DESTROY_ENABLED() (0)
#define MEMCACHED_CONN_DISPATCH(arg0, arg1)
#define MEMCACHED_CONN_DISPATCH_ENABLED() (0)
#define MEMCACHED_CONN_RELEASE(arg0)
#define MEMCACHED_CONN_RELEASE_ENABLED() (0)
#define MEMCACHED_ITEM_LINK(arg0, arg1, arg2)
#define MEMCACHED_ITEM_LINK_ENABLED() (0)
#define MEMCACHED_ITEM_REMOVE(arg0, arg1, arg2)
#define MEMCACHED_ITEM_REMOVE_ENABLED() (0)
#define MEMCACHED_ITEM_REPLACE(arg0, arg1, arg2, arg3, arg4, arg5)
#define MEMCACHED_ITEM_REPLACE_ENABLED() (0)
#define MEMCACHED_ITEM_UNLINK(arg0, arg1, arg2)
#define MEMCACHED_ITEM_UNLINK_ENABLED() (0)
#define MEMCACHED_ITEM_UPDATE(arg0, arg1, arg2)
#define MEMCACHED_ITEM_UPDATE_ENABLED() (0)
#define MEMCACHED_PROCESS_COMMAND_END(arg0, arg1, arg2)
#define MEMCACHED_PROCESS_COMMAND_END_ENABLED() (0)
#define MEMCACHED_PROCESS_COMMAND_START(arg0, arg1, arg2)
#define MEMCACHED_PROCESS_COMMAND_START_ENABLED() (0)
#define MEMCACHED_SLABS_ALLOCATE(arg0, arg1, arg2, arg3)
#define MEMCACHED_SLABS_ALLOCATE_ENABLED() (0)
#define MEMCACHED_SLABS_ALLOCATE_FAILED(arg0, arg1)
#define MEMCACHED_SLABS_ALLOCATE_FAILED_ENABLED() (0)
#define MEMCACHED_SLABS_FREE(arg0, arg1, arg2)
#define MEMCACHED_SLABS_FREE_ENABLED() (0)
#define MEMCACHED_SLABS_SLABCLASS_ALLOCATE(arg0)
#define MEMCACHED_SLABS_SLABCLASS_ALLOCATE_ENABLED() (0)
#define MEMCACHED_SLABS_SLABCLASS_ALLOCATE_FAILED(arg0)
#define MEMCACHED_SLABS_SLABCLASS_ALLOCATE_FAILED_ENABLED() (0)

#endif

#ifndef HASH_H
#define    HASH_H

#ifdef    __cplusplus
extern "C" {
#endif

// [branch 004] This function is called from a relaxed transaction
// [branch 005] This function is actually transaction safe
__attribute__((transaction_safe))
uint32_t hash(const void *key, size_t length, const uint32_t initval);

#ifdef    __cplusplus
}
#endif

#endif    /* HASH_H */

/*
 * Wrappers around strtoull/strtoll that are safer and easier to
 * use.  For tests and assumptions, see internal_tests.c.
 *
 * str   a NULL-terminated base decimal 10 unsigned integer
 * out   out parameter, if conversion succeeded
 *
 * returns true if conversion succeeded.
 */
// [branch 004b] This function is called from a relaxed transaction
// [branch 009b] With safe string functions, this becomes safe
__attribute__((transaction_safe))
bool safe_strtoull(const char *str, uint64_t *out);
bool safe_strtoll(const char *str, int64_t *out);
bool safe_strtoul(const char *str, uint32_t *out);
bool safe_strtol(const char *str, int32_t *out);

#ifndef HAVE_HTONLL
extern uint64_t htonll(uint64_t);
extern uint64_t ntohll(uint64_t);
#endif

#ifdef __GCC
# define __gcc_attribute__ __attribute__
#else
# define __gcc_attribute__(x)
#endif

/**
 * Vararg variant of perror that makes for more useful error messages
 * when reporting with parameters.
 *
 * @param fmt a printf format
 */
void vperror(const char *fmt, ...)
    __gcc_attribute__ ((format (printf, 1, 2)));

/*
 * Functions such as the libevent-related calls that need to do cross-thread
 * communication in multithreaded mode (rather than actually doing the work
 * in the current thread) are called via "dispatch_" frontends, which are
 * also #define-d to directly call the underlying code in singlethreaded mode.
 */

void thread_init(int nthreads, struct event_base *main_base);
int  dispatch_event_add(int thread, conn *c);
void dispatch_conn_new(int sfd, enum conn_states init_state, int event_flags, int read_buffer_size, enum network_transport transport);

/* Lock wrappers for cache functions that are called from main loop. */
enum delta_result_type add_delta(conn *c, const char *key,
                                 const size_t nkey, const int incr,
                                 const int64_t delta, char *buf,
                                 uint64_t *cas);
void accept_new_conns(const bool do_accept);
conn *conn_from_freelist(void);
bool  conn_add_to_freelist(conn *c);
int   is_listen_thread(void);
item *item_alloc(char *key, size_t nkey, int flags, rel_time_t exptime, int nbytes);
char *item_cachedump(const unsigned int slabs_clsid, const unsigned int limit, unsigned int *bytes);
void  item_flush_expired(void);
item *item_get(const char *key, const size_t nkey);
item *item_touch(const char *key, const size_t nkey, uint32_t exptime);
int   item_link(item *it);
void  item_remove(item *it);
// [branch 011b] This is called from a relaxed transaction
// [branch 012b] With fprintf oncommit, this becomes safe
__attribute__((transaction_safe))
int   item_replace(item *it, item *new_it, const uint32_t hv);
void  item_stats(ADD_STAT add_stats, void *c);
void  item_stats_totals(ADD_STAT add_stats, void *c);
void  item_stats_sizes(ADD_STAT add_stats, void *c);
void  item_unlink(item *it);
void  item_update(item *it);

// [branch 003b] Remove item_lock operations (keeping switch_item_lock_type for now)
void switch_item_lock_type(enum item_lock_types type);
// [branch 004] This function is called from a relaxed transaction
// [branch 007] Now this is safe, and uses a transaction internally
__attribute__((transaction_safe))
unsigned short tm_refcount_incr(unsigned short *refcount);
// [branch 004] This function is called from a relaxed transaction
// [branch 007] Now this is safe, and uses a transaction internally
__attribute__((transaction_safe))
unsigned short tm_refcount_decr(unsigned short *refcount);
// [branch 002] removed headers for STATS_LOCK and STATS_UNLOCK
void threadlocal_stats_reset(void);
// [branch 004] This function is called from a relaxed transaction
// [branch 010] With safe threadlocal stat locks, this becomes safe
__attribute__((transaction_safe))
void threadlocal_stats_aggregate(struct thread_stats *stats);
void slab_stats_aggregate(struct thread_stats *stats, struct slab_stats *out);

/* Stat processing functions */
// [branch 004] This function is called from a relaxed transaction
// [branch 011] No annotation any more
void append_stat(const char *name, ADD_STAT add_stats, conn *c,
                 const char *fmt, ...);

// [branch 011] safe clones of append_stat without varargs
__attribute__((transaction_safe))
void append_stat_llu(const char *name, ADD_STAT add_stats, conn *c,
                     const char *fmt, unsigned long long val);
__attribute__((transaction_safe))
void append_stat_u(const char *name, ADD_STAT add_stats, conn *c,
                   const char *fmt, unsigned int val);
__attribute__((transaction_safe))
void append_stat_d(const char *name, ADD_STAT add_stats, conn *c,
                   const char *fmt, int val);
__attribute__((transaction_safe))
void append_stat_lu(const char *name, ADD_STAT add_stats, conn *c,
                    const char *fmt, long val);
__attribute__((transaction_safe))
void append_stat_ld(const char *name, ADD_STAT add_stats, conn *c,
                    const char *fmt, long val);
__attribute__((transaction_safe))
void append_stat_s(const char *name, ADD_STAT add_stats, conn *c,
                   const char *fmt, const char *val);
__attribute__((transaction_safe))
void append_stat_ld_ld(const char *name, ADD_STAT add_stats, conn *c,
                       const char *fmt, long val1, long val2);
// [branch 011] These internal stat helper functions are useful in other
//              places, so they shouldn't be static
__attribute__((transaction_pure))
int tm_snprintf_d(char *str, size_t size, const char *format, int val);
__attribute__((transaction_pure))
int tm_snprintf_llu(char *str, size_t size, const char *format,
                    unsigned long long val);
__attribute__((transaction_pure))
int tm_snprintf_u(char *str, size_t size, const char *format,
                  unsigned int val);
__attribute__((transaction_pure))
int tm_snprintf_ds(char *str, size_t size, const char *format,
                   int val1, const char *val2);
// [branch 011] A snprintf variant for do_item_cachedump
__attribute__((transaction_pure))
int tm_snprintf_s_d_lu(char *str, size_t size, const char *format,
                       const char *val1, int val2, unsigned long val3);
__attribute__((transaction_pure))
int tm_snprintf_s_llu_llu_llu_llu(char *str, size_t size, const char *format,
                                 const char *val1, uint64_t val2, uint64_t val3,
                                 uint64_t val4, uint64_t val5);
// [branch 011b] This one is just for the b branch
__attribute__((transaction_pure))
int tm_snprintf_d_d(char *str, size_t size, const char *format, int val1, int val2);

enum store_item_type store_item(item *item, int comm, conn *c);

#if HAVE_DROP_PRIVILEGES
extern void drop_privileges(void);
#else
#define drop_privileges()
#endif

/* If supported, give compiler hints for branch prediction. */
#if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#define __builtin_expect(x, expected_value) (x)
#endif

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

static LIBEVENT_THREAD *threads;

void threadlocal_stats_aggregate(struct thread_stats *stats) {
	int ii, sid;

	for (ii = 0; ii < 10; ++ii) {
		__transaction_atomic {
			for (sid = 0; sid < 20; sid++) {
				stats->slab_stats[sid].set_cmds +=
					threads[ii].stats.slab_stats[sid].set_cmds;
			}
		}
	}
}
