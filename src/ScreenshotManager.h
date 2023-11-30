#pragma once

#include <string>

namespace ScreenshotManager
{
void TakeScreenshot(int width, int height);

std::string GetName();

constexpr const char* DirectoryPath = "./screenshots/";
constexpr const char* BaseName = "screenshot_";
constexpr const char* Extension = ".png";

}

