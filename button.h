#ifndef BUTTON_H
#define BUTTON_H

#include <cstdint>
#include <functional>
#include <thread>
#include <mutex>

enum struct button_states : int
{
    RELEASED = 0,
    PRESS = 1,
    PRESSED = 2,
    RELEASE = 3
};

class button
{
    button_states state;
    std::function<button_states(button_states)> transition_function;
    std::function<void(button_states)> action_function;
    std::thread t;
    std::mutex m;
    bool is_started{ false };
    void update();
public:
    button(button_states const start_state,
           std::function<button_states(button_states)> transition_function, 
           std::function<void(button_states)> action_function);
    void start();
    void press();
    void release();
    void stop();
    ~button() = default;
};

button::button(button_states const start_state,
               std::function<button_states(button_states)> transition_function, 
               std::function<void(button_states)> action_function)
: state{ start_state }, transition_function{ transition_function }, 
    action_function{ action_function }, t{ std::thread(&button::update) }
{
}

void
button::start()
{
    std::lock_guard<std::mutex> lock{ m };
    is_started = true;
}

void
button::press()
{
    std::lock_guard<std::mutex> lock{ m };
    if(state == button_states::RELEASED)
    {
        state = button_states::PRESS;
    }
}

void
button::release()
{
    std::lock_guard<std::mutex> lock{ m };
    if(state == button_states::PRESSED)
    {
        state = button_states::RELEASE;
    }
}

void
button::update()
{
    while(true)
    {
        if(is_started)
        {
            state = transition_function(state);
            action_function(state);
        }
    }
}

void
button::stop()
{
    std::lock_guard<std::mutex> lock{ m };
    is_started = false;
}



#endif // BUTTON_H