from sc_kpm import ScModule
from .ExampleAgent import ExampleAgent


class ExampleModule(ScModule):
    def __init__(self):
        super().__init__(ExampleAgent())
