#ifndef STATEMACHINEWIDGET_HPP
#define STATEMACHINEWIDGET_HPP

#include "StateMachine.hpp"
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>

class StateMachineWidget {
public:
    StateMachineWidget(StateMachine* _stateMachine);
    ~StateMachineWidget();
    Gtk::Grid& operator()();

private:
    bool update();
    void on_start_stop_button_clicked();
    void on_simulate_hand_button_clicked();

    StateMachine* stateMachine;
    Gtk::Grid container;
    Gtk::Button startStopButton;
    Gtk::Button simulateHandButton;
    Gtk::Label layerLabel;
    Gtk::Label stepLabel;
    bool paused;
};

#endif /* STATEMACHINEWIDGET_HPP */
