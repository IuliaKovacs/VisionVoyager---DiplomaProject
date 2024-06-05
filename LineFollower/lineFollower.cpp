#include "lineFollower.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include "../TextToSpeech/textToSpeech.h"
#include "../ObstacleAvoidance/obstacleAvoidanceUtils.h"

using namespace std;

VisionVoyager* LineFollower::robotVisionVoyager = nullptr;
vector<int> LineFollower::grayscale_data;
int LineFollower::stop_counter = 0;
State LineFollower::last_state = State::UNKNOWN;

void LineFollower::set_robot(VisionVoyager* robot)
{
    LineFollower::robotVisionVoyager = robot;
}

void LineFollower::get_grayscale_data()
{
    LineFollower::grayscale_data = robotVisionVoyager->read_grayscale_data();
}


vector<int> LineFollower::get_line_from_grayscale_data()
{
    vector<int> line_status = {
        static_cast<int>(grayscale_data[0] < LINE_THRESHOLD),
        static_cast<int>(grayscale_data[1] < LINE_THRESHOLD),
        static_cast<int>(grayscale_data[2] < LINE_THRESHOLD)
    };

    return line_status;
}


void LineFollower::follow_line()
{
    State current_state = State::UNKNOWN;
    /* center direction servo before starting */
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
    robotVisionVoyager->set_speed(1);

    while(true)
    {
        if(should_stop.load())
        {   
            robotVisionVoyager->stop();
            RouteRegistration::set_moving_state(MovingState::STATIONARY);
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_LINE_FOLLOWER_PREFIX << " ...Waiting... - Intervention from user - must wait the start signal" << endl;
            log_mutex.unlock();
            std::unique_lock<std::mutex> lock(mtx);
            cond_v.wait(lock, []{ return !should_stop.load(); });
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_LINE_FOLLOWER_PREFIX << " Waiting Ended " << endl;
            log_mutex.unlock();
        }


        if(ObstacleAvoidance::check_forward_safety())
        {
            (void)ObstacleAvoidance::obstacle_avoid();
        }

        if (verify_is_in_air())
        {   
            robotVisionVoyager->stop();
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_LINE_FOLLOWER_PREFIX << " --- Route Interrupted: The robot is either in air or on the edge of something! ---" << endl;
            logFile << log_time() << LOG_THREAD_LINE_FOLLOWER_PREFIX << " --- Displaying acoustical warning ---" << endl;
            log_mutex.unlock();
            severe_error.store(true);
            error_type = SevereErrorType::IN_AIR;  
            tts_mutex.lock();
            if(Language::EN == TextToSpeech::get_language())
            {   
                TextToSpeech::display_custom_message("The robot is in the air or on the edge of something \n\n\n Aborting the guiding process! \n\n\n Please contact the building staff!");
            }
            else
            {
                TextToSpeech::display_custom_message("Robotul este în aer sau pe marginea unei prăpăstii! \n\n\n Abandonare proces de ghidare \n\n\n Contactați personalul clădirii!");
            }
            tts_mutex.unlock();
        }

        /* check for severe error. e.g. robot is in the air, motor issue etc */
        if(severe_error.load())
        {
            robotVisionVoyager->stop(); 
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_LINE_FOLLOWER_PREFIX << " ...Aborting due to severe error..." << endl;
            log_mutex.unlock();
            lock_guard<mutex> lock2(mtx);
            should_stop.store(true);
            route_complete.store(true);
            cond_v.notify_all();
            speaking_condition.notify_all();
            return;
        }

        /* check if waiting command was given */
        if(should_stop.load())
        {   
            robotVisionVoyager->stop();
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " ...Waiting... - Intervention from user - must wait the start signal" << endl;
            log_mutex.unlock();
            std::unique_lock<std::mutex> lock(mtx);
            cond_v.wait(lock, []{ return !should_stop.load(); });
            robotVisionVoyager->move_forward();
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " Waiting Ended " << endl;
            log_mutex.unlock();
        }     

        // logFile << log_time() << stop_counter << endl;
        if (verify_no_line_available())
        {   
            RouteRegistration::set_moving_state(MovingState::STATIONARY);
            RouteRegistration::end_registration();
            //@ToDo RFID stop tag - daca am gasit tagul destinatie -> ruta completa, altfel mesaj corespunzator - nu mai avem linie de urmarit si nici nu suntem la destinatie
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Route completed --- " << endl;
            log_mutex.unlock();
            lock_guard<mutex> lock2(mtx);
            should_stop.store(true);
            route_complete.store(true);
            cond_v.notify_all();
            tts_mutex.lock();
            TextToSpeech::display_destination();
            tts_mutex.unlock();
            speaking_condition.notify_all();
            return;
        }

        get_grayscale_data();
        // logFile << log_time() << "Grayscale Data: " << grayscale_data[0] << "  " << grayscale_data[1] << "  " << grayscale_data[2] << endl;
        vector<int> grayscale_line_status = get_line_from_grayscale_data();
        // logFile << log_time() << "Grayscale Line Status: " << grayscale_line_status[0] << "  " << grayscale_line_status[1] << "  " << grayscale_line_status[2] << endl;
        last_state = current_state;
        current_state = compute_next_state(grayscale_line_status);
        execute_move(current_state);
        
    }
}


bool LineFollower::verify_is_in_air()
{
    get_grayscale_data();
    return ((grayscale_data[0] <= IN_AIR_THRESHOLD) && (grayscale_data[1] <= IN_AIR_THRESHOLD) && (grayscale_data[2] <= IN_AIR_THRESHOLD));
}


bool LineFollower::verify_no_line_available()
{
    if (stop_counter >= STOP_VALUE)
    {
        robotVisionVoyager->stop();
        RouteRegistration::register_current_move("stop");
        return true;
    }    
    else
    {
        return false;
    }
}


void LineFollower::search_line()
{
    if (verify_is_in_air())
    {
        robotVisionVoyager->stop();
        RouteRegistration::register_current_move("stop");
        RouteRegistration::set_moving_state(MovingState::STATIONARY);
    }

    if (last_state == State::LEFT)
    {   
        robotVisionVoyager->turn_right_max();
        /* search right */
        robotVisionVoyager->move_forward();
        RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
    }
    else if (last_state == State::RIGHT)
    {
        robotVisionVoyager->turn_left_max();
        /* search left */
        robotVisionVoyager->move_forward();
        RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
    }
        
}


State LineFollower::compute_next_state(vector<int> grayscale_line_status)
{
    if ((grayscale_line_status[0] == 0) && (grayscale_line_status[1] == 0) && (grayscale_line_status[2] == 0))
    {
        /* grayscale sensor does not see the line at all */
        stop_counter += 1;
        return State::UNKNOWN;
    }
    else if ((grayscale_line_status[0] == 0) && (grayscale_line_status[1] == 1) && (grayscale_line_status[2] == 1))
    {
        /* grayscale sensor sees the line with center and right sensor */
        last_state = State::LEFT;
        stop_counter = 0;
        return State::LEFT;
    }
    else if ((grayscale_line_status[0] == 1) && (grayscale_line_status[1] == 1) && (grayscale_line_status[2] == 0))
    {
        /* grayscale sensor sees the line with center and left sensor */
        last_state = State::RIGHT;
        stop_counter = 0;
        return State::RIGHT;
    }
    else if (grayscale_line_status[1] == 1)
    {
        /* grayscale sensor sees the line with center sensor */
        stop_counter = 0;  ;
        return State::FORWARD;
    }
    else if (grayscale_line_status[2] == 1)
    {
        /* grayscale sensor sees the line with right sensor */
        last_state = State::LEFT;
        stop_counter = 0;
        return State::LEFT;
    }
    else if (grayscale_line_status[0] == 1)
    {
        /* grayscale sensor sees the line with left sensor */
        last_state = State::RIGHT;
        stop_counter = 0;
        return State::RIGHT;
    }
}


void LineFollower::execute_move(State current_state)
{
    if (current_state == State::LEFT)
    {
        robotVisionVoyager->turn_right_max();
        robotVisionVoyager->move_forward();
        RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
    }
    else if (current_state == State::RIGHT)
    {
        robotVisionVoyager->turn_left_max();
        robotVisionVoyager->move_forward();
        RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
    }
    else if (current_state == State::FORWARD)
    {
        robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
        RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
        robotVisionVoyager->move_forward();
        RouteRegistration::register_current_move("forward", robotVisionVoyager->get_speed());
        RouteRegistration::set_moving_state(MovingState::MOVING_FORWARD);
    }
    else
    {
        search_line();
    }
}
