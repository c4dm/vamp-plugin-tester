/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    Vamp Plugin Tester
    Chris Cannam, cannam@all-day-breakfast.com
    Centre for Digital Music, Queen Mary, University of London.
    Copyright 2009-2014 QMUL.

    This program loads a Vamp plugin and tests its susceptibility to a
    number of common pitfalls, including handling of extremes of input
    data.  If you can think of any additional useful tests that are
    easily added, please send them to me.
  
    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR
    ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    Except as contained in this notice, the names of the Centre for
    Digital Music; Queen Mary, University of London; and Chris Cannam
    shall not be used in advertising or otherwise to promote the sale,
    use or other dealings in this Software without prior written
    authorization.
*/

#ifndef _TEST_STATIC_DATA_H_
#define _TEST_STATIC_DATA_H_

#include "Test.h"
#include "Tester.h"

class TestIdentifiers : public Test
{
public:
    TestIdentifiers() : Test() { }
    Results test(std::string key, Options options);
    
protected:
    Result testIdentifier(std::string ident, std::string desc);
    static Tester::TestRegistrar<TestIdentifiers> m_registrar;
};

class TestEmptyFields : public Test
{
public:
    TestEmptyFields() : Test() { }
    Results test(std::string key, Options options);
    
protected:
    Result testMandatory(std::string text, std::string desc);
    Result testRecommended(std::string text, std::string desc);
    static Tester::TestRegistrar<TestEmptyFields> m_registrar;
};

class TestValueRanges : public Test
{
public:
    TestValueRanges() : Test() { }
    Results test(std::string key, Options options);
    
protected:
    static Tester::TestRegistrar<TestValueRanges> m_registrar;
};

class TestCategory : public Test
{
public:
    TestCategory() : Test() { }
    Results test(std::string key, Options options);
    
protected:
    static Tester::TestRegistrar<TestCategory> m_registrar;
};

#endif
