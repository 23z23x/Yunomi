#pragma once
#include "Yunomi/Render/Instance.h"
#include "Yunomi/Render/Window.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

//Structs
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


namespace ynm
{
    class VulkanInstance : public Instance
    {
    public:
        VulkanInstance(GLFWwindow* m_Window, Shader* vertex, Shader* fragment, const InstanceProps& props);

        ~VulkanInstance();

    private:
        //Window
        GLFWwindow* window;
        //Vulkan instance
        VkInstance instance;
        //Debug Messenger
        VkDebugUtilsMessengerEXT debugMessenger;

        //Holds the names of validation layers
        std::vector<const char*> validationLayers;
        //Holds the names of device extensions
        std::vector<const char*> deviceExtensions;

        //Vulkan surface object
        VkSurfaceKHR surface;

        //Physical Device chosen
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        //Logical Device created
        VkDevice device;

        //Queue for graphics rendering
        VkQueue graphicsQueue;
        //Queue for displaying to screen
        VkQueue presentQueue;

        //Swap chain
        VkSwapchainKHR swapChain;
        //Images on the swap chain
        std::vector<VkImage> swapChainImages;
        //Image format of the swap chain
        VkFormat swapChainImageFormat;
        //Resolution of the images
        VkExtent2D swapChainExtent;
        //Image views for the images
        std::vector<VkImageView> swapChainImageViews;
        //Swap chain framebuffers
        std::vector<VkFramebuffer> swapChainFramebuffers;

        //Graphics pipeline
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        VkDescriptorSetLayout descriptorSetLayout;
        VkRenderPass renderPass;

        //Command pool/buffers
        VkCommandPool commandPool;


        //Class Methods
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();

        //Get/Set
        std::vector<const char*> getValidationLayers();
        VkInstance getInstance();

        //Debug
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);    
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        void setupDebugMessenger();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    
        //Physical Device
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        //Logical Device
        void createLogicalDevice();

        //Swap Chain
        void createSwapChain();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void cleanupSwapChain();

        //Image Views
        void createImageViews();

        //Graphics pipeline
        void createGraphicsPipeline(Shader* vertex, Shader* fragment);
        VkShaderModule createShaderModule(const std::vector<uint32_t>& code);
        void createRenderPass();
        void createDescriptorSetLayout();

        //Frame buffer
        void createFramebuffers();

        //Command pool/buffer
        void createCommandPool();

        //Helper Methods
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkImageView createImageView(VkImage image, VkFormat format);
    };

}

