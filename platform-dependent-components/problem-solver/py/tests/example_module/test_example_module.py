"""
This source file is part of an OSTIS project. For the latest info, see http://ostis.net
Distributed under the MIT License
(See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
"""

from pathlib import Path

from modules.example_module.example_module import ExampleModule
from sc_kpm.utils.action_utils import execute_agent
from tests.base_testcase import BaseTestCase

from sc_kpm.identifiers import CommonIdentifiers, QuestionStatus

WAIT_TIME = 5


class ExampleAgentTestCase(BaseTestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.tests_structures_dir_path = str(
            Path(Path(__file__).resolve().parent, "test-structures"))
        
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

    def test_example_agent(self):
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
