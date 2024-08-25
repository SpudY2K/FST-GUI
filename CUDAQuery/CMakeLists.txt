add_library(CUDAQuery STATIC
	"CUDAQuery.cpp"
	"CUDAQuery.hpp")
	
set_target_properties(CUDAQuery PROPERTIES CUDA_ARCHITECTURES "53")
set_source_files_properties(CUDAQuery.cpp PROPERTIES LANGUAGE CUDA)
target_include_directories(CUDAQuery PRIVATE ..)
