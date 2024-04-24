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
        VulkanInstance(GLFWwindow* m_Window, const InstanceProps& props);

        ~VulkanInstance();

    private:
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
        VkPhysicalDevice physicalDevice;


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
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    };

}

