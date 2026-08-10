#pragma once





/**
 * @brief This enum is used to define possible error state a function could return.
 *
 * These are generic and will be used throughout the program as the return types of \
 * the functions and any point where an error condition should be defined.
 * 
 */
typedef enum ErrorState
{
    STATE_SUCCESS = 0,
    STATE_FAIL,
    STATE_PARSE_FAILED,
    STATE_RETRY_EXCEEDED,
    STATE_NULL_POINTER,
    // -- //
    STATE_WOULD_BLOCK,
    STATE_WRITE_FAILED,
    STATE_TIMEOUT,
    STATE_READ_FAILED,
    STATE_PORT_UNAVAILABLE,
    STATE_INVALID_LENGTH,
    STATE_INVALID_DATA,
    STATE_QUEUE_FULL,
    STATE_QUEUE_EMPTY,
    STATE_INCOMPLETE_DATA,
    STATE_DATA_DISCARDED
}ErrorState_t;



/**
 * @brief Converts ErrorState to readable text.
 *
 * @param error Error code.
 *
 * @return Constant string representation.
 */
const char *errToString(ErrorState_t error);