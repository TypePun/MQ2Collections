//
// Collections.h
//
// Define the base interfaces for collection classes.
//

#pragma once

#include <memory>

namespace Collections
{ 
	//
	// Define a base class for an iterator on a collection.
	//
	// An iterator has the following methods and properties:
	//
	// Reset		- Position the iterator to the beginning of the collection.
	// End			- Position the iterator to the end of the collection.
	// Next			- Advance the iterator to the next element.
	// IsEnd		- Is the iterator at the end of the collection?
	//

	template<class coll>
	class IteratorBase
	{
	public:

		//
		// Make an empty iterator.
		//

		explicit IteratorBase(const coll & collection)
			: m_refCollection(collection)
		{
			Reset();
		}

		//
		// Destroy the iterator.
		//

		virtual ~IteratorBase()
		{
		}

		//
		// Reset the iterator to the start of the collection.
		//

		virtual void Reset()
		{
			m_iterator = m_refCollection.begin();
		}

		//
		// Advance the iterator.  Return true if the iterator could be advanced
		// and false otherwise.
		//

		virtual bool Advance()
		{
			if (m_iterator != m_refCollection.end())
			{
				++m_iterator;
				return true;
			}

			return false;
		}

		//
		// Return true if the iterator is at the end of the collection.
		//

		virtual bool IsEnd() const
		{
			return m_refCollection.end() == m_iterator;
		}

	protected:

		//
		// Iterator on m_refCollection.
		//

		typename coll::const_iterator  m_iterator;

		//
		// Reference to the underlying collection.
		//

		const coll & m_refCollection;
	};

	//
	// Define an iterator on a collection that only returns values.  For
	// example, an iterator on a set.
	//
	// A value iterator has methods of the IteratorBase class plus:
	//
	// Value		- Return the value under the current iterator
	//

	template<class coll, typename ValueType = coll::value_type>
	class ValueIterator : public IteratorBase<coll>
	{
	public:

		//
		// Make an empty iterator.
		//

		explicit ValueIterator(const coll & collection)
			: IteratorBase(collection)
		{
		}

		//
		// Destroy the iterator.
		//

		virtual ~ValueIterator()
		{
		}

		//
		// Return the value under the current iterator through the item parameter.
		// True is returned if item is valid and false if it is not.  Item is
		// not valid if the iterator points after the end of the collection.
		//

		virtual bool Value(const ValueType ** const item) const = 0;
	};

	//
	// Define an iterator on a collection that returns keys and values.  For
	// example, an iterator on a map.
	//
	// A value iterator has methods of the ValueIterator class plus:
	//
	// Key		- Return the key under the current iterator
	//

	template<class coll, typename KeyType, typename ValueType>
	class KeyValueIterator : public ValueIterator<coll, ValueType>
	{
	public:

		//
		// Make an empty iterator.
		//

		explicit KeyValueIterator(const coll & collection)
			: ValueIterator(collection)
		{
		}

		//
		// Destroy the iterator.
		//

		virtual ~KeyValueIterator()
		{
		}

		//
		// Return the key under the current iterator through the key parameter.
		// True is returned if key is valid and false if it is not.  Key is
		// not valid if the iterator points after the end of the collection,
		// or if the collection in question doesn't have keys, such as an
		// array or list.
		//

		virtual bool Key(const KeyType ** const key) const = 0;
	};

	//
	// A Collection defines the following methods and properties:
	//
	// Count		- Return the number of elements in the collection
	// Clear		- Delete all elements in the collection
	// First		- Return an Iterator referencing the first element.
	//

	template<
			typename coll,
			typename ValueType = coll::value_type,
			typename KeyType = ValueType,
			typename IteratorType = IteratorBase<coll>
	>
	class Collection
	{
	public:

		//
		// Make an empty collection.
		//

		Collection(void)
		{
		}

		//
		// Delete the collection.
		//

		virtual ~Collection()
		{
			//
			// Clear our container.
			//

			Clear();
		}

		//
		// Return the count of elements in the collection.
		//

		virtual size_t Count() const
		{
			return (size_t)m_coll.size();
		}

		//
		// Clear (delete all elements) in the collection.
		//

		virtual void Clear()
		{
			m_coll.clear();
		}

		//
		// Return a pointer to an iterator to the first element in the
		// collection or an iterator for which IsEnd is true.
		//

		virtual std::unique_ptr<IteratorType> First() const
		{
			return GetNewIterator(m_coll);
		}

	protected:

		//
		// Derived class must implement a method that creates a new Iterator
		// appropriate to that class.
		//

		virtual std::unique_ptr<IteratorType> GetNewIterator(
								const coll & refCollection
							) const = 0;

		//
		// Container maintained by the collection.
		//

		coll m_coll;
	};
}  // namespace Collections
