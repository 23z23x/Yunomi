#pragma once
#include "Yunomi/Render/Instance.h"
#include "Yunomi/Render/Window.h"
#include "Yunomi/Render/Buffer.h"
#include "Yunomi/Render/Texture.h"

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

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
    //Forward declarations
    class VulkanBuffer;


    struct VulkanInstanceProps
    {
        std::vector<const char*> VKvalidationLayers;
        std::vector<const char*> VKdeviceExtensions;

        //Defaults are for Vulkan, since it will work on most platforms
        VulkanInstanceProps(std::vector<const char*> validationlayers = { "VK_LAYER_KHRONOS_validation" },
            std::vector<const char*> deviceextensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME })
            : VKvalidationLayers(validationlayers), VKdeviceExtensions(deviceextensions)
        {
        }
    };


    class VulkanInstance : public Instance
    {
    public:
        VulkanInstance(GLFWwindow* m_Window, Shader* vertex, Shader* fragment, const VulkanInstanceProps& props = VulkanInstanceProps());

        ~VulkanInstance();

        //Buffer creation functions
        void CreateChunk(uint32_t size, VkBuffer* buffer, VkDeviceMemory* bufferMemory, uint32_t* ID, void* data, VkBufferUsageFlagBits vkType);
        void createUniformBuffers(std::vector<VkBuffer> *uniformBuffers, std::vector<VkDeviceMemory> *uniformBuffersMemory, std::vector<void*> *uniformBuffersMapped);

        void createTexture(std::string filename, VkImage* textureImage, VkDeviceMemory* textureImageMemory, VkImageView* textureImageView, VkSampler* textureSampler, uint32_t* mipLevels);

        //Buffer destruction functions
        void DeleteChunk(VkBuffer buffer, VkDeviceMemory bufferMemory, uint32_t ID);
        void destroyUniformBuffers(std::vector<VkBuffer> uniformBuffers, std::vector<VkDeviceMemory> uniformBuffersMemory);

        void destroyTexture(VkImage textureImage, VkDeviceMemory textureImageMemory, VkImageView textureImageView, VkSampler textureSampler);

        //Descriptor sets
        void createDescriptorSets(std::vector<VkBuffer>* uniformBuffers, VkImageView* textureImageView, VkSampler* textureSampler);

        //Drawing
        void VulkanStartDraw(VulkanBuffer* vertex, VulkanBuffer* index, VulkanBuffer* instance);
        void VulkanUpdateUniform(std::vector<void*>* uniformBuffersMapped);
        void VulkanEndDraw();

    private:
        //Frames in flight
        const int MAX_FRAMES_IN_FLIGHT = 2;

        uint32_t nextChunkID = 0;

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

        VkSampleCountFlagBits MaxMSAASamples = VK_SAMPLE_COUNT_1_BIT;
        VkSampleCountFlagBits MSAALevel = VK_SAMPLE_COUNT_1_BIT;

        //Descriptors
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        //Command pool/buffers
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        //Asynch primitives
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;

        //Variables shared between the start draw and stop draw methods
        uint32_t imageIndex = 0;
        VkResult result;

        //Depth buffering
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        //Images for MSAA
        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;


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
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

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
        public: void recreateSwapChain();

     private:
        //Image Views
        void createImageViews();

        //Graphics pipeline
        void createGraphicsPipeline(Shader* vertex, Shader* fragment);
        VkShaderModule createShaderModule(const std::vector<uint32_t>& code);
        void createRenderPass();
        void createDescriptorSetLayout();

        void createDescriptorPool();

        //Frame buffer
        void createFramebuffers();

        //Command pool/buffer
        void createCommandPool();
        void createCommandBuffers();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkBuffer vvb, uint32_t vvbSize, VkBuffer vib, uint32_t vibSize);

        //Asynch Primatives
        void createSyncObjects();

        //Creates a buffer for other classes
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        //Single time commands
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        //Texture creation helpers
        void createTextureSampler(VkSampler* textureSampler, uint32_t mipLevels);
        void createTextureImage(std::string filename, VkImage* textureImage, VkDeviceMemory* textureImageMemory, uint32_t* mipLevels);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t mipLevels, VkSampleCountFlagBits numSamples);
        void copyBufferToImage(VkBuffer* buffer, VkImage* image, uint32_t width, uint32_t height);
        void transitionImageLayout(VkImage* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

        //Depth buffering creation
        void createDepthResources();
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);

        //Mipmaps
        void generateMipmaps(VkImage* image, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

        //MSAA
        VkSampleCountFlagBits getMaxUsableSampleCount();
        void createColorResources();

        //Helper Methods
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    };

}

