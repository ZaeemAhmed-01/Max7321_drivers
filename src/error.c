#include "error.h"



const char *errToString(const ErrorState_t error)
{
    switch (error)
    {
    case STATE_SUCCESS:             return "STATE_SUCCESS";
    case STATE_FAIL:                return "STATE_FAIL";
    case STATE_PARSE_FAILED:        return "STATE_PARSE_FAILED";
    case STATE_RETRY_EXCEEDED:      return "STATE_RETRY_EXCEEDED";
    case STATE_NULL_POINTER:        return "STATE_NULL_POINTER";
    case STATE_WOULD_BLOCK:         return "STATE_WOULD_BLOCK";
    case STATE_WRITE_FAILED:        return "STATE_WRITE_FAILED";
    case STATE_TIMEOUT:             return "STATE_TIMEOUT";
    case STATE_READ_FAILED:         return "STATE_READ_FAILED";
    case STATE_PORT_UNAVAILABLE:    return "STATE_PORT_UNAVAILABLE";
    case STATE_INVALID_LENGTH:      return "STATE_INVALID_LENGTH";
    case STATE_INVALID_DATA:        return "STATE_INVALID_DATA";
    case STATE_QUEUE_FULL:          return "STATE_QUEUE_FULL";
    case STATE_QUEUE_EMPTY:         return "STATE_QUEUE_EMPTY";
    case STATE_INCOMPLETE_DATA:     return "STATE_INCOMPLETE_DATA";
    case STATE_DATA_DISCARDED:      return "STATE_DATA_DISCARDED";
    default:                        return "UNKNOWN_ERROR";
    }
}