#include "pch.h"
#include "VulkanInstance.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include <stb_image.h>
#include "VulkanBuffer.h"
#include "VulkanPipeline.h"
#include <Yunomi/Game_Objects/Object.h>

namespace ynm
{
    //Vertex binding description

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    //InstanceData binding description

    static VkVertexInputBindingDescription getInstanceBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(InstanceData);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        return bindingDescription;
    }

    //Attribute descriptions

    static std::array<VkVertexInputAttributeDescription, 8> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 8> attributeDescriptions{};
        
        //Object coordinates
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        //Texture coordinates
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        // Instance attributes
        //Model matrix
        for (uint32_t i = 0; i < 4; ++i) {
            attributeDescriptions[2 + i].binding = 1;
            attributeDescriptions[2 + i].location = 2 + i;
            attributeDescriptions[2 + i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[2 + i].offset = offsetof(InstanceData, modelMatrix) + i * sizeof(glm::vec4);
        }

        attributeDescriptions[6].binding = 1;
        attributeDescriptions[6].location = 6;
        attributeDescriptions[6].format = VK_FORMAT_R32_UINT;
        attributeDescriptions[6].offset = offsetof(InstanceData, textureID);

        attributeDescriptions[7].binding = 1;
        attributeDescriptions[7].location = 7;
        attributeDescriptions[7].format = VK_FORMAT_R32_UINT;
        attributeDescriptions[7].offset = offsetof(InstanceData, ID);

        return attributeDescriptions;
    }

    Instance* Instance::instanceref = nullptr;
    //Implementation of Instance methods
    Instance* Instance::Create(Window* m_Window, const InstanceProps& props)
    {
        //Convert InstanceProps to VulkanInstanceProps
        /*VulkanInstanceProps vulkanProps;
        vulkanProps.VKvalidationLayers = props.validationLayers;
        YNM_CORE_INFO("{0}", vulkanProps.VKdeviceExtensions[0]);

        for (const char* extension : props.deviceExtensions)
        {
            if(strcmp(extension, "VK_KHR_SWAPCHAIN_EXTENSION_NAME") == 0)
            { 
                vulkanProps.VKdeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            }
        }*/

        Instance* instance = new VulkanInstance((GLFWwindow*) m_Window->getWindow()/*, vulkanProps*/);
        instanceref = instance;
        return instance;
    }

    void Instance::SetPipeline(Pipeline* pipeline)
    {
        VulkanInstance* instance = (VulkanInstance*)instanceref;
        instance->VulkanSetPipeline((VulkanPipeline*)pipeline);
        instance->createFramebuffers();
    }

    void Instance::AddDescriptors(UniformBuffer* ub, std::vector<Texture*> tx)
    {
        VulkanInstance* instance = (VulkanInstance*) instanceref;

        std::vector<VkImageView*> imageViews;
        std::vector<VkSampler*> imageSamplers;

        for (Texture* texture : tx)
        {
            imageViews.push_back((VkImageView*)texture->getImageView());
            imageSamplers.push_back((VkSampler*)texture->getTextureSampler());
        }

        instance->createDescriptorSets((std::vector<VkBuffer>*)ub->getBuffer(), imageViews, imageSamplers, tx.size());
    }

    void Instance::StartDraw(Buffer* vertex, Buffer* index, Buffer* instance)
    {
        VulkanInstance* vkinstance = (VulkanInstance*)instanceref;
        VulkanBuffer* vulkanVertex = (VulkanBuffer*)vertex;
        VulkanBuffer* vulkanIndex = (VulkanBuffer*)index;
        VulkanBuffer* vulkanInstance = (VulkanBuffer*)instance;

        vkinstance->VulkanStartDraw(vulkanVertex, vulkanIndex, vulkanInstance);
    }

    void Instance::UpdateUniform(UniformBuffer* ub, UniformBufferObject* ubo)
    {
        VulkanInstance* instance = (VulkanInstance*)instanceref;
        instance->VulkanUpdateUniform((std::vector<void*>*)ub->getMap(), ubo);
    }

    void Instance::EndDraw()
    {
        VulkanInstance* instance = (VulkanInstance*)instanceref;
        instance->VulkanEndDraw();
    }

    //Vulkan Instance implementation

    VulkanInstance::VulkanInstance(GLFWwindow* m_Window, const VulkanInstanceProps& props)
    {
        this->validationLayers = props.VKvalidationLayers;
        this->deviceExtensions = props.VKdeviceExtensions;
        this->window = m_Window;

        //Check for necessary validation layers, throw error if not avalible
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw VulkanError("Requested validation layers are unavailable.", "VulkanInstance.cpp | 125");
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

        //Create instance with info
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw VulkanError("Instance creation failed.", "VulkanInstance.cpp | 155");
        }

        YNM_CORE_INFO("Vulkan: Instance created!");

        setupDebugMessenger();

        //Create surface using window
        if (glfwCreateWindowSurface(instance, m_Window, nullptr, &surface) != VK_SUCCESS) {
            throw VulkanError("Surface creation failed.", "VulkanInstance.cpp | 164");
        }

        YNM_CORE_INFO("Vulkan: Created window surface!");

        pickPhysicalDevice();

        YNM_CORE_INFO("Vulkan: Picked physical device!");

        createLogicalDevice();

        createSwapChain();

        createImageViews();

        createDescriptorSetLayout();

        createCommandPool();

        createCommandBuffers();

        createSyncObjects();

        createDescriptorPool();

    }

    VulkanInstance::~VulkanInstance()
    {

        cleanupSwapChain();

        pipeline->~VulkanPipeline();

        vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        //This also cleans up the command buffers
        vkDestroyCommandPool(device, commandPool, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        vkDestroyDevice(device, nullptr);

        YNM_CORE_INFO("Vulkan: Logical device destroyed!");

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(instance, surface, nullptr);

        YNM_CORE_INFO("Vulkan: Surface destroyed!");

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

    void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
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
                
                MaxMSAASamples = getMaxUsableSampleCount();
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
        //Record the minimum image count plus one
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 2;

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
        createInfo.presentMode = presentMode;
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
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
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

    void VulkanInstance::cleanupSwapChain() 
    {

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
            try {
                swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            }
            catch (const std::runtime_error& e) {
                // Log the error and continue with the next image view
                YNM_CORE_ERROR("Failed to create image view for swap chain image {0}: {1}", i, e.what());
                // You can choose to handle the error differently based on your application's requirements
                // For example, you might want to throw the error and stop further execution
            }
        }

        YNM_CORE_INFO("Vulkan: Image Views created!");
    }

    //Options for how things are drawn
    void VulkanInstance::createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding textureBinding = {};
        textureBinding.binding = 1;
        textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        textureBinding.descriptorCount = YNM_MAX_TEXTURES; // Define the maximum number of textures you want to support
        textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, textureBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create descriptor set layout!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Successfully created descriptor set layout!");
    }

    void VulkanInstance::createDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * YNM_MAX_TEXTURES;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create descriptor pool!");
            throw std::runtime_error("");
        }
    }

    void VulkanInstance::createDescriptorSets(std::vector<VkBuffer>* uniformBuffers, std::vector<VkImageView*> textureImageViews, std::vector<VkSampler*> textureSamplers, uint32_t activeTextureCount) {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to allocate descriptor sets!");
            throw std::runtime_error("");
        }

        YNM_CORE_INFO("Vulkan: Successfully allocated descriptor sets!");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = (*uniformBuffers)[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            std::vector<VkDescriptorImageInfo> imageInfos(YNM_MAX_TEXTURES);
            for (size_t j = 0; j < activeTextureCount; ++j) {
                imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfos[j].imageView = *textureImageViews[j];
                imageInfos[j].sampler = *textureSamplers[j];
                std::cout << j << std::endl;
            }
            // Set the rest of the imageInfos to default values
            for (size_t j = activeTextureCount; j < YNM_MAX_TEXTURES; ++j) {
                imageInfos[j] = {};
                imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfos[j].imageView = *textureImageViews[0];
                imageInfos[j].sampler = *textureSamplers[0];
            }

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = YNM_MAX_TEXTURES;
            descriptorWrites[1].pImageInfo = imageInfos.data();

            vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    //Frame buffer
    void VulkanInstance::createFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            std::array<VkImageView, 3> attachments = {
                *(pipeline->getColorImageView()),
                *(pipeline->getDepthImageView()),
                swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = *(pipeline->getRenderPass());
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                YNM_CORE_ERROR("Vulkan: Failed to create framebuffer!");
                throw std::runtime_error("");
            }
            YNM_CORE_INFO("Vulkan: Successfully created framebuffer!");
        }

    }

    //Command Pool/Buffer
    void VulkanInstance::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create command pool!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Successfully created command pool!");
    }

    void VulkanInstance::createCommandBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to allocate command buffers!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Successfully allocated command buffers");
    }

    void VulkanInstance::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VulkanBuffer* vertex, VulkanBuffer* index, VulkanBuffer* instance) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        // Set as one time here because, right now and for the forseeable future, technically all uses of the command buffer will be "one time" because it is used once per frame.
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to begin recording command buffer!");
            throw std::runtime_error("");
        }
        //YNM_CORE_INFO("Vulkan: Successfully began recording command buffer!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = *(pipeline->getRenderPass());
        renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 0.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *(pipeline->getPipeline()));

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        //Offsets into Vertex and Index buffers
        uint32_t vertexOffset = 0;
        uint32_t indexOffset = 0;

        //We can bind the index buffer out here since its always the same.
        vkCmdBindIndexBuffer(commandBuffer, index->getChunks()[0]->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        //For all meshes being rendered
        for (unsigned int i = 0; i < instance->getChunks().size(); i++)
        {
            //Bind the vertex buffer, and the current instance buffer
            VkBuffer vertexBuffers[] = { vertex->getChunks()[0]->getBuffer(), instance->getChunks()[i]->getBuffer() };
            VkDeviceSize offsets[] = { 0, 0 };
            //Subsequent calls overwrite what is bound, so vertex buffer is just bound here.
            vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *(pipeline->getPipelineLayout()), 0, 1, &descriptorSets[currentFrame], 0, nullptr);

            //Draw using current instance buffer and vertex and index offsets.
            vkCmdDrawIndexed(commandBuffer, index->getChunks()[0]->getOffsets()[i], instance->getChunks()[i]->getCount(), indexOffset, vertexOffset, 0);

            vertexOffset += vertex->getChunks()[0]->getOffsets()[i];
            indexOffset += index->getChunks()[0]->getOffsets()[i];
        }
        //

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to record command buffer!");
            throw std::runtime_error("");
        }
        //YNM_CORE_INFO("Vulkan: Successfully recorded to command buffer!");
    }

    void VulkanInstance::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        semaphoreInfo.flags = 0;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                YNM_CORE_ERROR("Vulkan: Failed to create synch object!");
                throw std::runtime_error("");
            }
        }
        YNM_CORE_INFO("Vulkan: Successfully created synch objects!");
    }

    //Buffer creation for other classes
    void VulkanInstance::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, float priority) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryPriorityAllocateInfoEXT priorityInfo = {};
        priorityInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        priorityInfo.priority = priority;

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = &priorityInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    uint32_t VulkanInstance::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        YNM_CORE_ERROR("Vulkan: Failed to find suitable memory type!");
		return UINT32_MAX;
    }

    //For now, each call to copyBuffer is going to create a command buffer and use the SingleTimeCommands() method. Need
    //to look into if this is generally how buffers are coppied or if I should write some code that loads everything at launch
    //in a loop with one queue.
    void VulkanInstance::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    //Buffer class initialization

    void VulkanInstance::CreateChunk(uint32_t size, VkBuffer* buffer, VkDeviceMemory* bufferMemory, uint32_t* ID, void* data, VkBufferUsageFlagBits vkType, BufferType type)
    {
        VkDeviceSize bufferSize = 0;
        float priority = 0;

        switch (type)
        {
        case BufferType::VERTEX:
            bufferSize = size * sizeof(Vertex);
            priority = 0.8f;
            break;
        case BufferType::INDEX:
            bufferSize = size * sizeof(uint32_t);
            priority = 0.8f;
            break;
        case BufferType::INSTANCE:
            bufferSize = size * sizeof(InstanceData);
            priority = 0.8f;
            break;
        default:
            YNM_CORE_ERROR("Vulkan: Unaccounted for buffer type created!");
            break;
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, 0.0f);

        void* staging;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &staging);
        memcpy(staging, data, (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | vkType, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *buffer, *bufferMemory, priority);

        copyBuffer(stagingBuffer, *buffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        //Technically, you could run out of IDs like this, but if a project ends up generating 2 billion chunks, then I will have run into bigger problems.
        *ID = this->nextChunkID;
        this->nextChunkID++;

        YNM_CORE_INFO("Vulkan: Chunk created! ID: {0}", *ID);
    }

    void VulkanInstance::createUniformBuffers(std::vector<VkBuffer> *uniformBuffers, std::vector<VkDeviceMemory> *uniformBuffersMemory, std::vector<void*> *uniformBuffersMapped) {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        (*uniformBuffers).resize(MAX_FRAMES_IN_FLIGHT);
        (*uniformBuffersMemory).resize(MAX_FRAMES_IN_FLIGHT);
        (*uniformBuffersMapped).resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*uniformBuffers)[i], (*uniformBuffersMemory)[i], 0.0f);

            vkMapMemory(device, (*uniformBuffersMemory)[i], 0, bufferSize, 0, &(*uniformBuffersMapped)[i]);
        }

        YNM_CORE_INFO("Vulkan: Uniform buffer created!");
    }

    void VulkanInstance::createTexture(std::string filename, VkImage* textureImage, VkDeviceMemory* textureImageMemory, VkImageView* textureImageView, VkSampler* textureSampler, uint32_t* mipLevels)
    {
        createTextureImage(filename, textureImage, textureImageMemory, mipLevels);
        *textureImageView = createImageView(*textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, *mipLevels);
        createTextureSampler(textureSampler, *mipLevels);

        YNM_CORE_INFO("Vulkan: Texture created!");
    }

    //Buffer class destruction
    void VulkanInstance::DeleteChunk(VkBuffer buffer, VkDeviceMemory bufferMemory, uint32_t ID)
    {
        vkDestroyBuffer(device, buffer, nullptr);
        vkFreeMemory(device, bufferMemory, nullptr);
        YNM_CORE_INFO("Vulkan: Chunk destroyed! ID: {0}", ID);
    }

    void VulkanInstance::destroyUniformBuffers(std::vector<VkBuffer> uniformBuffers, std::vector<VkDeviceMemory> uniformBuffersMemory)
    {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
        }
        YNM_CORE_INFO("Vulkan: Uniform buffers destroyed!");
    }

    void VulkanInstance::destroyTexture(VkImage textureImage, VkDeviceMemory textureImageMemory, VkImageView textureImageView, VkSampler textureSampler)
    {
        vkDestroySampler(device, textureSampler, nullptr);

        vkDestroyImageView(device, textureImageView, nullptr);


        vkDestroyImage(device, textureImage, nullptr);
        vkFreeMemory(device, textureImageMemory, nullptr);

        YNM_CORE_INFO("Vulkan: Texture destroyed!");
    }

    //Single time commands
    VkCommandBuffer VulkanInstance::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void VulkanInstance::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

    //Texture creation helpers

    void VulkanInstance::createTextureSampler(VkSampler* textureSampler, uint32_t mipLevels) {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        //TODO: SETTING
        samplerInfo.anisotropyEnable = VK_FALSE;

        //Set up a query for this at the beginning and pass
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        samplerInfo.maxAnisotropy = 1.0f;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f; // Optional
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f; // Optional

        if (vkCreateSampler(device, &samplerInfo, nullptr, textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }


    }

    void VulkanInstance::createTextureImage(std::string filename, VkImage* textureImage, VkDeviceMemory* textureImageMemory, uint32_t* mipLevels) {
        int texWidth, texHeight, texChannels;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        *mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

        if (!pixels) {
            YNM_CORE_ERROR("Vulkan: Failed to load texture image!");
            throw std::runtime_error("");
        }

        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, 1.0f);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(device, stagingBufferMemory);

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT |VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *textureImage, *textureImageMemory, *mipLevels, VK_SAMPLE_COUNT_1_BIT);

        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, *mipLevels);
        copyBufferToImage(&stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        generateMipmaps(textureImage, texWidth, texHeight, *mipLevels);
    }

    void VulkanInstance::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t mipLevels, VkSampleCountFlagBits numSamples) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = numSamples;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create image!");
            throw std::runtime_error("");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryPriorityAllocateInfoEXT priorityInfo = {};
        priorityInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        priorityInfo.priority = 1.0f;

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = &priorityInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to allocate image memory!");
            throw std::runtime_error("");
        }

        vkBindImageMemory(device, image, imageMemory, 0);
    }

    void VulkanInstance::copyBufferToImage(VkBuffer* buffer, VkImage* image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            *buffer,
            *image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        endSingleTimeCommands(commandBuffer);
    }

    void VulkanInstance::transitionImageLayout(VkImage* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;

        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = *image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            YNM_CORE_ERROR("Vulkan: Unsupported texture layout transition!");
            throw std::invalid_argument("");
        }
        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        endSingleTimeCommands(commandBuffer);
    }

    //DRAW COMMANDS
    void VulkanInstance::VulkanStartDraw(VulkanBuffer* vertex, VulkanBuffer* index, VulkanBuffer* instance)
    {
        //Fence which stops if CPU is ahead of GPU
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        //YNM_CORE_INFO("Vulkan: Successfully acquired swap chain image!");

        // Only reset the fence if we are submitting work
        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        //Reset, then start recording to the command buffer
        vkResetCommandBuffer(commandBuffers[currentFrame], 0);
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex, vertex, index, instance);
    }

    void VulkanInstance::VulkanUpdateUniform(std::vector<void*>* uniformBuffersMapped, UniformBufferObject *ubo)
    {
        memcpy((*uniformBuffersMapped)[currentFrame], ubo, sizeof(*ubo));
    }

    void VulkanInstance::VulkanEndDraw()
    {
        //Info for the buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        //Submit the draw operation to the graphics queue
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to submit draw command buffer!");
            throw std::runtime_error("");
        }

        //YNM_CORE_INFO("Vulkan: Successfully submitted draw command buffer!");

        //Info for presentation
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr; // Optional

        //Submit the operation to the present queue
        result = vkQueuePresentKHR(presentQueue, &presentInfo);

        if (result != VK_SUCCESS) 
        {
            YNM_CORE_ERROR("Vulkan: Failed to present swap chain image!");
            throw std::runtime_error("");
        }

        //YNM_CORE_INFO("Vulkan: Successfully presented swap chain image!");

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    //Mipmap
    void VulkanInstance::generateMipmaps(VkImage* image, int32_t mipWidth, int32_t mipHeight, uint32_t mipLevels) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        //These stay the same for all levels
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = *image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        //Loop recording each command
        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            //Specify regions used
            vkCmdBlitImage(commandBuffer,
                *image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                *image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        endSingleTimeCommands(commandBuffer);
    }

    //MSAA
    VkSampleCountFlagBits VulkanInstance::getMaxUsableSampleCount()
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
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

    VkImageView VulkanInstance::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }

    std::array<VkVertexInputBindingDescription, 2> VulkanInstance::VkgetBindingDescriptions()
    {
        auto vertexBindingDescription = getBindingDescription();
        auto instanceBindingDescription = getInstanceBindingDescription();

        std::array<VkVertexInputBindingDescription, 2> bindingDescriptions =
        {
            vertexBindingDescription,
            instanceBindingDescription
        };

        return bindingDescriptions;
    }

    std::array<VkVertexInputAttributeDescription, 8> VulkanInstance::VkgetAttributeDescriptions()
    {
        return getAttributeDescriptions();
    }


}
