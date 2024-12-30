#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Simulation.h"

namespace py = pybind11;

PYBIND11_MODULE(aircraft_simulation, m) {
    py::class_<Aircraft>(m, "Aircraft")
        .def("get_name", &Aircraft::get_name)
        .def("get_force", &Aircraft::get_force)
        .def("get_health", &Aircraft::get_health)
        .def("get_x", &Aircraft::get_x)
        .def("get_y", &Aircraft::get_y)
        .def("move_to", &Aircraft::move_to)
        .def("attack", &Aircraft::attack)
        .def("defend", &Aircraft::defend)
        .def("is_alive", &Aircraft::is_alive);

    py::class_<Simulation>(m, "Simulation")
        .def("add_aircraft", &Simulation::add_aircraft)
        .def("get_aircrafts", &Simulation::get_aircrafts, py::return_value_policy::reference_internal)
        .def("update_aircrafts", &Simulation::update_aircrafts)
        .def("is_quit", &Simulation::is_quit);

    m.def("get_simulation_instance", []() -> Simulation& {
        return Simulation::get_instance();  // Return a reference to avoid re-initializing
        }, py::return_value_policy::reference);// Ensure Python receives the same reference
}

// .def_static("get_instance", &Simulation::get_instance, py::return_value_policy::reference)