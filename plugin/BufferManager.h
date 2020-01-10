//
// Manage a sequence of items as a blob.
//

#pragma once

#include "DebugMemory.h"

#include <vector>

namespace Utilities
{
    namespace Buffers
    {
        //
        // Manage a buffer of items of a homogeneous type.
        //

        template<typename T>
        class BufferManager
        {
        public:

            //
            // Constructor.
            //

            BufferManager()
            {
            }

            //
            // Initialize the buffer from an array.
            //

            BufferManager(const T *array, size_t elements)
            {
                CopyToBuffer(array, elements);
            }

            //
            // Destructor.
            //

            virtual ~BufferManager()
            {
            }

            //
            // Set contents of the current buffer and return its pointer.
            //

            const T *SetBuffer(const T *array, size_t elements)
            {
                CopyToBuffer(array, elements);
                return GetBufferPointer();
            }

            //
            // Return the pointer to our buffer.
            //

            const T *GetBufferPointer() const
            {
                return m_Buffer.data();
            }

        private:

            //
            // Copy an array to our buffer.
            //

            void CopyToBuffer(const T *array, size_t elements)
            {
                if (elements > 0)
                {
                    //
                    // Allocate vector elements.
                    //

                    m_Buffer.resize(elements);

                    //
                    // Copy the array to the buffer.
                    //

                    std::uninitialized_copy(array, array + elements, m_Buffer.data());
                }
            }

            //
            // Pointer to a temporary buffer - allocated via
            // std::get_temporary_buffer.
            //

            std::vector<T> m_Buffer;
        };
    }  // namespace Buffers
}  // namespace Utilities
