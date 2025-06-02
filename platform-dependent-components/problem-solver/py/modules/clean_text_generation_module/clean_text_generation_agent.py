import logging
import os

import re
import requests
from requests import HTTPError
from sc_client.client import template_search, template_generate
from sc_client.constants import sc_types
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_kpm import ScAgentClassic
from sc_kpm import ScKeynodes
from sc_kpm.sc_result import ScResult
from sc_kpm.utils import create_link, get_link_content_data, get_system_idtf
from sc_kpm.utils.action_utils import (generate_action_result,
                                       finish_action_with_status,
                                       get_action_arguments)
from .standarized_response import get_standarized_response

from . import constants

logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(levelname)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class CleanTextGenerationAgent(ScAgentClassic):
    def __init__(self) -> None:
        super().__init__(constants.ACTION_GET_CLEAN_TEXT)

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successfull = result == ScResult.OK
        finish_action_with_status(action_element, is_successfull)
        self.logger.info(
            "CleanTextGenerationAgent finished %s", "successfully" if is_successfull else "unsuccessfully"
        )
        return result

    def run(self, action_element: ScAddr) -> ScResult:
        self.logger.info('Non-official API raw text processor began to run...')

        raw_text_structure, language_node, message_node = get_action_arguments(action_element, 3)
        if not raw_text_structure.is_valid() or not language_node.is_valid() or not message_node.is_valid():
            self.logger.error('Error: invalid arguments are passed.')
            return ScResult.ERROR_INVALID_PARAMS

        raw_text_node_template = ScTemplate()
        raw_text_node_template.triple(
            raw_text_structure,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.LINK_VAR >> "_link",
        )
        search_result = template_search(raw_text_node_template)
        if len(search_result) != 1:
            self.logger.error('Error: could not find raw text sc-link to process.')
            return ScResult.ERROR_INVALID_PARAMS

        raw_text_node = search_result[0].get("_link")

        language = get_system_idtf(language_node)
        if not language in constants.AVAILABLE_LANGUAGES:
            self.logger.error(f'Error: you have not passed available language as argument. You passed: {language}')
            return ScResult.ERROR_INVALID_PARAMS
        
        # Get raw text string
        raw_text = get_link_content_data(raw_text_node).replace('\n', ' ')
        if not isinstance(raw_text, str):
            self.logger.error(f'Error: your raw text link must be string type, but text of yours is {type(raw_text)}')
            return ScResult.ERROR_INVALID_TYPE
        self.logger.info('Raw text:' + raw_text)

        # Trying to get clean text 
        if not raw_text.isspace():       
            try:
                clean_text = self._get_clean_text(raw_text, language)            
            except HTTPError as err:
                self.logger.error(f'Error: {err}.\nThis error is on non-official API\'s side.')
                return ScResult.ERROR
        else:
            clean_text = 'К сожалению, в настоящее время нет информации, соответствующей вашему вопросу в базе знаний. Приносим извинения за неудобства.'
        
        # Check text for emptiness. If processed text is empty, that means that model does not work
        if clean_text is None or clean_text == '':
            self.logger.error(f'Error: Looks like that model does not work. Try to change one in configs.')
            return ScResult.ERROR

        self.logger.info('Clean text:' + clean_text)

        # Creating answer and finishing agent work
        answer_link = create_link(clean_text, ScLinkContentType.STRING)
        generate_action_result(action_element, answer_link)

        message_answer_gen_template = ScTemplate()
        message_answer_gen_template.triple_with_relation(
            message_node,
            sc_types.EDGE_D_COMMON_VAR,
            answer_link,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            ScKeynodes.resolve("nrel_reply", None),
        )

        template_generate(message_answer_gen_template)

        self.logger.info('Successfully processed the text using non-official API! Very well!')
        finish_action_with_status(action_element, True)
        return ScResult.OK
    
    def _get_clean_text(self, raw_text: str, language: str) -> str:
        raw_text = re.sub("<.*>|\*", '', raw_text)
        raw_text = raw_text.replace("\n", " ").strip()
        llm_api_key = os.environ.get("LLM_API_KEY")
        llm_api_url = os.environ.get("LLM_API_URL")
        llm_api_model = os.environ.get("LLM_API_MODEL")
        if llm_api_key is None:
            self.logger.error('Error: environment variable LLM_API_KEY is not set.')
            return raw_text
        if llm_api_url is None:
            self.logger.error('Error: environment variable LLM_API_URL is not set.')
            return raw_text
        if llm_api_model is None:
            self.logger.error('Error: environment variable LLM_API_MODEL is not set.')
            return raw_text
        headers = {"Authorization": f"Bearer {llm_api_key}", "Content-Type": "application/json"}
        llm_request_body = self._create_llm_request_body(raw_text, llm_api_model, language)
        self.logger.debug("request body: " + llm_request_body)
        response = requests.post(llm_api_url, llm_request_body.encode(encoding='UTF-8'), headers=headers)
        self.logger.debug(f"response status_code from api: {response.status_code}")
        self.logger.debug(f"response reason from api: {response.reason}")
        self.logger.debug("text from api: " + response.text)
        if (response.ok):
            response.encoding = "utf-8"
            response_body = response.json()
            if "error" in response_body:
                self.logger.error(f"Error: {response_body['error']}")
            standarized_response = get_standarized_response(llm_api_url, response_body)
            if standarized_response.is_error:
                self.logger.error(f"Error: {standarized_response.error_message}")
                return standarized_response.text
            return standarized_response.text
        else:
            return f"Api for clean text returned status code {response.status_code} with reason {response.reason}. However original text is: {raw_text}"

    def _create_llm_request_body(self, prompt: str, llm_api_model: str, language: str) -> str:
        return constants.REQUEST_BODY % (llm_api_model, constants.LANGUAGE_RULES[language], prompt)
