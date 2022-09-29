#include <Windows.h>
#include <Ole2.h>

#include <iostream>

#include "../Kinect/inc/NuiApi.h"
#include "../Kinect/inc/NuiImageCamera.h"
#include "../Kinect/inc/NuiSensor.h" 

#include <napi.h>

#define SENSOR_HEIGHT = 480;
#define SENSOR_WIDTH = 640;

Napi::Object skeletonPosition = {};

INuiSensor* KinectSensor; // Create a pointer using type*

using namespace Napi;

Napi::Boolean initKinect(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    Napi::Boolean False = Napi::Boolean::New(env, false);
    Napi::Boolean True = Napi::Boolean::New(env, true);

   // int numSensors;

  //  if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return False;
  //  if (NuiCreateSensorByIndex(0, &KinectSensor) < 0) return False ;

    // Initialize sensor
   // KinectSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON);
    //KinectSensor->NuiSkeletonTrackingEnable(NULL, 0);
    return True;
}

Vector4 NotTrackedVector = Vector4();

Napi::Object getSkeletalData(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object NoData = Napi::Object::New(env);

   if (!KinectSensor) {return NoData;}

    Napi::Object SkeletonData = Napi::Object::New(env);

    NUI_SKELETON_FRAME skeletonFrame = {0};
    if (KinectSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame) >= 0) {
        KinectSensor->NuiTransformSmooth(&skeletonFrame, NULL);

        for (int z = 0; z < NUI_SKELETON_COUNT; ++z) {
            const NUI_SKELETON_DATA& skeleton = skeletonFrame.SkeletonData[z];
            if (skeleton.eTrackingState == NUI_SKELETON_TRACKED) {
                for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) { 
                  SkeletonData.Set(i, skeleton.SkeletonPositions[i].x);
                   if (skeleton.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_NOT_TRACKED) {
                       SkeletonData.Delete(i);
                    }
                }
               return SkeletonData;
            }
        }
        
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "InitializeSensor"), Napi::Function::New(env, initKinect));
 // exports.Set(Napi::String::New(env, "GetSkeleton"), getSkeletalData);

  return exports;
}
