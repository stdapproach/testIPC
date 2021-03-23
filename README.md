# testIPC

Test to work all together with ipc, loading dll/so and C++14's features

The solution itself has 3 layers:

1. Tester
2. Gateway
3. Services

# Tester
Executable file
When executed takes 2 arguments
* Type of request (String)
* Argument (String)
It will send this command (via IPC) to gateway module and wait for the response.
If Tester executed without parameters, to invoke gateway there are 2 hard coded default parameters (see run0Param.sh).
If Tester runs with only 1 parameter, well it's an error (see run1Param.sh).
For running with 2 parameters, see run2Params.sh

# Gateway
This is an Executable.
Has 2 public interfaces
One takes input from tester module
Second: loading service (dll/so) by name (first argument, transfered from Tester)
According to service's name invoking particular function from corresponded service.
If the service's function couldn't be founded among the storage of loaded services,
Gateway tryes to load dll/so with the name of requested service.
If the proper service's name finally founded, the service's function been invokes and
the result sent to Tester (via IPC), if not - error message alse sent to Tester

# Services
A set of dll/so which could by loaded in run-time by name from request from Tester.
There is a list of preloaded services.
By default (it hard-coded) 2 services are auto-loaded: Type_A, Type_B.
Each services provides 2 function:
* function provides the name of service, using for finding particular services in the loaded services
* function takes string argument and returns string result. That result transfered upon Gateway to Tester.

# How to build
Set up the execution permission for all .sh files
run buildAll.sh

# How to test
Run up the set of prepared test cases
run0Param.sh
run1Param.sh
run2Params.sh
runUnknownService.sh
