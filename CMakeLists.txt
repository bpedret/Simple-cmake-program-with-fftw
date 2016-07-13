cmake_minimum_required (VERSION 2.6)
project (simplep)

FIND_PACKAGE(CUDA)
message ("${FFTW3_INCLUDE_DIR}")

if (CUDA_FOUND)
	INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
	set (CUDA_FFTW ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
	LINK_DIRECTORIES (${CUDA_FFTW})
#For the CUDA libraries
	set (PROJECT_LINK_LIBCu -lcufftw)
	CUDA_ADD_EXECUTABLE (fftwC src/FFTW1.cpp)
	CUDA_ADD_EXECUTABLE (fftw2C src/FFTWr2r.cpp)
	add_definitions (-DFFTW)
#Link all the different libraries and definitions
	target_link_libraries (fftwC ${PROJECT_LINK_LIBCu})
	target_link_libraries (fftw2C ${PROJECT_LINK_LIBCu})

else (CUDA_FOUND)
	message ("CUDA not found, looking for FFTW3...")

	if (FFTW_INCLUDES)
# Already in cache, be silent
		set (FFTW_FIND_QUIETLY TRUE)
	endif (FFTW_INCLUDES)
	find_path (FFTW_INCLUDES fftw3.h)
	find_library (FFTW_LIBRARIES NAMES fftw3)
# handle the QUIETLY and REQUIRED arguments and set FFTW_FOUND to TRUE if
# all listed variables are TRUE
	include (FindPackageHandleStandardArgs)
	find_package_handle_standard_args (FFTW DEFAULT_MSG FFTW_LIBRARIES FFTW_INCLUDES)
	mark_as_advanced (FFTW_LIBRARIES FFTW_INCLUDES)	

	if (FFTW_INCLUDES)
#For the normal CPU libraries:
		set (PROJECT_LINK_LIB fftw3 fftw3f)
		add_executable (fftw src/FFTW1.cpp)
		add_executable (fftw2 src/FFTWr2r.cpp)
#Link all the different libraries and definitions
		target_link_libraries (fftw ${PROJECT_LINK_LIB})
		target_link_libraries (fftw2 ${PROJECT_LINK_LIB})
	else (FFTW_INCLUDES)
		message (FATAL_ERROR "You don't have fftw!")
	endif (FFTW_INCLUDES)
endif (CUDA_FOUND)
