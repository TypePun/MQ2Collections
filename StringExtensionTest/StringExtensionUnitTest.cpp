#include "stdafx.h"
#include "CppUnitTest.h"

#include <string>
#include <vector>
#include <list>
#include <memory>

#include "StringExtensions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Extensions::Strings;

namespace StringExtensionTest
{	
	//
	// Test Split methods returning a stl::vector.
	//

	TEST_CLASS(StringExtensionVectorSplit)
	{
	public:
		
		TEST_METHOD(SplitOnVector1)
		{
			StringExtensions<std::string> extension("Hello, world!");
			std::unique_ptr<StringExtensions<std::string>::container_type> container;

			//
			// Split the string into a vector.
			//

			container.reset(extension.Split());

			//
			// There must only be two items in the returned vector.
			//

			Assert::IsTrue(container->size() == 2, L"Expected Split() to return 2 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnVector2)
		{
			StringExtensions<std::string> extension("Hello, I said, to the damsel, or so I thought.");
			std::unique_ptr<StringExtensions<std::string>::container_type> container;

			//
			// Split the string into a vectors.
			//

			container.reset(extension.Split());

			//
			// There must only be four items in the returned vector.
			//

			Assert::IsTrue(container->size() == 10, L"Expected Split() to return 10 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnVector3)
		{
			StringExtensions<std::string> extension("Hello, I said, to the damsel, or so I thought.");
			std::unique_ptr<StringExtensions<std::string>::container_type> container;

			//
			// Split the string into a vectors where the separators are , and space and remove
			// the empty strings -- there will be empty strings where a space follows a comma.
			//

			container.reset(extension.Split(std::string(", "), true));

			//
			// There should be ten items in the returned vector.
			//

			Assert::IsTrue(container->size() == 10, L"Expected Split() to return 10 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnVector4)
		{
			StringExtensions<std::string> extension(",,,,,");
			std::unique_ptr<StringExtensions<std::string>::container_type> container;

			//
			// Split the separators, removing empty strings.  An empty vector
			// should be returned.
			//

			container.reset(extension.Split(std::string(","), true));

			//
			// There should be no items in the returned vector.
			//

			Assert::IsTrue(container->size() == 0, L"Expected Split() to return 0 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnVectorEmpty)
		{
			StringExtensions<std::string> extension("");
			std::unique_ptr<StringExtensions<std::string>::container_type> container;

			//
			// Split the empty string - there should be a vector containing an empty string.
			//

			container.reset(extension.Split());

			Assert::IsTrue(container->size() == 1, L"Expected Split() to return 1 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnVectorNoSeparators)
		{
			StringExtensions<std::string> extension("This is a string with no separators.");
			std::unique_ptr<StringExtensions<std::string>::container_type> container;

			//
			// Split the string using a comma - there should be an empty vector.
			//

			container.reset(extension.Split(std::string(",")));

			Assert::IsTrue(container->size() == 1, L"Expected Split() to return 1 elements.", LINE_INFO());
		}
	};

	//
	// Test split methods returning a stl::list.
	//

	TEST_CLASS(StringExtensionListSplit)
	{
	public:

		TEST_METHOD(SplitOnList1)
		{
			StringExtensions<std::string, std::list<std::string>> extension("Hello, world!");
			std::unique_ptr<StringExtensions<std::string, std::list<std::string>>::container_type> container;

			//
			// Split the string into a vector.
			//

			container.reset(extension.Split());

			//
			// There must only be two items in the returned vector.
			//

			Assert::IsTrue(container->size() == 2, L"Expected Split() to return 2 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnList2)
		{
			StringExtensions<std::string, std::list<std::string>> extension("Hello, I said, to the damsel, or so I thought.");
			std::unique_ptr<StringExtensions<std::string, std::list<std::string>>::container_type> container;

			//
			// Split the string into a vectors.
			//

			container.reset(extension.Split());

			//
			// There must only be four items in the returned vector.
			//

			Assert::IsTrue(container->size() == 10, L"Expected Split() to return 10 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnList3)
		{
			StringExtensions<std::string, std::list<std::string>> extension("Hello, I said, to the damsel, or so I thought.");
			std::unique_ptr<StringExtensions<std::string, std::list<std::string>>::container_type> container;

			//
			// Split the string into a vectors where the separators are , and space and remove
			// the empty strings -- there will be empty strings where a space follows a comma.
			//

			container.reset(extension.Split(std::string(", "), true));

			//
			// There should be ten items in the returned vector.
			//

			Assert::IsTrue(container->size() == 10, L"Expected Split() to return 10 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnList4)
		{
			StringExtensions<std::string, std::list<std::string>> extension(",,,,,");
			std::unique_ptr<StringExtensions<std::string, std::list<std::string>>::container_type> container;

			//
			// Split the separators, removing empty strings.  An empty vector
			// should be returned.
			//

			container.reset(extension.Split(std::string(","), true));

			//
			// There should be no items in the returned vector.
			//

			Assert::IsTrue(container->size() == 0, L"Expected Split() to return 0 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnListEmpty)
		{
			StringExtensions<std::string, std::list<std::string>> extension("");
			std::unique_ptr<StringExtensions<std::string, std::list<std::string>>::container_type> container;

			//
			// Split the empty string - there should be a vector containing only an empty string.
			//

			container.reset(extension.Split());

			Assert::IsTrue(container->size() == 1, L"Expected Split() to return 1 elements.", LINE_INFO());
		}

		TEST_METHOD(SplitOnListNoSeparators)
		{
			StringExtensions<std::string, std::list<std::string>> extension("This is a string with no separators.");
			std::unique_ptr<StringExtensions<std::string, std::list<std::string>>::container_type> container;

			//
			// Split the string using a comma - there should be a vector with one element.
			//

			container.reset(extension.Split(std::string(",")));

			Assert::IsTrue(container->size() == 1, L"Expected Split() to return 1 elements.", LINE_INFO());
		}
	};

	//
	// Test trim methods.
	//

	TEST_CLASS(StringExtensionTrim)
	{
	public:

		TEST_METHOD(TrimStartVectorNoTrim)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("Hello, world!");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.TrimStart());

			Assert::AreEqual(extension.Contents(), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimStartVector1)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("     Hello, world!");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the results to be different.
			//

			newExtension.reset(extension.TrimStart());

			Assert::AreNotEqual(extension.Contents(), newExtension->Contents(), L"Expected trimmed instance to not be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimStartVector2)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("ABCDEF1234567890");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.TrimStart(std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			Assert::AreEqual(std::string("1234567890"), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimEndVectorNoTrim)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("Hello, world!");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.TrimEnd());

			Assert::AreEqual(extension.Contents(), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimEndVector1)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("Hello, world!      ");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the results to be different.
			//

			newExtension.reset(extension.TrimEnd());

			Assert::AreNotEqual(extension.Contents(), newExtension->Contents(), L"Expected trimmed instance to not be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimEndVector2)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("ABCDEF1234567890");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.TrimEnd(std::string("0123456789")));

			Assert::AreEqual(std::string("ABCDEF"), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimVectorNoTrim)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("Hello, world!");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.Trim());

			Assert::AreEqual(extension.Contents(), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimVector1)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("    Hello, world!    ");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.Trim());

			Assert::AreEqual(std::string("Hello, world!"), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}

		TEST_METHOD(TrimVector2)
		{
			StringExtensions<std::string, std::vector<std::string>> extension("abcdeHello, world!abcdefghijklmnop");
			std::unique_ptr<StringExtensions<std::string, std::vector<std::string>>> newExtension;

			//
			// Trim the string and expect the result to be the same.
			//

			newExtension.reset(extension.Trim(std::string("abcdefghijklmnopqrstuvwxyz")));

			Assert::AreEqual(std::string("Hello, world!"), newExtension->Contents(), L"Expected trimmed instance to be the same!", LINE_INFO());
		}
	};

	//
	// Test conversion methods.
	//

	TEST_CLASS(StringConversions)
	{
	public:

		TEST_METHOD(ToLong)
		{
			long lResult;

			lResult = -1;

			Assert::IsTrue(FromString(std::string("123"), &lResult));
			Assert::AreEqual(lResult, 123L, L"Expected 123 returned", LINE_INFO());

			Assert::IsTrue(FromString(std::string("     456"), &lResult));
			Assert::AreEqual(lResult, 456L, L"Expected 456 returned", LINE_INFO());

			Assert::IsTrue(FromString(std::string("789    "), &lResult));
			Assert::AreEqual(lResult, 789L, L"Expected 789 returned", LINE_INFO());

			Assert::IsTrue(FromString(std::string("   1010   "), &lResult));
			Assert::AreEqual(lResult, 1010L, L"Expected 1010 returned", LINE_INFO());

			lResult = -1;
			Assert::IsFalse(FromString(std::string("   78910a  "), &lResult));
			Assert::AreEqual(lResult, -1L, L"Expected -1 returned", LINE_INFO());
		}
	};
}