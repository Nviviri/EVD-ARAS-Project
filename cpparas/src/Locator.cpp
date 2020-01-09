#include "Locator.hpp"
#include "MarkerDetector.hpp"
#include "RegionExtractor.hpp"
#include <chrono>
#include <stdexcept>
#include <thread>

namespace cpparas {

Locator::Locator(std::shared_ptr<ImageLoader> imageLoader_)
    : locator_running(false)
    , locator_thread_starting(true)
    , first_frame(false)
    , moved_interupt(false)
    , imageLoader(imageLoader_)
    , PiCamera(1440, 1440)
    , RegExtractor(800,800)
{
}

//Kill thread, which in turn will kill camera thread as well;
Locator::~Locator()
{
    locator_running = false;
    if (locator_thread.joinable()) {
        locator_thread.join();
    }
}

//Start locator thread
void Locator::Start_Locator_thread()
{
    if (!locator_running) {
        locator_running = true;
        source_type = imageLoader->Get_source_type();
        locator_thread = std::thread(&Locator::Locator_thread, this);
        //lock calling thread until locator is running
        while (locator_thread_starting) {
            ;
        }
    }
}

// Stop Locator thread. Also stops camera thread if active
void Locator::Stop_Locator_thread()
{
    locator_running = false;
    if (locator_thread.joinable()) {
        locator_thread.join();
    }
}

void Locator::Locator_thread()
{
    if (source_type == CAMERA) {
        //start Camera thread
        PiCamera.Camera_thread_worker_start();
    }
    image_t* old_cut_frame = newRGB888Image(800,800);

    while (locator_running) {
        if (source_type == CAMERA) {
            //Get a new frame from camera
            new_full_frame = PiCamera.Camera_get_frame();
        } else if (source_type == IMAGE) {
            //Load user image
            new_full_frame = imageLoader->Get_source_image();
        } else {
            //smth is wrong I can feel it
            locator_running = false;
            break;
        }
        //if new frame is none, camera is dead, so we need to stop locator
        if (new_full_frame == nullptr) {
            locator_running = false;
            break;
        }

        //Get cameras center point
        Central_camera_point.col = new_full_frame->cols / 2;
        Central_camera_point.row = new_full_frame->rows / 2;

        //find corners
        corner_points = RegExtractor.updateImage(new_full_frame);
        if(corner_points.size() == 3){
            
            //caclulate center point based on 3 points
            Central_board_point.col = (corner_points[0].col + corner_points[2].col) / 2;
            Central_board_point.row = (corner_points[0].row + corner_points[2].row) / 2;
            
            //compare those points with the camera central point
            if((Central_board_point.col - Central_camera_point.col) > MAX_DIVIATION || (Central_board_point.col - Central_camera_point.col) < -MAX_DIVIATION){
                moved_interupt = true;
            }
                    
            if((Central_board_point.row - Central_camera_point.row) > MAX_DIVIATION || (Central_board_point.row - Central_camera_point.row) < -MAX_DIVIATION){
                moved_interupt = true;
            }

            // Get the new cut frame
            new_cut_frame = RegExtractor.getRegionImage();

            // Set first frame accuaried bool
            if(!first_frame){
                first_frame = true;
            }

            // Save a frame copy
            old_cut_frame = new_cut_frame;
        } 
        else
        {
            if(old_cut_frame->data != nullptr){
                //No points found but buffer has older, saved image, so use that instead
                new_cut_frame = old_cut_frame;
            }
            // If no picture was found at all, do nothing and try again
        }

        //only unlock the main thread after at least a full frame has been received.
        if(locator_thread_starting){
            locator_thread_starting = false;
        }
        //wait a bit, no need to run this at full powaa
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    deleteImage(old_cut_frame);
    PiCamera.Camera_thread_worker_stop();
}

//Get latest cut frame from locator
image_t* Locator::Get_new_frame()
{
    if (!locator_running) {
        throw std::runtime_error("Camera and locator thread stopped unexpectedly");
    }
    if (new_cut_frame->type != IMGTYPE_RGB888) {
        throw std::runtime_error("Locator tried to return wrong type (and probably empty) image. That's not good");
    }
    return new_cut_frame;
}

//Send a frame to image loader
void Locator::Send_frame_to_ui(image_t* frame)
{
    if (!locator_running) {
        throw std::runtime_error("Camera and locator thread stopped unexpectedly");
    }
    imageLoader->Set_UI_frame(frame);
}

// Check if at least one frame is available
bool Locator::First_frame_received()
{
    return first_frame;
}

// Check if at least one frame is available
bool Locator::Location_checker()
{
    return moved_interupt;
}

} // namespace cpparas
