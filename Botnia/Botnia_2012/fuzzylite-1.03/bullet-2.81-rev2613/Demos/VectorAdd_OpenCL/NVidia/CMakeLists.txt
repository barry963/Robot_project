



INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src 
	${NVIDIA_OPENCL_INCLUDES}
)

LINK_LIBRARIES(
	 LinearMath 
	${NVIDIA_OPENCL_LIBRARIES}
)

ADD_EXECUTABLE(AppVectorAdd_NVidia
../MiniCL_VectorAdd.cpp
../VectorAddKernels.cl
)

IF (UNIX)
  TARGET_LINK_LIBRARIES(AppVectorAdd_NVidia pthread)
ENDIF(UNIX)

IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
			SET_TARGET_PROPERTIES(AppVectorAdd_NVidia PROPERTIES  DEBUG_POSTFIX "_Debug")
			SET_TARGET_PROPERTIES(AppVectorAdd_NVidia PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
			SET_TARGET_PROPERTIES(AppVectorAdd_NVidia PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)