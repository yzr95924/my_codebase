/**
 * @file inMemoryIndex.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement a in-memory index
 * @version 0.1
 * @date 2021-05-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MY_CODEBASE_IN_MEMORY_DB_H
#define MY_CODEBASE_IN_MEMORY_DB_H

#include "abs_db.h"

class InMemoryDatabase : public AbsDatabase {
    protected:
        string my_name_ = "InMemoryDatabase";
        /*data*/
        unordered_map<string, string> index_obj_;

    public:
        /**
         * @brief Construct a new In Memory Database object
         * 
         * @param db_name the path of the db file
         */
        InMemoryDatabase(string db_name);

        /**
         * @brief Destroy the In Memory Database object
         * 
         */
        virtual ~InMemoryDatabase();

        /**
         * @brief open a database
         * 
         * @param db_name the db path 
         * @return true success
         * @return false fail
         */
        virtual bool OpenDB(string db_name) = 0;

        /**
         * @brief execute query over database
         * 
         * @param key key
         * @param value value
         * @return true exist
         * @return false not exist
         */
        bool Query(const string& key, string& value);

        /**
         * @brief insert the (key, value) pair
         * 
         * @param key key
         * @param value value
         * @return true exist
         * @return false not exist
         */
        bool Insert(const string& key, const string& value);

        /**
         * @brief insert the (key, value) pair
         * 
         * @param key the key 
         * @param buf the value buffer
         * @param buf_size the buffer size
         * @return true exist
         * @return false not exist
         */
        bool InsertBuffer(const string& key, const char* buf, size_t buf_size);

        /**
         * @brief insert the (key, value) pair
         * 
         * @param key the key
         * @param key_size the key size
         * @param buf the value buffer
         * @param buf_size the buffer size
         * @return true exist
         * @return false not exist
         */
        bool InsertBothBuffer(const char* key, size_t key_size, const char* buf,
            size_t buf_size);

        /**
         * @brief query the (key, value) pair
         * 
         * @param key the key
         * @param key_size the key size
         * @param value the value
         * @return true exist
         * @return false not exist
         */
        bool QueryBuffer(const char* key, size_t key_size, string& value);
};

#endif