from pathlib import Path

from modules.exampleModule.ExampleModule import ExampleModule
from sc_kpm.utils.action_utils import execute_agent
from tests.base_testcase import BaseTestCase

from sc_kpm.identifiers import CommonIdentifiers, QuestionStatus

WAIT_TIME = 5


class WeatherAgentTestCase(BaseTestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.tests_structures_dir_path = str(
            Path(Path(__file__).resolve().parent, "test_structures"))
        
    def setUp(self):
        super().setUp()
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "common.scs",
            )
        )

    def run_example_agent(self):
        kwargs = dict(
            arguments={},
            concepts=[CommonIdentifiers.QUESTION, "action_example_py"],
            reaction=QuestionStatus.QUESTION_FINISHED_SUCCESSFULLY,
            wait_time=WAIT_TIME,
        )
        self.assertTrue(execute_agent(**kwargs)[1])

    def test_message_is_not_about_weather(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_example_agent.scs",
            )
        )
        module = ExampleModule()
        self.server.add_modules(module)
        with self.server.register_modules():
            self.run_example_agent()

        self.server.remove_modules(module)
