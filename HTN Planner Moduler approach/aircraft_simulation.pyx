#include <pybind11/pybind11.h>
#include <string>
#include <SDL.h>  // For SDL rendering, if you need to render
#include "Aircraft.h"  // Include your Aircraft class header

namespace py = pybind11;

// Wrap the Aircraft class to be accessible from Python
PYBIND11_MODULE(aircraft_simulation, m) {
    py::class_<Aircraft>(m, "Aircraft")
        .def(py::init<const std::string&, const std::string&, int, float, float>(), 
             py::arg("name"), py::arg("force"), py::arg("health"), py::arg("startX"), py::arg("startY"))
        .def("get_name", &Aircraft::get_name)
        .def("get_force", &Aircraft::get_force)
        .def("get_health", &Aircraft::get_health)
        .def("get_x", &Aircraft::get_x)
        .def("get_y", &Aircraft::get_y)
        .def("move_to", &Aircraft::move_to)
        .def("attack", &Aircraft::attack)
        .def("defend", &Aircraft::defend)
        .def("is_alive", &Aircraft::is_alive)
        .def("draw", &Aircraft::draw)
        .def("wrap_around_screen", &Aircraft::wrap_around_screen);
}
