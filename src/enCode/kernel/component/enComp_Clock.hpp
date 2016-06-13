#ifndef EN_COMPONENT_CLOCK_H
#define EN_COMPONENT_CLOCK_H

#include <chrono>

#include "enComponentMeta.hpp"
#include "enComponentType.hpp"

class enComponent_Clock0 : public enComponentMeta {
public:  
    enComponent_Clock0() : enComponentMeta( enComponentType::CLOCK ), _is_running(false) {}
    virtual void                                     run();
    virtual void                                     pause();
    virtual bool                                     is_running() const;
    virtual void                                     set_frequency(double frequency_Hz);
    void                                             set_func_cb( std::function<void(void)> );
    bool                                             tick();
private:
    std::function<void(void)>                        _func_cb;
    bool                                             _is_running;
    double                                           _period_ms;
    std::chrono::high_resolution_clock::time_point   _time_prev;
    std::chrono::high_resolution_clock::time_point   _time;
    double                                           _time_since_start_ms;
};

#endif
