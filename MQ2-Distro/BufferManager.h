//
// Manage a sequence of items as a blob.
//

#pragma once

#include <memory>

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
                //
                // If there is a buffer, release it.
                //

                if (m_Buffer.second)
                {
                    std::return_temporary_buffer(m_Buffer.first);
                }
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
                return m_Buffer.first;
            }

        private:

            //
            // Copy an array to our buffer.
            //

            void CopyToBuffer(const T *array, size_t elements)
            {
                //
                // If there is a buffer, release it before allocating a new
                // one.
                //

                if (m_Buffer.second)
                {
                    std::return_temporary_buffer(m_Buffer.first);
                }

                m_Buffer = std::get_temporary_buffer<T>(elements);

                //
                // Copy the array to the buffer.
                //

                std::uninitialized_copy(array, array + m_Buffer.second, m_Buffer.first);
            }

            //
            // Pointer to a temporary buffer - allocated via
            // std::get_temporary_buffer.
            //

            std::pair<T *, std::ptrdiff_t> m_Buffer;
        };
    }  // namespace Buffers
}  // namespace Utilities
