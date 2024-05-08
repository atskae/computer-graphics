# Compiler flags
CFLAGS = -std=c++17 -O2
# Linker flags
# We use GLFW and the Vulkan loader
# Other flags are used by GLFW for threading and window management
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
