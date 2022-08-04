#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <utility>
#include <iterator>
#include <raspicam_cv.h>

#include <opencv4/opencv2/opencv.hpp>
#include <sockpp/udp_socket.h>

#define MAX_DGRAM_SIZE 65536U
#define MAX_IMG_DGRAM_SIZE MAX_DGRAM_SIZE - 64U



class UDP_Transmitter 
{
    sockpp::udp_socket sock;
    sockpp::inet_address addr; 
     
public:
    UDP_Transmitter(std::string address, size_t port)
        : sock{}, addr(address, port)
    {}

    void send(cv::Mat img)
    {
        // Compress
        std::vector<uchar> compressed;
        cv::imencode(".jpeg", img, compressed);
        // to byte
        size_t size = compressed.size();
        uchar *ptr  = compressed.data();
        uchar *end  = compressed.data() + size;

        for (uint8_t num_seg = static_cast<size_t>(std::ceil(static_cast<float>(size)/static_cast<float>(MAX_IMG_DGRAM_SIZE))); num_seg != 0; --num_seg)
        {
            size_t chunk = std::min((size_t)(end - ptr), MAX_IMG_DGRAM_SIZE);
            std::stringstream msg;
            msg << num_seg << std::string((char*)ptr, chunk);
            sock.send_to(msg.str(), addr);
            ptr += chunk;
        }
        std::cout << "Send" <<std::endl;
    }
};

int main()
{
    raspicam::RaspiCam_Cv Camera;
    Camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);
    Camera.setCaptureSize(720, 480);
    Camera.setFrameRate(60);
    auto udp = UDP_Transmitter("192.168.137.121", 1337);
    sleep(2);
    
    cv::Mat img{};

	//Open camera
	std::cout<< "Opening Camera..."<< std::endl;
    if (!Camera.open()){
        std::cerr << "Camera didn't start" << std::endl;
        return -1;
    }
    
    while (true)
    {
        Camera.grab();
        Camera.retrieve(img);

        if (img.empty()){
            std::cerr << "No image"<< std::endl;
        }

        udp.send(img);
    }
}