#include "button.h"
#include <iostream>
#include <cstdlib>

button_states transition(button_states const state)
{
    std::cerr << "transition : ";    
    return button_states::RELEASED;
}

void action(button_states)
{
    std::cerr << "action\n";
    return;
}

int main()
{
    button *  b = new button(button_states::RELEASED, 
                             transition,
                             action);
    b->start();
    std::this_thread::sleep_for(
        std::chrono::microseconds{1'000'000});
    b->press();
    b->release();
}