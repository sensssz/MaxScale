#pragma once
#ifndef _MAXSCALE_FILTER_CACHE_RULES_H
#define _MAXSCALE_FILTER_CACHE_RULES_H
/*
 * Copyright (c) 2016 MariaDB Corporation Ab
 *
 * Use of this software is governed by the Business Source License included
 * in the LICENSE.TXT file and at www.mariadb.com/bsl.
 *
 * Change Date: 2019-07-01
 *
 * On the date above, in accordance with the Business Source License, use
 * of this software will be governed by version 2 or later of the General
 * Public License.
 */

#include <maxscale/cdefs.h>
#include <stdbool.h>
#include <jansson.h>
#include <maxscale/buffer.h>
#include <maxscale/session.h>
#include <maxscale/pcre2.h>

MXS_BEGIN_DECLS

typedef enum cache_rule_attribute
{
    CACHE_ATTRIBUTE_COLUMN,
    CACHE_ATTRIBUTE_DATABASE,
    CACHE_ATTRIBUTE_QUERY,
    CACHE_ATTRIBUTE_TABLE,
    CACHE_ATTRIBUTE_USER,
} cache_rule_attribute_t;

typedef enum cache_rule_op
{
    CACHE_OP_EQ,
    CACHE_OP_NEQ,
    CACHE_OP_LIKE,
    CACHE_OP_UNLIKE
} cache_rule_op_t;


typedef struct cache_rule
{
    cache_rule_attribute_t attribute; // What attribute is evalued.
    cache_rule_op_t        op;        // What operator is used.
    char                  *value;     // The value from the rule file.
    struct
    {
        char *database;
        char *table;
        char *column;
    } simple;                         // Details, only for CACHE_OP_[EQ|NEQ]
    struct
    {
        pcre2_code       *code;
        pcre2_match_data *data;
    } regexp;                         // Regexp data, only for CACHE_OP_[LIKE|UNLIKE].
    uint32_t               debug;     // The debug level.
    struct cache_rule     *next;
} CACHE_RULE;

typedef struct cache_rules
{
    uint32_t    debug;        // The debug level.
    CACHE_RULE *store_rules;  // The rules for when to store data to the cache.
    CACHE_RULE *use_rules;    // The rules for when to use data from the cache.
} CACHE_RULES;

const char *cache_rule_attribute_to_string(cache_rule_attribute_t attribute);
const char *cache_rule_op_to_string(cache_rule_op_t op);

CACHE_RULES *cache_rules_create(uint32_t debug);
void cache_rules_free(CACHE_RULES *rules);

CACHE_RULES *cache_rules_load(const char *path, uint32_t debug);
CACHE_RULES *cache_rules_parse(const char *json, uint32_t debug);

bool cache_rules_should_store(CACHE_RULES *rules, const char *default_db, const GWBUF* query);
bool cache_rules_should_use(CACHE_RULES *rules, const SESSION *session);

MXS_END_DECLS

#endif