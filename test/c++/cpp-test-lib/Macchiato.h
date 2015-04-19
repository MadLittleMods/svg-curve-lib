// Macchiato
// v0.6.5
// https://github.com/MadLittleMods/macchiato
//
// Mocha.js inspired testing for C++
//
// Requires C++11. Works on the following platforms:
//		- Most desktop environments
//		- Visual Studio
//		- Arduino
//
// This is a no dependency/full independent version of Macchiato
// We simply just concat the files together and remove the `#include *.h` to the h file dependencies

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------

#if defined(ARDUINO)
#include "Arduino.h"
#endif



// By Luc Danton: http://chat.stackoverflow.com/transcript/message/21940188#21940188
// http://coliru.stacked-crooked.com/a/015660099e486a80
// Works as function<R, A, B, C> instead of std::function<R(A, B, C)>
// 
// v0.2.0

#ifndef functionSig_h
#define functionSig_h

#include <utility>
#include <memory>

template<typename R, typename... Args>
struct holder_base {
	virtual ~holder_base() = default;
	
	virtual std::unique_ptr<holder_base> clone() const = 0;
	virtual R call(Args... args) = 0;
};

template<typename Functor, typename R, typename... Args>
struct holder: holder_base<R, Args...> {
	explicit holder(Functor functor): functor(std::move(functor)) {}
	
private:
	Functor functor;

public:
	R call(Args... args) override { return functor(std::forward<Args>(args)...); }
	std::unique_ptr<holder_base<R, Args...>> clone() const override
	{ return std::unique_ptr<holder> { new holder(*this) }; }
};

template<typename Functor, typename... Args>
struct holder<Functor, void, Args...>: holder_base<void, Args...> {
	explicit holder(Functor functor): functor(std::move(functor)) {}
	
private:
	Functor functor;

public:
	void call(Args... args) override { functor(std::forward<Args>(args)...); }
	std::unique_ptr<holder_base<void, Args...>> clone() const override
	{ return std::unique_ptr<holder> { new holder(*this) }; }
};

template<typename R, typename... Args>
struct function {
	template<typename Functor>
	function(Functor functor): functor(new holder<Functor, R, Args...>(std::move(functor))) {}
	
	function(function const& other): functor(other.functor->clone()) {}
	function& operator=(function const& other) { functor = other.functor->clone(); return *this; }
	
	function(function&& other)
		: functor { std::move(other.functor) }
	{
		
	}
	function& operator=(function&& other) {
		functor = std::move(other.functor);
		
		return *this;
	}
	//function(function&&) = default;
	//function& operator=(function&&) = default;
	
	R operator()(Args... args) { return functor->call(std::forward<Args>(args)...); }
private:
	std::unique_ptr<holder_base<R, Args...>> functor;
};


#endif



// PlatformString
// v0.3.1
//
// Requires C++11. Works on the following platforms:
//		- Most desktop environments
//		- Visual Studio
//		- Arduino


#ifndef PlatformString_h
#define PlatformString_h


#if defined(ARDUINO)
#include "Arduino.h"

struct PlatformString {
	String value;
	
	PlatformString() 
		: PlatformString("")
	{
	};
	
	PlatformString(char const* val)
		: PlatformString(String(val))
	{
	};
	
	PlatformString(String val)
		: value(val)
	{
	};
	
	
	PlatformString(bool val)
		: value(val ? "true" : "false")
	{
	};
	
	PlatformString(int val)
		: value(String(val))
	{
	};
	PlatformString(unsigned int val)
		: value(String(val))
	{
	};
	
	PlatformString(double val)
		: PlatformString(val, 4) 
	{
	};
	
	PlatformString(double val, byte precision)
		: value(PlatformString::FloatToString(val, precision))
	{
	};
	
	PlatformString operator + (char const* s) {
		return PlatformString(this->value + String(s));
	};
	PlatformString operator + (const PlatformString &s) {
		return PlatformString(this->value + s.value);
	};

	// For left hand side operator overloading
	friend PlatformString operator + (char const* s, const PlatformString &ps) {
		return PlatformString(String(s) + ps.value);
	};

	PlatformString& operator +=  (char const* s) {
		this->value = this->value + String(s);
		return *this;
	};
	PlatformString& operator +=  (const PlatformString &s) {
		this->value = this->value + s.value;
		return *this;
	};
	
	operator String() { return this->value; }
	
	private:
		static String FloatToString(float val, byte precision) {
			// returns val with number of decimal places determine by precision
			// precision is a number from 0 to 6 indicating the desired decimial places
			// example: floatToString(3.1415, 2); // returns 3.14 (two decimal places)
		
			String output = "";
		
			if(val < 0.0){
				output += "-";
				val = -val;
			}
		
			output += int(val);  //prints the int part
			if(precision > 0) 
			{
				output += "."; // print the decimal point
		
				unsigned long frac;
				unsigned long mult = 1;
				byte padding = precision -1;
				while(precision--)
					mult *=10;
		
				if(val >= 0)
					frac = (val - int(val)) * mult;
				else
					frac = (int(val)- val ) * mult;
				unsigned long frac1 = frac;
		
				while( frac1 /= 10 )
					padding--;
				while(  padding--)
					output += "0";
		
				output += frac;
			}
		
			return output;
		};
};


#else
#include <string>
#include <iostream>
// std::min, std::max
#include <algorithm>

struct PlatformString {
	std::string value;
	
	PlatformString() 
		: PlatformString("")
	{
	};
	
	PlatformString(char const* val)
		: PlatformString(std::string(val))
	{
	};
	
	PlatformString(std::string val)
		: value(val) 
	{
	};
	
	
	PlatformString(bool val)
		: value(val ? "true" : "false")
	{
	};
	
	PlatformString(int val)
		: value(std::to_string(val))
	{
	};
	PlatformString(unsigned int val)
		: value(std::to_string(val))
	{
	};
	
	PlatformString(double val)
		: value(RemoveTrailingZeros(std::to_string(val)))
	{
	};
	
	PlatformString operator + (char const* s) {
		return PlatformString(this->value + std::string(s));
	};
	PlatformString operator + (const PlatformString &s) {
		return PlatformString(this->value + s.value);
	};

	// For left hand side operator overloading
	friend PlatformString operator + (char const* s, const PlatformString &ps) {
		return PlatformString(std::string(s) + ps.value);
	};

	PlatformString& operator += (char const* s) {
		this->value = this->value + std::string(s);
		return *this;
	};
	PlatformString& operator += (const PlatformString &s) {
		this->value = this->value + s.value;
		return *this;
	};
	
	friend std::ostream& operator << (std::ostream& stream, const PlatformString &s) {
		return stream << s.value;
	};

	operator std::string() { return this->value; }

	private:
		static std::string RemoveTrailingZeros(std::string str) {
			return str.erase((std::min)((int)str.find_last_not_of('0') + 2, (int)str.length()), std::string::npos);
		};
};

#endif


#endif




// Macchiato
// v0.6.5
// https://github.com/MadLittleMods/macchiato
//
// Mocha.js inspired testing for C++
//
// Requires C++11. Works on the following platforms:
//		- Most desktop environments
//		- Visual Studio
//		- Arduino


#ifndef Macchiato_h
#define Macchiato_h

#if defined(ARDUINO)
#include "Arduino.h"
#endif

// fabs
#include <cmath>

//#include "functionSig.h"
//#include "PlatformString.h"

namespace Macchiato {

	struct TestResult {
		PlatformString message;
		bool didPass = true;
	};

	struct testFlags {
		bool negate = false;
	};


	template <typename Ta, typename Te = Ta>
	struct MacchiatoPlugin {
		function<bool, Ta, Te> testFunc;
		function<PlatformString, Ta, Te, testFlags> failMessageFunc;

		MacchiatoPlugin(
			function<bool, Ta, Te> testFunc,
			function<PlatformString, Ta, Te, testFlags> failMessageFunc
		) : testFunc(testFunc), failMessageFunc(failMessageFunc) { };
	};




	struct _MacchiatoSettings {
		bool useAnsiColor = true;
		PlatformString indentToken = "\t";
	};
	_MacchiatoSettings MacchiatoSettings;

	struct __MacchiatoUtilClass {

		void incrementDepth() {
			this->currentDepth += 1;
		};
		void decrementDepth() {
			this->currentDepth -= 1;
		};


		PlatformString generateCurrentDepthString() {
			PlatformString depthString = "";
			for(unsigned int i = 0; i < currentDepth-1; i++) {
				depthString += MacchiatoSettings.indentToken;
			}

			return depthString;
		};
		PlatformString generateCurrentChildDepthString() {
			PlatformString childDepthString = "";
			for(unsigned int i = 0; i < currentDepth; i++) {
				childDepthString += MacchiatoSettings.indentToken;
			}

			return childDepthString;
		};

		PlatformString wrapInAnsiCyan(PlatformString input) {
			return MacchiatoSettings.useAnsiColor ? PlatformString("\033[36;1m" + input.value + "\033[0m") : input;
		};

		PlatformString wrapInAnsiGreen(PlatformString input) {
			return MacchiatoSettings.useAnsiColor ? PlatformString("\033[32;1m" + input.value + "\033[0m") : input;
		};
		PlatformString wrapInAnsiRed(PlatformString input) {
			return MacchiatoSettings.useAnsiColor ? PlatformString("\033[31;1m" + input.value + "\033[0m") : input;
		};


		// Just a helper to add to the resultant output
		void log(PlatformString message) {
			resultantOutputString += message;
		};


		// Helper to increment the summary of tests
		enum TestResultType {
			Pass, Fail, Pending
		};
		void logTestResult(TestResultType tType) {
			if(tType == TestResultType::Pass) {
				this->numTestsPassed++;
			}
			else if(tType == TestResultType::Fail) {
				this->numTestsFailed++;
			}
			else if(tType == TestResultType::Pending) {
				this->numTestsPending++;
			}
		};




		PlatformString getResultantTestOutput() {
			PlatformString result = PlatformString(this->resultantOutputString);
			// Add a small summary at the end
			result += PlatformString("\n\n") +
				this->wrapInAnsiGreen(PlatformString(this->numTestsPassed) + " passing") + "\n" +
				this->wrapInAnsiRed(PlatformString(this->numTestsFailed) + " failing") + "\n" +
				this->wrapInAnsiCyan(PlatformString(this->numTestsPending) + " pending");


			result += "\n";

			return result;
		};

		void clearTestResults() {
			this->resultantOutputString = "";

			this->numTestsPassed = 0;
			this->numTestsFailed = 0;
			this->numTestsPending = 0;

		};


		private:
			PlatformString resultantOutputString = "";

			unsigned int currentDepth = 0;

			unsigned int numTestsPassed = 0;
			unsigned int numTestsFailed = 0;
			unsigned int numTestsPending = 0;


	};
	__MacchiatoUtilClass _MacchiatoUtil;



	PlatformString GetResultantTestOutput() {
		return _MacchiatoUtil.getResultantTestOutput();
	};
	void ClearTestResults() {
		_MacchiatoUtil.clearTestResults();
	};



	void describe(
		PlatformString description,
		function<void> describeCb
	) {
		// We just got to this depth
		_MacchiatoUtil.incrementDepth();

		// Log the current subject
		_MacchiatoUtil.log(_MacchiatoUtil.generateCurrentDepthString() + description + "\n");

		// Run the describe and get the results
		// They should of put some `it` calls inside the callback
		describeCb();

		// Now that we are done, reduce the depth
		_MacchiatoUtil.decrementDepth();
	};





	void it(PlatformString testDescription) {
		_MacchiatoUtil.logTestResult(_MacchiatoUtil.TestResultType::Pending);

		PlatformString message = _MacchiatoUtil.generateCurrentChildDepthString() +
			_MacchiatoUtil.wrapInAnsiCyan(PlatformString("----: ") + testDescription) +
			"\n";

		_MacchiatoUtil.log(message);
	};

	void it(PlatformString testDescription, function<Macchiato::TestResult> itCb) {
		Macchiato::TestResult testResult = itCb();

		_MacchiatoUtil.logTestResult(testResult.didPass ? _MacchiatoUtil.TestResultType::Pass : _MacchiatoUtil.TestResultType::Fail);

		PlatformString message = _MacchiatoUtil.generateCurrentChildDepthString() + 
			(testResult.didPass ? _MacchiatoUtil.wrapInAnsiGreen("Pass") : _MacchiatoUtil.wrapInAnsiRed("Fail")) + ": " + 
			testDescription + 
			(testResult.didPass ? "" : "\n" + _MacchiatoUtil.generateCurrentChildDepthString() + MacchiatoSettings.indentToken + testResult.message) + 
			"\n";

		_MacchiatoUtil.log(message);
	};


	// expect: BDD
	template <typename Ta>
	struct expect_type {
		
		expect_type(Ta actual) : actual(actual) { };
		


		template <typename Te>
		expect_type* equal(Te expected) {
			this->addTestResult(
				this->actual == expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "equal " + PlatformString(expected)
			);
			
			return this;
		};
		template <typename Te>
		expect_type* eql(Te expected) {
			return this->equal(expected);
		};
		
		expect_type* closeTo(double expected) {
			return this->closeTo(expected, 0.0001);
		};
		expect_type* closeTo(double expected, double tolerance) {
			this->addTestResult(
				fabs(this->actual - expected) <= tolerance,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "equal " + PlatformString(expected) + " within tolerance of " + PlatformString(tolerance)
			);
			
			return this;
		};

		expect_type* within(double lower, double upper) {
			this->addTestResult(
				this->actual > lower && this->actual < upper,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be above " + PlatformString(lower) + " and below " + PlatformString(upper)
			);
			
			return this;
		};


		expect_type* above(double expected) {
			this->addTestResult(
				this->actual > expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be greater than " + PlatformString(expected)
			);
			
			return this;
		};
		expect_type* gt(double expected) {
			return this->above(expected);
		};
		expect_type* greaterThan(double expected) {
			return this->above(expected);
		};

		expect_type* least(double expected) {
			this->addTestResult(
				this->actual >= expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be greater than or equal to " + PlatformString(expected)
			);
			
			return this;
		};
		expect_type* gte(double expected) {
			return this->least(expected);
		};

		expect_type* below(double expected) {
			this->addTestResult(
				this->actual < expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be lesser than " + PlatformString(expected)
			);
			
			return this;
		};
		expect_type* lt(double expected) {
			return this->below(expected);
		};
		expect_type* lessThan(double expected) {
			return this->below(expected);
		};

		expect_type* most(double expected) {
			this->addTestResult(
				this->actual <= expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be less than or equal to " + PlatformString(expected)
			);
			
			return this;
		};
		expect_type* lte(double expected) {
			return this->most(expected);
		};

		expect_type* satisfy(function<bool, Ta> testFunc) {
			bool testResultBool = testFunc(this->actual);
			
			return this->satisfy(
				testResultBool,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "satisfy the given test"
			);
		};
		expect_type* satisfy(function<bool, Ta> testFunc, function<PlatformString, Ta, testFlags> failMessageFunc) {
			bool testResultBool = testFunc(this->actual);
			PlatformString failMessage = failMessageFunc(this->actual, this->flags);
			
			return this->satisfy(testResultBool, failMessage);
		};
		template <typename Te>
		expect_type* satisfy(MacchiatoPlugin<Ta, Te> plugin, Te expected) {
			bool testResultBool = plugin.testFunc(this->actual, expected);
			PlatformString failMessage = plugin.failMessageFunc(this->actual, expected, this->flags);
			
			return this->satisfy(testResultBool, failMessage);
		};
		expect_type* satisfy(bool testResultBool, PlatformString failMessage) {
			this->addTestResult(
				testResultBool,
				failMessage
			);
			
			return this;
		};
		
		


		class MemberLogicClass {
			expect_type* expectPointer;
			function<void, expect_type*> getterFunc;
			public:
				MemberLogicClass(expect_type *i, function<void, expect_type*> getterFunc) : expectPointer(i), getterFunc(getterFunc) {};
				
				// Setter
				expect_type* operator = (const expect_type i) {
					return this->expectPointer = (expect_type*)&i;
					
				};
				// Setter
				expect_type* operator = (const expect_type *i) {
					return this->expectPointer = (expect_type*)i;
				};
				// Getter
				expect_type* operator -> () {
					this->getterFunc(this->expectPointer);
					return this->expectPointer; 
				};
				// Getter
				operator expect_type* () const {
					this->getterFunc(this->expectPointer);
					return this->expectPointer;
				};
		};


		// Sets the negate flag when used
		// expect<int>(3).to->never->equal->(5);
		MemberLogicClass never{this, [&](expect_type* expectPointer) {
			expectPointer->flags.negate = !expectPointer->flags.negate;
		}};


		
		// Provided as chainable getters to improve the readability of your assertions.
		// They do not provide testing capabilities.
		expect_type* to = this;
		expect_type* be = this;
		expect_type* been = this;
		expect_type* is = this;
		expect_type* that = this;
		expect_type* which = this;
		// `and` is a reserved keyword
		expect_type* then = this;//expect_type* and = this;
		expect_type* has = this;
		expect_type* have = this;
		expect_type* with = this;
		expect_type* at = this;
		expect_type* of = this;
		expect_type* same = this;
		
		
		
		TestResult getResult() {
			return this->testResult;
		};
		
		operator bool() {
			return this->testResult.didPass;
		};


		protected: 
			Ta actual;
			testFlags flags;
			
			TestResult testResult;
			
			void addTestResult(bool testResultBool, PlatformString message) {
				bool didPass = (this->flags.negate ? !testResultBool : testResultBool);

				this->testResult.didPass = this->testResult.didPass && didPass;

				if(!didPass) {
					// Concat a newline if this is a consecutive test
					if(this->testResult.message.value.length() > 0) {
						this->testResult.message += "\n";
					}
					this->testResult.message += message;
				}

				

				// Reset the flag
				this->flags.negate = false;
			};


		//private:
	};

	template <typename T>
	expect_type<T> expect(T&& x) {
		return { std::forward<T>(x) };
	};



	void MacchiatoParseCLIArgs(int argc, char * const argv[]) {
		// Some CLI options/flags
		for(int i = 0; i < argc; i++) {
			//std::cout << argv[i] << std::endl;
			if(strcmp(argv[i], "-no-color") == 0 || strcmp(argv[i], "--no-color") == 0) {
				Macchiato::MacchiatoSettings.useAnsiColor = false;
			}
		}
	};



}



#include <vector>
#if defined(ARDUINO)
	// We just add a define in case anyone wants to detect it later to avoid multiple declaration errors
	#ifndef std_vector_arduino_throw_polyfill
	#define std_vector_arduino_throw_polyfill
		// via https://forum.pjrc.com/threads/23467-Using-std-vector?p=69787&viewfull=1#post69787
		namespace std {
			inline void __throw_bad_alloc() {
				Serial.println("Unable to allocate memory");
			}

			inline void __throw_length_error( char const*e ) {
				Serial.print("Length Error :");
				Serial.println(e);
			}
		}
	#endif
#endif


// MACCHIATO_RUNTESTS
// --------------------------------------------
// --------------------------------------------

// Just a simple way to get a unique name after passing in a name.
// Concats that name with the line number
//
// via: https://github.com/philsquared/Catch/blob/35f4266d0022ed039923fbbba2c4495435ad1465/single_include/catch.hpp#L58
#define MACCHIATO_INTERNAL_UNIQUE_NAME_LINE2( name, line ) name ## line
#define MACCHIATO_INTERNAL_UNIQUE_NAME_LINE( name, line ) MACCHIATO_INTERNAL_UNIQUE_NAME_LINE2( name, line )
#define MACCHIATO_INTERNAL_UNIQUE_NAME( name ) MACCHIATO_INTERNAL_UNIQUE_NAME_LINE( name, __LINE__ )

namespace Macchiato {
	std::vector<function<void>> run_registry;

	void _RunAllRegisteredTestsFromMacro() {
		// Run all of the calls made in `MACCHIATO_RUNTESTS([] { /* ... */ });
		for(function<void> i : Macchiato::run_registry) {
			i();
		}
	};

	struct runtests_macro_registrar_type {
		template <typename Func>
		runtests_macro_registrar_type(Func&& registrant) {
			// If they want our main, we need to save up the calls and call them in our main
			// below any settings that may have been set
			#if defined(MACCHIATO_MAIN)
				run_registry.emplace_back(registrant);
			// Otherwise just call it now because that is what they expect (synchronous)
			#else
				registrant();
			#endif
		}
	};
}

// Defines and Constructs an object of `Macchiato::runtests_macro_registrar_type` type. This means the constructor is exectuted with __VA_ARGS__ passed in.
// `INTERNAL_UNIQUE_NAME(run_registrar)` could be any name but this allows for easy uniqueness with it still being somewhat named
// The double colon tells the compiler to look in the global scope for that type. This is just in case someone defines somethingof the same name where this is called
//
// via: https://github.com/rmartinho/nonius/blob/01115c1d4062de88ffca798545ac7c70b4ad4623/include/nonius/benchmark.h%2B%2B#L96
#define MACCHIATO_RUNTESTS( ... ) \
	namespace { static ::Macchiato::runtests_macro_registrar_type MACCHIATO_INTERNAL_UNIQUE_NAME(runtests_macro_registrar)(__VA_ARGS__); }

// end MACCHIATO_RUNTESTS
// --------------------------------------------
// --------------------------------------------



// Define some runners if they want to go barebones
#if defined(MACCHIATO_MAIN)

	#if defined(ARDUINO)

		// Keeps track since we last sent a serial in our debug serial
		elapsedMicros sinceSerial;

		void setup() {
			Serial.begin(9600);
		}

		void loop() {
			// Print it out every 1 second
			if (sinceSerial >= 1 * 1000000) {
				Macchiato::ClearTestResults();
				Macchiato::_RunAllRegisteredTestsFromMacro();
				Serial.print(Macchiato::GetResultantTestOutput().value);

				sinceSerial = 0;
			}
		}
	#else // defined(ARDUINO)
		#include <iostream>
		#include <string>


		// Standard C/C++ main entry point
		int main (int argc, char * const argv[]) {
			Macchiato::MacchiatoParseCLIArgs(argc, argv);

			// The reset is only necessary if we were reptively runnin then outputting
			//Macchiato::ClearTestResults();
			Macchiato::_RunAllRegisteredTestsFromMacro();
			std::cout << Macchiato::GetResultantTestOutput() << std::endl;

			return 0;
		}
	#endif // defined(ARDUINO)

#endif // defined(MACCHIATO_MAIN)



#endif // Macchiato_h