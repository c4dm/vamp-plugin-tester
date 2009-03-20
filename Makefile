
LDFLAGS 	+= -lvamp-hostsdk -ldl
CXXFLAGS	+= -g -Wall -Wextra

OBJECTS		:= vamp-plugin-tester.o Tester.o Test.o TestStaticData.o TestInputExtremes.o TestMultipleRuns.o TestOutputs.o TestDefaults.o TestInitialise.o

vamp-plugin-tester:	$(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS)

distclean:	clean
	rm -f *~ vamp-plugin-tester

depend:
	makedepend -Y *.cpp *.h

# DO NOT DELETE

Test.o: Test.h
TestDefaults.o: TestDefaults.h Test.h Tester.h
TestInitialise.o: TestInitialise.h Test.h Tester.h
TestInputExtremes.o: TestInputExtremes.h Test.h Tester.h
TestMultipleRuns.o: TestMultipleRuns.h Test.h Tester.h
TestOutputs.o: TestOutputs.h Test.h Tester.h
TestStaticData.o: TestStaticData.h Test.h Tester.h
Tester.o: Tester.h Test.h
vamp-plugin-tester.o: Tester.h Test.h
TestDefaults.o: Test.h Tester.h
TestInitialise.o: Test.h Tester.h
TestInputExtremes.o: Test.h Tester.h
TestMultipleRuns.o: Test.h Tester.h
TestOutputs.o: Test.h Tester.h
TestStaticData.o: Test.h Tester.h
Tester.o: Test.h
