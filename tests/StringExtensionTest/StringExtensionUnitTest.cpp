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
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"StringExtentions", L"Vector Output")
        END_TEST_CLASS_ATTRIBUTE()

        //
        // Test the Split method on 'Hello, world!'. Delimiter is a space.
        //
        // Result: two strings should be returned.
        //

        TEST_METHOD(SplitOnSpaceIntoOneString)
        {
            StringExtensions extension("Hello, world!");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string into a vector.
            //

            container.reset(extension.Split().release());

            //
            // There must only be two items in the returned vector.
            //

            Assert::AreEqual((size_t)2, 
                    container->size(),                            
                    L"Expected Split() to return 2 elements.");
        }

        //
        // Test a complex string delimited by the a space.
        //
        // Result: ten strings should be returned.
        //

        TEST_METHOD(SplitOnSpaceIntoTenStrings)
        {
            StringExtensions extension("Hello, I said, to the damsel, or so I thought.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string into a vectors.
            //

            container.reset(extension.Split().release());

            //
            // There must only be 10 items in the returned vector.
            //

            Assert::AreEqual((size_t)10, 
                    container->size(),                             
                    L"Expected Split() to return 10 elements.");
        }

        //
        // Test a split on both a comma and space, removing empty strings.
        //
        // Result: ten strings should be returned.
        //

        TEST_METHOD(SplitOnCommaAndSpaceRemoveEmpty)
        {
            StringExtensions extension("Hello, I said, to the damsel, or so I thought.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string into a vectors where the separators are , and space and remove
            // the empty strings -- there will be empty strings where a space follows a comma.
            //

            container.reset(extension.Split(std::string(", "), true).release());

            //
            // There should be ten items in the returned vector.
            //

            Assert::AreEqual((size_t)10, 
                    container->size(),                             
                    L"Expected Split() to return 10 elements.");
        }

        //
        // Test that removing empty strings returns an empty vector when
        // the only delimiters are in the input.
        //
        // Result: an empty vector should be returned.
        //

        TEST_METHOD(SplitOnCommaWithOnlyDelimiters)
        {
            StringExtensions extension(",,,,,");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the separators, removing empty strings.  An empty vector
            // should be returned.
            //

            container.reset(extension.Split(std::string(","), true).release());

            //
            // There should be no items in the returned vector.
            //

            Assert::AreEqual((size_t)0,
                    container->size(),                             
                    L"Expected Split() to return 0 elements.");
        }

        //
        // Test split on an empty string.
        //
        // Result: an empty vector should be returned.
        //

        TEST_METHOD(SplitOnEmptyString)
        {
            StringExtensions extension("");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the empty string - there should be a vector containing an empty string.
            //

            container.reset(extension.Split().release());

            Assert::AreEqual((size_t)1,
                    container->size(),                             
                    L"Expected Split() to return 1 elements.");
        }

        //
        // Test a string with no separators.
        //
        // Result: one element (the original string) should be returned.
        //

        TEST_METHOD(SplitWithNoSeparator)
        {
            StringExtensions extension("This is a string with no separators.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a comma - there should be one element returned.
            //

            container.reset(extension.Split(std::string(",")).release());

            Assert::AreEqual((size_t)1, 
                    container->size(),                
                    L"Expected Split() to return 1 elements.");
        }

        //
        // Test a string with an included quoted (\) quote (\\) and verify that
        // the quote is included in the output.
        //
        // Result: one quoted character should be found.
        //

        TEST_METHOD(SplitOnSpaceWithQuotedCharacter)
        {
            //
            // Note: Quote the quoted character to ensure it is included!
            //

            StringExtensions extension("This is a string with a \\\\quoted character.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a comma - there should be one element returned.
            //

            container.reset(extension.Split(std::string(",")).release());

            Assert::AreEqual((size_t)1,
                container->size(),
                L"Expected Split() to return 1 elements.");

            //
            // The input should contain a single quoted ('\').
            //

            Assert::AreEqual((int)1,
                std::count(container->at(0).cbegin(),
                        container->at(0).cend(),
                        '\\'),
                L"Expected to contain one quoted element.");
        }

        //
        // Test a string enclosed in quotation marks. Ensure that the whole string is returned.
        //
        // Result: one string is returned.
        //

        TEST_METHOD(SplitOnSpaceWithSpacesInAQuotedString)
        {
            StringExtensions extension("\"This is a string enclosed in double quotation marks.\"");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a space - there should be one element returned.
            //

            container.reset(extension.Split(std::string(" ")).release());

            Assert::AreEqual((size_t)1,
                container->size(),
                L"Expected Split() to return 1 element.");
        }

        //
        // Test a string with two embedded strings enclosed in quotation marks. Ensure that
        // the quoted strings are returned as individual items.
        //
        // Result: three strings are returned.
        //

        TEST_METHOD(SplitOnSpaceWithMixedQuotedAndUnquotedStrings)
        {
            StringExtensions extension("\"Quote One\" \"Quote Two\" Three");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a space - there should be three elements returned.
            //

            container.reset(extension.Split(std::string(" ")).release());

            Assert::AreEqual((size_t)3,
                container->size(),
                L"Expected Split() to return 3 elements.");
        }

        //
        // Test a string with an embedded strings enclosed in quotation marks containing a
        // quoted quotation mark. Ensure that the quoted strings are returned as individual
        // items.
        //
        // Result: two strings are returned.
        //

        TEST_METHOD(SplitOnSpaceWithQuotedStringContainingQuotes)
        {
            StringExtensions extension("\"Quote \\\" One\" Two");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a space - there should be two elements returned.
            //

            container.reset(extension.Split(std::string(" ")).release());

            Assert::AreEqual((size_t)2,
                container->size(),
                L"Expected Split() to return 2 elements.");

            //
            // The input should contain a single quoted ('\').
            //

            Assert::AreEqual((int)1,
                std::count(container->at(0).cbegin(),
                    container->at(0).cend(),
                    '"'),
                L"Expected to contain one quotation mark.");
        }
    };

    //
    // Test split methods returning a stl::list.
    //

    TEST_CLASS(StringExtensionListSplit)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"StringExtentions", L"List Output")
        END_TEST_CLASS_ATTRIBUTE()

            //
            // Test the Split method on 'Hello, world!'. Delimiter is a space.
            //
            // Result: two strings should be returned.
            //

            TEST_METHOD(SplitOnSpaceIntoOneString)
        {
            StringExtensions extension("Hello, world!");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string into a vector.
            //

            container.reset(extension.Split().release());

            //
            // There must only be two items in the returned vector.
            //

            Assert::AreEqual((size_t)2,
                container->size(),
                L"Expected Split() to return 2 elements.");
        }

        //
        // Test a complex string delimited by the a space.
        //
        // Result: ten strings should be returned.
        //

        TEST_METHOD(SplitOnSpaceIntoTenStrings)
        {
            StringExtensions extension("Hello, I said, to the damsel, or so I thought.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string into a vectors.
            //

            container.reset(extension.Split().release());

            //
            // There must only be 10 items in the returned vector.
            //

            Assert::AreEqual((size_t)10,
                container->size(),
                L"Expected Split() to return 10 elements.");
        }

        //
        // Test a split on both a comma and space, removing empty strings.
        //
        // Result: ten strings should be returned.
        //

        TEST_METHOD(SplitOnCommaAndSpaceRemoveEmpty)
        {
            StringExtensions extension("Hello, I said, to the damsel, or so I thought.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string into a vectors where the separators are , and space and remove
            // the empty strings -- there will be empty strings where a space follows a comma.
            //

            container.reset(extension.Split(std::string(", "), true).release());

            //
            // There should be ten items in the returned vector.
            //

            Assert::AreEqual((size_t)10,
                container->size(),
                L"Expected Split() to return 10 elements.");
        }

        //
        // Test that removing empty strings returns an empty vector when
        // the only delimiters are in the input.
        //
        // Result: an empty vector should be returned.
        //

        TEST_METHOD(SplitOnCommaWithOnlyDelimiters)
        {
            StringExtensions extension(",,,,,");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the separators, removing empty strings.  An empty vector
            // should be returned.
            //

            container.reset(extension.Split(std::string(","), true).release());

            //
            // There should be no items in the returned vector.
            //

            Assert::AreEqual((size_t)0,
                container->size(),
                L"Expected Split() to return 0 elements.");
        }

        //
        // Test split on an empty string.
        //
        // Result: an empty vector should be returned.
        //

        TEST_METHOD(SplitOnEmptyString)
        {
            StringExtensions extension("");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the empty string - there should be a vector containing an empty string.
            //

            container.reset(extension.Split().release());

            Assert::AreEqual((size_t)1,
                container->size(),
                L"Expected Split() to return 1 elements.");
        }

        //
        // Test a string with no separators.
        //
        // Result: one element (the original string) should be returned.
        //

        TEST_METHOD(SplitWithNoSeparator)
        {
            StringExtensions extension("This is a string with no separators.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a comma - there should be one element returned.
            //

            container.reset(extension.Split(std::string(",")).release());

            Assert::AreEqual((size_t)1,
                container->size(),
                L"Expected Split() to return 1 elements.");
        }

        //
        // Test a string with an included quoted (\) quote (\\) and verify that
        // the quote is included in the output.
        //
        // Result: one quoted character should be found.
        //

        TEST_METHOD(SplitOnSpaceWithQuotedCharacter)
        {
            //
            // Note: Quote the quoted character to ensure it is included!
            //

            StringExtensions extension("This is a string with a \\\\quoted character.");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a comma - there should be one element returned.
            //

            container.reset(extension.Split(std::string(",")).release());

            Assert::AreEqual((size_t)1,
                container->size(),
                L"Expected Split() to return 1 elements.");

            //
            // The input should contain a single quoted ('\').
            //

            Assert::AreEqual((int)1,
                std::count(container->at(0).cbegin(),
                    container->at(0).cend(),
                    '\\'),
                L"Expected to contain one quoted element.");
        }

        //
        // Test a string enclosed in quotation marks. Ensure that the whole string is returned.
        //
        // Result: one string is returned.
        //

        TEST_METHOD(SplitOnSpaceWithSpacesInAQuotedString)
        {
            StringExtensions extension("\"This is a string enclosed in double quotation marks.\"");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a space - there should be one element returned.
            //

            container.reset(extension.Split(std::string(" ")).release());

            Assert::AreEqual((size_t)1,
                container->size(),
                L"Expected Split() to return 1 element.");
        }

        //
        // Test a string with two embedded strings enclosed in quotation marks. Ensure that
        // the quoted strings are returned as individual items.
        //
        // Result: three strings are returned.
        //

        TEST_METHOD(SplitOnSpaceWithMixedQuotedAndUnquotedStrings)
        {
            StringExtensions extension("\"Quote One\" \"Quote Two\" Three");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a space - there should be three elements returned.
            //

            container.reset(extension.Split(std::string(" ")).release());

            Assert::AreEqual((size_t)3,
                container->size(),
                L"Expected Split() to return 3 elements.");
        }

        //
        // Test a string with an embedded strings enclosed in quotation marks containing a
        // quoted quotation mark. Ensure that the quoted strings are returned as individual
        // items.
        //
        // Result: two strings are returned.
        //

        TEST_METHOD(SplitOnSpaceWithQuotedStringContainingQuotes)
        {
            StringExtensions extension("\"Quote \\\" One\" Two");
            std::unique_ptr<StringExtensions::container_type> container;

            //
            // Split the string using a space - there should be two elements returned.
            //

            container.reset(extension.Split(std::string(" ")).release());

            Assert::AreEqual((size_t)2,
                container->size(),
                L"Expected Split() to return 2 elements.");

            //
            // The input should contain a single quoted ('\"').
            //

            Assert::AreEqual((int)1,
                std::count(container->at(0).cbegin(),
                    container->at(0).cend(),
                    '"'),
                L"Expected to contain one quotation mark.");
        }
    };

    //
    // Test trim methods.
    //

    TEST_CLASS(StringExtensionTrim)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"StringExtentions", L"Trim")
        END_TEST_CLASS_ATTRIBUTE()

        //
        // Test trimming a string with no leading or trailing blanks.
        //
        // Result: the input string should be returned.
        //

        TEST_METHOD(TrimStartNoTrim)
        {
            StringExtensions extension("Hello, world!");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.TrimStart();

            Assert::AreEqual(extension.Contents(),
                    newExtension->Contents(),                                                 
                    L"Expected trimmed instance to be the same!");
        }

        //
        // Test trimming a string with only leading blanks.
        //
        // Result: strings should be different.
        //

        TEST_METHOD(TrimStartBlanks)
        {
            StringExtensions extension("     Hello, world!");

            //
            // Trim the string and expect the results to be different.
            //

            auto newExtension = extension.TrimStart();

            Assert::AreNotEqual(extension.Contents(),
                    newExtension->Contents(),                                    
                    L"Expected trimmed instance to not be the same!");
        }

        //
        // Trim all leading alphabetic characters from a string.
        //
        // Result: expect non-alphabetic characters to be returned.
        //

        TEST_METHOD(TrimStartAlpha)
        {
            StringExtensions extension("ABCDEF1234567890");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.TrimStart(std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

            Assert::AreEqual(std::string("1234567890"),
                    newExtension->Contents(),
                    L"Expected trimmed instance to be the same!");
        }

        //
        // Test trimming the end with nothing to trim.
        //
        // Result: returned string should be the same as the input string.
        //

        TEST_METHOD(TrimEndNoTrim)
        {
            StringExtensions extension("Hello, world!");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.TrimEnd();

            Assert::AreEqual(extension.Contents(),
                    newExtension->Contents(),
                    L"Expected trimmed instance to be the same!");
        }

        //
        // Test trimming the end of string containing blanks.
        //
        // Result: the blanks should be stripped from the end.
        //

        TEST_METHOD(TrimEndBlanks)
        {
            StringExtensions extension("Hello, world!      ");

            //
            // Trim the string and expect the results to be different.
            //

            auto newExtension = extension.TrimEnd();

            Assert::AreNotEqual(extension.Contents(),
                    newExtension->Contents(),
                    L"Expected trimmed instance to not be the same!");
        }

        //
        // Test trimming the end of a string containing an alphabetic pattern.
        //
        // Result: the alphabetic characters should be trimmed from the string.
        //

        TEST_METHOD(TrimEndAlpha)
        {
            StringExtensions extension("1234567890ABCDEF");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.TrimEnd(std::string("ABCDEF"));

            Assert::AreEqual(std::string("1234567890"),
                             newExtension->Contents(),
                             L"Expected trimmed instance to be the same!");
        }

        //
        // Test trimming blanks from both ends of a string in which there are
        // no
        // leading and trailing blanks.
        //
        // Result: the resultant string and input strings should be the same.
        //

        TEST_METHOD(TrimNoTrim)
        {
            StringExtensions extension("Hello, world!");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.Trim();

            Assert::AreEqual(extension.Contents(),
                             newExtension->Contents(),
                             L"Expected trimmed instance to be the same!");
        }

        //
        // Test trimming blanks from both ends of a string where there are blanks.
        //
        // Result: the resultant string should contain on leading or trailing blanks.
        //

        TEST_METHOD(TrimBlanks)
        {
            StringExtensions extension("    Hello, world!    ");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.Trim();

            Assert::AreEqual(std::string("Hello, world!"),
                             newExtension->Contents(),
                             L"Expected trimmed instance to be the same!");
        }

        //
        // Test trimming alphabetic characters from both ends of the string.
        //
        // Result: the resultant string is the text composed of non-delimiter characters
        // from the front and end.
        //

        TEST_METHOD(TrimAlpha)
        {
            StringExtensions extension("abcdeHello, world!abcdefghijklmnop");

            //
            // Trim the string and expect the result to be the same.
            //

            auto newExtension = extension.Trim(std::string("abcdefghijklmnopqrstuvwxyz"));

            Assert::AreEqual(std::string("Hello, world!"),
                             newExtension->Contents(),
                             L"Expected trimmed instance to be the same!");
        }
    };

    //
    // Test conversion methods.
    //

    TEST_CLASS(StringConversionsToLong)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"StringExtentions", L"Conversions To Long")
        END_TEST_CLASS_ATTRIBUTE()

        //
        // Test conversions of numbers with and without leading and trailing spaces
        // and error conditions.
        //
        // Result: expected longs should be returned.
        //
        TEST_METHOD(ToLong)
        {
            long lResult;

            lResult = -1;

            Assert::IsTrue(FromString(std::string("123"), &lResult));
            Assert::AreEqual(123L, lResult, L"Expected 123 returned");

            Assert::IsTrue(FromString(std::string("     456"), &lResult));
            Assert::AreEqual(456L, lResult, L"Expected 456 returned");

            Assert::IsTrue(FromString(std::string("789    "), &lResult));
            Assert::AreEqual(789L, lResult, L"Expected 789 returned");

            Assert::IsTrue(FromString(std::string("   1010   "), &lResult));
            Assert::AreEqual(1010L, lResult, L"Expected 1010 returned");

            lResult = -1;
            Assert::IsFalse(FromString(std::string("   78910a  "), &lResult));
            Assert::AreEqual(-1L, lResult, L"Expected -1 returned");
        }
    };
}
