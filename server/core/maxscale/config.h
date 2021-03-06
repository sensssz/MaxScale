#pragma once
/*
 * Copyright (c) 2016 MariaDB Corporation Ab
 *
 * Use of this software is governed by the Business Source License included
 * in the LICENSE.TXT file and at www.mariadb.com/bsl11.
 *
 * Change Date: 2019-07-01
 *
 * On the date above, in accordance with the Business Source License, use
 * of this software will be governed by version 2 or later of the General
 * Public License.
 */

/**
 * @file core/maxscale/config.h - The private config interface
 */

#include <maxscale/config.h>

#include <maxscale/ssl.h>

MXS_BEGIN_DECLS

#define DEFAULT_NBPOLLS         3       /**< Default number of non block polls before we block */
#define DEFAULT_POLLSLEEP       1000    /**< Default poll wait time (milliseconds) */
#define DEFAULT_NTHREADS        1       /**< Default number of polling threads */

/**
 * @brief Generate default module parameters
 *
 * Adds any default parameters to @c ctx that aren't already in it.
 *
 * @param ctx    Configuration context where the parameters are added
 * @param params Module parameters
 */
void config_add_defaults(CONFIG_CONTEXT *ctx, const MXS_MODULE_PARAM *params);

char*                 config_clean_string_list(const char* str);
MXS_CONFIG_PARAMETER* config_clone_param(const MXS_CONFIG_PARAMETER* param);
bool                  config_load(const char *);
void                  config_parameter_free(MXS_CONFIG_PARAMETER* p1);

/**
 * @brief Creates an empty configuration context
 *
 * @param section Context name
 * @return New context or NULL on memory allocation failure
 */
CONFIG_CONTEXT* config_context_create(const char *section);

/**
 * @brief Free a configuration context
 *
 * @param context The context to free
 */
void config_context_free(CONFIG_CONTEXT *context);

/**
 * @brief Add a parameter to a configuration context
 *
 * @param obj Context where the parameter should be added
 * @param key Key to add
 * @param value Value for the key
 * @return True on success, false on memory allocation error
 */
bool config_add_param(CONFIG_CONTEXT* obj, const char* key, const char* value);

/**
 * @brief Append to an existing parameter
 *
 * @param obj Configuration context
 * @param key Parameter name
 * @param value Value to append to the parameter
 * @return True on success, false on memory allocation error
 */
bool config_append_param(CONFIG_CONTEXT* obj, const char* key, const char* value);

/**
 * @brief Construct an SSL structure
 *
 * The SSL structure is used by both listeners and servers.
 *
 * TODO: Rename to something like @c config_construct_ssl
 *
 * @param obj Configuration context
 * @param require_cert Whether certificates are required
 * @param error_count Pointer to an int which is incremented for each error
 * @return New SSL_LISTENER structure or NULL on error
 */
SSL_LISTENER *make_ssl_structure(CONFIG_CONTEXT *obj, bool require_cert, int *error_count);

/**
 * @brief Check if all SSL parameters are defined
 *
 * Helper function to check whether all of the required SSL parameters are defined
 * in the configuration context. The checked parameters are 'ssl', 'ssl_key',
 * 'ssl_cert' and 'ssl_ca_cert'. The 'ssl' parameter must also have a value of
 * 'required'.
 *
 * @param obj Configuration context
 * @return True if all required parameters are present
 */
bool config_have_required_ssl_params(CONFIG_CONTEXT *obj);

MXS_END_DECLS
