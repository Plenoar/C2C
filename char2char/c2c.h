#ifndef RY_AUDIO_C2C_H
#define RY_AUDIO_C2C_H

#include <stddef.h>  // size_t
#include <stdbool.h> // bool

/**
 * @brief Opaque handle for the C2C Map.
 * The internal structure is hidden to abstract the hash implementation.
 */
typedef struct C2C C2C;

/**
 * @brief Function pointer type for iteration callbacks.
 * @param name The key (asset name).
 * @param path The value (file path).
 * @param user_data Pointer passed through from c2c_foreach (context).
 */
typedef void (*c2c_callback)(const char *name, const char *path, void *user_data);

/**
 * @brief Create and initialize a new C2C map.
 * @return Pointer to C2C instance, or NULL on memory failure.
 */
C2C* c2c_create(void);

/**
 * @brief Free the map and all strings contained within.
 */
void c2c_destroy(C2C *c2c);

/**
 * @brief Insert a name-path pair.
 * @note  Copies strings. Fails if name already exists.
 * @return 1 on success, 0 on memory error, -1 on duplicate name.
 */
int c2c_insert(C2C *c2c, const char *name, const char *path);

/**
 * @brief Update an existing name with a new path.
 * @return 1 on success, 0 on memory error, -1 if name not found.
 */
int c2c_update(C2C *c2c, const char *name, const char *new_path);

/**
 * @brief Retrieve the stored path by name.
 * @return The path string if found, NULL otherwise.
 */
const char* c2c_get(const C2C *c2c, const char *name);

/**
 * @brief Remove entry by name.
 * @return 1 on success, 0 if not found.
 */
int c2c_remove(C2C *c2c, const char *name);

/**
 * @brief Get the total number of items in the map.
 */
size_t c2c_count(const C2C *c2c);

/**
 * @brief Iterate over every item in the map.
 * @param c2c The map instance.
 * @param cb The callback function to execute for each item.
 * @param user_data Optional context pointer (can be NULL) passed to the callback.
 */
void c2c_foreach(C2C *c2c, c2c_callback cb, void *user_data);

/**
 * @brief Bind a runtime pointer (buffer) to an existing entry.
 * @param ptr The generic pointer to store (e.g., your AudioBuffer*).
 * @return 1 on success, 0 if the name doesn't exist.
 */
int c2c_bind_ptr(C2C *c2c, const char *name, void *ptr);

/**
 * @brief Retrieve the runtime pointer associated with the name.
 * @return The pointer, or NULL if not set or name not found.
 */
void* c2c_get_ptr(const C2C *c2c, const char *name);

#endif // RY_AUDIO_C2C_H


