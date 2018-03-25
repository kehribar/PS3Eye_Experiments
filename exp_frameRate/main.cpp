// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
// This example just grabs the frame from the camera and calculates the time
// it took between each frame. Does nothing with the frame itself.
// ----------------------------------------------------------------------------
#include <vector>
#include <chrono>
#include <cstdint>
#include <iostream>

// ----------------------------------------------------------------------------
#include "ps3eye.h"

// ----------------------------------------------------------------------------
using namespace std;
using namespace ps3eye;

// ----------------------------------------------------------------------------
// Valid (width, height) resoultions options are 640x480 and 320x240 only
// ----------------------------------------------------------------------------
const int32_t kCaptureWidth = 640;
const int32_t kCaptureHeight = 480;

// ----------------------------------------------------------------------------
// Valid FPS values for 640x480 resolution:
//   2, 3, 5, 8, 10, 15, 20, 25, 30, 40, 50, 60, 75
// ----------------------------------------------------------------------------
// Valid FPS values for 320x240 resolution:
//   2, 3, 5, 7, 10, 12, 15, 17, 30, 37, 40, 50, 60,
//   75, 90, 100, 125, 137, 150, 187
// ----------------------------------------------------------------------------
const int32_t kCaptureFPS = 60;

// ----------------------------------------------------------------------------
int main()
{
  // Search for devices ...
  vector<PS3EYECam::PS3EYERef> devices(PS3EYECam::getDevices());
  if(devices.empty())
  {
    cout << "No PS3 eye camera found!" << endl;
    return -1;
  }

  // We will be dealing with single camera in this example, so connect to the
  // first one.
  PS3EYECam::PS3EYERef eye = devices.at(0);
  bool rv = eye->init(
    kCaptureWidth,
    kCaptureHeight,
    kCaptureFPS,
    PS3EYECam::EOutputFormat::BGR
  );
  if(rv == false)
  {
    cout << "Invalid resolution or FPS!" << endl;
    return -1;
  }

  // Start with auto adjustment mode
  eye->start();
  eye->setAutogain(true);
  eye->setAutoWhiteBalance(true);

  // Create video frame memory.
  uint8_t* videoFrame = new uint8_t[eye->getWidth() * eye->getHeight() * 3];

  // Start the chronometer and frame counter
  auto start = chrono::system_clock::now();
  int32_t frame_counter = 0;

  cout << "Test starts" << endl;

  while(true)
  {
    // Get a frame from the camera. Notes:
    // - If there is no frame available, this function will block until one is.
    // - The output buffer must be sized correctly based on EOutputFormat.
    eye->getFrame(videoFrame);
    frame_counter += 1;

    // -- DO SOMETHING WITH THE VIDEO_FRAME HERE --

    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    float fps_calculated = (float)frame_counter / (elapsed.count() / 1000.0);
    cout << "Desired fps: " << kCaptureFPS
      << "  Calculated fps: " << fps_calculated << endl;

    // Exit after 15 seconds
    if(elapsed.count() > (15 * 1000))
    {
      cout << "Test finished." << endl;
      break;
    }
  }

  // Memory cleanup.
  if(eye)
  {
    eye->stop();
  }
  delete[] videoFrame;
  return 0;
}
