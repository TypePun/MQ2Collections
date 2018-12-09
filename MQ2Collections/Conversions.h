//
// Provide classes and functions to perform data type conversions.
//

#pragma once

#if !defined(__CONVERSION__)
#define __CONVERSIONS__

#include <cstdlib>
#include <cstdint>
#include <cerrno>

namespace Conversions
{
    //
    // Specialization for longs. Delegate to _ltoa_s for implementation.
    //

    template<size_t size>
    inline errno_t ValueToString (long value,
                char (&buffer)[size],
                uint16_t radix)
    {
        // Radix must be 2 <= radix <= 36 or an error is returned.
        return _ltoa_s (value, temp_buffer, (int)radix);
    }

    //
    // Specialization for unsigned longs. Delegate to _ltoa_s for implementation.
    //

    template<size_t size>
    inline errno_t ValueToString (unsigned int value,
                char (&buffer)[size],
                uint16_t radix)
    {
        return ValueToString ((unsigned long)value, buffer, radix);
    }

    //
    // Specialization for unsigned int. Delegate to _ltoa_s for implementation.
    //

    template<size_t size>
    inline errno_t ValueToString (unsigned long value,
        char (&buffer)[size],
        uint16_t radix)
    {
        // Radix must be 2 <= radix <= 36 or an error is returned.
        return _ultoa_s (value, buffer, (int)radix);
    }

    //
    // Convert a scalar value to an ASCII string.
    //
    // Returns
    //    0                     Successful completion.
    //    EINVAL                Invalid argument
    //    ERANGE                The output buffer is too short to contain the
    //                          result string.

    template<typename T>
    inline errno_t ToString (T value,
                char * buffer,
                size_t buff_size,
                uint16_t radix = 10)
    {
        char temp_buffer[_MAX_PATH];        // More than enough for any scalar.
        errno_t rc;

        // Radix must be 2 <= radix <= 36.
        rc = ValueToString (value, temp_buffer, radix);
        if (rc == 0)
        {
            rc = strcpy_s (buffer, buff_size, temp_buffer);
        }
        
        return rc;
    }
}
#endif

