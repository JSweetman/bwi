#include "ros/ros.h"
#include "std_msgs/String.h"
#include "wifi_lookup/WifiData.h"
#include "wifi_lookup/Wifi.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/cv_bridge.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

double db = 0;
uchar x = 0;
uchar y = 0;
cv::Mat grayImage;

void poseListen(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg) 
{
	x = (msg -> pose.pose.position.x);
	y = (msg -> pose.pose.position.y);
	//x = (x + 13.15)/20;
	//y = (y + 2.0)/20;

	//geometry_msgs/PoseWithCovarianceStamped Message
	ROS_INFO("AMCL POSE(%d,%d)", x,y);
 	
	if (grayImage.channels() == 1)
	{
		
		//y = 240 - y;
		x = x + 80;
		y = y + 100;		
		//ROS_INFO("AMCL POSE(%d,%d)", x,y);
		//int x2 = x;
		//int y2 = y;


		for (int i=-3; i <= 3; i++)  
		{
			
			for (int j=-3; j <= 3; j++)
			{	
				//grayImage.at<uchar>(3 * x+i,3 * y+j) = 255*(1/(db*3));
				grayImage.at<uchar>(3 * x+i,3 * y+j) = 0;
			}
		}


		ROS_INFO("(%d,%d)", x,y);

		//grayImage.at<uchar>(20,10) = 255; //white
		//grayImage.at<uchar>(10,11) = 255;
		//grayImage.at<uchar>(10,12) = 150;
		//grayImage.at<uchar>(10,13) = 150;
		//grayImage.at<uchar>(10,14) = 0;
		//grayImage.at<uchar>(10,15) = 0; //black
    }

	cv::imshow("Output", grayImage);
}

void readerCallback(const wifi_lookup::WifiData::ConstPtr& msg) 
{
	unsigned int max = 0;

	for(unsigned int i = 0; i < msg->length; i++) 
	{
		
		if (msg->HotSpots[i].dB > msg->HotSpots[max].dB)
		{
			max = i;
			db = msg->HotSpots[max].dB;
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
	ros::init(argc, argv, "wifi_test");
	ros::NodeHandle n;

 	cv::namedWindow("Output");
 	cvResizeWindow("Output", 320, 240);
  	cvStartWindowThread();

 	cv::Mat inputImage(cv::imread("maps/3ne-real-new.pgm"));
  	cv::cvtColor(inputImage, grayImage, CV_RGB2GRAY);

	// Need AMCL Pose (convert from location to pixel)
	

	//grayImage.at<double>(x,y) = 3*db;


/*    if (grayImage.channels() == 1)
	{
		grayImage.at<uchar>(x,y) = 0;
		ROS_INFO("(%d,%d)", x,y);

		//grayImage.at<uchar>(20,10) = 255; //white
		//grayImage.at<uchar>(10,11) = 255;
		//grayImage.at<uchar>(10,12) = 150;
		//grayImage.at<uchar>(10,13) = 150;
		//grayImage.at<uchar>(10,14) = 0;
		//grayImage.at<uchar>(10,15) = 0; //black
    }*/

	cv::imshow("Output", grayImage);

	ros::Subscriber sub = n.subscribe<wifi_lookup::WifiData> ("wifi_data", 10, readerCallback);
    

	// AMCL POSE!!!!
	//ros::Subscriber sub = n.subscribe<wifi_lookup::WifiData> ("wifi_data", 10, readerCallback);

	ros::Subscriber sub2 = n.subscribe("amcl_pose", 2, poseListen);
	ros::spin();


	return 0;
}


