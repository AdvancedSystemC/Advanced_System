//
// Created by agerberon on 18/10/23.
//

#ifndef COMMON_H
#define COMMON_H

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \param bin_name Name of the binary
 */
void print_usage(char* bin_name);

/**
 * Procedure checks if a variable needs to be freed (checks: ptr != NULL)
 *
 * \param to_free Pointer to an allocated memory
 */
void free_if_needed(void* to_free);

/**
 * Duplicate optarg as a string, handling errors
 *
 * \return Duplicated string or NULL in case of error
 */
char* dup_optarg_str();

void print_file_metadata(const char* filename);

#endif
