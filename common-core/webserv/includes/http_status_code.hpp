/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_code.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:54:54 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/05 17:54:56 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_STATUS_CODE_HPP
#define HTTP_STATUS_CODE_HPP

/**
 * Based on IETF RFCs and HTTP/1.1 specification
 */
enum StatusCode {
    // 2xx Success
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_ACCEPTED = 202,
    HTTP_NON_AUTHORITATIVE_INFORMATION = 203,
    HTTP_NO_CONTENT = 204,
    HTTP_RESET_CONTENT = 205,
    HTTP_PARTIAL_CONTENT = 206,

    HTTP_MULTIPLE_CHOICES = 300,
    HTTP_MOVED_PERMANENTLY = 301,
    HTTP_FOUND = 302,
    HTTP_SEE_OTHER = 303,
    HTTP_NOT_MODIFIED = 304,
    HTTP_USE_PROXY = 305,
    HTTP_TEMPORARY_REDIRECT = 307,

    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_PAYMENT_REQUIRED = 402,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_NOT_ACCEPTABLE = 406,
    HTTP_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_REQUEST_TIMEOUT = 408,
    HTTP_CONFLICT = 409,
    HTTP_GONE = 410,
    HTTP_LENGTH_REQUIRED = 411,
    HTTP_PRECONDITION_FAILED = 412,
    HTTP_REQUEST_ENTITY_TOO_LARGE = 413,
    HTTP_REQUEST_URI_TOO_LONG = 414,
    HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    HTTP_EXPECTATION_FAILED = 417,
    HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,

    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_BAD_GATEWAY = 502,
    HTTP_SERVICE_UNAVAILABLE = 503,
    HTTP_GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

/**
 * Returns the standard reason phrase for a given HTTP status code
 * @param HTTP status code
 * @return The corresponding reason phrase string
 */
inline const char* GetHttpStatusText(StatusCode status) {
    switch (status) {
        // 2xx Success
        case HTTP_OK: return "OK";
        case HTTP_CREATED: return "Created";
        case HTTP_ACCEPTED: return "Accepted";
        case HTTP_NON_AUTHORITATIVE_INFORMATION:
            return "Non-Authoritative Information";
        case HTTP_NO_CONTENT: return "No Content";
        case HTTP_RESET_CONTENT: return "Reset Content";
        case HTTP_PARTIAL_CONTENT: return "Partial Content";

        // 3xx Redirection
        case HTTP_MULTIPLE_CHOICES: return "Multiple Choices";
        case HTTP_MOVED_PERMANENTLY: return "Moved Permanently";
        case HTTP_FOUND: return "Found";
        case HTTP_SEE_OTHER: return "See Other";
        case HTTP_NOT_MODIFIED: return "Not Modified";
        case HTTP_USE_PROXY: return "Use Proxy";
        case HTTP_TEMPORARY_REDIRECT: return "Temporary Redirect";

        // 4xx Client Error
        case HTTP_BAD_REQUEST: return "Bad Request";
        case HTTP_UNAUTHORIZED: return "Unauthorized";
        case HTTP_PAYMENT_REQUIRED: return "Payment Required";
        case HTTP_FORBIDDEN: return "Forbidden";
        case HTTP_NOT_FOUND: return "Not Found";
        case HTTP_METHOD_NOT_ALLOWED: return "Method Not Allowed";
        case HTTP_NOT_ACCEPTABLE: return "Not Acceptable";
        case HTTP_PROXY_AUTHENTICATION_REQUIRED:
            return "Proxy Authentication Required";
        case HTTP_REQUEST_TIMEOUT: return "Request Timeout";
        case HTTP_CONFLICT: return "Conflict";
        case HTTP_GONE: return "Gone";
        case HTTP_LENGTH_REQUIRED: return "Length Required";
        case HTTP_PRECONDITION_FAILED: return "Precondition Failed";
        case HTTP_REQUEST_ENTITY_TOO_LARGE: return "Request Entity Too Large";
        case HTTP_REQUEST_URI_TOO_LONG: return "Request-URI Too Long";
        case HTTP_UNSUPPORTED_MEDIA_TYPE: return "Unsupported Media Type";
        case HTTP_REQUESTED_RANGE_NOT_SATISFIABLE:
            return "Requested Range Not Satisfiable";
        case HTTP_EXPECTATION_FAILED: return "Expectation Failed";
        case HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE:
            return "Request Header Fields Too Large";

        // 5xx Server Error
        case HTTP_INTERNAL_SERVER_ERROR: return "Internal Server Error";
        case HTTP_NOT_IMPLEMENTED: return "Not Implemented";
        case HTTP_BAD_GATEWAY: return "Bad Gateway";
        case HTTP_SERVICE_UNAVAILABLE: return "Service Unavailable";
        case HTTP_GATEWAY_TIMEOUT: return "Gateway Timeout";
        case HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported";

        default: return "Unknown Status Code";
    }
}

#endif  // HTTP_STATUS_CODE_HPP
