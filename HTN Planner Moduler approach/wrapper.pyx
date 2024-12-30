# cython: language_level=3
from libcpp.string cimport string
from libcpp.vector cimport vector

# Ensure that we import the C++ Aircraft class properly
cdef extern from "Aircraft.h":
    cdef cppclass Aircraft:
        Aircraft(const string& name, const string& force, int health)
        string get_name() const
        string get_force() const
        int get_health() const
        void move_to(float x, float y, float z)
        void attack(Aircraft& target)
        void defend()
        bint is_alive() const  # Use bint for boolean in Cython

cdef extern from "HTNCore.h":
    cdef cppclass HTNPlanner:
        vector[string] plan(Aircraft& aircraft)  # Plan method expects a reference

    cdef cppclass AttackPlanner(HTNPlanner):
        AttackPlanner()

    cdef cppclass DefendPlanner(HTNPlanner):
        DefendPlanner()

# Python wrapper for Aircraft
cdef class PyAircraft:
    cdef Aircraft* c_aircraft

    def __cinit__(self, name: str, force: str, health: int):
        self.c_aircraft = new Aircraft(name.encode("utf-8"), force.encode("utf-8"), health)

    def __dealloc__(self):
        del self.c_aircraft

    def get_name(self) -> str:
        return self.c_aircraft.get_name().decode("utf-8")

    def get_force(self) -> str:
        return self.c_aircraft.get_force().decode("utf-8")

    def get_health(self) -> int:
        return self.c_aircraft.get_health()

    def move_to(self, x: float, y: float, z: float):
        self.c_aircraft.move_to(x, y, z)

    def attack(self, target: PyAircraft):
        self.c_aircraft.attack(<Aircraft&>target.c_aircraft)

    def defend(self):
        self.c_aircraft.defend()

    def is_alive(self) -> bool:
        return self.c_aircraft.is_alive()

# Python wrapper for HTNPlanner
cdef class PyHTNPlanner:
    cdef HTNPlanner* c_planner

    def plan(self, aircraft: PyAircraft):
        # Pass the aircraft pointer directly as a reference to the plan method
        cdef vector[string] tasks = self.c_planner.plan(<Aircraft&>aircraft.c_aircraft)  # Use a reference explicitly
        return [task.decode("utf-8") for task in tasks]

# Python wrappers for specific planners
cdef class PyAttackPlanner(PyHTNPlanner):
    def __cinit__(self):
        self.c_planner = new AttackPlanner()

cdef class PyDefendPlanner(PyHTNPlanner):
    def __cinit__(self):
        self.c_planner = new DefendPlanner()
