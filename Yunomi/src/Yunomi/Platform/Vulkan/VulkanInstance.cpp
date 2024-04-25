#include "pch.h"
#include "VulkanInstance.h"

namespace ynm
{
    Instance* Instance::Create(Window* m_Window, const InstanceProps& props)
    {
        return new VulkanInstance((GLFWwindow*) m_Window->getWindow(), props);
    }

    VulkanInstance::VulkanInstance(GLFWwindow* m_Window, const InstanceProps& props)
    {
        this->validationLayers = props.vk_ValidationLayers;
        this->deviceExtensions = props.vk_DeviceExtensions;
        this->window = m_Window;

        //Check for necessary validation layers, throw error if not avalible
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            YNM_CORE_ERROR("Vulkan: Validation layers requested, but not available!");
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
            YNM_CORE_ERROR("Vulkan: Failed to create Vulkan instance!");
            throw std::runtime_error("");
        }

        YNM_CORE_INFO("Vulkan: Instance created!");

        //Create surface using window
        if (glfwCreateWindowSurface(instance, m_Window, nullptr, &surface) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create window surface!");
        }

        YNM_CORE_INFO("Vulkan: Created window surface!");

        pickPhysicalDevice();

        YNM_CORE_INFO("Vulkan: Picked physical device!");

        createLogicalDevice();

        createSwapChain();

        createImageViews();

    }

    VulkanInstance::~VulkanInstance()
    {
        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        cleanupSwapChain();

        vkDestroyDevice(device, nullptr);

        YNM_CORE_INFO("Vulkan: Logical device destroyed!");

        vkDestroySurfaceKHR(instance, surface, nullptr);

        YNM_CORE_INFO("Vulkan: Surface destroyed!");

        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, nullptr);
        }

        vkDestroyInstance(instance, nullptr);

        YNM_CORE_INFO("Vulkan: Instance destroyed!");
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
        if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        {
            //Off for now, can uncomment this if they are ever needed
            //YNM_CORE_INFO("Vulkan Validation Layer Verbose: {0}", pCallbackData->pMessage);
        }
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT && messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            YNM_CORE_INFO("Vulkan Validation Layer Info: {0}", pCallbackData->pMessage);
        }
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT && messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            YNM_CORE_WARN("Vulkan Validation Layer Warning : {0}", pCallbackData->pMessage);
        }
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            YNM_CORE_ERROR("Vulkan Validation Layer Error: {0}", pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    void VulkanInstance::setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    //Physical Device
    void VulkanInstance::pickPhysicalDevice() {

        //Standard Vulkan ritual
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            YNM_CORE_ERROR("Vulkan: Failed to find GPUs with Vulkan support!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Found at least one GPU with Vulkan support!");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        //For each device, check if device is suitable
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        //Throw if no device found
        if (physicalDevice == VK_NULL_HANDLE) {
            YNM_CORE_ERROR("Vulkan: Failed to find a suitable GPU!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Found a suitable GPU!");
    }

    bool VulkanInstance::isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);

        //check if extensions are supported
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        //swap chain is adequate if the supported formats are not empty and the present modes are not empty
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    bool VulkanInstance::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        //Standard vulkan ritual
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        //Get a set that contains each extension name
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        //remove from set as we find them
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        //If it's empty return all good, else bad
        return requiredExtensions.empty();
    }

    //Logical Device
    void VulkanInstance::createLogicalDevice() {
        //Find all queue families avalible
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        //Create info struct for each unique queue type
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        //Check out the features of the physical device
        VkPhysicalDeviceFeatures deviceFeatures{};
        //Enable anistropic filtering
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        //If in debug, activate validation layers
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        //else, do nothing
        else {
            createInfo.enabledLayerCount = 0;
        }

        //check for successful creation
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create logical device!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Created logical device!");

        //Create the queues required
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        YNM_CORE_INFO("Vulkan: Graphics queue created!");
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
        YNM_CORE_INFO("Vulkan: Present queue created!");
    }

    //Swap Chain
    void VulkanInstance::createSwapChain()
    {
        //Query for swap chain support
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

        //Choose surface format and present mode
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

        //Choose resolution from capabilities
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
        //Record the minimum iamge count plus one
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        //If swap chain supports more than the minimum, set it to the maximum
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        //Create info struct for swap chain
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

        //TO DO: Says this is the only thing supported, look into this at a later date when alpha is needed.
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;



        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        //If our present and graphics queues are different, set the sharing mode to concurrent, else exclusive (for efficency in the case they are the same)
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        //Create swap chain with info
        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create swap chain!");
            throw std::runtime_error("");
        }

        YNM_CORE_INFO("Vulkan: Created swap chain!");

        //Get the images in the swap chain and the extent and save them for later
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;

    }

    VkSurfaceFormatKHR VulkanInstance::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        //Choose one that has 32 bit RGBA and with nonlinear color space
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        //If none exist, just return the first
        return availableFormats[0];
    }

    VkPresentModeKHR VulkanInstance::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        //from avalible modes, see if we have mailbox
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        //If not, this one is always there
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanInstance::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        //if window manager tells us that we're working with a 1:1 pixel ratio, we're good
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        //Else, (if someone is using a fancy monitor, high DPI) we need to scale the pixel size
        else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanInstance::cleanupSwapChain() {
        for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
        }

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            vkDestroyImageView(device, swapChainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    //Image Views
    void VulkanInstance::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        for (uint32_t i = 0; i < swapChainImages.size(); i++) {
            swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat);
        }

        YNM_CORE_INFO("Vulkan: Image Views created!");
    }

    //Helper Methods
    QueueFamilyIndices VulkanInstance::findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        //Vulkan ritual
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        //for all avalible queue families, see if they support our needs for graphics and present
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    SwapChainSupportDetails VulkanInstance::querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        //Get capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        //Vulkan ritual
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        //If we don't have any formats, don't query
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        //Vulkan ritual II
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkImageView VulkanInstance::createImageView(VkImage image, VkFormat format) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }


}
