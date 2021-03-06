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

#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginLoader.h>

#include <iostream>

#include <cstring>
#include <cstdlib>
#include <cmath>
#include <set>

#include "Tester.h"

using Vamp::Plugin;
using Vamp::PluginHostAdapter;
using Vamp::RealTime;
using Vamp::HostExt::PluginLoader;
using Vamp::HostExt::PluginWrapper;
using Vamp::HostExt::PluginInputDomainAdapter;

using namespace std;

Tester::Tester(std::string key, Test::Options options, std::string singleTestId) :
    m_key(key),
    m_options(options),
    m_singleTest(singleTestId)
{
}

Tester::~Tester()
{
}

Tester::NameIndex &
Tester::nameIndex()
{
    static NameIndex ix;
    return ix;
}

Tester::Registry &
Tester::registry()
{
    static Registry r;
    return r;
}

void
Tester::listTests()
{
    cout << endl;
    cout << "Total tests: " << nameIndex().size() << "\n" << endl;
    cout << "ID | Name" << endl;
    cout << "---+-----" << endl;
    for (NameIndex::const_iterator i = nameIndex().begin();
         i != nameIndex().end(); ++i) {
        cout << i->first << " | " << i->second << endl;
    }
    cout << endl;
}

bool
Tester::test(int &notes, int &warnings, int &errors)
{
    /*
      
      Things I would like to see tested:

      * Identifiers for parameters, outputs, or plugin itself contain
        illegal characters - DONE

      * Any of the plugin's name, maker etc fields are empty - DONE

      * Default value of a parameter is not quantized as specified - DONE

      * Parameter minValue >= maxValue, or defaultValue < minValue
        or > maxValue - DONE

      * Plugin fails when given zero-length or very short input - DONE

      * Plugin fails when given "all digital zeros" input - DONE

      * Plugin fails when given input that exceeds +/-1 - DONE

      * Plugin fails when given "normal" random input (just in case!) - DONE

      * Plugin returns different results if another instance is
        constructed and run "interleaved" with it (from same thread) - DONE
 
      * Plugin's returned timestamps do not change as expected when
        run with a different base timestamp for input (though there
        could be legitimate reasons for this) - DONE

      * Plugin produces different results on second run, after reset
        called - DONE

      * Initial value of a parameter on plugin construction differs
        from its default value (i.e. plugin produces different
        results depending on whether parameter is set explicitly by
        host to default value or not) - DONE
        
      * If a plugin reports any programs, selecting default program
        explicitly changes results (as for default parameters) - DONE

      * Output feature does not hasTimestamp when output type is
        VariableSampleRate - DONE

      * Output feature hasTimestamp or hasDuration when output type is
        OneSamplePerStep (warning only, this is not an error) - DONE

      * Plugin returns features whose output numbers do not have
        a corresponding record in output descriptor list - DONE

      * Plugin fails to return any features on some output (warning
        only) - DONE

      * Constructor takes a long time to run.  A fuzzy concept, but
        suggests that some work should have been deferred to
        initialise().  Warning only - DONE

      * Plugin fails gracelessly when constructed with "weird" sample
        rate or initialised with "wrong" step size, block size, or
        number of channels

      Well, that's quite a lot of tests already.  What else?

    */

    bool good = true;

    try {

        if (m_options & Test::SingleTest) {

            if (registry().find(m_singleTest) != registry().end()) {

                good = performTest(m_singleTest, notes, warnings, errors);

            } else {

                std::cout << " ** ERROR: Unknown single-test id \""
                          << m_singleTest << "\"" << std::endl;
                good = false;
            }

        } else {
        
            for (Registry::const_iterator i = registry().begin();
                 i != registry().end(); ++i) {

                bool thisGood = performTest(i->first, notes, warnings, errors);
                if (!thisGood) good = false;
            }
        }

    } catch (Test::FailedToLoadPlugin) {
        std::cout << " ** ERROR: Failed to load plugin (key = \"" << m_key
                  << "\")" << std::endl;
        std::cout << " ** NOTE: Vamp plugin path is: " << std::endl;
        std::vector<std::string> pp = PluginHostAdapter::getPluginPath();
        for (size_t i = 0; i < pp.size(); ++i) {
            std::cout << "            " << pp[i] << std::endl;
        }
        good = false;
    }

    return good;
}

bool
Tester::performTest(std::string id, int &notes, int &warnings, int &errors)
{
    std::cout << " -- Performing test: "
              << id
              << " "
              << nameIndex()[id]
              << std::endl;
    
    Test *test = registry()[id]->makeTest();
    Test::Results results = test->test(m_key, m_options);
    delete test;

    set<string> printed;

    bool good = true;

    for (int j = 0; j < (int)results.size(); ++j) {
        string message = results[j].message();
        if (printed.find(message) != printed.end()) continue;
        printed.insert(message);
        switch (results[j].code()) {
        case Test::Result::Success:
            break;
        case Test::Result::Note:
            std::cout << " ** NOTE: " << results[j].message() << std::endl;
            ++notes;
            break;
        case Test::Result::Warning:
            std::cout << " ** WARNING: " << results[j].message() << std::endl;
            ++warnings;
            break;
        case Test::Result::Error:
            std::cout << " ** ERROR: " << results[j].message() << std::endl;
            ++errors;
            good = false;
            break;
        }
    }

    return good;
}

