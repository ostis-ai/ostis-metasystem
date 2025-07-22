from sc_kpm import ScModule

from .clean_text_generation_agent import CleanTextGenerationAgent


class CleanTextGenerationModule(ScModule):
    def __init__(self) -> None:
        super().__init__(CleanTextGenerationAgent())
