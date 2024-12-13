#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <regex>

// Base class for all media files
class MediaFile {
protected:
    std::string filename;
    double duration;  // in seconds
    double size;      // in bytes
    std::string format;

public:
    MediaFile(const std::string& name, double dur, double sz, const std::string& fmt)
        : filename(name), duration(dur), size(sz), format(fmt) {}

    virtual ~MediaFile() = default;
    virtual void displayInfo() const = 0;

    // Format validation
    static bool isValidFormat(const std::string& format, const std::vector<std::string>& validFormats) {
        return std::find(validFormats.begin(), validFormats.end(), format) != validFormats.end();
    }

    // Size conversion utilities
    static std::string formatSize(double size_in_bytes) {
        const double KB = 1024;
        const double MB = KB * 1024;
        const double GB = MB * 1024;

        if (size_in_bytes >= GB) {
            return std::to_string(size_in_bytes / GB) + " GB";
        }
        else if (size_in_bytes >= MB) {
            return std::to_string(size_in_bytes / MB) + " MB";
        }
        else if (size_in_bytes >= KB) {
            return std::to_string(size_in_bytes / KB) + " KB";
        }
        return std::to_string(size_in_bytes) + " bytes";
    }
};

// Video specific class
class VideoFile : public MediaFile {
private:
    int width;
    int height;
    double frameRate;
    std::string videoCodec;
    std::vector<std::string> supportedFormats = { ".mp4", ".mkv", ".avi", ".mov" };

public:
    VideoFile(const std::string& name, double dur, double sz, const std::string& fmt,
        int w, int h, double fps, const std::string& codec)
        : MediaFile(name, dur, sz, fmt), width(w), height(h),
        frameRate(fps), videoCodec(codec) {
        validateFormat();
    }

    void validateFormat() const {
        if (!isValidFormat(format, supportedFormats)) {
            throw std::invalid_argument("Unsupported video format. Supported formats: mp4, mkv, avi, mov");
        }
    }

    double calculateBitrate() const {
        return (size * 8) / (duration * 1000000); // Returns Mbps
    }

    std::string getResolutionName() const {
        if (width >= 3840 && height >= 2160) return "4K";
        if (width >= 1920 && height >= 1080) return "1080p";
        if (width >= 1280 && height >= 720) return "720p";
        return "SD";
    }

    void displayInfo() const override {
        std::cout << "\n=== Video Information ===\n";
        std::cout << "Filename: " << filename << format << "\n";

        // Duration formatting
        int hours = static_cast<int>(duration) / 3600;
        int minutes = (static_cast<int>(duration) % 3600) / 60;
        int seconds = static_cast<int>(duration) % 60;

        std::cout << "Duration: "
            << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds << "\n";

        std::cout << "Size: " << formatSize(size) << "\n";
        std::cout << "Resolution: " << width << "x" << height
            << " (" << getResolutionName() << ")\n";
        std::cout << "Frame Rate: " << frameRate << " fps\n";
        std::cout << "Video Codec: " << videoCodec << "\n";
        std::cout << "Bitrate: " << std::fixed << std::setprecision(2)
            << calculateBitrate() << " Mbps\n";
        std::cout << "=====================\n";
    }
};

int main() {
    try {
        std::string filename, format, codec;
        double duration, size, frameRate;
        int width, height;

        std::cout << "Enter video information:\n";

        std::cout << "Filename (without extension): ";
        std::getline(std::cin, filename);

        std::cout << "Format (e.g., .mp4, .mkv): ";
        std::getline(std::cin, format);

        std::cout << "Duration (in seconds): ";
        while (!(std::cin >> duration) || duration <= 0) {
            std::cout << "Please enter a valid duration: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Size (in bytes): ";
        while (!(std::cin >> size) || size <= 0) {
            std::cout << "Please enter a valid size: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Width (pixels): ";
        while (!(std::cin >> width) || width <= 0) {
            std::cout << "Please enter a valid width: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Height (pixels): ";
        while (!(std::cin >> height) || height <= 0) {
            std::cout << "Please enter a valid height: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Frame Rate (fps): ";
        while (!(std::cin >> frameRate) || frameRate <= 0) {
            std::cout << "Please enter a valid frame rate: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cin.ignore(); // Clear newline from previous input
        std::cout << "Video Codec (e.g., H.264, H.265): ";
        std::getline(std::cin, codec);

        VideoFile video(filename, duration, size, format, width, height, frameRate, codec);
        video.displayInfo();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}