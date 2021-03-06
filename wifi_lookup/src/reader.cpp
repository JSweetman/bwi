#include "ros/ros.h"
#include "std_msgs/String.h"
#include "wifi_lookup/WifiData.h"
#include "wifi_lookup/Wifi.h"

void readerCallback(const wifi_lookup::WifiData::ConstPtr& msg) {
	
	unsigned int max = 0;

	for(unsigned int i = 0; i < msg->length; i++) 
	{
		
		if (msg->HotSpots[i].dB > msg->HotSpots[max].dB)
		{
			max = i;
		}		
	}
	

	if (msg-> length != 0)
	{
	ROS_INFO("Address of Strongest Hotspot: [%s]", msg->HotSpots[max].MAC.c_str());
	ROS_INFO("Strength (dB): [%d]", msg->HotSpots[max].dB);
	}
	else
	{
	ROS_INFO("WiFi not detected");
	}


}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "reader");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe<wifi_lookup::WifiData> ("wifi_data", 10, readerCallback);
	ros::spin();

	return 0;
}


