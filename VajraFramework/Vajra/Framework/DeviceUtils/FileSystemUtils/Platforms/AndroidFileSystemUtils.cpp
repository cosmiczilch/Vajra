#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#ifdef PLATFORM_ANDROID

FileSystemUtils::FileSystemUtils() {
    this->init();
}

FileSystemUtils::~FileSystemUtils() {
    this->destroy();
}

void FileSystemUtils::init() {
    // TODO [Implement] Implement proper deviceResourcesPath on Android:
    this->deviceBaseResourcesPath = "/sdcard/download/";
    this->devicePictureResourcesPath = this->deviceBaseResourcesPath + "pictures/";
    this->deviceShaderResourcesPath  = this->deviceBaseResourcesPath + "shaders/";
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_ANDROID