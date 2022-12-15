EXECUTABLE := TiliaEngine

BUILD_DIR := .\build\\

CXX_SRCS := \
	Core\main.cpp \
	Core\Modules\Console\Logging.cpp \
	Core\Modules\Exceptions\Tilia_Exception.cpp \
	Core\Modules\File_System\Windows\File_System.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Texture_files\Cube_Map.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Texture_files\Cube_Map_Data.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Shader_files\Shader_Part.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Shader_files\Shader.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Texture_files\Texture.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Texture_files\Texture_2D.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Abstractions\Shader_files\Uniform_Buffer.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Batch.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Error_Handling.cpp \
	Core\Modules\Rendering\OpenGL\3.3\Renderer.cpp \
	Core\Temp\Input.cpp \
	Core\Temp\Limit_Fps.cpp \
	Core\Temp\Stopwatch.cpp \
	Core\Values\OpenGL\3.3\Utils.cpp \
	vendor\glm\include\glm\detail\glm.cpp \

C_SRCS := \
	vendor\glad\src\glad.c

OBJS := $(CXX_SRCS:%.cpp=$(BUILD_DIR)%.o)
OBJS += $(C_SRCS:%.c=$(BUILD_DIR)%.o)

LIBS := -lglfw3 -lopengl32 -lgdi32 -lpthread
LIBS_DIR := -L.\vendor\glfw\lib

CXX := g++
EXE_FLAGS := $(OBJS) -o $(BUILD_DIR)$(EXECUTABLE).exe $(LIBS_DIR) $(LIBS)
C_FLAGS := -Ofast -g -static -static-libgcc -static-libstdc++ -std=c++14
CPP_FLAGS := -I.

RM := del

.PHONY: all force clean

all: $(BUILD_DIR)$(EXECUTABLE).exe

force: 
	make -B

run: $(BUILD_DIR)$(EXECUTABLE).exe
	$(BUILD_DIR)$(EXECUTABLE).exe

$(BUILD_DIR)$(EXECUTABLE).exe: $(OBJS)
	$(CXX) $(C_FLAGS) $(CPP_FLAGS) $(EXE_FLAGS)

$(BUILD_DIR)%.o: %.cpp
	if not exist $(@D) ( mkdir $(@D) )
	$(CXX) $(C_FLAGS) $(CPP_FLAGS) -c $< -o $@

$(BUILD_DIR)%.o: %.c
	if not exist $(@D) ( mkdir $(@D) )
	$(CXX) $(C_FLAGS) $(CPP_FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BUILD_DIR)$(EXECUTABLE).exe