// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
// This example grabs the image from the camera and shows inside an openCV
// window.
//
// Please note that, imshow almost maxes out around ~50FPS and cant display
// all the frames that camera can deliver.
// ----------------------------------------------------------------------------
#include <vector>
#include <chrono>
#include <cstdint>
#include <iostream>

// ----------------------------------------------------------------------------
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// ----------------------------------------------------------------------------
#include "ps3eye.h"

// ----------------------------------------------------------------------------
using namespace cv;
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

  // Create a memory linked openCV Mat object.
  Mat cvFrame(
    Size(kCaptureWidth, kCaptureHeight), CV_8UC3, (void*)videoFrame
  );

  // Create an openCV window
  namedWindow("PS3 Eye Camera Test");

  // Start the chronometer and frame counter
  auto start = chrono::system_clock::now();
  int32_t frame_counter = 0;

  cout << "Test starts" << endl;

  bool isRunning = true;
  while(isRunning)
  {
    // Get a frame from the camera. Notes:
    // - If there is no frame available, this function will block until one is.
    // - The output buffer must be sized correctly based on EOutputFormat.
    eye->getFrame(videoFrame);
    frame_counter += 1;

    // Show the captured frame ...
    imshow("PS3 Eye Camera Test", cvFrame);
    switch(waitKey(1))
    {
      case 'q':
      {
        isRunning = false;
        cout << "Test finished." << endl;
        break;
      }
    }

    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    float fps_calculated = (float)frame_counter / (elapsed.count() / 1000.0);
    cout << "Desired fps: " << kCaptureFPS
      << "  Calculated fps: " << fps_calculated << endl;
  }

  // Memory cleanup.
  if(eye)
  {
    eye->stop();
  }
  delete[] videoFrame;
  return 0;
}
