// Copyright [2015] Takashi Ogura<t.ogura@gmail.com>

#include "driver.h"
#include <sensor_msgs/TimeReference.h>
#include "MVCameraCapture.h"

uint32_t triggerCounter;
ros::Time Imutimestamp;
void callback(const sensor_msgs::TimeReference::ConstPtr &time_ref) {
     Imutimestamp = time_ref->header.stamp + ros::Duration(0.105);
     triggerCounter = time_ref->header.seq;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "cv_camera");

  ros::NodeHandle private_node("~");

  cv_camera::Driver driver(private_node, private_node);

  ros::NodeHandle n;

  ros::Subscriber subTimeRef = n.subscribe("/imu/trigger_time", 1000, callback);

  try
  {
    driver.setup();
    while (ros::ok())
    {
	    driver.proceed();
      ros::spinOnce();
    }
  }
  catch (cv_camera::DeviceError &e)
  {
    ROS_ERROR_STREAM("cv camera open failed: " << e.what());
    return 1;
  }


  return 0;
}
