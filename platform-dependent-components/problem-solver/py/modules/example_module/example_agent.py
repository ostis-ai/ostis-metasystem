"""
This source file is part of an OSTIS project. For the latest info, see http://ostis.net
Distributed under the MIT License
(See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
"""

import logging
from sc_client.models import ScAddr, ScLinkContentType
from sc_client.constants import sc_type

from sc_kpm import ScAgentClassic, ScResult
from sc_kpm.utils import (
    generate_link,
)
from sc_kpm.utils.action_utils import (
    generate_action_result,
    finish_action_with_status
)


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class ExampleAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_example_py")

    def on_event(self, action_class: ScAddr, arc: ScAddr, action: ScAddr) -> ScResult:
        result = self.run(action)
        is_successful = result == ScResult.OK
        finish_action_with_status(action, is_successful)
        self.logger.info("ExampleAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("ExampleAgent started")
        link = generate_link(
            "Example agent is called", ScLinkContentType.STRING, link_type=sc_type.CONST_NODE_LINK)
        generate_action_result(action_node, link)
        return ScResult.OK
