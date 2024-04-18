import logging

import g4f
from g4f.client import Client
from requests import HTTPError
from sc_client.client import template_search
from sc_client.constants import sc_types
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_kpm import ScAgentClassic
from sc_kpm.sc_keynodes import Idtf
from sc_kpm.sc_result import ScResult
from sc_kpm.utils import create_link, get_link_content_data, get_system_idtf
from sc_kpm.utils.action_utils import (create_action_answer,
                                       finish_action_with_status,
                                       get_action_arguments)

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

        # Get sc-link with raw text        
        raw_text_node = get_action_arguments(action_element, 1)[0]
        if not raw_text_node:
            self.logger.error('Error: could not find raw text sc-link to process')
            return ScResult.ERROR_INVALID_PARAMS
        
        #Get language of raw text sc-link
        language_template = ScTemplate()
        language_template.triple(
            sc_types.NODE_CLASS,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            raw_text_node
        )
        search_result = template_search(language_template)
        if len(search_result) != 1:
            self.logger.error('Error: You have passed no language or too many arguments.')
            return ScResult.ERROR_INVALID_PARAMS
        language_node = search_result[0][0]
        language = get_system_idtf(language_node)
        if not language in constants.AVAILABLE_LANGUAGES:
            self.logger.error(f'Error: you have not passed available language as argument. You passed: {language}')
            return ScResult.ERROR_INVALID_PARAMS
        
        # Get raw text string
        raw_text = get_link_content_data(raw_text_node)        
        if not isinstance(raw_text, str):
            self.logger.error(f'Error: your raw text link must be string type, but text of yours is {type(raw_text)}')
            return ScResult.ERROR_INVALID_TYPE

        # Trying to get clean text        
        try:
            clean_text = self._get_clean_text(raw_text, language)            
        except HTTPError as err:
            self.logger.error(f'Error: {err}.\nThis error is on non-official API\'s side.')
            return ScResult.ERROR
        
        # Check text for emptiness. If processed text is empty, that means that model does not work
        if clean_text is None or clean_text == '':
            self.logger.error(f'Error: Looks like that model {constants.NON_OFFICIAL_API_DEFAULT_MODEL} does not work. Try to change one in configs.')
            return ScResult.ERROR

        # Creating answer and finishing agent work
        answer_link = create_link(clean_text, ScLinkContentType.STRING)        
        create_action_answer(action_element, answer_link)
        self.logger.info('Successfully processed the text using non-official API! Wery well!')
        finish_action_with_status(action_element, True)
        return ScResult.OK
    
    def _get_clean_text(self, raw_text: str, language: str) -> str:
        client = Client()
        messages = [{'role': 'user', 'content': constants.PROMPTS[language].format(raw_text)}]
        response = client.chat.completions.create(
            model='gpt-3.5-turbo',
            messages=messages,
            temperature=0,
        )
        self.logger.info(f'Successfully cleaned text for you\n: {response}')
        self.logger.info(response.choices[0].message.content)
        return response.choices[0].message.content      