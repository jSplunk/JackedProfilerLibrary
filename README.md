# JPL
JackedProfilerLibrary


## Setup
The library generates a .json file, which then can be used in ```chrome://tracing/``` in any Chromium based browser.

To use the functionallity, you need to specify a ```SESSION_START(name)``` and a ```SESSION_END()``` for what the range of the profiler should be.

##### Example.cpp
```cpp
#include <iostream>

void A()
{
    std::cout << "Hello" << std::endl;
}

int main()
{
    SESSION_START("Testing");
    A();
    SESSION_END();
    return 0;
}
```

Now in the session there also needs to be a profile. This can be used with the macro ```PROFILE_FUNC```, which will time the block of code it is placed inside.

```cpp
{
    PROFILE_FUNC();
}
```

This marks this block to be evaluated by the session. The session will always need to encapsulate any block specified with the macro
```PROFILE_FUNC```.

Here is a final example with everything together.

##### Example.cpp
```cpp
#include <iostream>
#include <JPL.h>

using namespace JPL;

void Calc()
{
	PROFILE_FUNC();

	int sum = 0;

	for (int i = 0; i < 100000; ++i)
	{
		int s = i % 3;

		sum += s;
	}

	std::cout << sum << std::endl;
}

int main()
{
	START_SESSION("Profiling Calculation");
	Calc();
	END_SESSION();
	return 0;
}
```

This then generates a ```results.json``` file by default.

The output in the browser is then:
![Profiler in Chrome](/screenshots/Example.PNG)
