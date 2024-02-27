"""
This source file is part of an OSTIS project. For the latest info, see http://ostis.net
Distributed under the MIT License
(See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
"""

import logging
from sc_client.models import ScAddr, ScLinkContentType
from sc_client.constants import sc_types

from sc_kpm import ScAgentClassic, ScResult
from sc_kpm.utils import (
    create_link,
)
from sc_kpm.utils.action_utils import (
    create_action_answer,
    finish_action_with_status
)


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class ExampleAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_example_py")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("ExampleAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("ExampleAgent started")
        link = create_link(
            "Example agent is called", ScLinkContentType.STRING, link_type=sc_types.LINK_CONST)
        create_action_answer(action_node, link)
        return ScResult.OK
