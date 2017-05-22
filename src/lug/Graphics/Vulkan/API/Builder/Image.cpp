#include <vector>

#include <lug/System/Logger/Logger.hpp>

#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Image::Image(const API::Device& device): _device(device) {}

bool Image::build(API::Image& image, VkResult* returnResult) {
    std::vector<uint32_t> queueFamilyIndices(_queueFamilyIndices.begin(), _queueFamilyIndices.end());
    VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // If we have move than one queueFamilyIndices and exclusive was not manually set
    if (queueFamilyIndices.size() > 1 && !_exclusive) {
        sharingMode = VK_SHARING_MODE_CONCURRENT;
    }

    VkFormat imageFormat = API::Image::findSupportedFormat(&_device, _preferedFormats, _tiling, _featureFlags);
    if (imageFormat == VK_FORMAT_UNDEFINED) {
        LUG_LOG.error("Image::build: Can't find image format");
        return false;
    }

    // Create the image creation information for vkCreateImage
    VkImageCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = _createFlags,
        createInfo.imageType = _imageType,
        createInfo.format = imageFormat,
        {}, // createInfo.extent
        createInfo.mipLevels = _mipLevels,
        createInfo.arrayLayers = _arrayLayers,
        createInfo.samples = _sampleCount,
        createInfo.tiling = _tiling,
        createInfo.usage = _usage,
        createInfo.sharingMode = sharingMode,
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size()),
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data(),  // Convert the set to raw data,  // Convert the set to raw data,
        createInfo.initialLayout = _initialLayout
    };

    createInfo.extent.width = _extent.width;
    createInfo.extent.height = _extent.height;
    createInfo.extent.depth = _extent.depth;

    // Create the image
    VkImage vkImage{VK_NULL_HANDLE};
    VkResult result = vkCreateImage(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkImage);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    image = API::Image(vkImage, &_device, {_extent.width, _extent.height}, imageFormat, false);

    return true;
}

std::unique_ptr<API::Image> Image::build(VkResult* returnResult) {
    std::unique_ptr<API::Image> image = std::make_unique<API::Image>();
    return build(*image, returnResult) ? std::move(image) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
