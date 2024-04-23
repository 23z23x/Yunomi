#include "pch.h"
#include "VulkanInstance.h"

namespace ynm
{
    VulkanInstance::VulkanInstance(std::vector<const char*> validationLayers)
    {
        this->validationLayers = validationLayers;

        //Check for necessary validation layers, throw error if not avalible
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            YNM_CORE_ERROR("validation layers requested, but not available!");
            throw std::runtime_error("");
        }

        //Info about application
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        //Name
        appInfo.pApplicationName = "Yunomi";
        //Version
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        //Engine
        appInfo.pEngineName = "Yunomi Engine";
        //Engine Version
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        //Highest API version meant to be used
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //Fill out create info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        //Get required extensions so we can make sure they are usable
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        //Create debug info
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        //Create instance with info
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            YNM_CORE_ERROR("Failed to create Vulkan instance!");
            throw std::runtime_error("");
        }

        YNM_CORE_INFO("Vulkan instance created!");

    }

    VulkanInstance::~VulkanInstance()
    {
        vkDestroyInstance(instance, nullptr);
    }

    //Class Methods
    bool VulkanInstance::checkValidationLayerSupport()
    {
        uint32_t layerCount;

        //Vulkan ritual
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        //For all validation layers, compare the ones we need to the ones we have been told are avalible
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> VulkanInstance::getRequiredExtensions() {
        //Get extensions needed for glfw
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        //add validation layers if we are using them
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    //Get/Set
    std::vector<const char*> VulkanInstance::getValidationLayers()
    {
        return validationLayers;
    }

    VkInstance VulkanInstance::getInstance()
    {
        return instance;
    }

    //Debug
    void VulkanInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        YNM_CORE_ERROR("Vulkan Validation Layer: {0}", pCallbackData->pMessage);

        return VK_FALSE;
    }
}
