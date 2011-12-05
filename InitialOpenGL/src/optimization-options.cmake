IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

message("Using build type ${CMAKE_BUILD_TYPE}")
if(CMAKE_COMPILER_IS_GNUCXX)
	message("Compiler is Gcc")
  
    set(CPU_ARCH "native")
	if("${CMAKE_SYSTEM_NAME}" EQUAL "Windows" AND "$ENV{NUMBER_OF_PROCESSORS}" EQUAL "8")
	    set(CPU_ARCH "corei7")
	    message("Using i7 without AVX")
	endif()
	set(CMAKE_CXX_FLAGS_DEBUG " -Wall -Wno-parentheses -std=c++0x -march=${CPU_ARCH} -mtune=native -O2")
	set(CMAKE_CXX_FLAGS_RELEASE "  -Wall -Wno-parentheses -std=c++0x -DNDEBUG -march=${CPU_ARCH} -mtune=native -O3 ")
	

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftree-loop-distribution -ftree-loop-im -ftree-loop-ivcanon -fivopts")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fassociative-math -fno-trapping-math -fno-signed-zeros -fno-math-errno")
  #-ffast-math? or -ffinite-math-only  -funsafe-math-optimizations
  
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops") 
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funsafe-loop-optimizations ")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftracer")

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s ")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -s ")
	set(CMAKE_LIBRARY_LINKER_FLAGS "${CMAKE_LIBRARY_LINKER_FLAGS} -s ")
elseif(MSVC)
	message("Compiler is msvc")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa /arch:SSE2 /W4") #SEH exceptions,SSE2 support, extra warnings
	set(CMAKE_CXX_FLAGS_RELEASE " /Ox /Ot /GL /Oy /Oi /GS- /fp:fast /fp:except- /MD -DNDEBUG")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /OPT:REF /OPT:ICF /MACHINE:X64 /INCREMENTAL:NO")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LTCG /OPT:REF /OPT:ICF /MACHINE:X64  /INCREMENTAL:NO")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}  /LTCG /OPT:REF /OPT:ICF /MACHINE:X64  /INCREMENTAL:NO")
	set(CMAKE_LIBRARY_LINKER_FLAGS "${CMAKE_LIBRARY_LINKER_FLAGS}  /LTCG /OPT:REF /OPT:ICF /MACHINE:X64  /INCREMENTAL:NO")
else()
	message(FATAL_ERROR "Unrecognized compiler!")
endif()

