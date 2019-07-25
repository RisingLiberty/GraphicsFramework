rem glslangValidator.exe -V ../src/VulkanTest.vert
rem glslangValidator.exe -V ../src/VulkanTest.frag
glslangValidator.exe -V ../src/vertexshader.vert
glslangValidator.exe -V ../src/fragmentshader.frag

MOVE frag.spv ../bin
MOVE vert.spv ../bin
pause