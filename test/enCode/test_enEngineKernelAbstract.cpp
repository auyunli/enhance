#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineKernelAbstract.h"
#include "enComponentType.h"
#include "enComponentMeta.h"

using namespace std;


class TestClassA : public enComponentMeta {
public:
    int _val;
    TestClassA() : _val(1), enComponentMeta(enComponentType::LOGIC_FRONTEND) {
    }
};

class TestClassB : public enComponentMeta {
public:
    int _val;
    TestClassB() : _val(2), enComponentMeta(enComponentType::LOGIC_BACKEND) {
    }
};

TEST_CASE( "EnEngineKernelAbstract", "[EnEngineKernelAbstract]" ) {
    SECTION( "RegisterComponent, GetComponent" ) {

	TestClassA instance_a;
	TestClassB instance_b;

	enEngineKernelAbstract engine_core;
	engine_core.RegisterComponent( &instance_a );
	engine_core.RegisterComponent( &instance_b );

	CHECK( engine_core._components.size() == 2 );

	TestClassA * component_frontend = engine_core.GetComponent<TestClassA>(enComponentType::LOGIC_FRONTEND);
	CHECK( component_frontend == &instance_a );
	
	TestClassB * component_backend = engine_core.GetComponent<TestClassB>(enComponentType::LOGIC_BACKEND);
	CHECK( component_backend == &instance_b );
    }
}
