// Copyright [2015] Takashi Ogura<t.ogura@gmail.com>

#include "driver.h"
#include <string>



namespace
{
const int32_t PUBLISHER_BUFFER_SIZE = 1;
}


boost::shared_ptr<cv_camera::MVCameraCapture> sCamera;


namespace cv_camera
{

Driver::Driver(ros::NodeHandle &private_node, ros::NodeHandle &camera_node)
    : private_node_(private_node),
      camera_node_(camera_node)
{
}

void Driver::setup()
{
  int32_t device_id(0);
  std::string device_path("");
  std::string frame_id("camera");
  std::string file_path("");

  private_node_.getParam("device_id", device_id);
  private_node_.getParam("frame_id", frame_id);

  int32_t image_width(752);
  int32_t image_height(480);

  camera_.reset(new MVCameraCapture(camera_node_,
                            "image_raw",
                            PUBLISHER_BUFFER_SIZE,
                            frame_id));

  sCamera = camera_;
  std::cout << "openin the camera device id \n";
  camera_->open(device_id);
  if (private_node_.getParam("image_width", image_width))
  {
    if (!camera_->setWidth(image_width))
    {
      ROS_WARN("fail to set image_width");
    }
  }
  if (private_node_.getParam("image_height", image_height))
  {
    if (!camera_->setHeight(image_height))
    {
      ROS_WARN("fail to set image_height");
    }
  }
  camera_->setAutoExposureMode(false);
  camera_->setExposure(10000.0);
  camera_->setAutoGainMode(false);
  camera_->setGain(25);
}



void Driver::proceed()
{
  if (camera_->capture())
  {
    camera_->publish();
  }
}

Driver::~Driver()
{
}

} // namespace cv_camera
